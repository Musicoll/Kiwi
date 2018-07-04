/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_FaustTilde.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_FaustCodeTokenizer.h>
#include <KiwiEngine/KiwiEngine_Factory.h>
#include <faust/dsp/llvm-dsp.h>

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "../../../Client/Source/KiwiApp_Ressources/KiwiApp_BinaryData.h"
//#include "../../../Client/Source/KiwiApp.h"

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    
    class FaustTilde::CodeEditor :
    public juce::Component,
    public juce::CodeDocument::Listener,
    public juce::Button::Listener,
    public juce::Timer
    {
    public:
        
        CodeEditor(FaustTilde& owner) :
        m_owner(owner),
        m_editor(m_document, &m_highlither),
        m_lock(true),
        m_code_changed(true)
        {
            setBounds(0, 0, 512, 384);
            m_editor.setColour(juce::CodeEditorComponent::backgroundColourId, juce::Colours::white);
            m_editor.setScrollbarThickness(8);
            //m_editor.setCommandManager(&KiwiApp::getCommandManager());
            addAndMakeVisible(&m_editor);
            
            auto img_sync = juce::ImageFileFormat::loadFrom(binary_data::images::refresh_png, binary_data::images::refresh_png_size);
            m_button_sync.setImages(false, true, true,
                                    img_sync, 1.f, juce::Colours::transparentWhite,
                                    img_sync, 1.f, juce::Colours::lightgrey.withAlpha(0.5f),
                                    img_sync, 1.f, juce::Colours::lightgrey.withAlpha(0.5f));
            m_button_sync.setClickingTogglesState(false);
            m_button_sync.setRadioGroupId(1);
            m_button_sync.addListener(this);
            m_button_sync.setTooltip("Update DSP code");
            addAndMakeVisible(&m_button_sync);
            
            auto img_lock = juce::ImageFileFormat::loadFrom(binary_data::images::locked_png, binary_data::images::locked_png_size);
            m_button_lock.setImages(false, true, true,
                                    img_lock, 1.f, juce::Colours::transparentWhite,
                                    img_lock, 1.f, juce::Colours::lightgrey.withAlpha(0.5f),
                                    img_lock, 1.f, juce::Colours::lightgrey.withAlpha(0.5f));
            m_button_lock.setClickingTogglesState(true);
            m_button_lock.setRadioGroupId(2);
            m_button_lock.addListener(this);
            m_button_lock.setTooltip("Grab the lock");
            addAndMakeVisible(&m_button_lock);
            
            auto img_force = juce::ImageFileFormat::loadFrom(binary_data::images::locked_png, binary_data::images::locked_png_size);
            m_button_forc.setImages(false, true, true,
                                    img_force, 1.f, juce::Colours::transparentWhite,
                                    img_force, 1.f, juce::Colours::lightgrey.withAlpha(0.5f),
                                    img_force, 1.f, juce::Colours::lightgrey.withAlpha(0.5f));
            m_button_forc.setClickingTogglesState(true);
            m_button_forc.setRadioGroupId(3);
            m_button_forc.addListener(this);
            m_button_forc.setTooltip("Force unlock");
            addAndMakeVisible(&m_button_forc);
            
            m_console.setMultiLine(true);
            m_console.setReadOnly(true);
            m_console.setColour(juce::TextEditor::backgroundColourId, juce::Colours::grey);
            m_console.setColour(juce::TextEditor::textColourId, juce::Colours::red);
            addAndMakeVisible(&m_console);
            resized();
            
            updateLockState();
            m_window.setContentNonOwned(this, true);
        }
        
        // The Juce Component interface
        // ================================================================================ //
        void paint (juce::Graphics& g) override
        {
            g.fillAll(juce::Colours::grey);
        }
        
        void paintOverChildren (juce::Graphics& g) override
        {
            if(!m_owner.hasLock())
            {
                g.fillAll(juce::Colours::grey.withAlpha(0.25f));
            }
        }
        
        void resized() override
        {
            const auto width  = getWidth();
            const auto height = getHeight();
            const auto bansize   = 40;
            const auto btnsize   = 24;
            const auto btnoffset = (bansize - btnsize) / 2;
            m_editor.setBounds(0, 0, width, height - bansize);
            m_button_sync.setBounds(bansize * 0 + btnoffset, height - bansize + btnoffset, btnsize, btnsize);
            m_button_lock.setBounds(bansize * 1 + btnoffset, height - bansize + btnoffset, btnsize, btnsize);
            m_button_forc.setBounds(bansize * 2 + btnoffset, height - bansize + btnoffset, btnsize, btnsize);
            m_console.setBounds(bansize * 3, height - bansize, width - bansize * 3, bansize);
        }
        
        void visibilityChanged() override
        {
            if(isVisible())
            {
                m_document.addListener(this);
                startTimer(1000);
            }
            else
            {
                m_document.removeListener(this);
                stopTimer();
            }
        }
        
        // The Juce Timer interface
        // ================================================================================ //
        
        void timerCallback() override
        {
            computeErrors();
        }
        
        // The Juce CodeDocument::Listener interface
        // ================================================================================ //
        
        void codeDocumentTextInserted(const juce::String& , int ) override
        {
            uploadToEditCode();
        }
        
        void codeDocumentTextDeleted(int , int ) override
        {
            uploadToEditCode();
        }
        
        // The Juce Button::Listener interface
        // ================================================================================ //
        void buttonClicked(juce::Button* btn) override
        {
            auto const iid = btn->getRadioGroupId();
            if(iid == 1)
            {
                uploadToDspCode();
            }
            else if(iid == 2)
            {
                setLock(btn->getToggleState());
            }
            else if(iid == 3)
            {
                m_owner.forceUnlock();
            }
        }
        
        // The public interface
        // ================================================================================ //
        
        // ================================================================================ //
        
        //! @brief Display the Code editor
        void show()
        {
            m_owner.deferMain([this]()
                              {
                                  downloadCode();
                                  if(!m_window.isShowing())
                                  {
                                      m_window.addToDesktop();
                                  }
                                  else
                                  {
                                      m_window.toFront(true);
                                  }
                              });
        }
        
        //! @brief Download the code from the owner and update the editor content
        void downloadCode()
        {
            setCode(m_owner.getEditCode());
        }
        
        void updateLockState()
        {
            const juce::MessageManagerLock mmLock;
            m_editor.setReadOnly(!m_owner.hasLock());
            m_button_lock.setToggleState(m_owner.hasLock(), juce::NotificationType::dontSendNotification);
            repaint();
        }
        
    private:
        
        // The private interface
        // ================================================================================ //
        
        //! @brief Set the current code
        void setCode(std::string&& code)
        {
            juce::CodeEditorComponent::State state(m_editor);
            m_lock = false;
            m_editor.loadContent(juce::String(std::move(code)));
            state.restoreState(m_editor);
            m_lock = true;
            m_code_changed = true;
        }
        
        //! @brief Get the current code
        std::string getCode() const
        {
            return m_document.getAllContent().toStdString();
        }
        
        //! @brief Upload the current code to the DSP code of the owner
        void uploadToDspCode()
        {
            m_owner.setDspCode(getCode());
        }
        
        //! @brief Upload the current code to the Edition code of the owner
        void uploadToEditCode()
        {
            if(m_lock)
            {
                m_owner.setEditCode(getCode());
            }
            m_code_changed = true;
        }
        
        //! @brief Get the compile options
        std::vector<std::string> getCompileOptions()
        {
            return m_owner.getCompileOptions();
        }
        
        //! @brief Compute the errors of the current code
        void computeErrors()
        {
            if(!m_code_changed)
            {
                return;
            }
            m_code_changed = false;
            if(m_document.getNumCharacters() == 0)
            {
                m_console.clear();
                return;
            }
            std::string errors;
            std::vector<std::string> options(m_owner.getCompileOptions());
            std::vector<char const*> argv(options.size());
            for(size_t i = 0; i < options.size(); ++i)
            {
                argv[i] = options[i].c_str();
            }
            
            if(startMTDSPFactories())
            {
                uptr_faust_factory nfactory = std::unique_ptr<llvm_dsp_factory, bool(*)(llvm_dsp_factory*)>(createDSPFactoryFromString("", getCode(), options.size(), argv.data(), std::string(), errors), deleteDSPFactory);
                stopMTDSPFactories();
            }
            m_console.setText(errors);
        }
        
        // Try to set the lock state
        void setLock(bool state)
        {
            m_owner.grabLock(state);
        }
        
        // ================================================================================ //
        
        class Window : public juce::DocumentWindow
        {
        public:
            
            Window() :
            juce::DocumentWindow("FAUST Code Editor",
                                 juce::Colours::white,
                                 juce::DocumentWindow::allButtons,
                                 false)
            {
                setUsingNativeTitleBar(true);
                setResizable(true, true);
                setVisible(true);
                setTopLeftPosition(40, 40);
            }
            
            void closeButtonPressed() override
            {
                removeFromDesktop();
            }
        };
        
        FaustTilde&                 m_owner;
        juce::ImageButton           m_button_sync;
        juce::ImageButton           m_button_lock;
        juce::ImageButton           m_button_forc;
        juce::TextEditor            m_console;
        
        FaustTokeniser              m_highlither;
        juce::CodeDocument          m_document;
        juce::CodeEditorComponent   m_editor;
        std::atomic<bool>           m_lock;
        std::atomic<bool>           m_code_changed;
        
        Window                      m_window;
    };
}}
