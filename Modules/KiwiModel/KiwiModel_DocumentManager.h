/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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

#include <flip/History.h>
#include <flip/HistoryStoreMemory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                   DOCUMENT MANAGER                               //
    // ================================================================================ //
    
    class DocumentManager
    {
    public:
        
        //! @brief Constructor.
        DocumentManager(flip::DocumentBase& document);
        
        //! @brief Destructor.
        ~DocumentManager();
        
        //! @brief Commit and push.
        //! @see startCommitGesture, endCommitGesture.
        static void commit(flip::Type& type,
                           std::string action = std::string());
        
        //! @brief  Connect the DocumentManager to a remote server
        static void connect(flip::Type& type,
                            const std::string host,
                            uint16_t port,
                            uint64_t session_id);
        
        //! @brief Pull changes from remote server
        static void pull(flip::Type& type);
        
        //! @brief Starts a commit gesture.
        //! @details Each call to this function must be followed by a call to endCommitGesture.
        //! @see endCommitGesture, commitGesture
        static void startCommitGesture(flip::Type& type);
        
        //! @brief Commit a gesture.
        //! @param label The label of the current gesture.
        //! @see startCommitGesture, endCommitGesture.
        static void commitGesture(flip::Type& type, std::string label);
        
        //! @brief Ends a commit gesture.
        //! @details Each call to this function must be preceded by a call to startCommitGesture.
        //! @see startCommitGesture.
        static void endCommitGesture(flip::Type& type);
        
        //! @brief Returns true if the document is currently commiting a gesture.
        static bool isInCommitGesture(flip::Type& type);
        
        //! @brief Returns true if there is an action to undo.
        bool canUndo();
        
        //! @brief Returns the label of the last undo action.
        std::string getUndoLabel();
        
        //! @brief Undo the last action.
        void undo();
        
        //! @brief Returns true if there is an action to redo.
        bool canRedo();
        
        //! @brief Returns the label of the next redo action.
        std::string getRedoLabel();
        
        //! @brief Redo the next action.
        void redo();
        
        //! @brief Returns the object's pointer or nullptr if not found in document.
        template<class T> T* get(flip::Ref const& ref)
        {
            return m_document.object_ptr<T>(ref);
        }
        
    private:
        
        //! @brief Commmits and pushes a transaction
        void commit(std::string label);
        
        //! @brief Pulls transactions stacked by a socket's process
        void pull();
        
        //! @brief Pushes a trasactions stacked by a socket's process
        void push();
        
        //! @brief Starts a commit gesture.
        void startCommitGesture();
        
        //! @brief Commit a gesture.
        void commitGesture(std::string label);
        
        //! @brief Ends a commit gesture.
        void endCommitGesture();
        
        class Session;
        
    private:
        
        flip::DocumentBase&                     m_document;
        flip::History<flip::HistoryStoreMemory> m_history;
        std::unique_ptr<Session>                m_session = nullptr;
        
    private:
        
        DocumentManager() = delete;
        DocumentManager(const DocumentManager& rhs) = delete;
        DocumentManager(DocumentManager&& rhs) = delete;
        DocumentManager& operator =(const DocumentManager& rhs) = delete;
        DocumentManager& operator =(DocumentManager&& rhs) = delete;
        bool operator ==(DocumentManager const& rhs) const = delete;
        bool operator !=(DocumentManager const& rhs) const = delete;
    };
    
    // ================================================================================ //
    //                                      SESSION                                     //
    // ================================================================================ //
    
    //! @brief The Session is used internally by the DocumentManager to
    //! handle the gesture commits.
    //! @details A session manages its own transaction stack,
    //! and squash each new transaction into a single one.
    //! @see DocumentManager::startCommitGesture, DocumentManager::commitGesture, DocumentManager::endCommitGesture, DocumentManager::isInCommitGesture
    class DocumentManager::Session
    {
    public:
        Session(flip::DocumentBase& document);
        virtual ~Session();
        
        //! @brief Starts a change
        void start();
        
        //! @brief Commits a change without a transaction label
        void commit();
        
        //! @brief Commits a change with a transaction label
        void commit(std::string label);
        
        //! @brief Clear history
        void end(flip::History<flip::HistoryStoreMemory>* master_history = nullptr);
        
        //! @brief Reverts all changes
        void revert();
        
    private:
        
        flip::DocumentBase& m_document;
        flip::History<flip::HistoryStoreMemory> m_history;
        flip::Transaction m_tx;
    };
}}
