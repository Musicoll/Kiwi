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
        m_code_changed(true),
        m_error_line(-1)
        {
            setBounds(0, 0, 512, 384);
            m_editor.setColour(juce::CodeEditorComponent::backgroundColourId, juce::Colours::white);
            m_editor.setColour(juce::CodeEditorComponent::lineNumberBackgroundId, juce::Colours::lightgrey);
            m_editor.setScrollbarThickness(8);
            addAndMakeVisible(&m_editor);
            
            m_button_sync.setButtonText("Sync");
            m_button_sync.setClickingTogglesState(false);
            m_button_sync.setName("update");
            m_button_sync.addListener(this);
            m_button_sync.setTooltip("Synchonize the audio engine with this code");
            m_button_sync.setWantsKeyboardFocus(false);
            addAndMakeVisible(&m_button_sync);
            
            m_button_lock.setName("lock");
            m_button_lock.addListener(this);
            m_button_lock.setWantsKeyboardFocus(false);
            addAndMakeVisible(&m_button_lock);
            
            m_button_errs.setButtonText(juce::String(juce::CharPointer_UTF8 ("•")));
            m_button_errs.setName("error");
            m_button_errs.addListener(this);
            m_button_errs.setTooltip("Move the caret to the error line");
            m_button_errs.setWantsKeyboardFocus(false);
            addAndMakeVisible(&m_button_errs);
            
            m_console.setReadOnly(true);
            m_console.setColour(juce::TextEditor::backgroundColourId, juce::Colours::lightgrey);
            m_console.setColour(juce::TextEditor::textColourId, juce::Colours::black);
            m_console.setBorder({ 1, 0, 1, 3 });
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
                g.setColour(juce::Colours::grey.withAlpha(0.2f));
                g.fillRect(0, 0, getWidth(), getHeight() - 26);
            }
        }
        
        void resized() override
        {
            const auto width   = getWidth();
            const auto height  = getHeight();
            const auto yoffset = height - 26;
            const auto wsize   = 50;
            m_editor.setBounds(0, 0, width, yoffset - 2);
            m_button_sync.setBounds(wsize * 0 + 2, yoffset, 48, 24);
            m_button_lock.setBounds(wsize * 1 + 2, yoffset, 48, 24);
            m_button_errs.setBounds(wsize * 2 + 2, yoffset, 22, 24);
            m_console.setBounds(wsize * 2 + 24 + 2, yoffset + 1, width - (wsize * 2 + 24 + 2) - 2, 22);
        }
        
        void visibilityChanged() override
        {
            if(isVisible())
            {
                m_code_changed = true;
                computeErrors();
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
            auto const name = btn->getName();
            if(name == "update")
            {
                uploadToDspCode();
            }
            else if(name == "lock")
            {
                if(!m_owner.canLock() && !m_owner.hasLock())
                {
                    forceUnlock();
                }
                else if(m_owner.canLock() && !m_owner.hasLock())
                {
                    setLock(true);
                }
                else
                {
                    setLock(false);
                }
            }
            else if(name == "error")
            {
                if(m_error_line >= 0)
                {
                    juce::CodeDocument::Position const pos(m_document, m_error_line-1, 0);
                    m_editor.moveCaretTo(pos, false);
                }
            }
        }
        
        void buttonStateChanged (juce::Button* btn) override
        {
            ;
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
            m_owner.deferMain([this]()
                              {
                                  const juce::MessageManagerLock mmLock;
                                  m_editor.setReadOnly(!m_owner.hasLock());
                                  m_button_sync.setEnabled(m_owner.hasLock());
                                  
                                  if(!m_owner.canLock() && !m_owner.hasLock())
                                  {
                                      m_button_lock.setButtonText("Force");
                                      m_button_lock.setTooltip("Force to unlock");
                                  }
                                  else if(m_owner.canLock() && !m_owner.hasLock())
                                  {
                                      m_button_lock.setButtonText("Grab");
                                      m_button_lock.setTooltip("Grab the lock");
                                  }
                                  else
                                  {
                                      m_button_lock.setButtonText("Leave");
                                      m_button_lock.setTooltip("Leave the lock");
                                  }
                                  repaint();
                              });
            
        }
        
        void updateCompileOptions()
        {
            m_code_changed = true;
        }
        
    private:
        
        // The private interface
        // ================================================================================ //
        
        //! @brief Set the current code
        void setCode(std::string const& code)
        {
            juce::CodeEditorComponent::State state(m_editor);
            m_lock = false;
            m_editor.loadContent(juce::String(code));
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
        
        //! @brief Ensure that the lock is freed and ask for to synchronize the code if needed
        bool shouldClose()
        {
            if(m_owner.hasLock())
            {
                auto const current_code = getCode();
                if(m_owner.getDspCode() != current_code)
                {
                    const int r = juce::AlertWindow::showYesNoCancelBox(juce::AlertWindow::QuestionIcon,
                                                                        juce::String("Closing document..."),
                                                                        juce::String("Do you want to synchronize the changes?"),
                                                                        juce::String("Synchronize"),
                                                                        juce::String("Discard changes"),
                                                                        juce::String("Cancel"),
                                                                        &m_window);
                    if(r == 1) // Synchronize
                    {
                        m_owner.setDspCode(std::string(current_code));
                        m_owner.grabLock(false);
                        return 1;
                    }
                    else if(r == 2) // Discard changes
                    {
                        m_owner.grabLock(false);
                        return 1;
                    }
                    return false;
                }
                m_owner.grabLock(false);
            }
            return true;
        }
        
        // Try to set the lock state
        void setLock(bool state)
        {
            m_owner.grabLock(state);
        }
        
        // Force unlock
        void forceUnlock()
        {
            const int r = juce::AlertWindow::showOkCancelBox(juce::AlertWindow::QuestionIcon,
                                                             juce::String("Force Unlock"),
                                                             juce::String("Are you sure, you want to force to unlock the editor?\nPerhaps somebody else is using it..."),
                                                             juce::String("Unlock"),
                                                             juce::String("Cancel"),
                                                             &m_window);
            
            if(r == 1) // save button
            {
                m_owner.forceUnlock();
            }
        }
        
        // Set the current error line
        void setErrorLine(int line)
        {
            m_error_line = line;
            if(m_error_line == -1)
            {
                m_console.setText(juce::String(""));
                m_console.repaint();
                m_button_errs.setEnabled(false);
                m_button_errs.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
                m_button_errs.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
            }
            else
            {
                m_button_errs.setEnabled(true);
                m_button_errs.setColour(juce::TextButton::textColourOnId, juce::Colours::red);
                m_button_errs.setColour(juce::TextButton::textColourOffId, juce::Colours::red);
            }
        }
        
        //! @brief Get the compile options
        std::vector<std::string> getCompileOptions()
        {
            return m_owner.getCompileOptions();
        }
        
        //! @brief Compute the errors of the current code
        void computeErrors()
        {
            if(!m_code_changed || !isVisible())
            {
                return;
            }
            m_code_changed = false;
            if(m_document.getNumCharacters() == 0)
            {
                setErrorLine(-1);
                return;
            }
            std::string const code = getCode();
            if(code.empty() || code.find_first_not_of("\n\r\t\v\f ") == std::string::npos)
            {
                setErrorLine(-1);
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
                uptr_faust_factory nfactory = std::unique_ptr<llvm_dsp_factory, bool(*)(llvm_dsp_factory*)>(createDSPFactoryFromString("", code, options.size(), argv.data(), std::string(), errors), deleteDSPFactory);
                stopMTDSPFactories();
            }
            auto pos = errors.find_first_of("\r\n");
            if(pos != std::string::npos)
            {
                errors.erase(pos);
            }
            pos = errors.find("ERROR : ");
            if(pos != std::string::npos)
            {
                errors.erase(pos, 8);
            }
            if(!errors.empty())
            {
                int line;
                errors.replace(0, 3, "l.");
                try
                {
                    line = std::stoi(errors.substr(2));
                }
                catch(...)
                {
                    line = -2;
                }
                setErrorLine(line);
                m_console.setText(errors);
            }
            else
            {
                setErrorLine(-1);
            }
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
                auto* content = dynamic_cast<FaustTilde::CodeEditor *>(getContentComponent());
                if(content)
                {
                    if(content->shouldClose())
                    {
                        removeFromDesktop();
                    }
                }
                else
                {
                    removeFromDesktop();
                }
            }
        };
        
        FaustTilde&                 m_owner;
        juce::TextButton            m_button_sync;
        juce::TextButton            m_button_lock;
        juce::TextButton            m_button_errs;
        juce::TextEditor            m_console;
        
        FaustTokeniser              m_highlither;
        juce::CodeDocument          m_document;
        juce::CodeEditorComponent   m_editor;
        std::atomic<bool>           m_lock;
        std::atomic<bool>           m_code_changed;
        int                         m_error_line;
        Window                      m_window;
    };
}}
