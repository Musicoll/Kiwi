/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#include <KiwiEngine/KiwiDocumentManager.hpp>
#include <KiwiCore/KiwiFile.hpp>

#include "jInstance.hpp"
#include "jPatcher.hpp"
#include "jGuiDevice.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                     jINSTANCE                                    //
    // ================================================================================ //

    size_t jInstance::m_untitled_patcher_index(0);
    
    jInstance::jInstance() :
    //m_user_id(flip::Ref::User::Offline),
    m_user_id(123456789ULL),
    m_instance(new engine::Instance(m_user_id, std::make_unique<jGuiDevice>())),
    m_console_window(new jConsoleWindow())
    {
        ;
    }
    
    jInstance::~jInstance()
    {
        m_console_window.reset();
        m_patcher_managers.clear();
    }
    
    uint64_t jInstance::getUserId() const noexcept
    {
        return m_user_id;
    }
    
    void jInstance::setUserId(uint64_t user_id)
    {
        m_user_id = user_id;
    }
    
    engine::Instance& jInstance::getEngineInstance()
    {
        return *m_instance;
    }
    
    engine::Instance const& jInstance::getEngineInstance() const
    {
        return *m_instance;
    }
    
    void jInstance::newPatcher()
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
        
        manager.newView();
        DocumentManager::commit(patcher, "pre-populate patcher");
    }
    
    void jInstance::openFile(kiwi::FilePath const& file)
    {
        if(file.isKiwiFile())
        {
            auto manager_it = m_patcher_managers.emplace(m_patcher_managers.end(), new jPatcherManager(*this, file));
            jPatcherManager& manager = *(manager_it->get());
            if(manager.getNumberOfView() == 0)
            {
                manager.newView();
            }
        }
        else
        {
            Console::error("can't open file");
        }
    }
    
    void jInstance::openPatcher()
    {
        juce::FileChooser openFileChooser("Open file",
                                          juce::File::getSpecialLocation (juce::File::userHomeDirectory),
                                          "*.kiwi");
        
        if (openFileChooser.browseForFileToOpen())
        {
            kiwi::FilePath open_file(openFileChooser.getResult().getFullPathName().toStdString());
            
            openFile(open_file);
        }
    }
    
    bool jInstance::closeAllWindows()
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
    
    void jInstance::openRemotePatcher()
    {
        std::string host("");
        uint16_t port(0);
        
        openRemoteDialogBox(host, port);
        
        std::unique_ptr<jPatcherManager> manager_uptr = nullptr;
        
        try
        {
            manager_uptr.reset(new jPatcherManager(*this, host, port));
        }
        catch(std::runtime_error &e)
        {
            Console::error(e.what());
        }
        
        if(manager_uptr)
        {
            auto manager_it = m_patcher_managers.emplace(m_patcher_managers.end(), std::move(manager_uptr));
            jPatcherManager& manager = *(manager_it->get());
            manager.newView();
        }
    }
    
    // ================================================================================ //
    //                                   Remote Settings                                //
    // ================================================================================ //
    
    class jRemoteSettings final : public juce::Component
    {
    public:
        jRemoteSettings();
        
        std::string getHost();
        uint16_t getPort();
        
        ~jRemoteSettings() = default;
        
    private:
        juce::TextEditor m_hostEditor;
        juce::TextEditor m_portEditor;
        
    private:
        jRemoteSettings(jRemoteSettings const& other) = delete;
        jRemoteSettings& operator=(jRemoteSettings const& other) = delete;
        jRemoteSettings(jRemoteSettings && other) = delete;
        jRemoteSettings& operator=(jRemoteSettings && other) = delete;
    };
    
    jRemoteSettings::jRemoteSettings()
    {
        setSize(300, 100);
        setVisible(true);
        
        addAndMakeVisible(m_hostEditor);
        addAndMakeVisible(m_portEditor);
        
        m_hostEditor.setBoundsRelative((1./5), (3./12), (3./5), (3./12));
        m_portEditor.setBoundsRelative((1./5), (7./12), (3./5), (3./12));
    }
    
    std::string jRemoteSettings::getHost()
    {
        return m_hostEditor.getText().toStdString();
    }
    
    uint16_t jRemoteSettings::getPort()
    {
        return m_portEditor.getText().getFloatValue();
    }
    
    void jInstance::openRemoteDialogBox(std::string & host, uint16_t & port)
    {
        jRemoteSettings remote_set_cmp;
        juce::OptionalScopedPointer<Component> remote_component(&remote_set_cmp, false);
        
        juce::DialogWindow::LaunchOptions option;
        option.dialogTitle = juce::String("Remote settings");
        option.content = remote_component;
        option.resizable = false;
        
        option.runModal();
        
        host = remote_set_cmp.getHost();
        port = remote_set_cmp.getPort();
    }
    
    // ================================================================================ //
    //                                 Settings Component                               //
    // ================================================================================ //
    
    class jSettings final : public juce::Component
    {
    public:
        jSettings(uint64_t client_id);
        
        uint64_t getClientId();
        
        ~jSettings() = default;
        
    private:
        juce::TextEditor m_client_id;
        
    private:
        jSettings(jSettings const & other) = delete;
        jSettings(jSettings && other) = delete;
        jSettings& operator=(jSettings const& other) = delete;
        jSettings& operator=(jSettings && other) = delete;
    };
    
    jSettings::jSettings(uint64_t client_id)
    {
        setSize(300, 100);
        setVisible(true);
        
        addAndMakeVisible(m_client_id);
        m_client_id.setBoundsRelative((1./5), (5./12), (3./5), (3./12));
        m_client_id.setText(juce::String(client_id));
    }
    
    uint64_t jSettings::getClientId()
    {
        return m_client_id.getText().getFloatValue();
    }
    
    void jInstance::openSettings()
    {
        jSettings set_cmp(getUserId());
        juce::OptionalScopedPointer<Component> settings_component(&set_cmp, false);
        
        juce::DialogWindow::LaunchOptions option;
        option.dialogTitle = juce::String("Settings");
        option.content = settings_component;
        option.resizable = false;
        
        option.runModal();
        
        setUserId(set_cmp.getClientId());
    }
    
    void jInstance::showConsoleWindow()
    {
        m_console_window->setVisible(true);
        m_console_window->toFront(true);
    }
    
    std::vector<uint8_t>& jInstance::getPatcherClipboardData()
    {
        return m_patcher_clipboard;
    }
    
    size_t jInstance::getNextUntitledNumberAndIncrement()
    {
        return m_untitled_patcher_index++;
    }
}
