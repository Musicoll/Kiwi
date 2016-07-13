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


#include "KiwiApp.hpp"
#include "KiwiApp_Instance.hpp"
#include "KiwiDocumentManager.hpp"
#include "jPatcher.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //

    size_t Instance::m_untitled_patcher_index(0);
    
    Instance::Instance() :
    m_user_id(flip::Ref::User::Offline),
    m_instance(new engine::Instance()),
    m_console_history(std::make_shared<ConsoleHistory>(*m_instance)),
    m_console_window(new jConsoleWindow(m_console_history)),
    m_document_explorer(new DocumentExplorer()),
    m_document_explorer_window(new DocumentExplorerWindow(*m_document_explorer, *this)),
    m_beacon_dispatcher_window(new BeaconDispatcherWindow(*m_instance)),
    m_last_opened_file(juce::File::getSpecialLocation(juce::File::userHomeDirectory))
    {
        ;
    }
    
    Instance::~Instance()
    {
        m_console_window.reset();
        m_patcher_managers.clear();
    }
    
    uint64_t Instance::getUserId() const noexcept
    {
        return m_user_id;
    }
    
    void Instance::setUserId(uint64_t user_id)
    {
        m_user_id = user_id;
    }
    
    engine::Instance& Instance::useEngineInstance()
    {
        return *m_instance;
    }
    
    engine::Instance const& Instance::getEngineInstance() const
    {
        return *m_instance;
    }
    
    void Instance::newPatcher()
    {
        auto manager_it = m_patcher_managers.emplace(m_patcher_managers.end(), new jPatcherManager(*this));
        
        jPatcherManager& manager = *(manager_it->get());
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
                                                         new jPatcherManager(*this, file));
            
            jPatcherManager& manager = *(manager_it->get());
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
    
    bool Instance::closeWindow(jWindow& window)
    {
        bool success = true;
        
        jPatcherWindow* pwin = dynamic_cast<jPatcherWindow*>(&window);
        if(pwin && !m_patcher_managers.empty())
        {
            jPatcherManager& manager = pwin->getManager();
            
            const auto manager_it = getPatcherManager(manager);
            if(manager_it != m_patcher_managers.end())
            {
                jPatcher& jpatcher = pwin->getjPatcher();
                
                success = manager.closePatcherViewWindow(jpatcher);
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
    
    void Instance::openRemotePatcher(std::string& host, uint16_t& port)
    {
        std::unique_ptr<jPatcherManager> manager_uptr = nullptr;
        
        try
        {
            manager_uptr.reset(new jPatcherManager(*this, host, port));
        }
        catch(std::runtime_error &e)
        {
            KiwiApp::error(e.what());
        }
        
        if(manager_uptr)
        {
            auto manager_it = m_patcher_managers.emplace(m_patcher_managers.end(), std::move(manager_uptr));
            jPatcherManager& manager = *(manager_it->get());
            
            if(manager.getNumberOfView() == 0)
            {
                manager.newView();
            }
        }
    }
    
    // ================================================================================ //
    //                                 Settings Component                               //
    // ================================================================================ //
    
    class jSettings final : public juce::Component
    {
    public:
        jSettings(uint64_t user_id)
        {
            setSize(300, 100);
            setVisible(true);
            
            addAndMakeVisible(m_user_id);
            m_user_id.setBoundsRelative((1./5), (5./12), (3./5), (3./12));
            m_user_id.setText(juce::String(user_id));
        }
      
        ~jSettings() = default;
        
        uint64_t getUserId() const
        {
            return m_user_id.getText().getFloatValue();
        }
        
    private:
        juce::TextEditor m_user_id;
        
    private:
        jSettings(jSettings const & other) = delete;
        jSettings(jSettings && other) = delete;
        jSettings& operator=(jSettings const& other) = delete;
        jSettings& operator=(jSettings && other) = delete;
    };
    
    void Instance::openSettings()
    {
        jSettings set_cmp(getUserId());
        juce::OptionalScopedPointer<juce::Component> settings_component(&set_cmp, false);
        
        juce::DialogWindow::LaunchOptions option;
        option.dialogTitle = juce::String("Settings");
        option.content = settings_component;
        option.resizable = false;
        
        option.runModal();
        
        setUserId(set_cmp.getUserId());
    }
    
    Instance::jPatcherManagers::const_iterator Instance::getPatcherManager(jPatcherManager const& manager) const
    {
        const auto find_it = [&manager](std::unique_ptr<jPatcherManager> const& manager_uptr)
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
    
    void Instance::showDocumentExplorerWindow()
    {
        m_document_explorer_window->setVisible(true);
        m_document_explorer_window->toFront(true);
    }
    
    void Instance::showBeaconDispatcherWindow()
    {
        m_beacon_dispatcher_window->setVisible(true);
        m_beacon_dispatcher_window->toFront(true);
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
