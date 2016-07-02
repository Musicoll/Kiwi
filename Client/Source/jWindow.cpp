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

#include "jWindow.hpp"
#include "CommandIDs.hpp"
#include "Application.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      JWINDOW                                     //
    // ================================================================================ //
    
    jWindow::jWindow(std::string const& name, juce::Colour color, int requiredButtons, bool addToDesktop) :
    DocumentWindow(name, color, requiredButtons, addToDesktop)
    {
        #if ! JUCE_MAC
        setMenuBar(KiwiApp::use().m_menu_model);
        #endif
        
        KiwiApp::bindToCommandManager(this);
        KiwiApp::bindToKeyMapping(this);
        
        setUsingNativeTitleBar(true);
        setSize(600, 500);
        centreWithSize(getWidth(), getHeight());
        setResizable(true, true);
        setVisible(true);
    }
    
    jWindow::~jWindow()
    {
        ;
    }
    
    void jWindow::closeButtonPressed()
    {
        juce::JUCEApplication::quit();
    }
    
    // ================================================================================ //
    //                              APPLICATION COMMAND TARGET                          //
    // ================================================================================ //
    
    ApplicationCommandTarget* jWindow::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void jWindow::getAllCommands(Array<CommandID>& commands)
    {
        commands.add(CommandIDs::closeWindow);
        commands.add(CommandIDs::minimizeWindow);
        commands.add(CommandIDs::maximizeWindow);
    }
    
    void jWindow::getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result)
    {
        switch (commandID)
        {
            case CommandIDs::closeWindow:
            {
                result.setInfo(TRANS("Close"), TRANS("Close Window"), CommandCategories::windows, 0);
                result.addDefaultKeypress('w', ModifierKeys::commandModifier);
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
    
    bool jWindow::perform(const InvocationInfo& info)
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