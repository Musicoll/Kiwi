/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Objects.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Objects.h>

#include <KiwiApp_Patcher/KiwiApp_Factory.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_Objects.h>
#include <KiwiApp.h>
#include <KiwiApp_General/KiwiApp_CommandIDs.h>
#include <KiwiApp_General/KiwiApp_IDs.h>

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
    
    //==============================================================================
    enum
    {
        objectHelpBaseID = 100,
        examplesBaseID = 300
    };
    
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
        #if ! JUCE_MAC
        if(sendCommandLineToPreexistingInstance())
        {
            DBG ("Another instance is running - quitting...");
            quit();
            return;
        }
        #endif
   
        model::DataModel::init();
        
        declareEngineObjects();
        
        declareObjectViews();
        
        initResources();
        
        juce::Desktop::getInstance().setGlobalScaleFactor(1.);
        
        juce::LookAndFeel::setDefaultLookAndFeel(&m_looknfeel);
        
        m_command_manager = std::make_unique<juce::ApplicationCommandManager>();
        
        m_settings = std::make_unique<StoredSettings>();
        
        m_settings->network().addListener(*this);
        
        m_menu_model.reset(new MainMenuModel());
        
        m_scheduler.reset(new tool::Scheduler<>());
        
        m_api_controller.reset(new ApiController());
        m_api.reset(new Api(*m_api_controller));
        
        m_instance = std::make_unique<Instance>();
        m_command_manager->registerAllCommandsForTarget(this);

        #if JUCE_WINDOWS
        openCommandFile(commandLine);
        #endif
        
        #if JUCE_MAC
        juce::PopupMenu macMainMenuPopup;
        macMainMenuPopup.addCommandItem(&getCommandManager(), CommandIDs::showAboutAppWindow);
        macMainMenuPopup.addSeparator();
        macMainMenuPopup.addCommandItem(&getCommandManager(), CommandIDs::showAppSettingsWindow);
        juce::MenuBarModel::setMacMainMenu(m_menu_model.get(), &macMainMenuPopup, TRANS("Open Recent"));
        #endif
        
        pingServer();
        startTimer(TimerIds::MainScheduler, 10); // main scheduler update
        startTimer(TimerIds::ServerPing, 3000); // Server ping
    }
    
    void KiwiApp::anotherInstanceStarted(juce::String const& command_line)
    {
        openCommandFile(command_line);
    }
    
    void KiwiApp::declareEngineObjects()
    {
        engine::NewBox::declare();
        engine::ErrorBox::declare();
        engine::Slider::declare();
        engine::Print::declare();
        engine::Receive::declare();
        engine::Plus::declare();
        engine::Times::declare();
        engine::Delay::declare();
        engine::Metro::declare();
        engine::Pipe::declare();
        engine::Bang::declare();
        engine::Toggle::declare();
        engine::AdcTilde::declare();
        engine::DacTilde::declare();
        engine::OscTilde::declare();
        engine::Loadmess::declare();
        engine::SigTilde::declare();
        engine::TimesTilde::declare();
        engine::PlusTilde::declare();
        engine::MeterTilde::declare();
        engine::DelaySimpleTilde::declare();
        engine::Message::declare();
        engine::NoiseTilde::declare();
        engine::PhasorTilde::declare();
        engine::SahTilde::declare();
        engine::SnapshotTilde::declare();
        engine::Trigger::declare();
        engine::LineTilde::declare();
        engine::Minus::declare();
        engine::Divide::declare();
        engine::Equal::declare();
        engine::Less::declare();
        engine::Greater::declare();
        engine::Different::declare();
        engine::Pow::declare();
        engine::Modulo::declare();
        engine::MinusTilde::declare();
        engine::DivideTilde::declare();
        engine::LessTilde::declare();
        engine::GreaterTilde::declare();
        engine::EqualTilde::declare();
        engine::DifferentTilde::declare();
        engine::LessEqual::declare();
        engine::LessEqualTilde::declare();
        engine::GreaterEqual::declare();
        engine::GreaterEqualTilde::declare();
        engine::Comment::declare();
        engine::Pack::declare();
        engine::Unpack::declare();
        engine::Random::declare();
        engine::Scale::declare();
        engine::Select::declare();
        engine::Number::declare();
        engine::NumberTilde::declare();
        engine::Hub::declare();
        engine::Mtof::declare();
        engine::Send::declare();
        engine::Gate::declare();
        engine::Switch::declare();
        engine::GateTilde::declare();
        engine::SwitchTilde::declare();
        engine::Float::declare();
        engine::ClipTilde::declare();
        engine::Clip::declare();
        engine::SfPlayTilde::declare();
        engine::SfRecordTilde::declare();
        engine::FaustTilde::declare();
        engine::Route::declare();
        engine::OSCReceive::declare();
        engine::OSCSend::declare();
    }
    
    void KiwiApp::declareObjectViews()
    {
        SliderView::declare();
        BangView::declare();
        ToggleView::declare();
        MeterTildeView::declare();
        MessageView::declare();
        CommentView::declare();
        NumberView::declare();
        NumberTildeView::declare();
    }
    
    void KiwiApp::shutdown()
    {
        #if JUCE_MAC
        juce::MenuBarModel::setMacMainMenu(nullptr);
        #endif
        
        stopTimer(TimerIds::MainScheduler);
        stopTimer(TimerIds::ServerPing);
        
        m_instance.reset();
        m_api->cancelAll();
        m_api.reset();
        m_api_controller.reset();
        m_scheduler.reset();
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
            if(m_instance->closeAllPatcherWindows())
            {
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
        return true;
    }
    
    void KiwiApp::timerCallback(int timer_id)
    {
        if(timer_id == TimerIds::MainScheduler)
        {
            m_instance->tick();
            m_scheduler->process();
        }
        else if(timer_id == TimerIds::ServerPing)
        {
            // ping server
            pingServer();
        }
    }
    
    bool KiwiApp::isMacOSX()
    {
        return (juce::SystemStats::getOperatingSystemType()
                & juce::SystemStats::MacOSX) != 0;
    }
    
    bool KiwiApp::isLinux()
    {
        return juce::SystemStats::getOperatingSystemType() == juce::SystemStats::Linux;
    }
    
    bool KiwiApp::isWindows()
    {
        return (juce::SystemStats::getOperatingSystemType()
                & juce::SystemStats::Windows) != 0;
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
        return *KiwiApp::use().m_instance;
    }
    
    Api& KiwiApp::useApi()
    {
        return *KiwiApp::use().m_api;
    }
    
    tool::Scheduler<>& KiwiApp::useScheduler()
    {
        return *KiwiApp::use().m_scheduler;
    }
    
    juce::File KiwiApp::getKiwiResourcesDirectory()
    {
        using juce::File;
        const auto apppath = File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile);
        
#if JUCE_MAC
        return apppath.getChildFile("Contents/Resources");
#else
        return apppath.getParentDirectory();
#endif
    }
    
    juce::File KiwiApp::getKiwiObjectHelpDirectory()
    {
        static const auto helps = getKiwiResourcesDirectory().getChildFile("helps");
        return helps;
    }
    
    juce::File KiwiApp::getKiwiExamplesDirectory()
    {
        static const auto examples = getKiwiResourcesDirectory().getChildFile("examples");
        return examples;
    }
    
    void KiwiApp::initResources()
    {
        // initialise help file aliases, ex:
        // helpfile > < operators
        // will associate classname "<" and ">" to helpfile "operators.kiwihelp"
        
        static const auto help_aliases_filename = "help-alias.txt";
        m_help_aliases.clear();
        
        const auto help_dir = KiwiApp::use().getKiwiObjectHelpDirectory();
        const auto help_aliases_file = help_dir.getChildFile(help_aliases_filename);
        if(help_aliases_file.existsAsFile())
        {
            static const auto help_extension = ".kiwihelp";
            
            juce::StringArray lines;
            help_aliases_file.readLines(lines);
            for (auto& line : lines)
            {
                auto tokens = juce::StringArray::fromTokens(line, " ", "\"");
                if(tokens.size() >= 3 && tokens[0] == "helpfile")
                {
                    auto const& file_token = tokens.getReference(tokens.size() - 1);
                    auto help_file = help_dir.getChildFile(file_token.unquoted() + help_extension);
                    if(help_file.existsAsFile())
                    {
                        for(int i = 1; i < tokens.size() - 1; ++i)
                        {
                            m_help_aliases.emplace(tokens[i].toStdString(), help_file);
                        }
                    }
                }
            }
        }
    }
    
    juce::File KiwiApp::findHelpFile(std::string const& classname) const
    {
        static const auto help_extension = ".kiwihelp";
        const auto help_dir = KiwiApp::use().getKiwiObjectHelpDirectory();
        
        auto help_file = help_dir.getChildFile(classname + help_extension);
        if(help_file.existsAsFile())
        {
            return help_file;
        }
        
        // look for aliases help file name
        
        const auto alias = m_help_aliases.find(classname);
        if(alias != m_help_aliases.cend())
        {
            help_file = alias->second;
            if(help_file.existsAsFile())
            {
                return help_file;
            }
        }
        
        return {};
    }
    
    void KiwiApp::setAuthUser(Api::AuthUser const& auth_user)
    {
        (*KiwiApp::use().m_api_controller).setAuthUser(auth_user);
        KiwiApp::useInstance().login();
    }
    
    Api::AuthUser const& KiwiApp::getCurrentUser()
    {
        return KiwiApp::use().m_api_controller->getAuthUser();
    }
    
    static juce::String getGlobalDirKey(KiwiApp::FileLocations location)
    {
        switch (location)
        {
            case KiwiApp::FileLocations::Save: { return "last_save_dir"; }
            case KiwiApp::FileLocations::Open: { return "last_open_dir"; }
            case KiwiApp::FileLocations::Download: { return "last_download_dir"; }
            case KiwiApp::FileLocations::Upload: { return "last_upload_dir"; }
                
            default: return {};
        }
    }
    
    juce::File KiwiApp::getGlobalDirectoryFor(FileLocations location)
    {
        const juce::String dir_key = getGlobalDirKey(location);
        
        if(dir_key.isNotEmpty())
        {
            auto global_dir = juce::File(getGlobalProperties().getValue(dir_key, ""));
            
            if(global_dir.exists() && global_dir.isDirectory())
            {
                return global_dir;
            }
        }
        
        return juce::File::getSpecialLocation(juce::File::userHomeDirectory);
    }
    
    void KiwiApp::setGlobalDirectoryFor(FileLocations location, juce::File path)
    {
        assert(path.isDirectory());
        
        const juce::String dir_key = getGlobalDirKey(location);
        
        if(dir_key.isNotEmpty())
        {
            getGlobalProperties().setValue(dir_key, path.getFullPathName());
        }
    }
    
    void KiwiApp::logout()
    {
        useInstance().handleConnectionLost();
        KiwiApp::use().m_api_controller->logout();
        KiwiApp::commandStatusChanged();
    }
    
    bool KiwiApp::canConnectToServer()
    {
        const auto server_version = KiwiApp::use().m_last_server_version_check;
        
        return (!server_version.empty()
                && server_version != "null"
                && KiwiApp::use().canConnectToServerVersion(server_version));
    }
    
    bool KiwiApp::canConnectToServerVersion(std::string const& server_version)
    {
        const auto& version = KiwiApp::use().getApplicationVersion();
        return (version.compare(server_version) == 0);
    }
    
    void KiwiApp::pingSucceed(std::string const& new_server_version)
    {
        bool was_connected = canConnectToServer();
        
        if((new_server_version == m_last_server_version_check) && was_connected)
            return;
        
        if (was_connected)
        {
            logout();
        }
        
        if (canConnectToServerVersion(new_server_version))
        {
            useInstance().login();
        }
        else if(m_last_server_version_check != new_server_version)
        {
            const auto current_version = KiwiApp::use().getApplicationVersion().toStdString();
            warning("Can't connect to server! Requires Kiwi " + new_server_version
                    + ", please visit:");
            warning("https://github.com/Musicoll/Kiwi/releases");
        }
        
        m_last_server_version_check = new_server_version;
        
        if(!was_connected && canConnectToServer())
        {
            const auto api_host = m_api_controller->getHost();
            post("Connection established to " + api_host);
        }
    }
    
    void KiwiApp::pingFailed(Api::Error error)
    {
        static const std::string ping_failed = "null";
        
        const bool was_connected = canConnectToServer();
        if(was_connected)
        {
            KiwiApp::error("Connection lost");
            m_instance->handleConnectionLost();
        }
        else if(m_last_server_version_check != ping_failed)
        {
            KiwiApp::error("Connection to server failed: " + error.getMessage());
        }
        
        m_last_server_version_check = ping_failed;
    }
    
    void KiwiApp::pingServer()
    {
        Api::CallbackFn<std::string const&> success = [this](std::string const& server_version) {
            
            KiwiApp::useScheduler().schedule([this, server_version]() {
                pingSucceed(server_version);
            });
            
        };
        
        useApi().getRelease(success, [this](Api::Error err) {
            
            KiwiApp::useScheduler().schedule([this, err = std::move(err)]() {
                pingFailed(err);
            });
            
        });
    }
    
    void KiwiApp::networkSettingsChanged(NetworkSettings const& settings, juce::Identifier const& id)
    {
        if (id == Ids::server_address)
        {
            auto& api = *m_api_controller;
            
            bool ping_required = false;
            
            if (api.getHost() != settings.getHost())
            {
                m_api_controller->setHost(settings.getHost());
                ping_required = true;
            }
            
            if (api.getPort() != settings.getApiPort())
            {
                m_api_controller->setPort(settings.getApiPort());
                ping_required = true;
            }
            
            if (api.verifyServer() != settings.getVerifyCertificate())
            {
                m_api_controller->setVerifyServer(settings.getVerifyCertificate());
                ping_required = true;
            }
            
            if (ping_required)
            {
                pingServer();
            }
        }
    }

    void KiwiApp::openCommandFile(juce::String const& command_line)
    {
        if (command_line.length() > 0)
        {
            juce::File file(command_line.unquoted());

            if (file.hasFileExtension(".kiwi;.kiwihelp") && m_instance)
            {
                m_instance->openFile(file);
            }
        }
    }
    
    uint64_t KiwiApp::userID()
    {
        // refactor this (maybe a useless method)
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
    
    LookAndFeel& KiwiApp::useLookAndFeel()
    {
        return KiwiApp::use().m_looknfeel;
    }
    
    TooltipWindow& KiwiApp::useTooltipWindow()
    {
        return KiwiApp::use().m_tooltip_window;
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
    
    void KiwiApp::closeWindow(Window& window)
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
            "Account", "File", "Edit", "View", "Options", "Window", "Help", nullptr
        };
        
        return juce::StringArray(names);
    }
    
    void KiwiApp::createMenu(juce::PopupMenu& menu, const juce::String& menuName)
    {
        if      (menuName == "Account") createAccountMenu   (menu);
        else if (menuName == "File")    createFileMenu      (menu);
        else if (menuName == "Edit")    createEditMenu      (menu);
        else if (menuName == "View")    createViewMenu      (menu);
        else if (menuName == "Options") createOptionsMenu   (menu);
        else if (menuName == "Window")  createWindowMenu    (menu);
        else if (menuName == "Help")    createHelpMenu      (menu);
        
        else assert(false); // names have changed?
    }
    
    void KiwiApp::createOpenRecentPatchersMenu(juce::PopupMenu& menu)
    {
        
    }
    
    void KiwiApp::createAccountMenu(juce::PopupMenu& menu)
    {
        menu.addCommandItem(&getCommandManager(), CommandIDs::remember_me);
        menu.addSeparator();
        menu.addCommandItem(&getCommandManager(), CommandIDs::login);
        menu.addCommandItem(&getCommandManager(), CommandIDs::signup);
        menu.addSeparator();
        menu.addCommandItem(&getCommandManager(), CommandIDs::logout);
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
        
        #if ! JUCE_MAC
        menu.addSeparator();
        menu.addCommandItem(m_command_manager.get(), juce::StandardApplicationCommandIDs::quit);
        #endif
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
        
        menu.addSeparator();
        menu.addCommandItem(m_command_manager.get(), CommandIDs::showAudioStatusWindow);
        
        #if ! JUCE_MAC
        menu.addCommandItem(&getCommandManager(), CommandIDs::showAppSettingsWindow);
        #endif
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
        #if ! JUCE_MAC
        menu.addCommandItem(m_command_manager.get(), CommandIDs::showAboutAppWindow);
        menu.addSeparator();
        #endif
        
        menu.addCommandItem(m_command_manager.get(), CommandIDs::openObjectHelp);
        
        {
            juce::PopupMenu objects_submenu;
            createObjectHelpPopupMenu(objects_submenu);
            if(m_num_help_files > 0)
            {
                menu.addSubMenu("Objects", objects_submenu);
            }
        }
        
        {
            juce::PopupMenu examples_submenu;
            createExamplesPopupMenu(examples_submenu);
            if(m_num_example_files > 0)
            {
                menu.addSubMenu("Examples", examples_submenu);
            }
        }
        
        menu.addSeparator();
        menu.addCommandItem(m_command_manager.get(), CommandIDs::showDocumentation);
    }
    
    void KiwiApp::handleMainMenuCommand(int menu_item_ID)
    {
        if (menu_item_ID >= objectHelpBaseID && menu_item_ID < (objectHelpBaseID + m_num_help_files))
        {
            findAndOpenHelpFile(menu_item_ID - objectHelpBaseID);
        }
        else if (menu_item_ID >= examplesBaseID && menu_item_ID < (examplesBaseID + m_num_help_files))
        {
            findAndOpenExample(menu_item_ID - examplesBaseID);
        }
    }
    
    void KiwiApp::createObjectHelpPopupMenu(juce::PopupMenu& menu)
    {
        m_num_help_files = 0;
        const auto help_dir = getKiwiObjectHelpDirectory();
        for (auto const& f : getSortedObjectHelpFilesInDirectory(help_dir))
        {
            menu.addItem(objectHelpBaseID + m_num_help_files, f.getFileNameWithoutExtension());
            ++m_num_help_files;
        }
    }
    
    void KiwiApp::findAndOpenHelpFile(int selected_index)
    {
        const auto help_files = getSortedObjectHelpFilesInDirectory(getKiwiObjectHelpDirectory());
        
        if (selected_index < help_files.size())
        {
            useInstance().openFile(help_files.getUnchecked(selected_index));
        }
    }
    
    juce::Array<juce::File> KiwiApp::getSortedObjectHelpFilesInDirectory(juce::File const& dir) const noexcept
    {
        juce::Array<juce::File> help_files;
        
        juce::DirectoryIterator iter (dir, false, "*.kiwihelp",
                                      juce::File::findFiles
                                      | juce::File::ignoreHiddenFiles);
        while(iter.next())
        {
            help_files.add (iter.getFile());
        }
        
        help_files.sort();
        
        return help_files;
    }
    
    void KiwiApp::createExamplesPopupMenu(juce::PopupMenu& menu) noexcept
    {
        m_num_example_files = 0;
        for (auto& dir : getSortedExampleDirectories())
        {
            juce::PopupMenu m;
            for (auto& f : getSortedExampleFilesInDirectory (dir))
            {
                m.addItem (examplesBaseID + m_num_example_files, f.getFileNameWithoutExtension());
                ++m_num_example_files;
            }
            
            menu.addSubMenu (dir.getFileName(), m);
        }
    }
    
    juce::Array<juce::File> KiwiApp::getSortedExampleDirectories() noexcept
    {
        juce::Array<juce::File> example_directories;
        
        const auto examples_dir = getKiwiExamplesDirectory();
        
        if (! examples_dir.exists() || ! examples_dir.isDirectory() || ! examples_dir.containsSubDirectories())
            return {};
        
        juce::DirectoryIterator iter (examples_dir, false, "*", juce::File::findDirectories);
        while (iter.next())
        {
            auto example_directory = iter.getFile();
            
            if (example_directory.getNumberOfChildFiles (juce::File::findFiles
                                                         | juce::File::ignoreHiddenFiles) > 0)
            {
                example_directories.add (example_directory);
            }
        }
        
        example_directories.sort();
        
        return example_directories;
    }
    
    juce::Array<juce::File> KiwiApp::getSortedExampleFilesInDirectory(juce::File const& directory) const noexcept
    {
        juce::Array<juce::File> example_files;
        
        juce::DirectoryIterator iter (directory, false, "*.kiwi", juce::File::findFiles);
        while (iter.next())
            example_files.add(iter.getFile());
        
        example_files.sort();
        
        return example_files;
    }
    
    void KiwiApp::findAndOpenExample(int selected_index)
    {
        juce::File example;
        
        for (auto& dir : getSortedExampleDirectories())
        {
            auto example_files = getSortedExampleFilesInDirectory(dir);
            
            if (selected_index < example_files.size())
            {
                example = example_files.getUnchecked(selected_index);
                break;
            }
            
            selected_index -= example_files.size();
        }
        
        // example doesn't exist?
        assert(example != juce::File());
        
        useInstance().openFile(example);
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
            CommandIDs::showAboutAppWindow,
            CommandIDs::showDocumentation,
            CommandIDs::showAudioStatusWindow,
            CommandIDs::showAppSettingsWindow,
            CommandIDs::showDocumentBrowserWindow,
            CommandIDs::showBeaconDispatcherWindow,
            CommandIDs::switchDsp,
            CommandIDs::startDsp,
            CommandIDs::stopDsp,
            CommandIDs::login,
            CommandIDs::signup,
            CommandIDs::logout,
            CommandIDs::remember_me,
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
            case CommandIDs::login:
            {
                auto const& user = getCurrentUser();
                const bool logged = user.isLoggedIn();
                result.setInfo(logged
                               ? juce::String(TRANS("Logged-in as ") + user.getName())
                               : TRANS("Login"),
                               TRANS("Show the \"Login form\" Window"),
                               CommandCategories::windows, 0);
                
                result.setActive(!logged);
                break;
            }
            case CommandIDs::signup:
            {
                result.setInfo(TRANS("Register"), TRANS("Show the \"Register form\" Window"),
                               CommandCategories::windows, 0);
                
                result.setActive(!getCurrentUser().isLoggedIn());
                break;
            }
            case CommandIDs::logout:
            {
                result.setInfo(TRANS("Logout"), TRANS("Log out current user"),
                               CommandCategories::windows, 0);
                
                result.setActive(getCurrentUser().isLoggedIn());
                break;
            }
            case CommandIDs::remember_me:
            {
                result.setInfo(TRANS("Remember me"), TRANS("Remember current user"),
                               CommandCategories::windows, 0);
                
                auto const& user = getCurrentUser();
                
                result.setActive(user.isLoggedIn());
                result.setTicked(getAppSettings().network().getRememberUserFlag());
                break;
            }
            case CommandIDs::showAboutAppWindow:
            {
                result.setInfo(TRANS("About Kiwi"), TRANS("Show the \"About Kiwi\" Window"),
                               CommandCategories::windows, 0);
                break;
            }
            case CommandIDs::showDocumentation:
            {
                result.setInfo(TRANS("Documentation"), TRANS("Show the documentation"),
                               CommandCategories::windows, 0);
                break;
            }
            case CommandIDs::showAppSettingsWindow:
            {
                result.setInfo(TRANS("Preferences..."), TRANS("Show kiwi application settings"),
                               CommandCategories::windows, 0);
                
                result.addDefaultKeypress(',', juce::ModifierKeys::commandModifier);
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
            case CommandIDs::switchDsp:
            {
                result.setInfo(TRANS("Switch global DSP state"), TRANS("Switch global DSP state"),
                               CommandCategories::general, 0);
                
                result.setTicked(m_instance->useEngineInstance().getAudioControler().isAudioOn());
                
                break;
            }
            case CommandIDs::startDsp:
            {
                result.setInfo(TRANS("Start dsp"), TRANS("Start dsp"),
                               CommandCategories::general, 0);

                result.setActive(!m_instance->useEngineInstance().getAudioControler().isAudioOn());
                
                break;
            }
            case CommandIDs::stopDsp:
            {
                result.setInfo(TRANS("Stop dsp"), TRANS("Stop dsp"),
                               CommandCategories::general, 0);
                
                result.setActive(m_instance->useEngineInstance().getAudioControler().isAudioOn());
                
                break;
            }
            case juce::StandardApplicationCommandIDs::quit:
            {
                result.setInfo(TRANS("Quit Kiwi"), TRANS("Quits the application"),
                               CommandCategories::general, 0);
                
                result.addDefaultKeypress('q', juce::ModifierKeys::commandModifier);
                break;
            }
            default:
            {
                break;
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
            case CommandIDs::login:
            {
                m_instance->showAuthWindow(AuthPanel::FormType::Login);
                break;
            }
            case CommandIDs::signup:
            {
                m_instance->showAuthWindow(AuthPanel::FormType::SignUp);
                break;
            }
            case CommandIDs::logout:
            {
                KiwiApp::logout();
                break;
            }
            case CommandIDs::remember_me:
            {
                auto& settings = getAppSettings().network();
                settings.setRememberUserFlag(!settings.getRememberUserFlag());
                commandStatusChanged();
                break;
            }
            case CommandIDs::showConsoleWindow :
            {
                m_instance->showConsoleWindow();
                break;
            }
            case CommandIDs::showAboutAppWindow :
            {
                m_instance->showAboutKiwiWindow();
                break;
            }
            case CommandIDs::showDocumentation :
            {
                const auto docs_index = getKiwiResourcesDirectory().getChildFile("docs/index.html");
                if(docs_index.existsAsFile())
                {
                    docs_index.startAsProcess();
                }
                else
                {
                    KiwiApp::error("can't open documentation!");
                }
                break;
            }
            case CommandIDs::showAppSettingsWindow :
            {
                m_instance->showAppSettingsWindow();
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
            case CommandIDs::switchDsp :
            {
                auto& audio_controler = m_instance->useEngineInstance().getAudioControler();
                if(audio_controler.isAudioOn())
                {
                    audio_controler.stopAudio();
                }
                else
                {
                    audio_controler.startAudio();
                }
                
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
