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

#include "KiwiApp_DocumentBrowser.h"
#include "../KiwiApp.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include "../KiwiApp_General/KiwiApp_IDs.h"

namespace kiwi
{
    // ================================================================================ //
    //                                 DOCUMENT BROWSER                                 //
    // ================================================================================ //
    
    DocumentBrowser::DocumentBrowser() :
    m_distant_drive(nullptr)
    {
        auto& settings = getAppSettings().network();
        settings.addListener(*this);
        
        m_distant_drive.reset(new Drive("Remote Drive", settings.getSessionPort()));
        
        int time = settings.getRefreshInterval();
        if(time > 0)
        {
            start(time);
        }
        
        authUserChanged(KiwiApp::getCurrentUser());
        KiwiApp::addApiConnectStatusListener(*this);
    }
    
    DocumentBrowser::~DocumentBrowser()
    {
        KiwiApp::removeApiConnectStatusListener(*this);
        getAppSettings().network().removeListener(*this);
        stop();
    }
    
    void DocumentBrowser::start(const int interval)
    {
        startTimer(interval);
    }
    
    void DocumentBrowser::stop()
    {
        stopTimer();
    }
    
    std::vector<DocumentBrowser::Drive*> DocumentBrowser::getDrives() const
    {
        return {m_distant_drive.get()};
    }
    
    void DocumentBrowser::networkSettingsChanged(NetworkSettings const& settings, const juce::Identifier& id)
    {
        if(id == Ids::session_port)
        {
            m_distant_drive->setSessionPort(settings.getSessionPort());
        }
        else if(id == Ids::refresh_interval)
        {
            auto time = settings.getRefreshInterval();
            
            if(time > 0)
            {
                start(time);
            }
            else
            {
                stop();
            }
        }
    }
    
    void DocumentBrowser::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void DocumentBrowser::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    void DocumentBrowser::timerCallback()
    {
        process();
    }
    
    void DocumentBrowser::process()
    {
        m_distant_drive->refresh();
    }
    
    void DocumentBrowser::handleDeniedRequest()
    {
        if (KiwiApp::getCurrentUser().isLoggedIn())
        {
            KiwiApp::logout();
            KiwiApp::error("Session has expired. Please login.");
        }
        else
        {
            KiwiApp::error("Request denied. Please login.");
        }
    }
    
    void DocumentBrowser::userLoggedIn(Api::AuthUser const& user)
    {
        m_distant_drive->setName(user.getName());
    }
    
    void DocumentBrowser::userLoggedOut(Api::AuthUser const&)
    {
        m_distant_drive->setName("logged out...");
    }
    
    void DocumentBrowser::authUserChanged(Api::AuthUser const& user)
    {
        m_distant_drive->setName(user.isLoggedIn() ? user.getName() : "logged out...");
    }
    
    // ================================================================================ //
    //                              DOCUMENT BROWSER DRIVE                              //
    // ================================================================================ //
    
    DocumentBrowser::Drive::Drive(std::string const& name,
                                  uint16_t session_port) :
    m_session_port(session_port),
    m_name(name),
    m_sort([](DocumentSession const& l_hs, DocumentSession const& r_hs)
    {
        return l_hs.getName() < r_hs.getName();
    })
    {
        ;
    };
    
    void DocumentBrowser::Drive::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void DocumentBrowser::Drive::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    void DocumentBrowser::Drive::setSessionPort(uint16_t port)
    {
        m_session_port = port;
    }
    
    uint16_t DocumentBrowser::Drive::getSessionPort() const
    {
        return m_session_port;
    }
    
    void DocumentBrowser::Drive::setName(std::string const& name)
    {
        m_name = name;
        m_listeners.call(&Listener::driveChanged);
    }
    
    std::string const& DocumentBrowser::Drive::getName() const
    {
        return m_name;
    }
    
    void DocumentBrowser::Drive::setSort(Comp comp)
    {
        m_sort = comp;
        
        std::sort(m_documents.begin(),
                  m_documents.end(),
                  [comp](std::unique_ptr<DocumentSession> const& l_hs,
                         std::unique_ptr<DocumentSession> const& r_hs)
        {
            return comp(*l_hs, *r_hs);
        });
        
        m_listeners.call(&Listener::driveChanged);
    }
    
    void DocumentBrowser::Drive::uploadDocument(std::string const& name, std::string const& data)
    {
        KiwiApp::useApi().uploadDocument(name,
                                         data,
                                         KiwiApp::use().getApplicationVersion().toStdString(),
                                         [this](Api::Response res, Api::Document document)
        {
            if (res.result() == beast::http::status::forbidden)
            {
                DocumentBrowser::handleDeniedRequest();
            }
            else if(res.error)
            {
                juce::MessageManager::callAsync([message = res.error.message()](){
                    KiwiApp::error("Error: can't create document");
                    KiwiApp::error("=> " + message);
                });
            }
            else
            {
                juce::MessageManager::callAsync([this]()
                {
                    refresh();
                });
            }
        });
    }
    
