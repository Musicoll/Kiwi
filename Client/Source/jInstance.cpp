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

#include "jInstance.hpp"
#include "jPatcher.hpp"

namespace kiwi
{
    jInstance::jInstance() :
    m_instance(new engine::Instance(123456789ULL, "Main")),
    m_console_window(new jConsoleWindow())
    {
        ;
    }
    
    jInstance::~jInstance()
    {
        m_document.reset();
    }
    
    void jInstance::newPatcher()
    {
        m_document.reset();
        m_document = m_instance->createPatcherDocument(*this);
        
        model::Patcher& patcher = m_document->root<model::Patcher>();
        populatePatcher(patcher);
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
        
        engine::DocumentManager::commit(patcher, "load initial objects and links");
    }
    
    void jInstance::document_changed(model::Patcher& patcher)
    {
        if(patcher.added())
        {
            auto& window = patcher.entity().emplace<jWindow>();
            auto& jpatcher = patcher.entity().emplace<jPatcher>();
            window.setContentNonOwned(&jpatcher, true);
        }
        
        // Notify jPatcher
        auto& jpatcher = patcher.entity().use<jPatcher>();
        jpatcher.document_changed(patcher);
        
        // Notify Engine
        m_instance->document_changed(patcher);
        
        if(patcher.removed())
        {
            patcher.entity().erase<jPatcher>();
            patcher.entity().erase<jWindow>();
        }
    }
}
