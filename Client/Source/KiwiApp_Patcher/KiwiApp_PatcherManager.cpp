/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <chrono>

#include <flip/contrib/DataProviderFile.h>
#include <flip/BackEndIR.h>
#include <flip/BackEndBinary.h>
#include <flip/contrib/DataConsumerFile.h>

#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Def.h>

#include <KiwiEngine/KiwiEngine_Patcher.h>
#include <KiwiEngine/KiwiEngine_Instance.h>

#include <KiwiApp_Patcher/KiwiApp_PatcherManager.h>

#include <KiwiApp.h>
#include <KiwiApp_Application/KiwiApp_Instance.h>
#include <KiwiModel/KiwiModel_DocumentManager.h>
#include <KiwiApp_Patcher/KiwiApp_PatcherView.h>
#include <KiwiApp_Patcher/KiwiApp_PatcherComponent.h>

namespace kiwi
{
    // ================================================================================ //
    //                                  PATCHER MANAGER                                 //
    // ================================================================================ //
    
    PatcherManager::PatcherManager(Instance& instance) :
    m_instance(instance),
    m_validator(),
    m_document(model::DataModel::use(), *this, m_validator,
               m_instance.getUserId(), 'cicm', 'kpat'),
    m_file(),
    m_socket(m_document),
    m_need_saving_flag(false),
    m_session(nullptr)
    {
        ;
    }
    
    PatcherManager::~PatcherManager()
    {
        forceCloseAllWindows();
        disconnect();
    }
    
    void PatcherManager::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void PatcherManager::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    void PatcherManager::pull()
    {
        if (isRemote())
        {
            m_socket.process();
            model::DocumentManager::pull(getPatcher());
        }
    }
    
    void PatcherManager::onStateTransition(flip::CarrierBase::Transition transition,
                                           flip::CarrierBase::Error error)
    {
        if (transition == flip::CarrierBase::Transition::Disconnected)
        {
            model::Patcher & patcher = getPatcher();
            
            patcher.setName(m_session->getName());
            model::DocumentManager::commit(patcher);
            
            m_session->useDrive().removeListener(*this);
            m_session = nullptr;
            
            m_connected_users.clear();
            
            m_listeners.call(&Listener::connectedUserChanged, *this);
            
            flip::Collection<model::Patcher::View> & views = getPatcher().useSelfUser().getViews();
            
            for(auto & view : views)
            {
                view.entity().use<PatcherViewWindow>().removeUsersIcon();
                
                view.entity().use<PatcherView>().updateWindowTitle();
            }
        }
    }
    
    void PatcherManager::disconnect()
    {
        if (isRemote())
        {
            m_socket.disconnect();
        }
    }
    
    
    bool PatcherManager::connect(std::string const& host,
                                 uint16_t port,
                                 DocumentBrowser::Drive::DocumentSession& session)
    {
        disconnect();
        
        model::Patcher& patcher = getPatcher();
        
        m_user_connected_signal_cnx = patcher.signal_user_connect.connect([this](uint64_t user_id){
            
            if(m_connected_users.insert(user_id).second)
            {
                m_listeners.call(&Listener::connectedUserChanged, *this);
            }
        });
        
        m_user_disconnected_signal_cnx = patcher.signal_user_disconnect.connect([this](uint64_t user_id){
            
            const auto user_to_erase = m_connected_users.find(user_id);
            if(user_to_erase != m_connected_users.cend())
            {
                m_connected_users.erase(user_id);
                m_listeners.call(&Listener::connectedUserChanged, *this);
            }
        });
        
        m_receive_connected_users_signal_cnx = patcher.signal_receive_connected_users.connect([this](std::vector<uint64_t> users){
            
            // Todo : make a diff of the changes and notify listeners only if the list really changed.
            m_connected_users.clear();
            m_connected_users.insert(users.begin(), users.end());
            m_listeners.call(&Listener::connectedUserChanged, *this);
        });
        
        std::string metadata = KIWI_MODEL_VERSION_STRING;
        
        m_socket.connect(host, port, session.getSessionId(), metadata);
        
        bool patcher_loaded = false;
        
        m_socket.listenTransferBackend([&patcher_loaded](size_t cur, size_t total)
        {
            patcher_loaded = cur == total;
            
        });
        
        const auto init_time = std::chrono::steady_clock::now();
        
        const std::chrono::duration<int> time_out(2);
        
        while(!m_socket.isConnected() && std::chrono::steady_clock::now() - init_time < time_out)
        {
            m_socket.process();
        }
        
        while(!patcher_loaded && m_socket.isConnected())
        {
            m_socket.process();
        }
        
        if (m_socket.isConnected() && patcher_loaded)
        {
            m_session = &session;
            
            m_session->useDrive().addListener(*this);
            
            m_socket.listenStateTransition([this](flip::CarrierBase::Transition state,
                                                  flip::CarrierBase::Error error)
            {
                onStateTransition(state, error);
            });
            
            model::DocumentManager::pull(patcher);
            
            patcher.useSelfUser();
            model::DocumentManager::commit(patcher);
            
            patcher.entity().use<engine::Patcher>().sendLoadbang();
        }
        
        return m_socket.isConnected() && patcher_loaded;
    }
    
