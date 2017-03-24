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
    m_console_window(new ConsoleWindow(m_console_history)),
    m_app_settings_window(new SettingsPanelWindow()),
    m_document_browser_window(new DocumentBrowserWindow(m_browser)),
    m_beacon_dispatcher_window(new BeaconDispatcherWindow(m_instance)),
    m_audio_setting_window(new AudioSettingWindow(dynamic_cast<DspDeviceManager&>(m_instance.getAudioControler()))),
    m_last_opened_file(juce::File::getSpecialLocation(juce::File::userHomeDirectory))
    {
        std::srand(std::time(0));
        m_user_id = std::rand();
    }
    
    Instance::~Instance()
    {
        m_console_window.reset();
        m_app_settings_window.reset();
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
                                                         new PatcherManager(*this, file));
            
            PatcherManager& manager = *(manager_it->get());
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
    
    bool Instance::closeWindow(Window& window)
    {
        bool success = true;
        
        PatcherViewWindow* pwin = dynamic_cast<PatcherViewWindow*>(&window);
        if(pwin && !m_patcher_managers.empty())
        {
            PatcherManager& manager = pwin->getManager();
            
            auto manager_it = getPatcherManager(manager);
            
            if(manager_it != m_patcher_managers.end())
            {
                PatcherView& patcherview = pwin->getPatcherView();
                
                success = manager.closePatcherViewWindow(patcherview);
                if(success && manager.getNumberOfView() == 0)
                {
                    m_patcher_managers.erase(manager_it);
                }
            }
        }
        
        return success;
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
    
    PatcherManager* Instance::openRemotePatcher(std::string const& host,
                                                uint16_t port,
                                                uint64_t session_id)
    {
        std::unique_ptr<PatcherManager> manager_uptr = nullptr;
        
        try
        {
            manager_uptr.reset(new PatcherManager(*this, host, port, session_id));
        }
        catch(std::runtime_error &e)
        {
            KiwiApp::error(e.what());
        }
        
        if(manager_uptr)
        {
            auto manager_it = m_patcher_managers.emplace(m_patcher_managers.end(), std::move(manager_uptr));
            PatcherManager& manager = *(manager_it->get());
            
            if(manager.getNumberOfView() == 0)
            {
                manager.newView();
            }
            
            return manager_it->get();
        }
        
        return nullptr;
    }
    
    Instance::PatcherManagers::iterator Instance::getPatcherManager(PatcherManager const& manager)
    {
        const auto find_it = [&manager](std::unique_ptr<PatcherManager> const& manager_uptr)
        {
            return &manager == manager_uptr.get();
        };
        
        return std::find_if(m_patcher_managers.begin(), m_patcher_managers.end(), find_it);
    }
    
    void Instance::showConsoleWindow()
    {
        m_console_window->setVisible(true);
        m_console_window->toFront(true);
    }
    
    void Instance::showDocumentBrowserWindow()
    {
        m_document_browser_window->setVisible(true);
        m_document_browser_window->toFront(true);
    }
    
    void Instance::showBeaconDispatcherWindow()
    {
        m_beacon_dispatcher_window->setVisible(true);
        m_beacon_dispatcher_window->toFront(true);
    }
    
    void Instance::showAppSettingsWindow()
    {
        m_app_settings_window->setVisible(true);
        m_app_settings_window->toFront(true);
    }
    
    void Instance::showAudioSettingsWindow()
    {
        m_audio_setting_window->addToDesktop();
        m_audio_setting_window->setVisible(true);
        m_audio_setting_window->toFront(true);
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
