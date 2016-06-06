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
    jPatcherWindow::jPatcherWindow(jPatcherManager& manager, jPatcher& jpatcher) : jWindow(),
    m_manager(manager),
    m_jpatcher(jpatcher)
    {
        ;
    }
    
    void jPatcherWindow::closeButtonPressed()
    {
        m_manager.closePatcherViewWindow(m_jpatcher);
    }
    
    // ================================================================================ //
    //                                  JPATCHER MANAGER                                //
    // ================================================================================ //
    
    jPatcherManager::jPatcherManager(jInstance& instance) :
    m_instance(instance),
    m_document(nullptr),
    m_need_saving_flag(false)
    {
        ;
    }
    
    jPatcherManager::jPatcherManager(jInstance& instance, File const& file):
    m_instance(instance),
    m_document(new flip::Document(model::PatcherModel::use(), *this, m_instance.getUserId(), 'cicm', 'kpat'))
    {
        model::Patcher& patcher = getPatcher();
        DocumentManager::load(patcher, file);
        
        DocumentManager::commit(patcher);
        m_need_saving_flag = false;
        
        patcher.createUserIfNotAlreadyThere(m_instance.getUserId());
        
        patcher.setName(file.getName());
        
        DocumentManager::commit(patcher, "Add User");
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
        
        DocumentManager::commit(patcher);
        
        return patcher;
    }
    
    model::Patcher& jPatcherManager::getPatcher()
    {
        return m_document->root<model::Patcher>();
    }
    
    void jPatcherManager::newView()
    {
        auto& patcher = getPatcher();
        auto* user = patcher.getUser(m_instance.getUserId());
        if(user)
        {
            user->addView();
            
            DocumentManager::commit(patcher);
        }
    }
    
    size_t jPatcherManager::getNumberOfView()
    {
        auto& patcher = getPatcher();
        auto& user = *patcher.getUser(m_instance.getUserId());
        auto& views = user.getViews();
        
        return std::count_if(views.begin(), views.end(), [](model::Patcher::View& view){
            return !view.removed();
        });
    }
    
    bool jPatcherManager::saveDocument()
    {
        auto& patcher = getPatcher();
        File const& current_save_file = DocumentManager::getSelectedFile(patcher);
        
        if (current_save_file.exist())
        {
            DocumentManager::save(patcher, current_save_file);
            m_need_saving_flag = false;
            return true;
        }
        else
        {
            juce::FileChooser saveFileChooser("Save file",
                                              juce::File::getSpecialLocation (juce::File::userHomeDirectory),
                                              "*.kiwi");
            
            if(saveFileChooser.browseForFileToSave(true))
            {
                File save_file (saveFileChooser.getResult().getFullPathName().toStdString());
                DocumentManager::save(patcher, save_file);
                m_need_saving_flag = false;
                
                patcher.setName(save_file.getName());
                DocumentManager::commit(patcher);
                return true;
            }
        }
        
        return false;
    }
    
    FileBasedDocument::SaveResult jPatcherManager::saveIfNeededAndUserAgrees()
    {
        if (! m_need_saving_flag)
        {
            return FileBasedDocument::savedOk;
        }
        
        auto& patcher = getPatcher();
        
        const std::string document_name = patcher.getName();
        
        const int r = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon,
                                                      TRANS("Closing document..."),
                                                      TRANS("Do you want to save the changes to \"")
                                                      + document_name + "\"?",
                                                      TRANS("Save"),
                                                      TRANS("Discard changes"),
                                                      TRANS("Cancel"));
        
        // save changes
        if(r == 1)
        {
            return (saveDocument()) ? FileBasedDocument::savedOk : FileBasedDocument::failedToWriteToFile;
        }
        
        // discard changes
        if(r == 2)
        {
            return FileBasedDocument::savedOk;
        }
        
        return FileBasedDocument::userCancelledSave;
    }
    
    void jPatcherManager::forceCloseAllWindows()
    {
        auto& patcher = getPatcher();
        auto& user = *patcher.getUser(m_instance.getUserId());
        auto& views = user.getViews();
        
        bool view_has_been_removed = false;
        
        for(auto it = views.begin(); it != views.end();)
        {
            model::Patcher::View& view = *it;
            it = user.removeView(view);
        }
        
        if(view_has_been_removed)
        {
            DocumentManager::commit(patcher);
        }
    }
    
    bool jPatcherManager::askAllWindowsToClose()
    {
        auto& patcher = getPatcher();
        auto& user = *patcher.getUser(m_instance.getUserId());
        auto& views = user.getViews();
        
        size_t number_of_views = std::count_if(views.begin(), views.end(), [](model::Patcher::View& view){
            return !view.removed();
        });
        
        bool success = true;
        
        for(auto it = views.begin(); it != views.end();)
        {
            model::Patcher::View& view = *it;
            bool need_saving = m_need_saving_flag && (number_of_views <= 1);
            
            if(!need_saving || (need_saving && saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk))
            {
                it = user.removeView(view);
                DocumentManager::commit(patcher);
            }
            else
            {
                return false;
            }
            
            number_of_views--;
        }
        
        return success;
    }
    
    bool jPatcherManager::closePatcherViewWindow(jPatcher& patcher_j)
    {
        auto& patcher = getPatcher();
        auto& user = *patcher.getUser(m_instance.getUserId());
        auto& patcher_view_m = patcher_j.getPatcherViewModel();
        
        auto& views = user.getViews();
        
        size_t number_of_views = std::count_if(views.begin(), views.end(), [](model::Patcher::View& view){
            return !view.removed();
        });
        
        bool need_saving = m_need_saving_flag && (number_of_views <= 1);
        
        for(auto& view : views)
        {
            if(&view == &patcher_view_m)
            {
                if(!need_saving || saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
                {
                    user.removeView(patcher_view_m);
                    DocumentManager::commit(patcher);
                    return true;
                }
            }
        }
        
        return false;
    }
    
    void jPatcherManager::document_changed(model::Patcher& patcher)
    {
        if(patcher.added())
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
        
        if(patcher.resident() && (patcher.objectsChanged() || patcher.linksChanged()))
        {
            m_need_saving_flag = true;
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
            auto& jpatcher = view.entity().emplace<jPatcher>(*this, m_instance, patcher, view);
            
            auto& window = view.entity().emplace<jPatcherWindow>(*this, jpatcher);
            window.setContentNonOwned(&jpatcher.getViewport(), true);
            jpatcher.updateWindowTitle();
            jpatcher.grabKeyboardFocus();
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
            view.entity().erase<jPatcherWindow>();
        }
    }
}