    void PatcherManager::readDocument()
    {
        flip::DataProviderFile provider(m_file.getFullPathName().toStdString().c_str());
        flip::BackEndIR back_end;
        
        back_end.register_backend<flip::BackEndBinary>();
        back_end.read(provider);
        
        m_document.read(back_end);
        
        m_need_saving_flag = false;
    }
    
    void PatcherManager::loadFromFile(juce::File const& file)
    {
        if (file.hasFileExtension("kiwi"))
        {
            m_file = file;
            
            readDocument();
            
            model::Patcher& patcher = getPatcher();
            
            patcher.useSelfUser();
            patcher.setName(file.getFileNameWithoutExtension().toStdString());
            
            model::DocumentManager::commit(patcher);
            
            patcher.entity().use<engine::Patcher>().sendLoadbang();
        }
    }
    
    model::Patcher& PatcherManager::getPatcher()
    {
        return m_document.root<model::Patcher>();
    }
    
    model::Patcher const& PatcherManager::getPatcher() const
    {
        return m_document.root<model::Patcher>();
    }
    
    bool PatcherManager::isRemote() const noexcept
    {
        return m_socket.isConnected();
    }
    
    uint64_t PatcherManager::getSessionId() const noexcept
    {
        return m_session ? m_session->getSessionId() : 0ull;
    }
    
    std::string PatcherManager::getDocumentName() const
    {
        return isRemote() ? m_session->getName() : getPatcher().getName();
    }
    
    void PatcherManager::newView()
    {
        auto& patcher = getPatcher();
        if(!model::DocumentManager::isInCommitGesture(patcher))
        {
            patcher.useSelfUser().addView();
            model::DocumentManager::commit(patcher);
        }
    }
    
    size_t PatcherManager::getNumberOfUsers()
    {
        return m_connected_users.size();
    }
    
    std::unordered_set<uint64_t> PatcherManager::getConnectedUsers()
    {
        return m_connected_users;
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
    
    juce::File const& PatcherManager::getSelectedFile() const
    {
        return m_file;
    }
    
    bool PatcherManager::needsSaving() const noexcept
    {
        return !isRemote() && m_need_saving_flag;
    }
    
    void PatcherManager::writeDocument()
    {
        flip::BackEndIR back_end =  m_document.write();
        flip::DataConsumerFile consumer(m_file.getFullPathName().toStdString().c_str());
        back_end.write<flip::BackEndBinary>(consumer);
        m_need_saving_flag = false;
    }
    
    bool PatcherManager::saveDocument()
    {
        if (needsSaving())
        {
            if (m_file.existsAsFile())
            {
                writeDocument();
            }
            else
            {
                auto directory = juce::File::getSpecialLocation(juce::File::userHomeDirectory);
                
                juce::FileChooser saveFileChooser("Save file", directory, "*.kiwi");
                
                if (saveFileChooser.browseForFileToSave(true))
                {
                    m_file = saveFileChooser.getResult();
                    
                    writeDocument();
                    
                    model::Patcher& patcher = getPatcher();
                    patcher.setName(m_file.getFileNameWithoutExtension().toStdString());
                    model::DocumentManager::commit(patcher);
                }
            }
        }
        
        return !m_need_saving_flag;
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
        
        for(auto it = views.begin(); it != views.end();)
        {
            it = user.removeView(*it);
        }
        
        model::DocumentManager::commit(patcher);
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
            bool need_saving = m_need_saving_flag && (number_of_views <= 1);
            
            if(!need_saving || (need_saving && saveIfNeededAndUserAgrees() == juce::FileBasedDocument::savedOk))
            {
                it = user.removeView(*it);
                model::DocumentManager::commit(patcher);
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
                    model::DocumentManager::commit(patcher);
                    return true;
                }
            }
        }
        
