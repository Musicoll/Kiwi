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
#include <faust/gui/UI.h>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_FaustTilde.h>
#include <KiwiModel/KiwiModel_DocumentManager.h>
#include <KiwiModel/KiwiModel_DocumentManager.h>

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "../../../Client/Source/KiwiApp_Ressources/KiwiApp_BinaryData.h"

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    
    class FaustTilde::CodeEditor
    {
    public:
        
        CodeEditor(FaustTilde& o) : m_owner(o), m_manager(*this)
        {
            m_window.setContentNonOwned(&m_manager, true);
        }
        
        void open()
        {
            m_owner.deferMain([this]()
            {
                if(!m_window.isShowing())
                {
                    m_manager.setCode(m_owner.getEditCode());
                    m_window.addToDesktop();
                }
            });
        }
        
        void update()
        {
            m_manager.setCode(m_owner.getEditCode());
        }
        
        void codeChanged()
        {
            m_owner.setEditCode(m_manager.getCode());
        }
        
        void synchronize()
        {
            m_owner.setDspCode(m_manager.getCode());
        }
        
        std::vector<std::string> getCompileOptions()
        {
            return m_owner.getCompileOptions();
        }
        
    private:
        
        // ================================================================================ //
        
        class CodeEditorManager :
        public juce::Component,
        public juce::CodeDocument::Listener,
        public juce::Button::Listener,
        public juce::Timer
        {
        public:
            
            CodeEditorManager(CodeEditor& o) :
            m_owner(o),
            m_editor(m_document, &m_highlither),
            m_lock(true),
            m_code_changed(true)
            {
                setBounds(0, 0, 512, 384);
                m_editor.setColour(juce::CodeEditorComponent::backgroundColourId, juce::Colours::white);
                m_editor.setScrollbarThickness(8);
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
                
                m_console.setMultiLine(true);
                m_console.setReadOnly(true);
                m_console.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
                m_console.setColour(juce::TextEditor::textColourId, juce::Colours::black);
                addAndMakeVisible(&m_console);
                resized();
            }
            
            void paint (juce::Graphics& g) override
            {
                g.fillAll(juce::Colours::grey);
            }
            
            void resized() override
            {
                const auto width  = getWidth();
                const auto height = getHeight();
                const auto bansize   = 40;
                const auto btnsize   = 24;
                const auto btnoffset = (bansize - btnsize) / 2;
                m_editor.setBounds(0, 0, width, height - bansize);
                m_button_sync.setBounds(0 + btnoffset, height - bansize + btnoffset, btnsize, btnsize);
                m_button_lock.setBounds(bansize + btnoffset, height - bansize + btnoffset, btnsize, btnsize);
                m_console.setBounds(bansize * 2, height - bansize, width - bansize * 2, bansize);
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
            
            void notifyOwner()
            {
                if(m_lock)
                {
                    m_owner.codeChanged();
                }
                m_code_changed = true;
            }
            
            void timerCallback() override
            {
                if(!m_code_changed)
                {
                    return;
                }
                m_code_changed = false;
                std::vector<std::string> options(m_owner.getCompileOptions());
                std::vector<char const*> argv(options.size());
                for(size_t i = 0; i < options.size(); ++i)
                {
                    argv[i] = options[i].c_str();
                }
            
                if(startMTDSPFactories())
                {
                    uptr_faust_factory nfactory = std::unique_ptr<llvm_dsp_factory, bool(*)(llvm_dsp_factory*)>(createDSPFactoryFromString("", getCode(), options.size(), argv.data(), std::string(), m_errors), deleteDSPFactory);
                    stopMTDSPFactories();
                }
                m_console.setText(m_errors);
            }
            
            void codeDocumentTextInserted(const juce::String& , int ) override
            {
                notifyOwner();
            }
            
            void codeDocumentTextDeleted(int , int ) override
            {
                notifyOwner();
            }
            

            
            void buttonClicked(juce::Button* btn) override
            {
                auto const iid = btn->getRadioGroupId();
                if(iid == 1)
                {
                    m_owner.synchronize();
                }
            }
            
            void setCode(std::string&& code)
            {
                juce::CodeEditorComponent::State state(m_editor);
                m_lock = false;
                m_editor.loadContent(juce::String(std::move(code)));
                state.restoreState(m_editor);
                m_lock = true;
                m_code_changed = true;
            }
            
            std::string getCode()
            {
                return m_document.getAllContent().toStdString();
            }
            
        private:
            CodeEditor&                 m_owner;
            
            juce::ImageButton           m_button_sync;
            juce::ImageButton           m_button_lock;
            juce::TextEditor            m_console;
            
            FaustTokeniser              m_highlither;
            juce::CodeDocument          m_document;
            juce::CodeEditorComponent   m_editor;
            std::atomic<bool>           m_lock;
            std::atomic<bool>           m_code_changed;
            std::string                 m_errors;
        };
        
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
        
        // ================================================================================ //
        
        FaustTilde&       m_owner;
        Window            m_window;
        CodeEditorManager m_manager;
    };
}}
