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

namespace kiwi
{
    jPatcherManager::jPatcherManager(jInstance& instance) :
    m_instance(instance),
    m_document(model::PatcherModel::use(), *this, m_instance.getUserId(), 'cicm', 'kpat')
    {
        model::Patcher & patcher = getPatcher();
        
        patcher.createUserIfNotAlreadyThere(m_instance.getUserId());
        DocumentManager::commit(patcher);
    }
    
    jPatcherManager::jPatcherManager(jInstance & instance, const std::string host, uint16_t port) :
    m_instance(instance),
    m_document(model::PatcherModel::use(), *this, m_instance.getUserId(), 'cicm', 'kpat')
    {
        model::Patcher & patcher = getPatcher();
        
        try
        {
            DocumentManager::connect(patcher, host, port);
        }
        catch (std::runtime_error &e)
        {
            throw e;
        }
        
        patcher.createUserIfNotAlreadyThere(m_instance.getUserId());
        DocumentManager::commit(patcher);
    }
    
    jPatcherManager::~jPatcherManager()
    {
    }
    
    model::Patcher& jPatcherManager::getPatcher()
    {
        return m_document.root<model::Patcher>();
    }
    
    void jPatcherManager::newView()
    {
        auto* user = getPatcher().getUser(m_instance.getUserId());
        if(user)
        {
            user->addView();
            
            DocumentManager::commit(*user);
        }
    }
    
    void jPatcherManager::document_changed(model::Patcher& patcher)
    {
        if (patcher.added())
        {
            patcher.entity().emplace<DocumentManager>(patcher.document(), m_instance.getEngineInstance());
            patcher.entity().emplace<engine::Patcher>();
        }
        //! engine
        patcher.entity().use<engine::Patcher>().document_changed(patcher);
        
        notifyPatcherViews(patcher);
        
        if(patcher.removed())
        {
            patcher.entity().erase<engine::Patcher>();
            patcher.entity().erase<DocumentManager>();
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
            auto& jpatcher = view.entity().emplace<jPatcher>(patcher, view);
            window.setContentNonOwned(&jpatcher, true);
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
