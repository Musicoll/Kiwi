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
    
    DocumentBrowser::DocumentBrowser(std::string const& drive_name, int refresh_time) :
    m_distant_drive(new Drive(drive_name)),
    m_refresh_time(refresh_time)
    {   
        if(m_refresh_time > 0)
        {
            startTimer(m_refresh_time);
        }
    }
    
    DocumentBrowser::~DocumentBrowser()
    {
        stopTimer();
    }
    
    void DocumentBrowser::setDriveName(std::string const& name)
    {
        m_distant_drive->setName(name);
    }
    
    DocumentBrowser::Drive* DocumentBrowser::getDrive() const
    {
        return m_distant_drive.get();
    }
    
    void DocumentBrowser::timerCallback()
    {
        m_distant_drive->refresh_internal();
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
    
    // ================================================================================ //
    //                              DOCUMENT BROWSER DRIVE                              //
    // ================================================================================ //
    
    DocumentBrowser::Drive::Drive(std::string const& name) :
    m_name(name),
    m_documents(),
    m_listeners(),
    m_sort([](DocumentSession const& l_hs, DocumentSession const& r_hs)
    {
        return l_hs.getName() < r_hs.getName();
    }),
    m_drive(this, [](DocumentBrowser::Drive*){})
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
        std::weak_ptr<DocumentBrowser::Drive> drive(m_drive);
        
        KiwiApp::useApi().uploadDocument(name,
                                         data,
                                         KiwiApp::use().getApplicationVersion().toStdString(),
                                         [drive](Api::Response res, Api::Document document)
        {
            KiwiApp::useScheduler().schedule([drive, res, document]()
            {
                std::shared_ptr<DocumentBrowser::Drive> drive_ptr = drive.lock();
                
                if (drive_ptr != nullptr)
                {
                    if (res.result() == beast::http::status::forbidden)
                    {
                        DocumentBrowser::handleDeniedRequest();
                    }
                    else if(res.error)
                    {
                        KiwiApp::error("Error: can't create document");
                        KiwiApp::error("=> " + res.error.message());
                    }
                    else
                    {
                        drive_ptr->refresh();
                    }
                }
            });
        });
    }
    
    void DocumentBrowser::Drive::createNewDocument(std::string const& name)
    {
        std::weak_ptr<DocumentBrowser::Drive> drive(m_drive);
        
        KiwiApp::useApi().createDocument(name, [drive](Api::Response res, Api::Document document)
        {
            KiwiApp::useScheduler().schedule([drive, res, document]()
            {
                std::shared_ptr<DocumentBrowser::Drive> drive_ptr = drive.lock();
                
                if(drive_ptr != nullptr)
                {
                    if (res.result() == beast::http::status::forbidden)
                    {
                        DocumentBrowser::handleDeniedRequest();
                    }
                    else if(res.error)
                    {
                        KiwiApp::error("Error: can't create document");
                        KiwiApp::error("=> " + res.error.message());
                    }
                    else
                    {
                        drive_ptr->refresh();
                    }
                }
            });
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
    
    void DocumentBrowser::Drive::refresh_internal()
    {
        const bool is_connected = KiwiApp::canConnectToServer();
        const bool was_connected = m_was_connected;
        
        if(was_connected != is_connected)
        {
            // dirty connection state manager :(
            // todo: need to refactor all document browser system
            m_listeners.call(&Listener::driveConnectionStatusChanged, is_connected);
        }
        
        m_was_connected = is_connected;
        
        if(was_connected && !is_connected)
        {
            m_drive->updateDocumentList({});
            return;
        }
        
        std::weak_ptr<DocumentBrowser::Drive> drive(m_drive);
        
        KiwiApp::useApi().getDocuments([drive](Api::Response res, Api::Documents docs)
        {
            KiwiApp::useScheduler().schedule([drive, res, docs]()
            {
                std::shared_ptr<DocumentBrowser::Drive> drive_ptr = drive.lock();
                
                if (drive_ptr != nullptr
                    && res.result() == beast::http::status::ok
                    && !res.error)
                {
                    drive_ptr->updateDocumentList(docs);
                }
            });
        });
    }
    
    void DocumentBrowser::Drive::refresh()
    {
        std::weak_ptr<DocumentBrowser::Drive> drive(m_drive);
        
        KiwiApp::useApi().getDocuments([drive](Api::Response res, Api::Documents docs)
        {
            KiwiApp::useScheduler().schedule([drive, res, docs]()
            {
                std::shared_ptr<DocumentBrowser::Drive> drive_ptr = drive.lock();
                
                if(drive_ptr != nullptr)
                {
                    if (res.result() == beast::http::status::forbidden)
                    {
                        DocumentBrowser::handleDeniedRequest();
                    }
                    else if(res.error)
                    {
                        KiwiApp::error("Kiwi API error: can't get documents => " + res.error.message());
                    }
                    else
                    {
                        drive_ptr->updateDocumentList(docs);
                    }
                }
            });
        });
    }
    
    // ================================================================================ //
    //                                  DRIVE DOCUMENT                                  //
    // ================================================================================ //
    
    DocumentBrowser::Drive::DocumentSession::DocumentSession(DocumentBrowser::Drive& parent,
                                                             Api::Document document) :
    m_drive(parent),
    m_document(std::move(document)),
    m_open_token(""),
    m_session(this, [](DocumentSession *){})
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
        std::weak_ptr<DocumentSession> session(m_session);
        
        KiwiApp::useApi().untrashDocument(m_document._id, [session](Api::Response res)
        {
            KiwiApp::useScheduler().schedule([session, res]()
            {
                std::shared_ptr<DocumentSession> session_ptr = session.lock();
                
                if (session_ptr != nullptr)
                {
                    if (res.result() == beast::http::status::forbidden)
                    {
                        DocumentBrowser::handleDeniedRequest();
                    }
                    else if(res.error)
                    {
                        KiwiApp::error(res.error.message());
                    }
                    else
                    {
                        session_ptr->m_drive.refresh();
                    }
                }
            });
        });
    }
    
    void DocumentBrowser::Drive::DocumentSession::trash()
    {
        std::weak_ptr<DocumentSession> session(m_session);
        
        KiwiApp::useApi().trashDocument(m_document._id, [session](Api::Response res)
        {
            KiwiApp::useScheduler().schedule([session, res]()
            {
                std::shared_ptr<DocumentSession> session_ptr = session.lock();
                
                if (session_ptr != nullptr)
                {
                    if (res.result() == beast::http::status::forbidden)
                    {
                        DocumentBrowser::handleDeniedRequest();
                    }
                    else if(res.error)
                    {
                        KiwiApp::error(res.error.message());
                    }
                    else
                    {
                        session_ptr->m_drive.refresh();
                    }
                }
            });
        });
    }
    
    void DocumentBrowser::Drive::DocumentSession::duplicate()
    {
        std::weak_ptr<DocumentSession> session(m_session);
        
        KiwiApp::useApi().duplicateDocument(m_document._id, [session](Api::Response res)
        {
            KiwiApp::useScheduler().schedule([res, session]()
            {
                std::shared_ptr<DocumentSession> session_ptr = session.lock();
                
                if (session_ptr != nullptr)
                {
                    if (res.result() == beast::http::status::forbidden)
                    {
                        DocumentBrowser::handleDeniedRequest();
                    }
                    else if (res.result() == beast::http::status::not_found)
                    {
                        KiwiApp::error("error: document not found");
                    }
                    else if(res.error)
                    {
                        KiwiApp::error(res.error.message());
                    }
                    else
                    {
                        session_ptr->m_drive.refresh();
                    }
                }
            });
        });
    }
    
    void DocumentBrowser::Drive::DocumentSession::rename(std::string const& new_name)
    {
        if(new_name.empty())
        {
            return;
        }
        
        std::weak_ptr<DocumentSession> session(m_session);
        
        KiwiApp::useApi().renameDocument(m_document._id, new_name, [session](Api::Response res)
        {
            KiwiApp::useScheduler().schedule([session, res]()
            {
                std::shared_ptr<DocumentSession> session_ptr = session.lock();
                
                if (session_ptr != nullptr)
                {
                    if (res.result() == beast::http::status::forbidden)
                    {
                        DocumentBrowser::handleDeniedRequest();
                    }
                    else if(res.error)
                    {
                        KiwiApp::error(res.error.message());
                    }
                    else
                    {
                        session_ptr->m_drive.refresh();
                    }
                }
            });
        });
    }
    
    void DocumentBrowser::Drive::DocumentSession::download(std::function<void(std::string const&)> callback)
    {
        std::weak_ptr<DocumentSession> session(m_session);
        
        KiwiApp::useApi().downloadDocument(m_document._id,
                                           [session, cb = std::move(callback)](Api::Response res)
        {
            KiwiApp::useScheduler().schedule([session, res, func = std::bind(cb, res.body)]()
            {
                std::shared_ptr<DocumentSession> session_ptr = session.lock();
                
                if (session_ptr)
                {
                    if (res.result() == beast::http::status::forbidden)
                    {
                        DocumentBrowser::handleDeniedRequest();
                    }
                    else if (res.result() == beast::http::status::not_found)
                    {
                        KiwiApp::error("error: document not found");
                    }
                    else if(res.error)
                    {
                        KiwiApp::error(res.error.message());
                    }
                    else
                    {
                        func();
                    }
                }
            });
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
        std::weak_ptr<DocumentSession> session(m_session);
        
        auto success = [session](std::string const& token)
        {
            KiwiApp::useScheduler().schedule([session, token]()
            {
                std::shared_ptr<DocumentSession> session_ptr = session.lock();
                
                if (session_ptr != nullptr)
                {
                    session_ptr->m_open_token = token;
                    session_ptr->m_drive.refresh();
                    KiwiApp::useInstance().openRemotePatcher(*session_ptr);
                }
            });
        };
        
        auto error = [session](Api::Error er)
        {
            KiwiApp::useScheduler().schedule([session, er]()
            {
                std::shared_ptr<DocumentSession> session_ptr = session.lock();
                
                if (session_ptr != nullptr)
                {
                    if (er.getStatusCode() == static_cast<unsigned>(beast::http::status::forbidden))
                    {
                        DocumentBrowser::handleDeniedRequest();
                    }
                    else
                    {
                        KiwiApp::error(er.getMessage());
                    }
                }
            });
        };
        
        KiwiApp::useApi().getOpenToken(m_document._id, success, error);
    }
}
