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

#include <unordered_set>
#include <memory>

#include <juce_gui_extra/juce_gui_extra.h>

#include <flip/Document.h>
#include <flip/DocumentObserver.h>

#include <KiwiModel/KiwiModel_PatcherUser.h>
#include <KiwiModel/KiwiModel_PatcherValidator.h>

#include <KiwiApp_Network/KiwiApp_DocumentBrowser.h>
#include <KiwiApp_Network/KiwiApp_CarrierSocket.h>

namespace kiwi
{
    class Instance;
    class PatcherView;
    
    // ================================================================================ //
    //                                  PATCHER MANAGER                                 //
    // ================================================================================ //
    
    //! @brief The main DocumentObserver.
    //! @details The Instance dispatch changes to all other DocumentObserver objects
    class PatcherManager :  public flip::DocumentObserver<model::Patcher>,
                            public DocumentBrowser::Drive::Listener
    {
    public: // nested classes
        
        struct Listener;
        
    public: // methods
        
        //! @brief Constructor.
        PatcherManager(Instance& instance);
        
        //! @brief Destructor.
        ~PatcherManager();
        
        //! @brief Try to connect this patcher to a remote server.
        void connect(DocumentBrowser::Drive::DocumentSession& session);
        
        //! @brief Pull changes from server if it is remote.
        void pull();
        
        //! @brief Load patcher datas from file.
        void loadFromFile(juce::File const& file);
        
        //! @brief Save the document.
        //! @details Returns true if saving document succeeded false otherwise.
        bool saveDocument();
        
        //! @brief Returns true if the patcher needs to be saved.
        bool needsSaving() const noexcept;
        
        //! @brief Returns the file currently used to save document.
        juce::File const& getSelectedFile() const;
        
        //! @brief Returns the Patcher model
        model::Patcher& getPatcher();
        
        //! @brief Returns the Patcher model
        model::Patcher const& getPatcher() const;
        
        //! @brief Returns true if the this is a remotely connected document.
        bool isRemote() const noexcept;
        
        //! @brief Returns the session ID of the document.
        //! @details This function returns 0 if the document is loaded from disk or memory.
        //! @see isRemote
        uint64_t getSessionId() const noexcept;
        
        //! @brief Returns the name of the document.
        //! @details This function returns 0 if the document is loaded from disk or memory.
        //! @see isRemote
        std::string getDocumentName() const;
        
        //! @brief Returns the number of users connected to the patcher document.
        size_t getNumberOfUsers();
        
        //! @brief Returns the list of users connected to the patcher document.
        std::unordered_set<uint64_t> getConnectedUsers();
        
        //! @brief Returns the number of patcher views.
        size_t getNumberOfView();
        
        //! @brief create a new patcher view window.
        void newView();
        
        //! @brief Brings the first patcher view to front.
        void bringsFirstViewToFront();
        
        //! @brief Force all windows to close without asking user to save document.
        void forceCloseAllWindows();
        
        //! @brief Attempt to close all document windows, after asking user to save them if needed.
        //! @return True if all document have been closed, false if the user cancel the action.
        bool askAllWindowsToClose();
        
        //! @brief Close the window that contains a given patcherview.
        //! @details if it's the last patcher view, it will ask the user the save the document before closing if needed.
        bool closePatcherViewWindow(PatcherView& patcherview);
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
        //! @brief Called when a document session has been added.
        void documentAdded(DocumentBrowser::Drive::DocumentSession& doc) override;
        
        //! @brief Called when a document session changed.
        void documentChanged(DocumentBrowser::Drive::DocumentSession& doc) override;
        
        //! @brief Called when a document session has been removed.
        void documentRemoved(DocumentBrowser::Drive::DocumentSession& doc) override;
        
    private:
        
        //! @internal Write data into file.
        void writeDocument();
        
        //! @internal Reads data from file.
        void readDocument();
        
        //! @internal If the patcher is remote, tries to disconnect it.
        void disconnect();
        
        //! @internal flip::DocumentObserver<model::Patcher>::document_changed
        void document_changed(model::Patcher& patcher) override final;
        
        //! @internal Notify and create PatcherViews.
        void notifyPatcherViews(model::Patcher& patcher);
        
        //! @internal React to the fact that a View has just been added to the document.
        //! @details create a PatcherViewWindow.
        void createPatcherWindow(model::Patcher& patcher,
                                 model::Patcher::User const& user,
                                 model::Patcher::View& view);
        
        //! @internal View is resident and internal value changed.
        void notifyPatcherView(model::Patcher& patcher,
                               model::Patcher::User const& user,
                               model::Patcher::View& view);
        
        //! @internal Vser will be removed from the document.
        void removePatcherWindow(model::Patcher& patcher,
                                 model::Patcher::User const& user,
                                 model::Patcher::View& view);
        
        //! @internal Save document if needed and if user agrees.
        juce::FileBasedDocument::SaveResult saveIfNeededAndUserAgrees();

    private: // members
        
        Instance&                                   m_instance;
        model::PatcherValidator                     m_validator;
        flip::Document                              m_document;
        juce::File                                  m_file;
        std::unique_ptr<CarrierSocket>              m_socket;
        bool                                        m_need_saving_flag;
        DocumentBrowser::Drive::DocumentSession*    m_session;
        
        flip::SignalConnection                      m_user_connected_signal_cnx;
        flip::SignalConnection                      m_user_disconnected_signal_cnx;
        flip::SignalConnection                      m_receive_connected_users_signal_cnx;
        
        std::unordered_set<uint64_t>                m_connected_users;
        
        engine::Listeners<Listener>                 m_listeners;
    };
    
    // ================================================================================ //
    //                              PATCHER MANAGER LISTENER                            //
    // ================================================================================ //
 
    struct PatcherManager::Listener
    {
        virtual ~Listener() {};
        
        //! @brief Called when one or more users are connecting or disconnecting to the Patcher Document.
        virtual void connectedUserChanged(PatcherManager& manager) {};
    };
}
