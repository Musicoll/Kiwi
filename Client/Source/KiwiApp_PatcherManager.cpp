/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiModel/KiwiModel_DataModel.hpp>
#include <KiwiModel/KiwiModel_PatcherUser.hpp>
#include <KiwiEngine/KiwiEngine_Patcher.hpp>

#include "KiwiApp.hpp"
#include "KiwiApp_Instance.hpp"
#include "KiwiApp_Network/KiwiApp_DocumentManager.hpp"
#include "KiwiApp_PatcherManager.hpp"
#include "KiwiApp_PatcherView.hpp"
#include "KiwiApp_PatcherViewHelper.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                PATCHER VIEW WINDOW                               //
    // ================================================================================ //

    PatcherViewWindow::PatcherViewWindow(PatcherManager& manager, PatcherView& patcherview) :
    Window("Untitled", true),
    m_patcher_manager(manager),
    m_patcherview(patcherview)
    {
        #if ! JUCE_MAC
        setMenuBar(KiwiApp::getMenuBarModel());
        #endif
        
        setSize(600, 500);
        centreWithSize(getWidth(), getHeight());
        setResizable(true, true);
        setVisible(true);
    }
    
    void PatcherViewWindow::closeButtonPressed()
    {
        if(m_patcher_manager.closePatcherViewWindow(m_patcherview) && m_patcher_manager.getNumberOfView() == 0)
        {
            KiwiApp::use().useInstance().removePatcher(m_patcher_manager);
        }
    }
    
    // ================================================================================ //
    //                                  PATCHER MANAGER                                 //
    // ================================================================================ //
    
    PatcherManager::PatcherManager(Instance& instance) :
    m_instance(instance),
    m_validator(),
    m_document(model::DataModel::use(), *this, m_validator, m_instance.getUserId(), 'cicm', 'kpat'),
    m_is_remote(false)
    {
        model::Patcher & patcher = getPatcher();
        
        patcher.useSelfUser();
        DocumentManager::commit(patcher);
        
        m_need_saving_flag = false;
    }
    
    PatcherManager::PatcherManager(Instance& instance, juce::File const& file):
    m_instance(instance),
    m_validator(),
    m_document(model::DataModel::use(), *this, m_validator, m_instance.getUserId(), 'cicm', 'kpat'),
    m_is_remote(false)
    {
        model::Patcher& patcher = getPatcher();
        DocumentManager::load(patcher, file);
        patcher.useSelfUser();
        patcher.setName(file.getFileNameWithoutExtension().toStdString());
        
        DocumentManager::commit(patcher);
        m_need_saving_flag = false;
        
        patcher.entity().use<engine::Patcher>().sendLoadbang();
    }
    
    PatcherManager::PatcherManager(Instance & instance, const std::string host, uint16_t port, uint64_t session_id) :
    m_instance(instance),
    m_validator(),
    m_document(model::DataModel::use(), *this, m_validator, m_instance.getUserId(), 'cicm', 'kpat'),
    m_is_remote(true)
    {
        model::Patcher& patcher = getPatcher();
        
        DocumentManager::connect(patcher, host, port, session_id);
        
        patcher.useSelfUser();
        DocumentManager::commit(patcher);
        
        patcher.entity().use<engine::Patcher>().sendLoadbang();
    }
    
    PatcherManager::~PatcherManager()
    {
        m_listeners.call(&Listener::patcherManagerRemoved, *this);
    }
    
    model::Patcher& PatcherManager::getPatcher()
    {
        return m_document.root<model::Patcher>();
    }
    
    model::Patcher const& PatcherManager::getPatcher() const
    {
        return m_document.root<model::Patcher>();
    }
    
    void PatcherManager::newView()
    {
        auto& patcher = getPatcher();
        patcher.useSelfUser().addView();
        DocumentManager::commit(patcher);
    }
    
    size_t PatcherManager::getNumberOfView()
    {
        auto& patcher = getPatcher();
        auto& user = patcher.useSelfUser();
        auto& views = user.getViews();
        
        return std::count_if(views.begin(), views.end(), [](model::Patcher::View const& view){
            return !view.removed();
        });
    }
    
    bool PatcherManager::needsSaving() const
    {
        return (!m_is_remote) && m_need_saving_flag;
    }
    
    void PatcherManager::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void PatcherManager::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    bool PatcherManager::saveDocument()
    {
        auto& patcher = getPatcher();
        juce::File const& current_save_file = DocumentManager::getSelectedFile(patcher);
        
        if (current_save_file.existsAsFile())
        {
            DocumentManager::save(patcher, current_save_file);
            m_need_saving_flag = false;
            DocumentManager::commit(patcher);
            return true;
        }
        else
        {
            juce::FileChooser saveFileChooser("Save file",
                                              juce::File::getSpecialLocation (juce::File::userHomeDirectory),
                                              "*.kiwi");
            
            if(saveFileChooser.browseForFileToSave(true))
            {
                juce::File save_file(saveFileChooser.getResult().getFullPathName());
                DocumentManager::save(patcher, save_file);
                m_need_saving_flag = false;
                
                patcher.setName(save_file.getFileNameWithoutExtension().toStdString());
                DocumentManager::commit(patcher);
                return true;
            }
        }
        
        return false;
    }
    
    juce::FileBasedDocument::SaveResult PatcherManager::saveIfNeededAndUserAgrees()
    {
        if (! needsSaving())
        {
            return juce::FileBasedDocument::savedOk;
        }
        
        auto& patcher = getPatcher();
        
        const std::string document_name = patcher.getName();
        
        const int r = juce::AlertWindow::showYesNoCancelBox(juce::AlertWindow::QuestionIcon,
                                                      TRANS("Closing document..."),
                                                      TRANS("Do you want to save the changes to \"")
                                                      + document_name + "\"?",
                                                      TRANS("Save"),
                                                      TRANS("Discard changes"),
                                                      TRANS("Cancel"));
        
        // save changes
        if(r == 1)
        {
            return (saveDocument()) ? juce::FileBasedDocument::savedOk : juce::FileBasedDocument::failedToWriteToFile;
        }
        
        // discard changes
        if(r == 2)
        {
            return juce::FileBasedDocument::savedOk;
        }
        
        return juce::FileBasedDocument::userCancelledSave;
    }
    
    void PatcherManager::forceCloseAllWindows()
    {
        auto& patcher = getPatcher();
        auto& user = patcher.useSelfUser();
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
    
    bool PatcherManager::askAllWindowsToClose()
    {
        auto& patcher = getPatcher();
        auto& user = patcher.useSelfUser();
        auto& views = user.getViews();
        
        size_t number_of_views = std::count_if(views.begin(), views.end(), [](model::Patcher::View& view){
            return !view.removed();
        });
        
        bool success = true;
        
        for(auto it = views.begin(); it != views.end();)
        {
            model::Patcher::View& view = *it;
            bool need_saving = m_need_saving_flag && (number_of_views <= 1);
            
            if(!need_saving || (need_saving && saveIfNeededAndUserAgrees() == juce::FileBasedDocument::savedOk))
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
    
    bool PatcherManager::closePatcherViewWindow(PatcherView& patcher_view)
    {
        auto& patcher = getPatcher();
        auto& user = patcher.useSelfUser();
        auto& patcher_view_m = patcher_view.getPatcherViewModel();
        
        auto& views = user.getViews();
        
        size_t number_of_views = std::count_if(views.begin(), views.end(), [](model::Patcher::View& view){
            return !view.removed();
        });
        
        bool need_saving = m_need_saving_flag && (number_of_views <= 1);
        
        for(auto& view : views)
        {
            if(&view == &patcher_view_m)
            {
                if(!need_saving || saveIfNeededAndUserAgrees() == juce::FileBasedDocument::savedOk)
                {
                    user.removeView(patcher_view_m);
                    DocumentManager::commit(patcher);
                    return true;
                }
            }
        }
        
        return false;
    }
    
    void PatcherManager::bringsFirstViewToFront()
    {
        auto& patcher = getPatcher();
        auto& user = patcher.useSelfUser();
            
        auto& views = user.getViews();
        
        const auto view_it = views.begin();
        
        if(view_it != views.cend())
        {
            model::Patcher::View& view = *view_it;
            if(view.entity().has<PatcherViewWindow>())
            {
                view.entity().use<PatcherViewWindow>().toFront(true);
            }
        }
    }
    
    void PatcherManager::document_changed(model::Patcher& patcher)
    {
        if(patcher.added())
        {
            patcher.entity().emplace<DocumentManager>(patcher.document());
            patcher.entity().emplace<engine::Patcher>(patcher, m_instance.useEngineInstance());
        }
        
        patcher.entity().use<engine::Patcher>().modelChanged();
        
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
    
    void PatcherManager::notifyPatcherViews(model::Patcher& patcher)
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
                
                //KiwiApp::post("New user !!");
            }
        }
    }

    void PatcherManager::createPatcherWindow(model::Patcher& patcher,
                                              model::Patcher::User& user,
                                              model::Patcher::View& view)
    {
        if(user.getId() == m_instance.getUserId())
        {
            auto& patcherview = view.entity().emplace<PatcherView>(*this, m_instance, patcher, view);
            
            auto& window = view.entity().emplace<PatcherViewWindow>(*this, patcherview);
            window.setContentNonOwned(&patcherview.getViewport(), true);
            patcherview.updateWindowTitle();
            patcherview.grabKeyboardFocus();
        }
    }

    void PatcherManager::notifyPatcherView(model::Patcher& patcher,
                                            model::Patcher::User& user,
                                            model::Patcher::View& view)
    {
        if(user.getId() == m_instance.getUserId())
        {
            // Notify PatcherView
            auto& patcherview = view.entity().use<PatcherView>();
            patcherview.patcherChanged(patcher, view);
        }
    }

    void PatcherManager::removePatcherWindow(model::Patcher& patcher,
                                              model::Patcher::User& user,
                                              model::Patcher::View& view)
    {
        if(user.getId() == m_instance.getUserId())
        {
            view.entity().erase<PatcherView>();
            view.entity().erase<PatcherViewWindow>();
        }
    }
}
