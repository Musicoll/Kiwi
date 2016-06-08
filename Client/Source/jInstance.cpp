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

namespace kiwi
{
    // ================================================================================ //
    //                                     jINSTANCE                                    //
    // ================================================================================ //

    size_t jInstance::m_untitled_patcher_index(0);
    
    jInstance::jInstance() :
    m_user_id(flip::Ref::User::Offline),
    m_instance(new engine::Instance(m_user_id)),
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
    
    void jInstance::newPatcher()
    {
        auto manager_it = m_patcher_managers.emplace(m_patcher_managers.end(), new jPatcherManager(*this));
        
        jPatcherManager& manager = *(manager_it->get());
        model::Patcher& patcher = manager.init();
        
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
