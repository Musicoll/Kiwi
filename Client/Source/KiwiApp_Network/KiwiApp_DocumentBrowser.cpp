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

#include "KiwiApp_DocumentBrowser.hpp"
#include "../KiwiApp.hpp"

#include "../KiwiApp_Network/KiwiApp_Api.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

namespace kiwi
{
    // ================================================================================ //
    //                                 DOCUMENT BROWSER                                 //
    // ================================================================================ //
    
    DocumentBrowser::DocumentBrowser() :
    m_distant_drive(new Drive("Remote", "54.234.132.205", 80, 9090))
    {
        ;
    }
    
    DocumentBrowser::~DocumentBrowser()
    {
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
    m_host(host),
    m_sessions_port(session_port),
    m_name(name)
    {
        //processSession(session);
    };
    
    void DocumentBrowser::Drive::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void DocumentBrowser::Drive::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    uint16_t DocumentBrowser::Drive::getApiPort() const
    {
        return m_api.getPort();
    }
    
    uint16_t DocumentBrowser::Drive::getSessionsPort() const
    {
        return m_sessions_port;
    }
    
    std::string DocumentBrowser::Drive::getHost() const
    {
        return m_host;
    }
    
    std::string DocumentBrowser::Drive::getName() const
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
                
                auto it = m_documents.emplace(m_documents.end(), *this, std::move(document));
                
                m_listeners.call(&Listener::documentAdded, *it);
                
                it->open();
            });
            
        });
        
    }
    
    std::list<DocumentBrowser::Drive::DocumentSession> const& DocumentBrowser::Drive::getDocuments() const
    {
        return m_documents;
    }
    
    bool DocumentBrowser::Drive::operator==(Drive const& drive) const
    {
        return (m_host == drive.m_host)
        && (getApiPort() == drive.getApiPort())
        && (m_sessions_port == drive.m_sessions_port)
        && (m_name == drive.m_name);
    }
    
    void DocumentBrowser::Drive::refresh()
    {
        m_api.getDocuments([this](Api::Response res, Api::Documents docs) {
            
            if(res.error)
            {
                juce::MessageManager::callAsync([message = res.error.message](){
                    KiwiApp::error("Error: can't get documents");
                    KiwiApp::error("=> " + message);
                });
                
                return;
            }
            
            std::vector<DocumentSession> new_documents;
            new_documents.reserve(docs.size());
            
            for(auto && doc : docs)
            {
                new_documents.emplace_back(*this, std::move(doc));
            }
            
            // drive removed notification
            for(auto doc_it = m_documents.begin(); doc_it != m_documents.end();)
            {
                auto it = std::find(new_documents.begin(), new_documents.end(), *doc_it);
                
                if(it == new_documents.end())
                {
                    juce::MessageManager::callAsync([this, doc_it](){
                        m_listeners.call(&Listener::documentRemoved, *doc_it);
                    });
                    
                    doc_it = m_documents.erase(doc_it);
                    continue;
                }
                
                ++doc_it;
            }
            
            // drive added or changed notification
            for(auto const& new_doc : new_documents)
            {
                auto it = std::find(m_documents.begin(), m_documents.end(), new_doc);
                
                // new document
                if(it == m_documents.end())
                {
                    it = m_documents.emplace(it, new_doc);
                    
                    juce::MessageManager::callAsync([this, it](){
                        m_listeners.call(&Listener::documentAdded, *it);
                    });
                }
                else
                {
                    // name is currently the only document field that can change.
                    if(new_doc.getName() != it->getName())
                    {
                        it->m_document.name = new_doc.getName();
                        
                        juce::MessageManager::callAsync([this, it](){
                            m_listeners.call(&Listener::documentChanged, *it);
                        });
                    }
                }
            }
        });
    }
    
    // ================================================================================ //
    //                                  DRIVE DOCUMENT                                  //
    // ================================================================================ //
    
    DocumentBrowser::Drive::DocumentSession::DocumentSession(DocumentBrowser::Drive const& parent,
                                                             Api::Document document) :
    m_drive(parent),
    m_document(std::move(document))
    {
        
    }
    
    DocumentBrowser::Drive::DocumentSession::~DocumentSession()
    {
        if(m_patcher_manager)
        {
            m_patcher_manager->removeListener(*this);
        }
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
    
    void DocumentBrowser::Drive::DocumentSession::patcherManagerRemoved(PatcherManager const& manager)
    {
        m_patcher_manager = nullptr;
    }
    
    bool DocumentBrowser::Drive::DocumentSession::operator==(DocumentSession const& other_doc) const
    {
        return (&m_drive == &other_doc.useDrive()) && (m_document == other_doc.m_document);
    }
    
    void DocumentBrowser::Drive::DocumentSession::open()
    {
        if(m_patcher_manager != nullptr)
        {
            m_patcher_manager->bringsFirstViewToFront();
        }
        else
        {
            const auto settings = getAppSettings().useDefault();
            
            std::string host = settings.getProperty("distant_server_host", "localhost")
            .toString().toStdString();
            
            std::string port_str = settings.getProperty("distant_server_flip_port", "9090").toString().toStdString();
            
            uint16_t port = std::stoul(port_str);
            
            m_patcher_manager = KiwiApp::useInstance().openRemotePatcher(host,
                                                                         port,
                                                                         m_document.session_id);
            
            /*
            m_patcher_manager = KiwiApp::useInstance().openRemotePatcher(m_drive.getHost(),
                                                                         m_drive.getPort(),
                                                                         m_session_id);
            */
            if(m_patcher_manager)
            {
                m_patcher_manager->addListener(*this);
            }
        }
    }
}
