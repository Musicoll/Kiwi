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

#include <KiwiModel/KiwiPatcherModel.hpp>
#include <KiwiEngine/KiwiDocumentManager.hpp>
#include <KiwiEngine/KiwiPatcher.hpp>

#include "jInstance.hpp"
#include "jPatcherManager.hpp"
#include "jPatcher.hpp"
#include "jPatcherHelper.hpp"

namespace kiwi
{
    jPatcherManager::jPatcherManager(jInstance& instance) :
    m_instance(instance),
    m_document(nullptr)
    {
        ;
    }
    
    jPatcherManager::jPatcherManager(jInstance& instance, File const& file):
    m_instance(instance),
    m_document(new flip::Document(model::PatcherModel::use(), *this, m_instance.getUserId(), 'cicm', 'kpat'))
    {
        
        model::Patcher& patcher = getPatcher();
        DocumentManager::load(patcher, file);
        
        patcher.createUserIfNotAlreadyThere(m_instance.getUserId());
        DocumentManager::commit(patcher);
    }
    
    jPatcherManager::~jPatcherManager()
    {
        m_document.reset();
    }
    
    model::Patcher& jPatcherManager::init()
    {
        m_document.reset();
        m_document = std::make_unique<flip::Document>(model::PatcherModel::use(), *this,
                                                      m_instance.getUserId(), 'cicm', 'kpat');
        
        model::Patcher& patcher = m_document->root<model::Patcher>();
        patcher.createUserIfNotAlreadyThere(m_instance.getUserId());
        
        DocumentManager::commit(patcher, "Add User");
        
        return patcher;
    }
    
    model::Patcher& jPatcherManager::getPatcher()
    {
        return m_document->root<model::Patcher>();
    }
    
    void jPatcherManager::newView()
    {
        if(m_model)
        {
            auto* user = m_model->getUser(m_instance.getUserId());
            if(user)
            {
                user->addView();
                
                DocumentManager::commit(*m_model, "Add view");
            }
        }
    }
    
    void jPatcherManager::document_changed(model::Patcher& patcher)
    {
        if(patcher.added())
        {
            m_model = &patcher;
            
            patcher.entity().emplace<DocumentManager>(patcher.document());
            
            patcher.entity().emplace<engine::Patcher>();
        }
        
        //! engine
        patcher.entity().use<engine::Patcher>().document_changed(patcher);
        
        notifyPatcherViews(patcher);
        
        if(patcher.removed())
        {
            patcher.entity().erase<engine::Patcher>();
            
            patcher.entity().erase<DocumentManager>();
            
            m_model = nullptr;
        }
    }
    
    void jPatcherManager::notifyPatcherViews(model::Patcher& patcher)
    {
        for(auto& user : patcher.getUsers())
        {
            if(user.getId() == m_instance.getUserId())
            {
                for(auto& view : user.getViews())
                {
                    if(view.added())
                    {
                        createPatcherWindow(patcher, user, view);
                    }
                    
                    notifyPatcherView(patcher, user, view);
                    
                    if(view.removed())
                    {
                        removePatcherWindow(patcher, user, view);
                    }
                }
            }
            else
            {
                // handle external users.
                
                Console::post("New user !!");
            }
        }
    }

    void jPatcherManager::createPatcherWindow(model::Patcher& patcher,
                                              model::Patcher::User& user,
                                              model::Patcher::View& view)
    {
        if(user.getId() == m_instance.getUserId())
        {
            auto& window = view.entity().emplace<jWindow>();
            auto& jpatcher = view.entity().emplace<jPatcher>(m_instance, patcher, view);
            window.setContentNonOwned(&jpatcher.getViewport(), true);
        }
    }

    void jPatcherManager::notifyPatcherView(model::Patcher& patcher,
                                            model::Patcher::User& user,
                                            model::Patcher::View& view)
    {
        if(user.getId() == m_instance.getUserId())
        {
            // Notify jPatcher
            auto& jpatcher = view.entity().use<jPatcher>();
            jpatcher.patcherChanged(patcher, view);
        }
    }

    void jPatcherManager::removePatcherWindow(model::Patcher& patcher,
                                              model::Patcher::User& user,
                                              model::Patcher::View& view)
    {
        if(user.getId() == m_instance.getUserId())
        {
            view.entity().erase<jPatcher>();
            view.entity().erase<jWindow>();
        }
    }
}