    void DocumentBrowser::Drive::createNewDocument()
    {
        KiwiApp::useApi().createDocument("", [this](Api::Response res, Api::Document document) {
            
            if (res.result() == beast::http::status::forbidden)
            {
                DocumentBrowser::handleDeniedRequest();
            }
            else if(res.error)
            {
                juce::MessageManager::callAsync([message = res.error.message()](){
                    KiwiApp::error("Error: can't create document");
                    KiwiApp::error("=> " + message);
                });
            }
            else
            {
                juce::MessageManager::callAsync([this]()
                {
                    refresh();
                });
            }
        });
    }
    
    DocumentBrowser::Drive::DocumentSessions const& DocumentBrowser::Drive::getDocuments() const
    {
        return m_documents;
    }
    
    DocumentBrowser::Drive::DocumentSessions& DocumentBrowser::Drive::getDocuments()
    {
        return m_documents;
    }
    
    bool DocumentBrowser::Drive::operator==(Drive const& drive) const
    {
        return (getSessionPort() == drive.getSessionPort()) && (m_name == drive.getName());
    }
    
    void DocumentBrowser::Drive::updateDocumentList(Api::Documents docs)
    {
        bool changed = false;
        
        DocumentSessions new_documents;
        new_documents.reserve(docs.size());
        
        for(auto && doc : docs)
        {
            new_documents.emplace_back(std::make_unique<DocumentSession>(*this, std::move(doc)));
        }
        
        // drive removed notification
        for(auto doc_it = m_documents.begin(); doc_it != m_documents.end();)
        {
            auto it = std::find_if(new_documents.begin(), new_documents.end(),
                                   [&new_doc = *(doc_it->get())](auto const& doc){
                                       return *(doc.get()) == new_doc;
                                   });
            
            if(it == new_documents.end())
            {
                m_listeners.call(&Listener::documentRemoved, *(doc_it->get()));
                doc_it = m_documents.erase(doc_it);
                changed = true;
                continue;
            }
            
            ++doc_it;
        }
        
        // drive added or changed notification
        for(auto && new_doc : new_documents)
        {
            auto it = std::find_if(m_documents.begin(), m_documents.end(),
                                   [&new_doc = *new_doc](auto const& doc){
                                       return *(doc.get()) == new_doc;
                                   });
            
            // new document
            if(it == m_documents.end())
            {
                it = m_documents.emplace(it, std::move(new_doc));
                auto& doc = *(it->get());
                
                m_listeners.call(&Listener::documentAdded, doc);
                changed = true;
            }
            else
            {
                // name is currently the only document field that can change.
                if(new_doc->getName() != (*it)->getName()
                   || new_doc->isTrashed() != (*it)->isTrashed()
                   || new_doc->getOpenedDate() != (*it)->getOpenedDate())
                {
                    (*it)->m_document = new_doc->m_document;
                    
                    auto& doc = *(it->get());
                    m_listeners.call(&Listener::documentChanged, doc);
                    changed = true;
                }
            }
        }
        
        if(changed)
        {
            std::sort(m_documents.begin(),
                      m_documents.end(),
                      [this](std::unique_ptr<DocumentSession> const& l_hs,
                             std::unique_ptr<DocumentSession> const& r_hs)
            {
                return m_sort(*l_hs, *r_hs);
                
            });
            
            m_listeners.call(&Listener::driveChanged);
        }
    }
    
    void DocumentBrowser::Drive::refresh()
    {
        KiwiApp::useApi().getDocuments([this](Api::Response res, Api::Documents docs) {
            
            if (res.result() == beast::http::status::forbidden)
            {
                KiwiApp::useInstance().useScheduler().schedule([]()
                {
                    DocumentBrowser::handleDeniedRequest();
                });
            }
            else if(res.error)
            {
                KiwiApp::error("Kiwi API error: can't get documents => " + res.error.message());
            }
            else
            {
                KiwiApp::useInstance().useScheduler().schedule([this, docs]() {
                    updateDocumentList(docs);
                });
            }
        });
    }
    
    // ================================================================================ //
    //                                  DRIVE DOCUMENT                                  //
    // ================================================================================ //
    
    DocumentBrowser::Drive::DocumentSession::DocumentSession(DocumentBrowser::Drive& parent,
                                                             Api::Document document) :
    m_drive(parent),
    m_document(std::move(document)),
    m_open_token("")
    {
        
    }
    
    DocumentBrowser::Drive::DocumentSession::~DocumentSession()
    {
        ;
    }
    
    std::string DocumentBrowser::Drive::DocumentSession::getName() const
    {
        return m_document.name;
    }
    
    uint64_t DocumentBrowser::Drive::DocumentSession::getSessionId() const
    {
        return m_document.session_id;
    }
    
    DocumentBrowser::Drive const& DocumentBrowser::Drive::DocumentSession::useDrive() const
    {
        return m_drive;
    }
    
    std::string const& DocumentBrowser::Drive::DocumentSession::getCreationDate() const
    {
        return m_document.creation_date;
    }
    
    std::string const& DocumentBrowser::Drive::DocumentSession::getAuthor() const
    {
        return m_document.author_name;
    }
    
