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

#include <KiwiModel/KiwiModel_DataModel.hpp>

#include "KiwiApp.hpp"
#include "KiwiApp_CommandIDs.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      MENU                                        //
    // ================================================================================ //
    
    KiwiApp::MainMenuModel::MainMenuModel()
    {
        setApplicationCommandManagerToWatch(&getCommandManager());
    }
    
    juce::StringArray KiwiApp::MainMenuModel::getMenuBarNames()
    {
        return KiwiApp::use().getMenuNames();
    }
    
    juce::PopupMenu KiwiApp::MainMenuModel::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)
    {
        juce::PopupMenu menu;
        KiwiApp::use().createMenu(menu, menuName);
        return menu;
    }
    
    void KiwiApp::MainMenuModel::menuItemSelected(int menuItemID, int topLevelMenuIndex)
    {
        KiwiApp::use().handleMainMenuCommand(menuItemID);
    }
    
    // ================================================================================ //
    //                               ASYNC QUIT RETRIER                                 //
    // ================================================================================ //
    
    class KiwiApp::AsyncQuitRetrier : private juce::Timer
    {
    public:
        AsyncQuitRetrier() { startTimer (500); }
        
        void timerCallback()
        {
            stopTimer();
            delete this;
            
            if (JUCEApplicationBase* app = JUCEApplicationBase::getInstance())
            {
                app->systemRequestedQuit();
            }
        }
        
        JUCE_DECLARE_NON_COPYABLE(AsyncQuitRetrier)
    };
    
    // ================================================================================ //
    //                                  JUCEApplication                                 //
    // ================================================================================ //
    
    void KiwiApp::initialise(juce::String const& commandLine)
    {
        model::DataModel::init();
        
        juce::Desktop::getInstance().setGlobalScaleFactor(1.);
        
        m_look = std::make_unique<LookAndFeel>();
        
        juce::LookAndFeel::setDefaultLookAndFeel(m_look.get());
        juce::LookAndFeel::getDefaultLookAndFeel().setUsingNativeAlertWindows(true);
        
        m_command_manager = std::make_unique<juce::ApplicationCommandManager>();
        
        m_settings = std::make_unique<StoredSettings>();
        m_instance = std::make_unique<Instance>();
        
        m_command_manager->registerAllCommandsForTarget(this);
        
        m_menu_model.reset(new MainMenuModel());
        
        #if JUCE_MAC
        juce::PopupMenu macMainMenuPopup;
        macMainMenuPopup.addCommandItem(&getCommandManager(), CommandIDs::showAboutAppWindow);
        macMainMenuPopup.addSeparator();
        macMainMenuPopup.addCommandItem(&getCommandManager(), CommandIDs::showAudioStatusWindow);
        juce::MenuBarModel::setMacMainMenu(m_menu_model.get(), &macMainMenuPopup, TRANS("Open Recent"));
        #endif
    }
    
    void KiwiApp::anotherInstanceStarted(juce::String const& command_line)
    {
        if(m_instance)
        {
            m_instance->openFile(juce::File(command_line.unquoted()));
        }
    }
    
    void KiwiApp::shutdown()
    {
        #if JUCE_MAC
        juce::MenuBarModel::setMacMainMenu(nullptr);
        #endif
        
        m_settings.reset();
    }
    
    void KiwiApp::systemRequestedQuit()
    {
        if(juce::ModalComponentManager::getInstance()->cancelAllModalComponents())
        {
            new AsyncQuitRetrier();
        }
        else
        {
            if(m_instance->closeAllWindows())
            {
                m_instance.reset();
                
                quit();
            }
        }
    }
    
    const juce::String KiwiApp::getApplicationName()
    {
        return ProjectInfo::projectName;
    }
    
    const juce::String KiwiApp::getApplicationVersion()
    {
        return ProjectInfo::versionString;
    }
    
    bool KiwiApp::moreThanOneInstanceAllowed()
    {
        return false;
    }
    
    // ================================================================================ //
    //                                    STATIC QUERY                                  //
    // ================================================================================ //
    
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
    
    engine::Instance& KiwiApp::useEngineInstance()
    {
        return KiwiApp::use().m_instance->useEngineInstance();
    }
    
    Instance& KiwiApp::useInstance()
    {
        return *KiwiApp::use().m_instance.get();
    }
    
    uint64_t KiwiApp::userID()
    {
        return KiwiApp::use().m_instance->getUserId();
    }
    
    StoredSettings& KiwiApp::useSettings()
    {
        return *KiwiApp::use().m_settings;
    }
    
    juce::MenuBarModel* KiwiApp::getMenuBarModel()
    {
        return KiwiApp::use().m_menu_model.get();
    }
    
    // ================================================================================ //
    //                                      CONSOLE                                     //
    // ================================================================================ //
    
    void KiwiApp::log(std::string const& text)
    {
        useEngineInstance().log(text);
    }
    
    void KiwiApp::post(std::string const& text)
    {
        useEngineInstance().post(text);
    }
    
    void KiwiApp::warning(std::string const& text)
    {
        useEngineInstance().warning(text);
    }
    
    void KiwiApp::error(std::string const& text)
    {
        useEngineInstance().error(text);
    }
    
    void KiwiApp::closeWindow(AppWindow& window)
    {
        if(m_instance)
        {
            m_instance->closeWindow(window);
        }
    }
    
    // ================================================================================ //
    //                                APPLICATION COMMAND                               //
    // ================================================================================ //
    
    void KiwiApp::bindToCommandManager(ApplicationCommandTarget* target)
    {
        KiwiApp& app = KiwiApp::use();
        if(app.m_command_manager)
        {
            app.m_command_manager->registerAllCommandsForTarget(target);
        }
    }
    
    void KiwiApp::bindToKeyMapping(juce::Component* target)
    {
        KiwiApp& app = KiwiApp::use();
        if(app.m_command_manager)
        {
            target->addKeyListener(app.m_command_manager->getKeyMappings());
        }
    }
    
    juce::ApplicationCommandManager& KiwiApp::getCommandManager()
    {
        juce::ApplicationCommandManager* cm = KiwiApp::use().m_command_manager.get();
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
    
    juce::KeyPressMappingSet* KiwiApp::getKeyMappings()
    {
        KiwiApp* const app = KiwiApp::getApp();
        if(app && app->m_command_manager)
        {
            return app->m_command_manager->getKeyMappings();
        }
        
        return nullptr;
    }
    
    // ================================================================================ //
    //                                  APPLICATION MENU                                //
    // ================================================================================ //
    
    juce::StringArray KiwiApp::getMenuNames()
    {
        const char* const names[] =
        {
            "File", "Edit", "View", "Options", "Window", "Help", nullptr
        };
        
        return juce::StringArray(names);
    }
    
    void KiwiApp::createMenu(juce::PopupMenu& menu, const juce::String& menuName)
    {
        if		(menuName == "File")        createFileMenu		(menu);
        else if (menuName == "Edit")        createEditMenu		(menu);
        else if (menuName == "View")        createViewMenu		(menu);
        else if (menuName == "Options")     createOptionsMenu	(menu);
        else if (menuName == "Window")      createWindowMenu	(menu);
        else if (menuName == "Help")		createHelpMenu		(menu);
        
        else assert(false); // names have changed?
    }
    
    void KiwiApp::createOpenRecentPatchersMenu(juce::PopupMenu& menu)
    {
        
    }
    
    void KiwiApp::createFileMenu(juce::PopupMenu& menu)
    {
        menu.addCommandItem(m_command_manager.get(), CommandIDs::newPatcher);
        menu.addSeparator();
        
        menu.addCommandItem(m_command_manager.get(), CommandIDs::openFile);
        createOpenRecentPatchersMenu(menu);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::closeWindow);
        menu.addSeparator();
        
        menu.addCommandItem(m_command_manager.get(), CommandIDs::save);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::saveAs);
        menu.addSeparator();
    }
    
    void KiwiApp::createEditMenu(juce::PopupMenu& menu)
    {
        menu.addCommandItem(m_command_manager.get(), juce::StandardApplicationCommandIDs::undo);
        menu.addCommandItem(m_command_manager.get(), juce::StandardApplicationCommandIDs::redo);
        menu.addSeparator();
        menu.addCommandItem(m_command_manager.get(), juce::StandardApplicationCommandIDs::cut);
        menu.addCommandItem(m_command_manager.get(), juce::StandardApplicationCommandIDs::copy);
        menu.addCommandItem(m_command_manager.get(), juce::StandardApplicationCommandIDs::paste);
        menu.addCommandItem(m_command_manager.get(), juce::StandardApplicationCommandIDs::del);
        menu.addSeparator();
        menu.addCommandItem(m_command_manager.get(), CommandIDs::pasteReplace);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::duplicate);
        menu.addCommandItem(m_command_manager.get(), juce::StandardApplicationCommandIDs::selectAll);
        menu.addCommandItem(m_command_manager.get(), juce::StandardApplicationCommandIDs::deselectAll);
        menu.addSeparator();
    }
    
    void KiwiApp::createViewMenu(juce::PopupMenu& menu)
    {
        menu.addCommandItem(m_command_manager.get(), CommandIDs::newPatcherView);
        menu.addSeparator();
        menu.addCommandItem(m_command_manager.get(), CommandIDs::editModeSwitch);
        menu.addSeparator();
        menu.addCommandItem(m_command_manager.get(), CommandIDs::zoomIn);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::zoomOut);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::zoomNormal);
    }
    
    void KiwiApp::createOptionsMenu(juce::PopupMenu& menu)
    {
        menu.addCommandItem(m_command_manager.get(), CommandIDs::startDsp);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::stopDsp);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::showAudioStatusWindow);
    }
    
    void KiwiApp::createWindowMenu(juce::PopupMenu& menu)
    {
        menu.addCommandItem(m_command_manager.get(), CommandIDs::minimizeWindow);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::maximizeWindow);
        menu.addSeparator();
        
        menu.addCommandItem(m_command_manager.get(), CommandIDs::showConsoleWindow);
        menu.addSeparator();
        
        menu.addCommandItem(m_command_manager.get(), CommandIDs::showDocumentBrowserWindow);
        menu.addCommandItem(m_command_manager.get(), CommandIDs::showBeaconDispatcherWindow);
    }
    
    void KiwiApp::createHelpMenu(juce::PopupMenu& menu)
    {
        ;
    }
    
    void KiwiApp::handleMainMenuCommand(int menuItemID)
    {
        ;
    }
    
    //==============================================================================
    
    void KiwiApp::getAllCommands(juce::Array<juce::CommandID>& commands)
    {
        juce::JUCEApplication::getAllCommands(commands); // get the standard quit command
        
        const juce::CommandID ids[] =
        {
            CommandIDs::newPatcher,
            CommandIDs::openFile,
            CommandIDs::showConsoleWindow,
            CommandIDs::showAudioStatusWindow,
            CommandIDs::showDocumentBrowserWindow,
            CommandIDs::showBeaconDispatcherWindow,
            CommandIDs::startDsp,
            CommandIDs::stopDsp
        };
        
        commands.addArray(ids, juce::numElementsInArray(ids));
    }
    
    void KiwiApp::getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result)
    {
        switch(commandID)
        {
            case CommandIDs::newPatcher:
            {
                result.setInfo(TRANS("New Patcher..."), TRANS("Create a new Patcher"),
                               CommandCategories::general, 0);
                
                result.addDefaultKeypress('n', juce::ModifierKeys::commandModifier);
                break;
            }
            case CommandIDs::openFile:
            {
                result.setInfo(TRANS("Open..."), TRANS("Open a Patcher File"),
                               CommandCategories::general, 0);
                
                result.addDefaultKeypress('o', juce::ModifierKeys::commandModifier);
                break;
            }
            case CommandIDs::showConsoleWindow:
            {
                result.setInfo(TRANS("Console"), TRANS("Show Kiwi Console Window"),
                               CommandCategories::windows, 0);
                
                result.addDefaultKeypress('k', juce::ModifierKeys::commandModifier);
                break;
            }
            case CommandIDs::showAudioStatusWindow:
            {
                result.setInfo(TRANS("Audio Settings"), TRANS("Show kiwi settings"),
                               CommandCategories::windows, 0);
                
                break;
            }
            case CommandIDs::showDocumentBrowserWindow:
            {
                result.setInfo(TRANS("Show Document Browser panel"), TRANS("Show Document Browser panel"),
                               CommandCategories::windows, 0);
                
                break;
            }
            case CommandIDs::showBeaconDispatcherWindow:
            {
                result.setInfo(TRANS("Show Beacon dispatcher window"), TRANS("Show Beacon dispatcher window"),
                               CommandCategories::windows, 0);
                
                break;
            }
            case CommandIDs::startDsp:
            {
                result.setInfo(TRANS("Start dsp"), TRANS("Start dsp"),
                               CommandCategories::general, 0);

                result.setActive(!m_instance->getEngineInstance().getAudioControler().isAudioOn());
                
                break;
            }
            case CommandIDs::stopDsp:
            {
                result.setInfo(TRANS("Stop dsp"), TRANS("Stop dsp"),
                               CommandCategories::general, 0);
                
                result.setActive(m_instance->getEngineInstance().getAudioControler().isAudioOn());
                
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
            case CommandIDs::newPatcher :
            {
                m_instance->newPatcher();
                break;
            }
            case CommandIDs::openFile :
            {
                m_instance->askUserToOpenPatcherDocument();
                break;
            }
            case CommandIDs::showConsoleWindow :
            {
                m_instance->showConsoleWindow();
                break;
            }
            case CommandIDs::showAudioStatusWindow :
            {
                m_instance->showAudioSettingsWindow();
                break;
            }
            case CommandIDs::showDocumentBrowserWindow :
            {
                m_instance->showDocumentBrowserWindow(); break;
            }
            case CommandIDs::showBeaconDispatcherWindow :
            {
                m_instance->showBeaconDispatcherWindow();
                break;
            }
            case CommandIDs::startDsp :
            {
                m_instance->useEngineInstance().getAudioControler().startAudio();
                break;
            }
            case CommandIDs::stopDsp  :
            {
                m_instance->useEngineInstance().getAudioControler().stopAudio();
                break;
            }
            
            default : return JUCEApplication::perform(info);
        }
        
        return true;
    }
}
