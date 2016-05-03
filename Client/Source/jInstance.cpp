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
        m_documents.clear();
        m_windows.clear();
    }
    
    void jInstance::newPatcherDocument()
    {
        auto doc_it = m_documents.emplace(m_documents.end(),
                                      std::make_unique<flip::Document>
                                      (model::Model::use(), *this, m_instance->getUserId(), 'cicm', 'kpat'));
        
        auto& document = *doc_it->get();
        auto& patcher = document.root<model::Patcher>();
        patcher.addView();
        
        document.commit();
        document.push();
    }
    
    void jInstance::document_changed(model::Patcher& patcher)
    {
        std::cout << "jInstance::document_changed" << '\n';
        if(patcher.added())
        {
            patcher.entity().emplace<engine::DocumentManager>(patcher.document());
            m_instance->document_changed(patcher);
            return;
        }
        
        if(patcher.viewChanged())
        {
            patcherViewChanged(patcher.getFirstView());
        }
        
        m_instance->document_changed(patcher);
        
        if(patcher.removed())
        {
            patcher.entity().erase<engine::DocumentManager>();
        }
    }
    
    void jInstance::patcherViewChanged(model::Patcher::View& patcher_view)
    {
        if(patcher_view.added())
        {
            auto window_it = m_windows.emplace(m_windows.end(), std::make_unique<jWindow>());
            
            auto& window = *window_it->get();
            window.setContentOwned(new jPatcher(patcher_view), true);
        }
        
        if(patcher_view.removed())
        {
            // remove Window
        }
    }
}
