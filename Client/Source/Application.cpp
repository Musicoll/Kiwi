/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#include "Application.hpp"

#include "CommandIDs.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      MENU                                        //
    // ================================================================================ //
    
    KiwiApp::MainMenuModel::MainMenuModel()
    {
        setApplicationCommandManagerToWatch(&getCommandManager());
    }
    
    StringArray KiwiApp::MainMenuModel::getMenuBarNames()
    {
        return KiwiApp::use().getMenuNames();
    }
    
    PopupMenu KiwiApp::MainMenuModel::getMenuForIndex(int topLevelMenuIndex, const String& menuName)
    {
        PopupMenu menu;
        KiwiApp::use().createMenu(menu, menuName);
        return menu;
    }
    
    void KiwiApp::MainMenuModel::menuItemSelected(int menuItemID, int topLevelMenuIndex)
    {
        KiwiApp::use().handleMainMenuCommand(menuItemID);
    }
    
    // ================================================================================ //
    //                                      APPLICATION                                 //
    // ================================================================================ //
    
    void KiwiApp::initialise(String const& commandLine)
    {
        initCommandManager();
        
        m_menu_model.reset(new MainMenuModel());
        
        m_instance = std::make_unique<jInstance>();
        
        #if JUCE_MAC
        PopupMenu macMainMenuPopup;
        macMainMenuPopup.addCommandItem(&getCommandManager(), CommandIDs::showAboutAppWindow);
        macMainMenuPopup.addSeparator();
        macMainMenuPopup.addCommandItem(&getCommandManager(), CommandIDs::showAppSettingsWindow);
        MenuBarModel::setMacMainMenu(m_menu_model.get(), &macMainMenuPopup, TRANS("Open Recent"));
        #endif
        
        m_instance->newPatcher();
    }
    
    void KiwiApp::shutdown()
    {
        #if JUCE_MAC
        MenuBarModel::setMacMainMenu(nullptr);
        #endif
    }
    
    void KiwiApp::systemRequestedQuit()
    {
        m_instance.reset();
        
        quit();
    }
    
    KiwiApp& KiwiApp::use()
    {
        KiwiApp* const app = getApp();
        assert(app != nullptr);
        return *app;
    }
    
    KiwiApp* KiwiApp::getApp()
    {
        return dynamic_cast<KiwiApp*>(JUCEApplication::getInstance());
    }
    
    uint64_t KiwiApp::userID()
    {
        return KiwiApp::use().m_instance->getUserId();
    }
    
    void KiwiApp::bindToCommandManager(ApplicationCommandTarget* target)
    {
        KiwiApp& app = KiwiApp::use();
        if(app.m_command_manager)
        {
            app.m_command_manager->registerAllCommandsForTarget(target);
        }
    }
    
    void KiwiApp::bindToKeyMapping(Component* target)
    {
        KiwiApp& app = KiwiApp::use();
        if(app.m_command_manager)
        {
            target->addKeyListener(app.m_command_manager->getKeyMappings());
        }
    }
    
    ApplicationCommandManager& KiwiApp::getCommandManager()
    {
        ApplicationCommandManager* cm = KiwiApp::use().m_command_manager.get();
        assert(cm != nullptr);
        return *cm;
    }
    
    void KiwiApp::commandStatusChanged()
    {
        KiwiApp* const app = KiwiApp::getApp();
        if(app && app->m_command_manager)
        {
            app->m_command_manager->commandStatusChanged();
        }
    }
    
    KeyPressMappingSet* KiwiApp::getKeyMappings()
    {
        KiwiApp* const app = KiwiApp::getApp();
        if(app && app->m_command_manager)
        {
            return app->m_command_manager->getKeyMappings();
        }
        
        return nullptr;
    }
    
    //==============================================================================
    
    StringArray KiwiApp::getMenuNames()
    {
        const char* const names[] =
        {
            "File", "Edit", "View", "Object", "Arrange", "Options", "Window", "Help", nullptr
        };
        
        return StringArray(names);
    }
    
    void KiwiApp::createMenu(PopupMenu& menu, const String& menuName)
    {
        if		(menuName == "File")        createFileMenu		(menu);
        else if (menuName == "Edit")        createEditMenu		(menu);
        else if (menuName == "View")        createViewMenu		(menu);
        else if (menuName == "Object")      createObjectMenu	(menu);
        else if (menuName == "Arrange")     createArrangeMenu	(menu);
        else if (menuName == "Options")     createOptionsMenu	(menu);
        else if (menuName == "Window")      createWindowMenu	(menu);
        else if (menuName == "Help")		createHelpMenu		(menu);
        
        else assert(false); // names have changed?
    }
    
    void KiwiApp::createOpenRecentPageMenu(PopupMenu& menu)
    {
        
    }
    
    void KiwiApp::createFileMenu(PopupMenu& menu)
    {
        menu.addCommandItem(m_command_manager.get(), CommandIDs::newPatcher);
        menu.addSeparator();
        
        menu.addCommandItem(m_command_manager.get(), CommandIDs::openFile);
        createOpenRecentPageMenu (menu);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::closeWindow);
        menu.addSeparator();
        
        menu.addCommandItem(m_command_manager.get(), CommandIDs::save);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::saveAs);
        menu.addSeparator();
        
