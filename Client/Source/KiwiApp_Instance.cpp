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

#include <juce_audio_utils/juce_audio_utils.h>

#include "KiwiApp.hpp"
#include "KiwiApp_Instance.hpp"
#include "KiwiApp_PatcherView.hpp"

#include "KiwiApp_Network/KiwiApp_DocumentManager.hpp"

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
        
        m_windows.emplace(std::make_unique<ConsoleWindow>(m_console_history));
        m_windows.emplace(std::make_unique<DocumentBrowserWindow>(m_browser));
        m_windows.emplace(std::make_unique<BeaconDispatcherWindow>(m_instance));
        m_windows.emplace(std::make_unique<SettingsPanelWindow>());
    }
    
    Instance::~Instance()
    {
        closeAllWindows();
        m_patcher_managers.clear();
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

    void Instance::removePatcherWindow(PatcherViewWindow & patcher_window)
    {
        if (!m_patcher_managers.empty())
        {
            PatcherManager& manager = patcher_window.getPatcherManager();

            auto manager_it = getPatcherManager(manager);

            if (manager_it != m_patcher_managers.end())
            {
                PatcherView& patcherview = patcher_window.getPatcherView();

                 manager.closePatcherViewWindow(patcherview);

                if ( manager.getNumberOfView() == 0)
                {
                    m_patcher_managers.erase(manager_it);
                }
            }
        }
    }
    
    void Instance::closeWindow(AppWindow& window)
    {
        struct IsEqual
        {
            IsEqual(AppWindow const& window):m_window(window){}
            
            bool operator()(std::unique_ptr<AppWindow> const& window){return window.get() == &m_window;};
            
            AppWindow const& m_window;
        };
        
        std::set<std::unique_ptr<AppWindow>>::iterator found_window =
                std::find_if(m_windows.begin(), m_windows.end(), IsEqual(window));
        
        m_windows.erase(found_window);
        
        #if ! JUCE_MAC
        struct IsMainWindow
        {
            bool operator()(std::unique_ptr<AppWindow> const& app_window){return app_window->isMainWindow();};
        };
        
        size_t main_windows = std::count_if(m_windows.begin(), m_windows.end(), IsMainWindow());
        
        if (main_windows == 0)
        {
            KiwiApp::use().systemRequestedQuit();
        }
        #endif
    }
    
    bool Instance::closeAllWindows()
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
        struct IsEqual
        {
            IsEqual(PatcherManager const& manager):m_patcher_manager(manager){};
            
            bool operator()(std::unique_ptr<PatcherManager> const& patcher){return patcher.get() == &m_patcher_manager;}
            
            PatcherManager const& m_patcher_manager;
        };
        
        PatcherManagers::iterator found_patcher =
            std::find_if(m_patcher_managers.begin(), m_patcher_managers.end(), IsEqual(patcher_manager));
        
        if (found_patcher != m_patcher_managers.end())
        {
            m_patcher_managers.erase(found_patcher);
        }
        
    }
    
    Instance::PatcherManagers::iterator Instance::getPatcherManager(PatcherManager const& manager)
    {
        const auto find_it = [&manager](std::unique_ptr<PatcherManager> const& manager_uptr)
        {
            return &manager == manager_uptr.get();
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
        m_windows.emplace(std::make_unique<ConsoleWindow>(m_console_history));;
    }
    
    void Instance::showDocumentBrowserWindow()
    {
        m_windows.emplace(std::make_unique<DocumentBrowserWindow>(m_browser));
    }
    
    void Instance::showBeaconDispatcherWindow()
    {
        m_windows.emplace(std::make_unique<BeaconDispatcherWindow>(m_instance));
    }
    
    void Instance::showAppSettingsWindow()
    {
        m_windows.emplace(std::make_unique<SettingsPanelWindow>());
    }
    
    void Instance::showAudioSettingsWindow()
    {
        m_windows.emplace(std::make_unique<AudioSettingWindow>(dynamic_cast<DspDeviceManager&>(m_instance.getAudioControler())));
    }
    
    std::vector<uint8_t>& Instance::getPatcherClipboardData()
    {
        return m_patcher_clipboard;
    }
    
    size_t Instance::getNextUntitledNumberAndIncrement()
    {
        return m_untitled_patcher_index++;
    }
}
