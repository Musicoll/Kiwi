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

#ifndef KIWI_APP_DOCUMENT_BROWSER_HPP_INCLUDED
#define KIWI_APP_DOCUMENT_BROWSER_HPP_INCLUDED

#include <KiwiEngine/KiwiEngine_Listeners.hpp>

#include <juce_events/juce_events.h>

#include "../KiwiApp_PatcherManager.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                 DOCUMENT BROWSER                                 //
    // ================================================================================ //
    
    //! @brief Explore service running on the Local Area Network.
    class DocumentBrowser : public juce::Timer
    {
    public: // nested classes
        
        struct Listener;
        class Drive;
        
    public: // methods
        
        struct Session
        {
            uint32_t    ip;
            uint16_t    port;
            std::string version;
            std::map<std::string, std::string> metadata;
        };
        
        //! @brief Constructor
        DocumentBrowser();
        
        //! @brief Destructor
        ~DocumentBrowser();
        
        //! @brief start processing
        void start(const int interval = 10);
        
        //! @brief stop processing
        void stop();
        
        //! @brief Scan the LAN to find a service provider.
        void process();
        
        //! @brief juce::Timer callback.
        void timerCallback() override;
        
        //! @brief Returns a list of drives.
        std::vector<Drive*> getDrives() const;
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
        //! @brief Retrieves a metadata entry in a session.
        static std::string getSessionMetadata(Session const& session,
                                              std::string const& key,
                                              std::string const& notfound = "");
        
    private: // members
        
        std::map<std::string, std::unique_ptr<Drive>>   m_drives = {};
        engine::Listeners<Listener>                     m_listeners = {};
    };
    
    // ================================================================================ //
    //                              DOCUMENT BROWSER LISTENER                           //
    // ================================================================================ //
    
    //! @brief Listen to document explorer changes.
    struct DocumentBrowser::Listener
    {
        //! @brief Destructor.
        virtual ~Listener() = default;
        
        //! @brief Called when the document list changed.
        virtual void driveAdded(DocumentBrowser::Drive& drive) = 0;
        
        //! @brief Called when the document list changed.
        virtual void driveChanged(DocumentBrowser::Drive const& drive) = 0;
        
        //! @brief Called when the document list changed.
        virtual void driveRemoved(DocumentBrowser::Drive const& drive) = 0;
    };
    
    // ================================================================================ //
    //                              DOCUMENT BROWSER DRIVE                              //
    // ================================================================================ //
    
    class DocumentBrowser::Drive
    {
    public: // nested classes
        
        struct Listener;
        class DocumentSession;
        
    public: // methods
        
        Drive(std::string hostname, DocumentBrowser::Session const& session);
        ~Drive() = default;
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
        //! @brief Returns the session port.
        uint16_t getPort() const;
        
        //! @brief Returns the session ip.
        uint32_t getIp() const;
        
        //! @brief Returns the session host.
        std::string getHost() const;
        
        //! @brief Returns the session hostname.
        std::string getHostName() const;
        
        //! @brief Returns true if this is a remote drive.
        bool isRemote() const;
        
        //! @brief Creates and opens a new document on this drive.
        void createNewDocument() const;
        
        //! @brief Returns the documents.
        std::list<DocumentSession> const& getDocuments() const;
        
        //! @brief Returns true if the drive match the session
        //! @details this operator only compares ip and port.
        bool operator==(Session const& session) const;
        
        //! @brief Returns true if the drive match the other drive
        //! @details this operator only compares ip and port.
        bool operator==(Drive const& drive) const;
        
    private: // methods
        
        bool processSession(DocumentBrowser::Session const& session);
        
    private: // members
        
        uint32_t                    m_ip;
        uint16_t                    m_port;
        std::string                 m_version;
        std::string                 m_hostname;
        uint64_t                    m_new_session_id;
        bool                        m_is_remote;
        std::list<DocumentSession>  m_documents;
        engine::Listeners<Listener> m_listeners;
        
        friend class DocumentBrowser;
    };
    
    bool operator==(DocumentBrowser::Session const& session, DocumentBrowser::Drive const& drive);
    
    // ================================================================================ //
    //                           DOCUMENT BROWSER LISTENER                           //
    // ================================================================================ //
    
    //! @brief Listen to document browser changes.
    struct DocumentBrowser::Drive::Listener
    {
        //! @brief Destructor.
        virtual ~Listener() = default;
        
        //! @brief Called when a document session has been added.
        virtual void documentAdded(DocumentBrowser::Drive::DocumentSession const& doc) = 0;
        
        //! @brief Called when a document session changed.
        virtual void documentChanged(DocumentBrowser::Drive::DocumentSession const& doc) = 0;
        
        //! @brief Called when a document session has been removed.
        virtual void documentRemoved(DocumentBrowser::Drive::DocumentSession const& doc) = 0;
    };
    
    // ================================================================================ //
    //                                  DRIVE DOCUMENT                                  //
    // ================================================================================ //
    
    class DocumentBrowser::Drive::DocumentSession : public PatcherManager::Listener
    {
    public: // methods
        
        //! @brief Constructor.
        DocumentSession(DocumentBrowser::Drive const& parent, std::string name, uint64_t session_id);
        
        //! @brief Destructor.
        ~DocumentSession();
        
        //! @brief Tells the Kiwi instance to open up this document.
        void open();
        
        //! @brief Returns the document name
        std::string getName() const;
        
        //! @brief Returns the session id of the document.
        uint64_t getSessionId() const;
        
        //! @brief Returns the drive that holds this document.
        DocumentBrowser::Drive const& useDrive() const;
        
        //! @brief Called when the PatcherManager is closed.
        void patcherManagerRemoved(PatcherManager const& manager) override;
        
        //! @brief Returns true if the DocumentSession match another DocumentSession
        //! @details this operator uses the session_id field to compare.
        bool operator==(DocumentSession const& other_doc) const;
        
    private: // members
        
        DocumentBrowser::Drive const&       m_drive;
        std::string                         m_name;
        uint64_t                            m_session_id;
        //std::map<std::string, std::string>  m_metadata;
        PatcherManager*                     m_patcher_manager = nullptr;
        
        friend class DocumentBrowser::Drive;
    };
}


#endif // KIWI_APP_DOCUMENT_BROWSER_HPP_INCLUDED