        return false;
    }
    
    PatcherViewWindow & PatcherManager::getFirstWindow()
    {
        auto first_view = getPatcher().useSelfUser().getViews().begin();
        return (*first_view).entity().use<PatcherViewWindow>();
    }
    
    void PatcherManager::bringsFirstViewToFront()
    {
        auto& patcher = getPatcher();
        auto& user = patcher.useSelfUser();
            
        auto& views = user.getViews();
        
        auto view_it = views.begin();
        
        if(view_it != views.end())
        {
            model::Patcher::View& view = *view_it;
            if(view.entity().has<PatcherViewWindow>())
            {
                view.entity().use<PatcherViewWindow>().toFront(true);
            }
        }
    }
    
    void PatcherManager::documentAdded(DocumentBrowser::Drive::DocumentSession& doc)
    {
        ;
    }
    
    void PatcherManager::documentChanged(DocumentBrowser::Drive::DocumentSession& doc)
    {
        if(m_session && (m_session == &doc))
        {
            for(auto& view : getPatcher().useSelfUser().getViews())
            {
                auto& patcherview = view.entity().use<PatcherView>();
                patcherview.updateWindowTitle();
            }
        }
    }
    
    void PatcherManager::documentRemoved(DocumentBrowser::Drive::DocumentSession& doc)
    {
        if(m_session && (m_session == &doc))
        {
            disconnect();
            
            for(auto& view : getPatcher().useSelfUser().getViews())
            {
                auto& patcherview = view.entity().use<PatcherView>();
                patcherview.updateWindowTitle();
            }
        }
    }
    
    void PatcherManager::document_changed(model::Patcher& patcher)
    {
        if(patcher.added())
        {
            std::unique_lock<std::mutex> lock(m_instance.useEngineInstance().getScheduler().lock());
            
            patcher.entity().emplace<model::DocumentManager>(patcher.document());
            patcher.entity().emplace<engine::Patcher>(m_instance.useEngineInstance(), patcher);
        }
        
        {   
            patcher.entity().use<engine::Patcher>().modelChanged(patcher);
        }
        
        notifyPatcherViews(patcher);
        
        if(patcher.removed())
        {
            std::unique_lock<std::mutex> lock(m_instance.useEngineInstance().getScheduler().lock());
            
            patcher.entity().erase<engine::Patcher>();
            patcher.entity().erase<model::DocumentManager>();
        }
        
        if(patcher.resident() && (patcher.objectsChanged() || patcher.linksChanged()))
        {
            m_need_saving_flag = true;
        }
    }
    
    void PatcherManager::notifyPatcherViews(model::Patcher& patcher)
    {
        bool changed = false;
        for(auto& user : patcher.getUsers())
        {
            changed = (changed || user.added() || user.removed());
            
            if(user.getId() == m_document.user())
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
        }
    }

    void PatcherManager::createPatcherWindow(model::Patcher& patcher,
                                             model::Patcher::User const& user,
                                             model::Patcher::View& view)
    {
        if(user.getId() == m_document.user())
        {
            auto& patcherview = view.entity().emplace<PatcherView>(*this, m_instance, patcher, view);
            view.entity().emplace<PatcherViewWindow>(*this, patcherview);
            patcherview.updateWindowTitle();
        }
    }

    void PatcherManager::notifyPatcherView(model::Patcher& patcher,
                                           model::Patcher::User const& user,
                                           model::Patcher::View& view)
    {
        if(user.getId() == m_document.user())
        {
            // Notify PatcherView
            auto& patcherview = view.entity().use<PatcherView>();
            patcherview.patcherChanged(patcher, view);
        }
    }

    void PatcherManager::removePatcherWindow(model::Patcher& patcher,
                                             model::Patcher::User const& user,
                                             model::Patcher::View& view)
    {
        if(user.getId() == m_document.user())
        {
            view.entity().erase<PatcherView>();
            view.entity().erase<PatcherViewWindow>();
        }
    }
}
