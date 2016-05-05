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
    jInstance::jInstance() : m_instance(new engine::Instance(123456789ULL, "Main"))
    {
        model::Model::init("v0.0.1");
    }
    
    jInstance::~jInstance()
    {
        m_document.reset();
    }
    
    void jInstance::newPatcher()
    {
        m_document = std::make_unique<flip::Document> (model::Model::use(), *this, m_instance->getUserId(), 'cicm', 'kpat');
        
        model::Patcher& patcher = m_document->root<model::Patcher>();
        auto& plus = patcher.addPlus();
        plus.setPosition(100, 100);
        auto& print = patcher.addPrint();
        print.setPosition(300, 300);
        
        patcher.addLink(plus, 0, print, 0);
        
        m_document->commit();
        m_document->push();
    }
    
    void jInstance::document_changed(model::Patcher& patcher)
    {
        std::cout << "jInstance::document_changed" << '\n';
        if(patcher.added())
        {
            patcher.entity().emplace<engine::DocumentManager>(patcher.document());
            
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
            
            patcher.entity().erase<engine::DocumentManager>();
        }
    }
}
