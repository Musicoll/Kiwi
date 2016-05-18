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
        
        populatePatcher();
        DocumentManager::commit(patcher, "load initial objects and links");
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
    
    void jPatcherManager::populatePatcher()
    {
        model::Patcher & patcher = getPatcher();
        
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
    }
    
    void jPatcherManager::document_changed(model::Patcher& patcher)
    {
        if (patcher.added())
        {
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
