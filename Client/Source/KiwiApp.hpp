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

#ifndef KIWI_KIWIAPP_HPP_INCLUDED
#define KIWI_KIWIAPP_HPP_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "KiwiApp_Instance.hpp"
#include "KiwiApp_StoredSettings.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  KiWi APPLICATION                                //
    // ================================================================================ //
    
    class KiwiApp : public juce::JUCEApplication
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
        
        //==============================================================================
        
        //! @brief Get the current running application instance.
        static KiwiApp& use();
        
        //! @brief Get the current running application instance.
        static KiwiApp* getApp();
        
        //! @brief Get the current running engine instance.
        static engine::Instance& useEngineInstance();
        
        //! @brief Get the user id associated to this running application instance.
        static uint64_t userID();
        
        //! @brief Returns the application stored settings.
        static StoredSettings& useSettings();
        
        //! @brief Returns the MenuBarModel.
        static juce::MenuBarModel* getMenuBarModel();
        
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
        bool closeWindow(Window& window);
        
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
        void createFileMenu(juce::PopupMenu& menu);
        void createEditMenu(juce::PopupMenu& menu);
        void createViewMenu(juce::PopupMenu& menu);
        void createObjectMenu(juce::PopupMenu& menu);
        void createArrangeMenu(juce::PopupMenu& menu);
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

        //! @brief Initialise the command manager
        void initCommandManager();
        
    private: // members
        
        std::unique_ptr<Instance>                           m_instance;
        std::unique_ptr<juce::ApplicationCommandManager>	m_command_manager;
        std::unique_ptr<MainMenuModel>                      m_menu_model;
        
        std::unique_ptr<StoredSettings>                     m_settings;
    };
}

#endif // KIWI_KIWIAPP_HPP_INCLUDED
