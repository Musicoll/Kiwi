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
    
    Instance::Instance() :
    m_instance(std::make_unique<DspDeviceManager>(), KiwiApp::useScheduler()),
    m_browser(KiwiApp::getCurrentUser().isLoggedIn() ? KiwiApp::getCurrentUser().getName(): "logged out",
              1000),
    m_console_history(std::make_shared<ConsoleHistory>(m_instance)),
    m_last_opened_file(juce::File::getSpecialLocation(juce::File::userHomeDirectory))
    {
        startTimer(10);
        
        // reserve space for singleton windows.
        m_windows.resize(std::size_t(WindowId::count));
        
        //showAppSettingsWindow();
        //showBeaconDispatcherWindow();
        showDocumentBrowserWindow();
        showConsoleWindow();
    }
    
    Instance::~Instance()
    {
        closeAllPatcherWindows();
        stopTimer();
    }
    
    void Instance::timerCallback()
    {
        for(auto manager = m_patcher_managers.begin(); manager != m_patcher_managers.end();)
        {
            bool keep_patcher = true;
            
            if ((*manager)->isRemote())
            {
                (*manager)->pull();
                
                if (!(*manager)->isRemote())
                {
                    keep_patcher
                    = (*manager)->getFirstWindow().showOkCancelBox(juce::AlertWindow::QuestionIcon,
                                                                   "Connetion lost",
                                                                   "Do you want to continue editing document \""
                                                                   + (*manager)->getDocumentName() +"\" offline",
                                                                   "Ok",
                                                                   "Cancel");
                }
            }
            
            if (!keep_patcher)
            {
                manager = m_patcher_managers.erase(manager);
            }
            else
            {
                ++manager;
            }
        }
    }
    
    uint64_t Instance::getUserId() const noexcept
    {
        const auto& user = KiwiApp::getCurrentUser();
        return user.isLoggedIn() ? user.getIdAsInt() : flip::Ref::User::Offline;
    }
    
    void Instance::login()
    {
        m_browser.setDriveName(KiwiApp::getCurrentUser().getName());
        
        m_windows[std::size_t(WindowId::DocumentBrowser)]->getContentComponent()->setEnabled(true);
    }
    
    void Instance::logout()
    {
        m_browser.setDriveName("logged out");
        
        for(auto manager = m_patcher_managers.begin(); manager != m_patcher_managers.end();)
        {
            if ((*manager)->isRemote())
            {
                bool keep_patcher
                = (*manager)->getFirstWindow().showOkCancelBox(juce::AlertWindow::QuestionIcon,
                                                               "User logged out",
                                                               "Do you want to continue editing document \""
                                                               + (*manager)->getDocumentName() +"\" offline",
                                                               "Ok",
                                                               "Cancel");
                
                if (!keep_patcher)
                {
                    manager = m_patcher_managers.erase(manager);
                }
                else
                {
                    (*manager)->disconnect();
                    ++manager;
                }
            }
        }
        
        m_windows[std::size_t(WindowId::DocumentBrowser)]->getContentComponent()->setEnabled(false);
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
        bool open_succeeded = false;
        
        if(file.hasFileExtension("kiwi"))
        {
            auto manager_it = getPatcherManagerForFile(file);
            
            if (manager_it == m_patcher_managers.end())
            {
                std::string patcher_name = file.getFileNameWithoutExtension().toStdString();
                
                std::unique_ptr<PatcherManager> patcher_manager (new PatcherManager(*this, patcher_name));
                
                if (patcher_manager->loadFromFile(file))
                {
                    auto manager_it = m_patcher_managers.emplace(m_patcher_managers.end(),
                                                                 std::move(patcher_manager));
                    
                    open_succeeded = true;
                    
                    if((*manager_it)->getNumberOfView() == 0)
                    {
                        (*manager_it)->newView();
                    }
                }
                else
                {
                    KiwiApp::error("Can't open document. Version is not up to date. Please download latest Kiwi version.");
                }
            }
            else
            {
                (*manager_it)->bringsFirstViewToFront();
            }
        }
        else
        {
            KiwiApp::error("can't open file (bad file extension)");
        }
        
        return open_succeeded;
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
    
    void Instance::removePatcher(PatcherManager const& patcher_manager)
    {
        const auto it = getPatcherManager(patcher_manager);
        
        if (it != m_patcher_managers.end())
        {
            m_patcher_managers.erase(it);
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
            return (!manager_uptr->isRemote() && file == manager_uptr->getSelectedFile());
        };
        
        return std::find_if(m_patcher_managers.begin(), m_patcher_managers.end(), find_it);
    }
    
    Instance::PatcherManagers::iterator Instance::getPatcherManagerForSession(DocumentBrowser::Drive::DocumentSession& session)
    {
        const auto find_it = [session_id = session.getSessionId()](std::unique_ptr<PatcherManager> const& manager_uptr)
        {
            return (manager_uptr->isRemote()
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
            return std::make_unique<Window>("Document Browser",
                                            std::make_unique<DocumentBrowserView>(browser,
                                                                                  KiwiApp::getCurrentUser().isLoggedIn()),
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
