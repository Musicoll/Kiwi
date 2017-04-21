/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <juce_gui_basics/juce_gui_basics.h>

#include "KiwiApp.hpp"
#include "KiwiApp_Window.hpp"
#include "KiwiApp_CommandIDs.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      WINDOW                                      //
    // ================================================================================ //
    
    Window::Window(std::string const& name, std::unique_ptr<juce::Component> content,
                   bool resizable,
                   bool is_main_window,
                   juce::String settings_name,
                   bool add_menubar) :
    DocumentWindow(name, juce::Colours::lightgrey, allButtons, true),
    m_settings_name(settings_name)
    {
        setUsingNativeTitleBar(true);
        
        if(!resizable)
        {
            setTitleBarButtonsRequired(juce::DocumentWindow::minimiseButton
                                       | juce::DocumentWindow::closeButton, true);
        }
        
        if(content)
        {
            int width = content->getWidth() > 0 ? content->getWidth() : 300;
            int height = content->getHeight() > 0 ? content->getHeight() : 300;
            
            content->setSize(width, height);
            setContentOwned(content.release(), true);
        }
        
        KiwiApp::bindToCommandManager(this);
        KiwiApp::bindToKeyMapping(this);
        
        if(add_menubar)
        {
            setMenuBar(KiwiApp::getMenuBarModel());
        }
        
        setResizable(resizable, false);
        restoreWindowState();
    }
    
    Window::~Window()
    {
        saveWindowState();
    }
    
    void Window::restoreWindowState()
    {
        if(m_settings_name.isNotEmpty())
        {
            juce::String window_state(getGlobalProperties().getValue(m_settings_name));
            
            if(window_state.isNotEmpty())
            {
                auto last_bounds = getBounds();
                restoreWindowStateFromString(window_state);
                
                if(!isResizable())
                {
                    setSize(last_bounds.getWidth(), last_bounds.getHeight());
                }
            }
        }
    }
    
    void Window::saveWindowState()
    {
        if(m_settings_name.isNotEmpty())
        {
            getGlobalProperties().setValue(m_settings_name, getWindowStateAsString());
        }
    }
    
    bool Window::isMainWindow() const
    {
        return m_is_main_window;
    }
    
    void Window::closeButtonPressed()
    {
        KiwiApp::use().closeWindow(*this);
    }
    
    // ================================================================================ //
    //                              APPLICATION COMMAND TARGET                          //
    // ================================================================================ //
    
    juce::ApplicationCommandTarget* Window::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void Window::getAllCommands(juce::Array<juce::CommandID>& commands)
    {
        commands.add(CommandIDs::closeWindow);
        commands.add(CommandIDs::minimizeWindow);
        commands.add(CommandIDs::maximizeWindow);
    }
    
    void Window::getCommandInfo(const juce::CommandID commandID, juce::ApplicationCommandInfo& result)
    {
        switch (commandID)
        {
            case CommandIDs::closeWindow:
            {
                result.setInfo(TRANS("Close"), TRANS("Close Window"), CommandCategories::windows, 0);
                result.addDefaultKeypress('w', juce::ModifierKeys::commandModifier);
                result.setActive(getDesktopWindowStyleFlags() & allButtons || getDesktopWindowStyleFlags() & closeButton);
                break;
            }
            case CommandIDs::minimizeWindow:
                result.setInfo(TRANS("Minimize"), TRANS("Minimize Window"), CommandCategories::windows, 0);
                result.setActive(getDesktopWindowStyleFlags() & allButtons || getDesktopWindowStyleFlags() & minimiseButton);
                break;
                
            case CommandIDs::maximizeWindow:
                result.setInfo(TRANS("Maximize"), TRANS("Maximize Window"), CommandCategories::windows, 0);
                result.setActive(getDesktopWindowStyleFlags() & allButtons || getDesktopWindowStyleFlags() & maximiseButton);
                break;
                
            default: break;
        }
    }
    
    bool Window::perform(const InvocationInfo& info)
    {
        switch (info.commandID)
        {
            case CommandIDs::minimizeWindow: minimiseButtonPressed(); break;
            case CommandIDs::maximizeWindow: maximiseButtonPressed(); break;
            case CommandIDs::closeWindow:    closeButtonPressed();    break;
            default: return false;
        }
        return true;
    }
}