#if ! JUCE_MAC
        menu.addSeparator();
        menu.addCommandItem(m_command_manager.get(), StandardApplicationCommandIDs::quit);
#endif
    }
    
    void KiwiApp::createEditMenu(PopupMenu& menu)
    {
        menu.addCommandItem(m_command_manager.get(), StandardApplicationCommandIDs::undo);
        menu.addCommandItem(m_command_manager.get(), StandardApplicationCommandIDs::redo);
        menu.addSeparator();
        menu.addCommandItem(m_command_manager.get(), StandardApplicationCommandIDs::cut);
        menu.addCommandItem(m_command_manager.get(), StandardApplicationCommandIDs::copy);
        menu.addCommandItem(m_command_manager.get(), StandardApplicationCommandIDs::paste);
        menu.addCommandItem(m_command_manager.get(), StandardApplicationCommandIDs::del);
        menu.addSeparator();
        menu.addCommandItem(m_command_manager.get(), CommandIDs::pasteReplace);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::duplicate);
        menu.addCommandItem(m_command_manager.get(), StandardApplicationCommandIDs::selectAll);
        menu.addCommandItem(m_command_manager.get(), StandardApplicationCommandIDs::deselectAll);
        menu.addSeparator();
    }
    
    void KiwiApp::createViewMenu(PopupMenu& menu)
    {
        menu.addCommandItem(m_command_manager.get(), CommandIDs::editModeSwitch);
    }
    
    void KiwiApp::createObjectMenu(PopupMenu& menu)
    {
        ;
    }
    
    void KiwiApp::createArrangeMenu(PopupMenu& menu)
    {
        menu.addCommandItem(m_command_manager.get(), CommandIDs::toFront);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::toBack);
    }
    
    void KiwiApp::createOptionsMenu(PopupMenu& menu)
    {
        ;
    }
    
    void KiwiApp::createWindowMenu(PopupMenu& menu)
    {
        menu.addCommandItem(m_command_manager.get(), CommandIDs::minimizeWindow);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::maximizeWindow);
        menu.addSeparator();
        
        menu.addCommandItem(m_command_manager.get(), CommandIDs::showConsoleWindow);
    }
    
    void KiwiApp::createHelpMenu(PopupMenu& menu)
    {
        ;
    }
    
    void KiwiApp::handleMainMenuCommand(int menuItemID)
    {
        ;
    }
    
    //==============================================================================
    
    void KiwiApp::getAllCommands(Array<CommandID>& commands)
    {
        JUCEApplication::getAllCommands(commands); // get the standard quit command
        
        const CommandID ids[] =
        {
            CommandIDs::newPatcher,
            CommandIDs::openFile,
            CommandIDs::showConsoleWindow
        };
        
        commands.addArray(ids, numElementsInArray(ids));
    }
    
    void KiwiApp::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
    {
        switch(commandID)
        {
            case CommandIDs::newPatcher:
            {
                result.setInfo(TRANS("New Patcher..."), TRANS("Create a new Patcher"),
                               CommandCategories::general, 0);
                
                result.addDefaultKeypress('n', ModifierKeys::commandModifier);
                break;
            }
            case CommandIDs::openFile:
            {
                result.setInfo(TRANS("Open..."), TRANS("Open a File"),
                               CommandCategories::general, 0);
                
                result.addDefaultKeypress('o', ModifierKeys::commandModifier);
                break;
            }
            case CommandIDs::showConsoleWindow:
            {
                result.setInfo(TRANS("Console"), TRANS("Show Kiwi Console Window"),
                               CommandCategories::windows, 0);
                
                result.addDefaultKeypress('k', ModifierKeys::commandModifier);
                break;
            }
            default:
            {
                JUCEApplication::getCommandInfo(commandID, result); break;
            }
        }
    }
    
    bool KiwiApp::perform(InvocationInfo const& info)
    {
        switch(info.commandID)
        {
            case CommandIDs::newPatcher :           { m_instance->newPatcher(); break; }
            case CommandIDs::showConsoleWindow :    { m_instance->showConsoleWindow(); break; }
            
            default : return JUCEApplication::perform(info);
        }
        
        return true;
    }
    
    void KiwiApp::initCommandManager()
    {
        m_command_manager.reset(new ApplicationCommandManager());
        
        m_command_manager->registerAllCommandsForTarget(this);
    }
}