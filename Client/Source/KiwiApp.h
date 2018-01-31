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

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "KiwiApp_Application/KiwiApp_Instance.h"
#include "KiwiApp_General/KiwiApp_StoredSettings.h"
#include "KiwiApp_General/KiwiApp_LookAndFeel.h"

#include "KiwiApp_Components/KiwiApp_TooltipWindow.h"

#include "KiwiApp_Network/KiwiApp_ApiController.h"

namespace ProjectInfo
{
    const char* const  projectName    = "Kiwi";
    const char* const  versionString  = "v1.0.0-beta.draft.1";
    const int          versionNumber  = 0x010;
}

namespace kiwi
{
    // ================================================================================ //
    //                                  KiWi APPLICATION                                //
    // ================================================================================ //
    
    class KiwiApp : public juce::JUCEApplication,
                    public NetworkSettings::Listener
    {
    public: // methods
        
        // ================================================================================ //
        //                                  JUCEApplication                                 //
        // ================================================================================ //
        
        //! @brief Called when the application starts.
        void initialise(juce::String const& commandLine) override;
        
        //! @brief Indicates that the user has tried to start up another instance of the app.
        void anotherInstanceStarted(juce::String const& command_line) override;
        
        //! @brief Called to allow the application to clear up before exiting.
        void shutdown() override;
        
        //! @brief Called when the operating system is trying to close the application.
        void systemRequestedQuit() override;
        
        //! @brief Returns the application's name.
        const juce::String getApplicationName() override;
        
        //! @brief Returns the application's version number.
        const juce::String getApplicationVersion() override;
        
        //! @brief Checks whether multiple instances of the app are allowed.
        bool moreThanOneInstanceAllowed() override;
        
        //! @brief Returns true if the app is running in a Mac OSX operating system.
        static bool isMacOSX();
        
        //! @brief Returns true if the app is running in a Linux operating system.
        static bool isLinux();
        
        //! @brief Returns true if the app is running in a Windows operating system.
        static bool isWindows();
        
        //==============================================================================
        
        //! @brief Get the current running application instance.
        static KiwiApp& use();
        
        //! @brief Get the current running application instance.
        static KiwiApp* getApp();
        
        //! @brief Get the current running kiwi instance.
        static Instance& useInstance();
        
        //! @brief Get the Api object.
        static Api& useApi();
        
        //! @brief Attempt to log-in the user (Async).
        //! @param name_or_email The name or email of the user.
        //! @param password The user password.
        //! @see logout, getCurrentUser
        static void login(std::string const& name_or_email,
                          std::string const& password,
                          std::function<void()> success_callback,
                          Api::ErrorCallback error_callback);
        
        //! @brief Attempt to register/signup the user.
        //! @param username user name
        //! @param email email address
        //! @param password password
        static void signup(std::string const& username,
                           std::string const& email,
                           std::string const& password,
                           std::function<void(std::string)> success_callback,
                           Api::ErrorCallback error_callback);
        
        //! @brief Returns the current user
        static Api::AuthUser const& getCurrentUser();
        
        //! @brief Log-out the user
        static void logout();
        
        //! @brief Get the current running engine instance.
        static engine::Instance& useEngineInstance();
        
        //! @brief Get the user id associated to this running application instance.
        static uint64_t userID();
        
        //! @brief Returns the application stored settings.
        static StoredSettings& useSettings();
        
        //! @brief Returns the MenuBarModel.
        static juce::MenuBarModel* getMenuBarModel();
        
        //! @brief Returns the application look and feel.
        static LookAndFeel& useLookAndFeel();
        
        //! @brief Returns the Tooltip Window component.
        static TooltipWindow& useTooltipWindow();
        
        // ================================================================================ //
        //                                      CONSOLE                                     //
        // ================================================================================ //
        
        //! @brief post a log message in the Console.
        static void log(std::string const& text);
        
        //! @brief post a message in the Console.
        static void post(std::string const& text);
        
        //! @brief post a warning message in the Console.
        static void warning(std::string const& text);
        
        //! @brief post an error message in the Console.
        static void error(std::string const& text);
        
        //==============================================================================
        
        //! @brief Attempt to close the given window asking user to save file if needed.
        void closeWindow(Window& window);
        
        // ================================================================================ //
        //                                  APPLICATION MENU                                //
        // ================================================================================ //
        
        //! @brief The Kiwi Application menu model class
        struct MainMenuModel : public juce::MenuBarModel
        {
            MainMenuModel();
            juce::StringArray getMenuBarNames();
            juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName);
            void menuItemSelected(int menuItemID, int topLevelMenuIndex);
        };
        
