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

#pragma once

#include <KiwiTool/KiwiTool_Listeners.h>

#include <juce_data_structures/juce_data_structures.h>
#include <juce_events/juce_events.h>

#include "../KiwiApp_Network/KiwiApp_Api.h"

#include "../KiwiApp_General/KiwiApp_StoredSettings.h"

#include "KiwiApp_ApiController.h"

namespace kiwi
{
    // ================================================================================ //
    //                                 DOCUMENT BROWSER                                 //
    // ================================================================================ //
    
    //! @brief Request Patcher document informations through a Kiwi API.
    class DocumentBrowser
    : public juce::Timer
    , public NetworkSettings::Listener
    , public ApiConnectStatusListener
    {
    public: // nested classes
        
        struct Listener;
        class Drive;
        
    public: // methods
        
        //! @brief Constructor
        DocumentBrowser();
        
        //! @brief Destructor
        ~DocumentBrowser();
        
        //! @brief start processing
        void start(const int interval = 5000);
        
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
        
    private: // methods
        
        void networkSettingsChanged(NetworkSettings const&, const juce::Identifier& id) override;
        
        void userLoggedIn(Api::AuthUser const&) override;
        void userLoggedOut(Api::AuthUser const&) override;
        void authUserChanged(Api::AuthUser const&) override;
        
    private: // variables
        
        std::unique_ptr<Drive>                          m_distant_drive;
        tool::Listeners<Listener>                     m_listeners = {};
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
        
        using DocumentSessions = std::vector<std::unique_ptr<DocumentSession>>;
        
    public: // methods
        
        Drive(std::string const& name, uint16_t session_port);
        
        ~Drive() = default;
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
        //! @brief Set the kiwi document session port.
        void setSessionPort(uint16_t port);
        
        //! @brief Returns the kiwi document session port.
        uint16_t getSessionPort() const;
        
        //! @brief Set the name of this drive.
        void setName(std::string const& host);
        
        //! @brief Returns the name of this drive.
        std::string const& getName() const;
        
        //! @brief Creates and opens a new document on this drive.
        void createNewDocument();
        
        //! @brief Returns the documents.
        DocumentSessions const& getDocuments() const;
        
        //! @brief Returns the documents.
        DocumentSessions& getDocuments();
        
        //! @brief Returns true if the drive match the other drive
        //! @details this operator only compares ip and port.
        bool operator==(Drive const& drive) const;
        
        //! @brief Refresh all the document list.
        void refresh();
        
    private: // members
        
        //! @internal Update the document list (need to be called in the juce Message thread)
        void updateDocumentList(Api::Documents docs);
        
        uint16_t                    m_session_port = 9090;
        std::string                 m_name = "Drive";
        DocumentSessions            m_documents;
        tool::Listeners<Listener>   m_listeners;
        
        friend class DocumentBrowser;
    };
    
    // ================================================================================ //
    //                           DOCUMENT BROWSER LISTENER                           //
    // ================================================================================ //
    
    //! @brief Listen to document browser changes.
    struct DocumentBrowser::Drive::Listener
    {
        //! @brief Destructor.
        virtual ~Listener() = default;
        
        //! @brief Called when a document session has been added.
        virtual void documentAdded(DocumentBrowser::Drive::DocumentSession& doc) {};
        
        //! @brief Called when a document session changed.
        virtual void documentChanged(DocumentBrowser::Drive::DocumentSession& doc) {};
        
        //! @brief Called when a document session has been removed.
        virtual void documentRemoved(DocumentBrowser::Drive::DocumentSession& doc) {};
        
        //! @brief Called when one or more documents has been added, removed or changed.
        virtual void driveChanged() {};
    };
    
    // ================================================================================ //
    //                                  DRIVE DOCUMENT                                  //
    // ================================================================================ //
    
    class DocumentBrowser::Drive::DocumentSession
    {
    public: // methods
        
        //! @brief Constructor.
        DocumentSession(DocumentBrowser::Drive& parent, Api::Document document);
        
        //! @brief Destructor.
        ~DocumentSession();
        
        //! @brief Returns the document drive
        Drive& useDrive();
        
        //! @brief Tells the Kiwi instance to open up this document.
        void open();
        
        //! @brief Returns the document name
        std::string getName() const;
        
        //! @brief Returns the session id of the document.
        uint64_t getSessionId() const;
        
        //! @brief Returns the drive that holds this document.
        DocumentBrowser::Drive const& useDrive() const;
        
        //! @brief Rename the document.
        void rename(std::string const& new_name);
        
        //! @brief Returns true if the DocumentSession match another DocumentSession
        //! @details this operator uses the session_id field to compare.
        bool operator==(DocumentSession const& other_doc) const;
        
    private: // members
        
        DocumentBrowser::Drive&             m_drive;
        Api::Document                       m_document;
        
        friend class DocumentBrowser::Drive;
    };
}