    bool DocumentBrowser::Drive::DocumentSession::isTrashed() const
    {
        return m_document.trashed;
    }
    
    std::string const& DocumentBrowser::Drive::DocumentSession::getTrashedDate() const
    {
        return m_document.trashed_date;
    }
    
    std::string const& DocumentBrowser::Drive::DocumentSession::getOpenedDate() const
    {
        return m_document.opened_date;
    }
    
    std::string const& DocumentBrowser::Drive::DocumentSession::getOpenedUser() const
    {
        return m_document.opened_user;
    }
    
    std::string const& DocumentBrowser::Drive::DocumentSession::getOpenToken() const
    {
        return m_open_token;
    }
    
    void DocumentBrowser::Drive::DocumentSession::untrash()
    {
        KiwiApp::useApi().untrashDocument(m_document._id, [this](Api::Response res)
        {
            if (res.result() == beast::http::status::forbidden)
            {
                KiwiApp::useInstance().useScheduler().schedule([]()
                {
                    DocumentBrowser::handleDeniedRequest();
                });
            }
            else if(res.error)
            {
                KiwiApp::error(res.error.message());
            }
            else
            {
                KiwiApp::useInstance().useScheduler().schedule([this]()
                                                               {
                                                                   m_drive.refresh();
                                                               });
            }
        });
    }
    
    void DocumentBrowser::Drive::DocumentSession::trash()
    {
        KiwiApp::useApi().trashDocument(m_document._id, [this](Api::Response res) {
            
            if (res.result() == beast::http::status::forbidden)
            {
                KiwiApp::useInstance().useScheduler().schedule([]()
                {
                    DocumentBrowser::handleDeniedRequest();
                    
                });
            }
            else if(res.error)
            {
                KiwiApp::error(res.error.message());
            }
            else
            {
                KiwiApp::useInstance().useScheduler().schedule([this]()
                {
                    m_drive.refresh();
                });
            }
        });
    }
    
    void DocumentBrowser::Drive::DocumentSession::duplicate()
    {
        KiwiApp::useApi().duplicateDocument(m_document._id, [this](Api::Response res) {
            
            if (res.result() == beast::http::status::forbidden)
            {
                KiwiApp::useInstance().useScheduler().schedule([]()
                {
                    DocumentBrowser::handleDeniedRequest();
                });
            }
            else if(res.error)
            {
                KiwiApp::error(res.error.message());
            }
            else
            {
                KiwiApp::useInstance().useScheduler().schedule([this]()
                {
                    m_drive.refresh();
                });
            }
        });
    }
    
    void DocumentBrowser::Drive::DocumentSession::rename(std::string const& new_name)
    {
        if(new_name.empty())
        {
            return;
        }
        
        KiwiApp::useApi().renameDocument(m_document._id, new_name, [this](Api::Response res) {
            
            if (res.result() == beast::http::status::forbidden)
            {
                KiwiApp::useInstance().useScheduler().schedule([]()
                {
                    DocumentBrowser::handleDeniedRequest();
                });
            }
            else if(res.error)
            {
                KiwiApp::error(res.error.message());
            }
            else
            {
                KiwiApp::useInstance().useScheduler().schedule([this]()
                {
                    m_drive.refresh();
                });
            }
        });
    }
    
    void DocumentBrowser::Drive::DocumentSession::download(std::function<void(std::string const&)> callback)
    {
        KiwiApp::useApi().downloadDocument(m_document._id,
                                           [this, cb = std::move(callback)](Api::Response res)
        {
            
            if (res.result() == beast::http::status::forbidden)
            {
                KiwiApp::useInstance().useScheduler().schedule([]()
                {
                    DocumentBrowser::handleDeniedRequest();
                    
                });
            }
            else if(res.error)
            {
                KiwiApp::error(res.error.message());
            }
            else
            {
                KiwiApp::useInstance().useScheduler().schedule(std::bind(cb, res.body));
            }
        });
    }
    
    bool DocumentBrowser::Drive::DocumentSession::operator==(DocumentSession const& other_doc) const
    {
        return (&m_drive == &other_doc.useDrive()) && (m_document == other_doc.m_document);
    }
    
    DocumentBrowser::Drive& DocumentBrowser::Drive::DocumentSession::useDrive()
    {
        return m_drive;
    }
    
    void DocumentBrowser::Drive::DocumentSession::open()
    {
        auto success = [this](std::string const& token)
        {
            m_open_token = token;
            
            KiwiApp::useInstance().useScheduler().schedule([this]()
            {
                KiwiApp::useInstance().openRemotePatcher(*this);
                m_drive.refresh();
            });
        };
        
        auto error = [this](Api::Error er)
        {
            if (er.getStatusCode() == static_cast<unsigned>(beast::http::status::forbidden))
            {
                KiwiApp::useInstance().useScheduler().schedule([]()
                {
                    DocumentBrowser::handleDeniedRequest();
                });
            }
            else
            {
                KiwiApp::useInstance().useScheduler().schedule([er]()
                {
                    KiwiApp::error(er.getMessage());
                });
            }
        };
        
        KiwiApp::useApi().getOpenToken(m_document._id, success, error);
    }
}
