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
        m_patcher_manager.reset();
    }
    
    uint64_t jInstance::getUserId() const noexcept
    {
        return m_user_id;
    }
    
    void jInstance::newPatcher()
    {
        m_patcher_manager.reset(new jPatcherManager(*this));
        
        model::Patcher& patcher = m_patcher_manager->init();
        
        patcher.setName("Untitled");
        
        m_patcher_manager->newView();
        DocumentManager::commit(patcher, "pre-populate patcher");
    }
    
    void jInstance::openFile(kiwi::File const& file)
    {
        if(file.isKiwiFile())
        {
            m_patcher_manager.reset(new jPatcherManager(*this, file));
            if(m_patcher_manager->getNumberOfView() == 0)
            {
                m_patcher_manager->newView();
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
            File open_file (openFileChooser.getResult().getFullPathName().toStdString());
            
            openFile(open_file);
        }
    }
    
    bool jInstance::closeAllWindows()
    {
        bool success = true;
        
        if(m_patcher_manager)
        {
            if(!m_patcher_manager->askAllWindowsToClose())
            {
                success = false;
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
    
    void jInstance::populatePatcher(model::Patcher& patcher)
    {
        {
            // simple print
            auto& plus = patcher.addObject("plus 44");
            plus.setPosition(50, 50);
            auto& print = patcher.addObject("print");
            print.setPosition(50, 100);
            patcher.addLink(plus, 0, print, 0);
        }
        
        {
            // set rhs value
            auto& plus_1 = patcher.addObject("plus 1");
            plus_1.setPosition(150, 50);
            
            auto& plus_2 = patcher.addObject("plus 10");
            plus_2.setPosition(220, 50);
            
            auto& plus_3 = patcher.addObject("plus");
            plus_3.setPosition(150, 100);
            
            auto& print = patcher.addObject("print");
            print.setPosition(150, 150);
            
            patcher.addLink(plus_1, 0, plus_3, 0);
            patcher.addLink(plus_2, 0, plus_3, 1);
            patcher.addLink(plus_3, 0, print, 0);
        }
        
        {
            // basic counter
            auto& plus_1 = patcher.addObject("plus");
            plus_1.setPosition(350, 100);
            
            auto& plus_2 = patcher.addObject("plus");
            plus_2.setPosition(405, 70);
            
            auto& plus_3 = patcher.addObject("plus 10");
            plus_3.setPosition(300, 20);
            
            auto& plus_4 = patcher.addObject("plus -10");
            plus_4.setPosition(380, 20);
            
            auto& print = patcher.addObject("print zozo");
            print.setPosition(350, 150);
            
            patcher.addLink(plus_1, 0, plus_2, 0);
            patcher.addLink(plus_2, 0, plus_1, 1);
            patcher.addLink(plus_1, 0, print, 0);
            
            patcher.addLink(plus_3, 0, plus_1, 0);
            patcher.addLink(plus_4, 0, plus_1, 0);
        }
        
        {
            // stack overflow
            auto& plus_1 = patcher.addObject("plus");
            plus_1.setPosition(550, 100);
            
            auto& plus_2 = patcher.addObject("plus");
            plus_2.setPosition(605, 70);
            
            auto& plus_3 = patcher.addObject("plus 10");
            plus_3.setPosition(500, 20);
            
            auto& plus_4 = patcher.addObject("plus -10");
            plus_4.setPosition(580, 20);
            
            auto& print = patcher.addObject("print zozo");
            print.setPosition(550, 150);
            
            patcher.addLink(plus_1, 0, plus_2, 0);
            patcher.addLink(plus_2, 0, plus_1, 0);
            patcher.addLink(plus_1, 0, print, 0);
            
            patcher.addLink(plus_3, 0, plus_1, 0);
            patcher.addLink(plus_4, 0, plus_1, 0);
        }
        
        DocumentManager::commit(patcher, "pre-populate patcher");
    }
}
