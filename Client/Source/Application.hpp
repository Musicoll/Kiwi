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

#ifndef KIWI_APPLICATION_HPP_INCLUDED
#define KIWI_APPLICATION_HPP_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "jInstance.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  KiWi APPLICATION                                //
    // ================================================================================ //
    
    class KiwiApp : public JUCEApplication
    {
    public:
        KiwiApp() = default;
        ~KiwiApp() = default;
        
        void initialise(String const& commandLine) override;
        
        void shutdown() override;
        
        void systemRequestedQuit() override;
        
        const String getApplicationName() override       { return ProjectInfo::projectName; }
        const String getApplicationVersion() override    { return ProjectInfo::versionString; }
        bool moreThanOneInstanceAllowed() override       { return true; }
        
        //==============================================================================
        
        //! @brief Retrieve the current running Application instance.
        static KiwiApp& use();
        
        //! @brief Retrieve the current running Application instance.
        static KiwiApp* getApp();
        
        //==============================================================================
        
        //! @brief The Kiwi Application menu model class
        struct MainMenuModel : public MenuBarModel
        {
            MainMenuModel();
            StringArray getMenuBarNames();
            PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName);
            void menuItemSelected(int menuItemID, int topLevelMenuIndex);
        };
        
        //! @brief Called by MainMenuModel to get the menu names
        StringArray getMenuNames();
        
        //! @brief Called by MainMenuModel to create menus
        void createMenu (PopupMenu& menu, const String& menuName);
        
        //! @brief Called by createMenu to create each menu
        void createOpenRecentPageMenu	(PopupMenu& menu);
        void createFileMenu				(PopupMenu& menu);
        void createEditMenu				(PopupMenu& menu);
        void createViewMenu				(PopupMenu& menu);
        void createObjectMenu			(PopupMenu& menu);
        void createArrangeMenu			(PopupMenu& menu);
        void createOptionsMenu			(PopupMenu& menu);
        void createWindowMenu			(PopupMenu& menu);
        void createHelpMenu				(PopupMenu& menu);
        
        //! @brief Called by MainMenuModel to handle the main menu command
        void handleMainMenuCommand (int menuItemID);
        
        //==============================================================================
        
        //! @brief Bind a command target to the command manager.
        //! @details The function binds a command target to the command manager,
        //! and add the command manager as a listener.
        //! @param target The command target.
        static void bindToCommandManager(ApplicationCommandTarget* target);
        
        //! @brief Bind a component to the key mapping set.
        static void bindToKeyMapping(Component* target);
        
        //! @brief Get the global ApplicationCommandManager
        static ApplicationCommandManager& getCommandManager();
        
        //! @brief Notify command manager that a command status changed
        static void commandStatusChanged();
        
        //! @brief Get the command manager key mapping.
        static KeyPressMappingSet* getKeyMappings();
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        //! @brief This must return a complete list of commands that this target can handle.
        void getAllCommands(Array <CommandID>& commands) override;
        
        //! @brief This must provide details about one of the commands that this target can perform.
        void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
        
        //! @brief This must actually perform the specified command.
        bool perform(InvocationInfo const& info) override;
        
    private:

        //! @brief Initialise the command manager
        void initCommandManager();
        
    private:
        
        std::unique_ptr<jInstance>                  m_instance;
        std::unique_ptr<ApplicationCommandManager>	m_command_manager;
        std::unique_ptr<MainMenuModel>				m_menu_model;
    };
}

#endif // KIWI_APPLICATION_HPP_INCLUDED
