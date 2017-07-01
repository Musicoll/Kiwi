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

#include "../KiwiApp_Network/KiwiApp_Api.h"

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
        
        m_distant_drive.reset(new Drive("Remote patchers",
                                        settings.getHost(),
                                        settings.getApiPort(),
                                        settings.getSessionPort()));
        
        int time = settings.getRefreshInterval();
        if(time > 0)
        {
            start(time);
        }
        
        settings.addListener(*this);
    }
    
    DocumentBrowser::~DocumentBrowser()
    {
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
        if(id == Ids::host)
        {
            m_distant_drive->setHost(settings.getHost());
        }
        else if(id == Ids::api_port)
        {
            m_distant_drive->setApiPort(settings.getApiPort());
        }
        else if(id == Ids::session_port)
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
    
    // ================================================================================ //
    //                              DOCUMENT BROWSER DRIVE                              //
    // ================================================================================ //
    
    DocumentBrowser::Drive::Drive(std::string const& name,
                                  std::string const& host,
                                  uint16_t api_port,
                                  uint16_t session_port) :
    m_api(host, api_port),
    m_session_port(session_port),
    m_name(name)
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
    
    Api& DocumentBrowser::Drive::useApi()
    {
        return m_api;
    }
    
    uint16_t DocumentBrowser::Drive::getApiPort() const
    {
        return m_api.getPort();
    }
    
    void DocumentBrowser::Drive::setApiPort(uint16_t port)
    {
        m_api.setPort(port);
    }
    
    void DocumentBrowser::Drive::setSessionPort(uint16_t port)
    {
        m_session_port = port;
    }
    
    uint16_t DocumentBrowser::Drive::getSessionPort() const
    {
        return m_session_port;
    }
    
    void DocumentBrowser::Drive::setHost(std::string const& host)
    {
        m_api.setHost(host);
    }
    
    std::string const& DocumentBrowser::Drive::getHost() const
    {
        return m_api.getHost();
    }
    
    void DocumentBrowser::Drive::setName(std::string const& name)
    {
        m_name = name;
    }
    
    std::string const& DocumentBrowser::Drive::getName() const
    {
        return m_name;
    }
    
    void DocumentBrowser::Drive::createNewDocument()
    {
        m_api.createDocument([this](Api::Response res, Api::Document document) {
            
            if(res.error)
            {
                juce::MessageManager::callAsync([message = res.error.message](){
                    KiwiApp::error("Error: can't create document");
                    KiwiApp::error("=> " + message);
                });
                
                return;
            }
            
            juce::MessageManager::callAsync([this, document]() {
                
                auto it = m_documents.emplace(m_documents.end(), std::make_unique<DocumentSession>(*this, std::move(document)));
                
                m_listeners.call(&Listener::documentAdded, *(it->get()));
                m_listeners.call(&Listener::driveChanged);
                
                (*it)->open();
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
    
    bool DocumentBrowser::Drive::operator==(Drive const& drive) const
    {
        return (getHost() == drive.getHost())
        && (getApiPort() == drive.getApiPort())
        && (getSessionPort() == drive.getSessionPort())
        && (m_name == drive.getName());
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
                if(new_doc->getName() != (*it)->getName())
                {
                    (*it)->m_document.name = new_doc->getName();
                    auto& doc = *(it->get());
                    
                    m_listeners.call(&Listener::documentChanged, doc);
                    changed = true;
                }
            }
        }
        
        if(changed)
        {
            m_listeners.call(&Listener::driveChanged);
        }
    }
    
    void DocumentBrowser::Drive::refresh()
    {
        m_api.getDocuments([this](Api::Response res, Api::Documents docs) {
            
            if(res.error)
            {
                 KiwiApp::error("Kiwi API error: can't get documents => " + res.error.message);
            }
            else
            {
                KiwiApp::useInstance().useScheduler().schedule([this, docs]()
                {
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
    m_document(std::move(document))
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
    
    std::string DocumentBrowser::Drive::DocumentSession::getHost() const
    {
        return m_drive.getHost();
    }
    
    uint64_t DocumentBrowser::Drive::DocumentSession::getSessionId() const
    {
        return m_document.session_id;
    }
    
    uint16_t DocumentBrowser::Drive::DocumentSession::getSessionPort() const
    {
        return m_drive.getSessionPort();
    }
    
    DocumentBrowser::Drive const& DocumentBrowser::Drive::DocumentSession::useDrive() const
    {
        return m_drive;
    }
    
    void DocumentBrowser::Drive::DocumentSession::rename(std::string const& new_name)
    {
        if(new_name.empty())
        {
            return;
        }
        
        m_drive.useApi().renameDocument([](Api::Response res) {
            
            if(!res.error)
            {
                std::cout << "document successfully updated" << '\n';
            }
            else
            {
                std::cout << "document update failed, err: " + res.error.message << '\n';
            }
            
        }, m_document._id, new_name);
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
        KiwiApp::useInstance().openRemotePatcher(*this);
    }
}