        //! @brief Called by MainMenuModel to get the menu names
        juce::StringArray getMenuNames();
        
        //! @brief Called by MainMenuModel to create menus
        void createMenu (juce::PopupMenu& menu, const juce::String& menuName);
        
        //! @brief Called by createMenu to create each menu
        void createOpenRecentPatchersMenu(juce::PopupMenu& menu);
        void createAccountMenu(juce::PopupMenu& menu);
        void createFileMenu(juce::PopupMenu& menu);
        void createEditMenu(juce::PopupMenu& menu);
        void createViewMenu(juce::PopupMenu& menu);
        void createOptionsMenu(juce::PopupMenu& menu);
        void createWindowMenu(juce::PopupMenu& menu);
        void createHelpMenu(juce::PopupMenu& menu);
        
        //! @brief Called by MainMenuModel to handle the main menu command
        void handleMainMenuCommand(int menuItemID);
        
        // ================================================================================ //
        //                                APPLICATION COMMAND                               //
        // ================================================================================ //
        
        //! @brief Bind a command target to the command manager.
        //! @details The function binds a command target to the command manager,
        //! and add the command manager as a listener.
        //! @param target The command target.
        static void bindToCommandManager(ApplicationCommandTarget* target);
        
        //! @brief Bind a component to the key mapping set.
        static void bindToKeyMapping(juce::Component* target);
        
        //! @brief Get the global ApplicationCommandManager
        static juce::ApplicationCommandManager& getCommandManager();
        
        //! @brief Notify command manager that a command status changed
        static void commandStatusChanged();
        
        //! @brief Get the command manager key mapping.
        static juce::KeyPressMappingSet* getKeyMappings();
        
        //! @brief This must return a complete list of commands that this target can handle.
        void getAllCommands(juce::Array<juce::CommandID>& commands) override;
        
        //! @brief This must provide details about one of the commands that this target can perform.
        void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
        
        //! @brief This must actually perform the specified command.
        bool perform(InvocationInfo const& info) override;
        
    public: // internal methods
        
        KiwiApp() = default;
        ~KiwiApp() = default;
        
    private: // methods
        
        //! @internal Utility to quit the app asynchronously.
        class AsyncQuitRetrier;
        
        //! @internal Initializes engine's factory declaring engine objects
        void declareEngineObjects();
        
        //! @internal Initializes gui specific objects.
        void declareObjectViews();
        
        //! @internal Checks if current Kiwi version is the latest. Show popup if version not up to date.
        void checkLatestRelease();
        
        // @brief Handles changes of server address.
        void networkSettingsChanged(NetworkSettings const& settings, juce::Identifier const& ids) override final;
        
    private: // members
        
        std::unique_ptr<ApiController>                      m_api_controller;
        std::unique_ptr<Api>                                m_api;
        
        std::unique_ptr<Instance>                           m_instance;
        std::unique_ptr<juce::ApplicationCommandManager>	m_command_manager;
        std::unique_ptr<MainMenuModel>                      m_menu_model;
        
        LookAndFeel                                         m_looknfeel;
        TooltipWindow                                       m_tooltip_window;
        std::unique_ptr<StoredSettings>                     m_settings;
    };
}
