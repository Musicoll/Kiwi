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
#include "KiwiApp_DocumentManager.hpp"
#include "../KiwiApp.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                 DOCUMENT BROWSER                                 //
    // ================================================================================ //
    
    DocumentBrowser::DocumentBrowser()
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
        std::vector<DocumentBrowser::Drive*> drives(m_drives.size());
        for(auto& drive_pair : m_drives)
        {
            drives.emplace_back(drive_pair.second.get());
        }
        
        return drives;
    }
    
    void DocumentBrowser::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void DocumentBrowser::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    std::string DocumentBrowser::getSessionMetadata(Session const& session,
                                                    std::string const& key,
                                                    std::string const& notfound)
    {
        assert(!key.empty());
        
        auto const& metadata = session.metadata;
        const auto it = metadata.find(key);
        return (it != metadata.end()) ? it->second : notfound;
    }
    
    void DocumentBrowser::timerCallback()
    {
        if(m_explorer == nullptr)
        {
            try
            {
                m_explorer = std::make_unique<flip::MulticastServiceExplorer>();
            }
            catch(std::runtime_error const& e)
            {
                m_explorer = nullptr;
                //std::cerr << "fail to initialize the MulticastServiceExplorer (you may check your connection)\n";
            }
        }
        
        process();
    }
    
    void DocumentBrowser::process()
    {
        if(m_explorer == nullptr) return; // abort
        
        m_explorer->process();
        auto const& explorer = *m_explorer.get();
        
        std::vector<Drive*> drives_added;
        std::vector<Drive*> drives_changed;
        std::vector<Drive*> drives_removed;
        
        // find and store removed drives
        for(auto const& drive_pair : m_drives)
        {
            Drive& drive = *drive_pair.second.get();
            if(std::find(explorer.begin(), explorer.end(), drive) == explorer.end())
            {
                drives_removed.emplace_back(&drive);
            }
        }
        
        // find and store new drives
        for(auto const& session : explorer)
        {
            const std::string hostname = getSessionMetadata(session, "computer_name");
            if(!m_drives.count(hostname))
            {
                auto it = m_drives.emplace(hostname, std::make_unique<Drive>(hostname, session));
                drives_added.emplace_back(it.first->second.get());
            }
            else
            {
                Drive& drive = *m_drives[hostname].get();
                
                // look for document session changes
                if(drive.processSession(session))
                {
                    drives_changed.emplace_back(&drive);
                }
            }
        }
        
        // drive added notification
        for(Drive* const drive : drives_added)
        {
            m_listeners.call(&Listener::driveAdded, *drive);
        }
        
        // drive changed notification
        for(Drive* const drive : drives_changed)
        {
            m_listeners.call(&Listener::driveChanged, *drive);
        }
        
        // drive removed notification
        for(Drive* const drive : drives_removed)
        {
            m_listeners.call(&Listener::driveRemoved, *drive);
            m_drives.erase(drive->getHostName());
        }
    }
    
    // ================================================================================ //
    //                              DOCUMENT BROWSER DRIVE                              //
    // ================================================================================ //
    
    DocumentBrowser::Drive::Drive(std::string hostname, DocumentBrowser::Session const& session) :
    m_ip(session.ip),
    m_port(session.port),
    m_version(session.version),
    m_hostname(hostname),
    m_is_remote(m_hostname != juce::SystemStats::getComputerName().toStdString())
    {
        processSession(session);
    };
    
    void DocumentBrowser::Drive::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void DocumentBrowser::Drive::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    uint16_t DocumentBrowser::Drive::getPort() const
    {
        return m_port;
    }
    
    uint32_t DocumentBrowser::Drive::getIp() const
    {
        return m_ip;
    }
    
    std::string DocumentBrowser::Drive::getHost() const
    {
        const uint32_t ip_address = getIp();
        char host[16];
        
        if(ip_address)
        {
            snprintf(host, sizeof host, "%u.%u.%u.%u",
                     (ip_address & 0xff000000) >> 24,
                     (ip_address & 0x00ff0000) >> 16,
                     (ip_address & 0x0000ff00) >> 8,
                     (ip_address & 0x000000ff));
        }
        
        return host;
    }
    
    std::string DocumentBrowser::Drive::getHostName() const
    {
        return m_hostname;
    }
    
    bool DocumentBrowser::Drive::isRemote() const
    {
        return m_is_remote;
    }
    
    void DocumentBrowser::Drive::createNewDocument() const
    {
        KiwiApp::useInstance().openRemotePatcher(getHost(), getPort(), m_new_session_id);
    }
    
    std::list<DocumentBrowser::Drive::DocumentSession> const& DocumentBrowser::Drive::getDocuments() const
    {
        return m_documents;
    }
    
    std::list<DocumentBrowser::Drive::DocumentSession>& DocumentBrowser::Drive::getDocuments()
    {
        return m_documents;
    }
    
    bool DocumentBrowser::Drive::operator==(Session const& session) const
    {
        return (m_ip == session.ip) && (m_port == session.port);
    }
    
    bool DocumentBrowser::Drive::operator==(Drive const& drive) const
    {
        return (m_ip == drive.m_ip) && (m_port == drive.m_port);
    }
    
    bool DocumentBrowser::Drive::processSession(DocumentBrowser::Session const& session)
    {
        assert(*this == session);
        
        bool changed = false;
        std::vector<DocumentSession> new_documents;
        
        const auto getMeta = &DocumentBrowser::getSessionMetadata;
        
        m_new_session_id = std::stoull(getMeta(session, "new_session_id", "12345"));

        const juce::String files = getMeta(session, "backend_files_list", "");
        juce::var json_files;
        if(juce::JSON::parse(files, json_files).wasOk())
        {
            if(json_files.isArray())
            {
                for(juce::var const& var : *json_files.getArray())
                {
                    if(var.isObject())
                    {
                        const auto obj = var.getDynamicObject();
                        const auto name = obj->getProperty("name").toString().toStdString();
                        const auto session_id = obj->getProperty("session_id").toString().toStdString();
                        
                        new_documents.emplace_back(*this, name, std::stoull(session_id));
                    }
                }
            }
        }
        
        // drive removed notification
        for(auto doc_it = m_documents.begin(); doc_it != m_documents.end();)
        {
            auto it = std::find(new_documents.begin(), new_documents.end(), *doc_it);
            
            if(it == new_documents.end())
            {
                m_listeners.call(&Listener::documentRemoved, *doc_it);
                
                doc_it = m_documents.erase(doc_it);
                changed = true;
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
                m_listeners.call(&Listener::documentAdded, *it);
                changed = true;
            }
            else
            {
                // name is currently the only document field that can change.
                if(new_doc.getName() != it->getName())
                {
                    it->m_name = new_doc.getName();
                    m_listeners.call(&Listener::documentChanged, *it);
                    changed = true;
                }
            }
        }
        
        return changed;
    }
    
    bool operator==(DocumentBrowser::Session const& session, DocumentBrowser::Drive const& drive)
    {
        return (drive.getIp() == session.ip) && (drive.getPort() == session.port);
    }
    
    // ================================================================================ //
    //                                  DRIVE DOCUMENT                                  //
    // ================================================================================ //
    
    DocumentBrowser::Drive::DocumentSession::DocumentSession(DocumentBrowser::Drive const& parent,
                                                             std::string name, uint64_t session_id) :
    m_drive(parent),
    m_name(name),
    m_session_id(session_id)
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
        return m_name;
    }
    
    uint64_t DocumentBrowser::Drive::DocumentSession::getSessionId() const
    {
        return m_session_id;
    }
    
    DocumentBrowser::Drive const& DocumentBrowser::Drive::DocumentSession::useDrive() const
    {
        return m_drive;
    }
    
    void DocumentBrowser::Drive::DocumentSession::patcherManagerRemoved(PatcherManager const& manager)
    {
        m_patcher_manager = nullptr;
    }
    
    bool DocumentBrowser::Drive::DocumentSession::rename(std::string const& new_name)
    {
        const bool ok = KiwiApp::useInstance().useServer().renameDocumentSession(m_session_id, new_name);
        
        std::cout << "doc renamed OK !!! = " << std::to_string(ok) << "\n";
        
        if(ok && m_patcher_manager != nullptr)
        {
            model::Patcher& patcher = m_patcher_manager->getPatcher();
            patcher.setName(new_name);
            DocumentManager::commit(patcher);
            std::cout << "doc renamed !!!\n";
        }
        
        return ok;
    }
    
    bool DocumentBrowser::Drive::DocumentSession::operator==(DocumentSession const& other_doc) const
    {
        return (&m_drive == &other_doc.useDrive()) && (m_session_id == other_doc.m_session_id);
    }
    
    void DocumentBrowser::Drive::DocumentSession::open()
    {
        if(m_patcher_manager != nullptr)
        {
            m_patcher_manager->bringsFirstViewToFront();
        }
        else
        {
            m_patcher_manager = KiwiApp::useInstance().openRemotePatcher(m_drive.getHost(),
                                                                         m_drive.getPort(),
                                                                         m_session_id);
            m_patcher_manager->addListener(*this);
        }
    }
}
