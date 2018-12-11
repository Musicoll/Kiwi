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
    class PatcherViewWindow;
    
    // ================================================================================ //
    //                                  PATCHER MANAGER                                 //
    // ================================================================================ //
    
    //! @brief The main DocumentObserver.
    //! @details The Instance dispatch changes to all other DocumentObserver objects
    class PatcherManager
    : public flip::DocumentObserver<model::Patcher>
    , public DocumentBrowser::Drive::Listener
    {
    public: // nested classes
        
        struct Listener;
        
    public: // methods
        
        //! @brief Constructor.
        PatcherManager(Instance& instance, std::string const& name);
        
        //! @brief Destructor.
        ~PatcherManager();
        
        //! @brief Try to create a patcher manager from a file
        //! @details This method throws a std::runtime exception if error occurs during read.
        //! @return A PatcherManager if succeed, nullptr otherwise.
        static std::unique_ptr<PatcherManager> createFromFile(Instance& instance,
                                                              juce::File const& file);
        
        //! @brief Try to connect this patcher to a remote server.
        bool connect(std::string const& host, uint16_t port, DocumentBrowser::Drive::DocumentSession& session);
        
        //! @brief Disconnects the patcher manager.
        void disconnect();
        
        //! @brief Pull changes from server if it is remote.
        void pull();
        
        //! @brief Save the document.
        //! @details Returns true if saving document succeeded false otherwise.
        bool saveDocument(bool save_as);
        
        //! @brief Save the document as another document.
        //! @details Returns true if saving document succeeded false otherwise.
        bool saveDocumentAs();
        
        //! @brief Returns true if the patcher needs to be saved.
        bool needsSaving() const noexcept;
        
        //! @brief Returns the file currently used to save document.
        juce::File const& getSelectedFile() const;
        
        //! @brief Returns the Patcher model
        model::Patcher& getPatcher();
        
        //! @brief Returns the Patcher model
        model::Patcher const& getPatcher() const;
        
        //! @brief Returns true if the this is a remotely connected document.
        bool isConnected() const noexcept;
        
        //! @brief Returns the session ID of the document.
        //! @details This function returns 0 if the document is loaded from disk or memory.
        //! @see isConnected
        uint64_t getSessionId() const noexcept;
        
        //! @brief Returns the name of the document.
        //! @details This function returns 0 if the document is loaded from disk or memory.
        //! @see isConnected
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
        
        //! @brief Attempt to close all document windows, after asking user to save them if needed.
        //! @return True if all document have been closed, false if the user cancel the action.
        bool askAllWindowsToClose();
        
        //! @brief Returns the first window of the patcher manager.
        PatcherViewWindow & getFirstWindow();
        
        //! @brief Close the window that contains a given patcherview.
        //! @details if it's the last patcher view, it will ask the user the save the document before closing if needed.
        void closePatcherViewWindow(PatcherView& patcherview);
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
        //! @brief Force all windows to close without asking user to save document.
        void forceCloseAllWindows();
        
        //! @brief This method is used to reset the stack count when a stack-overflow has previously been detected.
        //! @details it will signal the Patcher model
        void clearStackOverflow();
        
        bool hasStackOverflow() const;
        
    private:
        
        //! @brief Called when a document session has been added.
        void documentAdded(DocumentBrowser::Drive::DocumentSession& doc) override;
        
        //! @brief Called when a document session changed.
        void documentChanged(DocumentBrowser::Drive::DocumentSession& doc) override;
        
        //! @brief Called when the drive connection status changed.
        void driveConnectionStatusChanged(bool is_online) override;
        
        //! @internal Called from socket process to notify changing state.
        void onStateTransition(flip::CarrierBase::Transition transition, flip::CarrierBase::Error error);
        
        //! @brief Called when a stack-overflow is detected
        void onStackOverflowDetected(std::vector<flip::Ref> refs);
        
        //! @brief Called when a stack-overflow is cleared
        void onStackOverflowCleared();
        
        //! @internal Write data into file.
        void writeDocument();
        
        //! @internal Reads from binary data file.
        bool readBackEndBinary(flip::DataProviderBase& data_provider);
        
        //! @brief Load patcher datas from file.
        //! @details Loading document from file could fail if the file can not be read.
        //! This could happen if the file has not a valid extension,
        //! or if the document version is incompatible.
        //! @exception Throws a std::runtime exception if file loading fail.
        bool loadFromFile(juce::File const& file);
        
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
        //! returns true if user wants to continue editing.
        bool saveIfNeededAndUserAgrees();
        
        //! @internal Updates the title bar of specific view.
        void updateTitleBar(model::Patcher::View & view);
        
        //! @internal Updates windows title bars according to remote state,
        //! need saving flag, name and selected file.
        void updateTitleBars();
        
        //! @internal Sets the need saving flags. Updates title bar if requested.
        void setNeedSaving(bool need_saving);
        
        //! @internal Sets the patcher manager's name. Updates title bar if requested.
        void setName(std::string const& name);

    private: // members
        
        std::string                                 m_name;
        Instance&                                   m_instance;
        model::PatcherValidator                     m_validator;
        flip::Document                              m_document;
        juce::File                                  m_file;
        CarrierSocket                               m_socket;
        bool                                        m_need_saving_flag = false;
        DocumentBrowser::Drive::DocumentSession*    m_session = nullptr;
        
        flip::SignalConnection                      m_user_connected_signal_cnx;
        flip::SignalConnection                      m_user_disconnected_signal_cnx;
        flip::SignalConnection                      m_receive_connected_users_signal_cnx;
        flip::SignalConnection                      m_stack_overflow_detected_signal_cnx;
        flip::SignalConnection                      m_stack_overflow_cleared_signal_cnx;
        
        std::unordered_set<uint64_t>                m_connected_users;
        
        bool                                        m_has_stack_overflow = false;
        
        tool::Listeners<Listener>                   m_listeners;
    };
    
    // ================================================================================ //
    //                              PATCHER MANAGER LISTENER                            //
    // ================================================================================ //
 
    struct PatcherManager::Listener
    {
        virtual ~Listener() {};
        
        //! @brief Called when one or more users are connecting or disconnecting to the Patcher Document.
        virtual void connectedUserChanged(PatcherManager& manager) {};
        
        //! @brief Called when a stack-overflow is detected.
        virtual void stackOverflowDetected(PatcherManager& manager, std::vector<flip::Ref> culprits) {};
        
        //! @brief Called when a stack-overflow is cleared.
        virtual void stackOverflowCleared(PatcherManager& manager) {};
    };
}
