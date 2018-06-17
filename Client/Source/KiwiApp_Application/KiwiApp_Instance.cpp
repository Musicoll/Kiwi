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

#include <juce_audio_utils/juce_audio_utils.h>

#include <KiwiModel/KiwiModel_DocumentManager.h>

#include "KiwiApp_Instance.h"
#include "KiwiApp_AboutWindow.h"

#include "../KiwiApp.h"
#include "../KiwiApp_Components/KiwiApp_Window.h"
#include "../KiwiApp_Patcher/KiwiApp_PatcherView.h"
#include "../KiwiApp_Patcher/KiwiApp_PatcherComponent.h"

#include "../KiwiApp_General/KiwiApp_CommandIDs.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //

    size_t Instance::m_untitled_patcher_index(1);
    
    Instance::Instance()
    : m_instance(std::make_unique<DspDeviceManager>(), KiwiApp::useScheduler())
    , m_browser("Offline", 1000)
    , m_console_history(std::make_shared<ConsoleHistory>(m_instance))
    , m_last_opened_file(juce::File::getSpecialLocation(juce::File::userHomeDirectory))
    {
        // reserve space for singleton windows.
        m_windows.resize(std::size_t(WindowId::count));
        
        showDocumentBrowserWindow();
        showConsoleWindow();
    }
    
    Instance::~Instance()
    {
        forceCloseAllPatcherWindows();
    }
    
    uint64_t Instance::getUserId() const noexcept
    {
        const auto& user = KiwiApp::getCurrentUser();
        return user.isLoggedIn() ? user.getIdAsInt() : flip::Ref::User::Offline;
    }
    
    void Instance::login()
    {
        if(getUserId() > flip::Ref::User::Offline)
        {
            m_browser.setDriveName(KiwiApp::getCurrentUser().getName());
            m_windows[std::size_t(WindowId::DocumentBrowser)]->getContentComponent()->setEnabled(true);
        }
        else
        {
            showAuthWindow(AuthPanel::FormType::Login);
        }
    }
    
    void Instance::handleConnectionLost()
    {
        m_browser.setDriveName("Offline");
        m_windows[std::size_t(WindowId::DocumentBrowser)]->getContentComponent()->setEnabled(false);
    }
    
    void Instance::tick()
    {
        static bool is_pulling = false;
        if(!is_pulling)
        {
            is_pulling = true;
            pullRemoteDocuments();
            is_pulling = false;
        }
    }
    
    bool Instance::askUserToContinueEditingDocumentOffline(PatcherManager& manager,
                                                           std::string const& reason) const
    {
        auto& first_window = manager.getFirstWindow();
        auto message = std::string("Do you want to continue editing document \"");
        message += manager.getDocumentName() + "\" offline ?";
        
        return first_window.showOkCancelBox(juce::AlertWindow::QuestionIcon,
                                            reason, message, "Yes", "No");
    }
    
    void Instance::pullRemoteDocuments()
    {
        const bool user_logged_in = KiwiApp::getCurrentUser().isLoggedIn();
        const bool is_connected_to_api = (KiwiApp::canConnectToServer() && user_logged_in);
        
        auto manager_it = m_patcher_managers.begin();
        while(manager_it != m_patcher_managers.end())
        {
            auto& manager = *(*manager_it);
            
            if (manager.isConnected())
            {
                manager.pull(); // This is here we pull the flip document.
                
                const bool is_still_connected = manager.isConnected();
                const bool connection_lost = !is_still_connected;
                
                if(connection_lost
                   || (is_still_connected
                       && (!is_connected_to_api || !user_logged_in)))
                {
                    auto reason = user_logged_in ? "Connection lost" : "You are logged out";
                    bool keep_edit = askUserToContinueEditingDocumentOffline(manager, reason);
                    
                    if(!keep_edit)
                    {
                        // the user wants to close the document
                        manager.forceCloseAllWindows();
                        manager_it = m_patcher_managers.erase(manager_it);
                        continue;
                    }
                    else if(!is_connected_to_api)
                    {
                        manager.disconnect();
                    }
                }
            }
            
            ++manager_it;
        }
    }
    
    engine::Instance& Instance::useEngineInstance()
    {
        return m_instance;
    }
    
    engine::Instance const& Instance::useEngineInstance() const
    {
        return m_instance;
    }
    
    void Instance::newPatcher()
    {
        std::string patcher_name = "Untitled "
                                   + std::to_string(getNextUntitledNumberAndIncrement());
        
        PatcherManager & manager = (*m_patcher_managers.emplace(m_patcher_managers.end(),
                                                                new PatcherManager(*this, patcher_name))->get());
        
        if(manager.getNumberOfView() == 0)
        {
            manager.newView();
        }
        
        model::DocumentManager::commit(manager.getPatcher());
    }
    
    bool Instance::openFile(juce::File const& file)
    {
        if(!file.hasFileExtension("kiwi"))
        {
            KiwiApp::error("can't open file (bad file extension)");
            return false;
        }
        
        {
            auto manager_it = getPatcherManagerForFile(file);
            
            // If a patcher already manages this file, just brings it to front
            if (manager_it != m_patcher_managers.end())
            {
                (*manager_it)->bringsFirstViewToFront();
                return true;
            }
        }
        
        // there is no manager for this file so lets create one

        std::unique_ptr<PatcherManager> temp_manager = nullptr;

        try
        {
            temp_manager = PatcherManager::createFromFile(*this, file);
        }
        catch (std::runtime_error const& err)
        {
            const std::string error = err.what();
            const std::string filename = file.getFileName().toStdString();
            KiwiApp::error("Can't open document \"" + filename + "\"");
            KiwiApp::error("error: " + error);
            KiwiApp::error("Please download latest Kiwi version.");
            return false;
        }
        
        if(temp_manager)
        {
            auto manager_it = m_patcher_managers.emplace(m_patcher_managers.end(),
                                                         std::move(temp_manager));
            auto& manager = *(manager_it->get());
            
            if(manager.getNumberOfView() == 0)
            {
                manager.newView();
            }
            
            return true;
        }
        
        return false;
    }
    
    void Instance::askUserToOpenPatcherDocument()
    {
        juce::FileChooser file_chooser("Open file", m_last_opened_file, "*.kiwi");
        
        if(file_chooser.browseForFileToOpen())
        {
            juce::File selected_file = file_chooser.getResult();
            const bool success = openFile(selected_file);
            
            if(success)
            {
                selected_file.setAsCurrentWorkingDirectory();
                m_last_opened_file = selected_file;
            }
        }
    }

    void Instance::removePatcherWindow(PatcherViewWindow& patcher_window)
    {
        if (!m_patcher_managers.empty())
        {
            PatcherManager& manager = patcher_window.getPatcherManager();

            auto manager_it = getPatcherManager(manager);

            if (manager_it != m_patcher_managers.end())
            {
                PatcherView& patcherview = patcher_window.getPatcherView();

                manager.closePatcherViewWindow(patcherview);

                if(manager.getNumberOfView() == 0)
                {
                    m_patcher_managers.erase(manager_it);
                }
            }
        }
    }
    
    void Instance::closeWindow(Window& window)
    {
        auto is_equal_fn = [&window](std::unique_ptr<Window> const& w)
        {
            return (w.get() == &window);
        };

        auto found_window = std::find_if(m_windows.begin(), m_windows.end(), is_equal_fn);
        
        if(found_window != m_windows.end())
        {
            // if it's a regular window, simply reset the ptr
            if(found_window < m_windows.begin() + std::size_t(WindowId::count))
            {
                found_window->reset();
            }
            else
            {
                m_windows.erase(found_window);
            }
        }
        
        #if ! JUCE_MAC
        auto is_main_window_fn = [](std::unique_ptr<Window> const& window)
        {
            return window && window->isMainWindow();
        };
        
        size_t main_windows = std::count_if(m_windows.begin(), m_windows.end(), is_main_window_fn);
        
        if (main_windows == 0)
        {
            KiwiApp::use().systemRequestedQuit();
        }
        #endif
    }
    
    void Instance::closeWindowWithId(WindowId window_id)
    {
        auto& window_uptr = m_windows[std::size_t(window_id)];
        if(!window_uptr)
        {
            closeWindow(*window_uptr);
        }
    }
    
    bool Instance::closeAllPatcherWindows()
    {
        bool success = true;
        
        if(!m_patcher_managers.empty())
        {
            for(auto& manager_uptr : m_patcher_managers)
            {
                if(!manager_uptr->askAllWindowsToClose())
                {
                    success = false;
                    break;
                }
            }
        }
        
        return success;
    }
    
    void Instance::forceCloseAllPatcherWindows()
    {
        for(auto& manager : m_patcher_managers)
        {
            manager->forceCloseAllWindows();
        }
        
        m_patcher_managers.clear();
    }
    
    void Instance::openRemotePatcher(DocumentBrowser::Drive::DocumentSession& session)
    {
        auto mng_it = getPatcherManagerForSession(session);
        
        if(mng_it != m_patcher_managers.end())
        {
            PatcherManager& manager = *(mng_it->get());
            manager.bringsFirstViewToFront();
        }
        else
        {
            auto manager_uptr = std::make_unique<PatcherManager>(*this, session.getName());
            
            NetworkSettings& network_settings = getAppSettings().network();
            
            if (manager_uptr->connect(network_settings.getHost(), network_settings.getSessionPort(), session))
            {
                auto manager_it = m_patcher_managers.emplace(m_patcher_managers.end(), std::move(manager_uptr));
                PatcherManager& manager = *(manager_it->get());
                
                if(manager.getNumberOfView() == 0)
                {
                    manager.newView();
                }
            }
            else
            {
                KiwiApp::error("Failed to connect to the document [" + session.getName() + "]");
            }
        }
    }
    
    Instance::PatcherManagers::iterator Instance::getPatcherManager(PatcherManager const& manager)
    {
        const auto find_fn = [&manager](std::unique_ptr<PatcherManager> const& other)
        {
            return &manager == other.get();
        };
        
        return std::find_if(m_patcher_managers.begin(), m_patcher_managers.end(), find_fn);
    }
    
    Instance::PatcherManagers::iterator Instance::getPatcherManagerForFile(juce::File const& file)
    {
        const auto find_it = [&file](std::unique_ptr<PatcherManager> const& manager_uptr)
        {
            return (!manager_uptr->isConnected() && file == manager_uptr->getSelectedFile());
        };
        
        return std::find_if(m_patcher_managers.begin(), m_patcher_managers.end(), find_it);
    }
    
    Instance::PatcherManagers::iterator Instance::getPatcherManagerForSession(DocumentBrowser::Drive::DocumentSession& session)
    {
        const auto find_it = [session_id = session.getSessionId()](std::unique_ptr<PatcherManager> const& manager_uptr)
        {
            return (manager_uptr->isConnected()
                    && session_id != 0
                    && session_id == manager_uptr->getSessionId());
            
            return false;
        };
        
        return std::find_if(m_patcher_managers.begin(), m_patcher_managers.end(), find_it);
    }
    
    void Instance::showConsoleWindow()
    {
        showWindowWithId(WindowId::Console, [&history = m_console_history](){
            return std::make_unique<Window>("Kiwi Console",
                                            std::make_unique<Console>(history),
                                            true, true, "console_window",
                                            !KiwiApp::isMacOSX());
        });
    }
    
    void Instance::showAuthWindow(AuthPanel::FormType type)
    {
        showWindowWithId(WindowId::FormComponent, [type]() {
            
            auto window = std::make_unique<Window>("Kiwi",
                                                   std::make_unique<AuthPanel>(type),
                                                   false, false);
            window->centreWithSize(window->getWidth(), window->getHeight());
            window->enterModalState(true);
            
            return window;
        });
    }
    
    void Instance::showAboutKiwiWindow()
    {
        showWindowWithId(WindowId::AboutKiwi, [](){ return std::make_unique<AboutWindow>(); });
    }
    
    void Instance::showDocumentBrowserWindow()
    {
        showWindowWithId(WindowId::DocumentBrowser, [&browser = m_browser](){
            
            const bool can_connect = (KiwiApp::canConnectToServer()
                                      && KiwiApp::getCurrentUser().isLoggedIn());
            
            return std::make_unique<Window>("Document Browser",
                                            std::make_unique<DocumentBrowserView>(browser, can_connect),
                                            true, false, "document_browser_window");
        });
    }
    
    void Instance::showBeaconDispatcherWindow()
    {
        showWindowWithId(WindowId::BeaconDispatcher, [&instance = m_instance](){
            return std::make_unique<Window>("Beacon dispatcher",
                                            std::make_unique<BeaconDispatcher>(instance),
                                            false, true, "beacon_dispatcher_window");
        });
    }
    
    void Instance::showAppSettingsWindow()
    {
        showWindowWithId(WindowId::ApplicationSettings, [](){
            return std::make_unique<Window>("Application settings",
                                            std::make_unique<SettingsPanel>(),
                                            false, true, "application_settings_window");
        });
    }
    
    void Instance::showAudioSettingsWindow()
    {
        showWindowWithId(WindowId::AudioSettings, [&instance = m_instance](){
            
            auto& manager = dynamic_cast<DspDeviceManager&>(instance.getAudioControler());
            auto device_selector =
            std::make_unique<juce::AudioDeviceSelectorComponent>(manager,
                                                                 1, 20, 1, 20,
                                                                 false, false, false, false);
            
            device_selector->setSize(300, 300);
            
            return std::make_unique<Window>("Audio Settings",
                                            std::move(device_selector),
                                            true, false, "audio_settings_window");
        });
    }
    
    std::vector<uint8_t>& Instance::getPatcherClipboardData()
    {
        return m_patcher_clipboard;
    }
    
    size_t Instance::getNextUntitledNumberAndIncrement()
    {
        return m_untitled_patcher_index++;
    }
    
    void Instance::showWindowWithId(WindowId id, std::function<std::unique_ptr<Window>()> create_fn)
    {
        auto& window_uptr = m_windows[std::size_t(id)];
        if(!window_uptr)
        {
            window_uptr = create_fn();
        }
        
        window_uptr->setVisible(true);
        window_uptr->toFront(true);
    }
}
