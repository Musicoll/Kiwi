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

#include "KiwiApp_Instance.hpp"
#include "KiwiApp_AboutWindow.hpp"

#include "../KiwiApp.hpp"
#include "../KiwiApp_Network/KiwiApp_DocumentManager.hpp"
#include "../KiwiApp_Components/KiwiApp_Window.hpp"
#include "../KiwiApp_Patcher/KiwiApp_PatcherView.hpp"
#include "../KiwiApp_Patcher/KiwiApp_PatcherComponent.hpp"

#include <cstdlib>
#include <ctime>

namespace kiwi
{
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //

    size_t Instance::m_untitled_patcher_index(0);
    
    Instance::Instance() :
    m_instance(std::make_unique<DspDeviceManager>()),
    m_browser(),
    m_console_history(std::make_shared<ConsoleHistory>(m_instance)),
    m_last_opened_file(juce::File::getSpecialLocation(juce::File::userHomeDirectory))
    {
        std::srand(std::time(0));
        m_user_id = std::rand();
        
        // reserve space for singleton windows.
        m_windows.resize(std::size_t(WindowId::count));
        
        //showAppSettingsWindow();
        showBeaconDispatcherWindow();
        showDocumentBrowserWindow();
        showConsoleWindow();
    }
    
    Instance::~Instance()
    {
        closeAllPatcherWindows();
    }
    
    uint64_t Instance::getUserId() const noexcept
    {
        return m_user_id;
    }
    
    engine::Instance& Instance::useEngineInstance()
    {
        return m_instance;
    }
    
    engine::Instance const& Instance::getEngineInstance() const
    {
        return m_instance;
    }
    
    void Instance::newPatcher()
    {
        auto manager_it = m_patcher_managers.emplace(m_patcher_managers.end(), new PatcherManager(*this));
        
        PatcherManager& manager = *(manager_it->get());
        model::Patcher& patcher = manager.getPatcher();
        
        const size_t next_untitled = getNextUntitledNumberAndIncrement();
        std::string patcher_name = "Untitled";
        
        if(next_untitled > 0)
        {
            patcher_name += " " + std::to_string(next_untitled);
        }
        
        patcher.setName(patcher_name);
        
        if(manager.getNumberOfView() == 0)
        {
            manager.newView();
        }
        
        DocumentManager::commit(patcher);
    }
    
    bool Instance::openFile(juce::File const& file)
    {
        if(file.hasFileExtension("kiwi"))
        {
            auto manager_it = m_patcher_managers.emplace(m_patcher_managers.end(),
                                                         new PatcherManager(*this));
            
            PatcherManager& manager = *(manager_it->get());
            
            manager.loadFromFile(file);
            
            if(manager.getNumberOfView() == 0)
            {
                manager.newView();
            }
            
            return true;
        }
        else
        {
            KiwiApp::error("can't open file (bad file extension)");
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
    
    PatcherManager* Instance::openRemotePatcher(DocumentBrowser::Drive::DocumentSession& session)
    {
        auto mng_it = getPatcherManagerForSession(session);
        if(mng_it != m_patcher_managers.end())
        {
            PatcherManager& manager = *(mng_it->get());
            manager.bringsFirstViewToFront();
            return &manager;
        }
        
        auto manager_uptr = std::make_unique<PatcherManager>(*this);
        
        try
        {
            manager_uptr->connect(session);
        }
        catch(std::runtime_error &e)
        {
            KiwiApp::error(e.what());
            return nullptr;
        }
        
        auto manager_it = m_patcher_managers.emplace(m_patcher_managers.end(), std::move(manager_uptr));
        PatcherManager& manager = *(manager_it->get());
        
        if(manager.getNumberOfView() == 0)
        {
            manager.newView();
        }
        
        return manager_it->get();
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
    
    void Instance::showAboutKiwiWindow()
    {
        showWindowWithId(WindowId::AboutKiwi, [](){ return std::make_unique<AboutWindow>(); });
    }
    
    void Instance::showDocumentBrowserWindow()
    {
        showWindowWithId(WindowId::DocumentBrowser, [&browser = m_browser](){
            return std::make_unique<Window>("Document Browser",
                                            std::make_unique<DocumentBrowserView>(browser),
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
                                            true, true, "application_settings_window");
        });
    }
    
    void Instance::showAudioSettingsWindow()
    {
        showWindowWithId(WindowId::AudioSettings, [&instance = m_instance](){
            
            auto& manager = dynamic_cast<DspDeviceManager&>(instance.getAudioControler());
            auto device_selector =
            std::make_unique<juce::AudioDeviceSelectorComponent>(manager,
                                                                 1, 20, 1, 20,
                                                                 false, false, false, true);
            
            device_selector->setSize(300, 300);
            
            return std::make_unique<Window>("Audio Settings",
                                            std::move(device_selector),
                                            false, false, "audio_settings_window");
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
