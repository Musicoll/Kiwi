/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright(c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2(or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_ENGINE_DOCUMENT_MANAGER_HPP_INCLUDED
#define KIWI_ENGINE_DOCUMENT_MANAGER_HPP_INCLUDED

#include "flip/History.h"
#include "flip/HistoryStoreMemory.h"

#include "KiwiCarrierSocket.hpp"
#include "KiwiTimer.hpp"

namespace kiwi
{
    namespace engine
    {
        class Instance;
    }
    
    class DocumentManager final
    {
    public:
        
        DocumentManager(flip::DocumentBase & document, engine::Instance const& instance);
        
        ~DocumentManager();
        
        //! @brief Commit and push.
        //! @see startCommitGesture, endCommitGesture.
        static void commit(flip::Type& type, std::string action = std::string());
        
        //! @brief  Connect the DocumentManager to a remote server
        static void connect(flip::Type& type, const std::string host, uint16_t port);
        
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
        bool hasUndo();
        
        //! @brief Returns the label of the last undo action.
        std::string getUndoLabel();
        
        //! @brief Undo the last action.
        void undo();
        
        //! @brief Returns true if there is an action to redo.
        bool hasRedo();
        
        //! @brief Returns the label of the next redo action.
        std::string getRedoLabel();
        
        //! @brief Redo the next transaction.
        void redo();
        
        //! @brief Called at a regular frequency to pull document
        void tick();
        
    private:
        
        //! @brief Commmits and pushes a transaction
        void commit(std::string action);
        
        //! @brief Pulls transactions stacked by a socket's process
        void pull();
        
        //! @brief Pushes a trasactions stacked by a socket's process
        void push();
        
        //! @brief Connects the document manager and download the patcher's initial state
        void connect(std::string const host, uint16_t port);
        
        //! @brief Returns true if the document manager is connected false otherwise
        bool isConnected();
        
        //! @brief Disconnects the document manager from the server
        void disconnect();
        
        //! @brief Starts a timer callback on the message thread that that pulls
        void startPulling();
        
        //! @brief Stops the pulling callback
        void stopPulling();
        
        //! @brief Called once document manager is connected and starts pulling
        void onConnected();
        
        //! @brief Called once document manager is disconnected and stops pulling
        void onDisconnected();
        
        //! @brief Called once the initial load happened
        void onLoaded();
        
        void startCommitGesture();
        void commitGesture(std::string action);
        void endCommitGesture();
        
    private:
        
        engine::Instance const&                 m_instance;
        flip::DocumentBase&                     m_document;
        flip::History<flip::HistoryStoreMemory> m_history;
        CarrierSocket                           m_socket;
        
        bool                                    m_gesture_flag = false;
        size_t                                  m_gesture_cnt = 0;
        std::atomic_bool                        m_loaded;
        
        std::unique_ptr<engine::Timer>          m_timer;
        
    private:
        
        DocumentManager() = delete;
        DocumentManager(const DocumentManager& rhs) = delete;
        DocumentManager(DocumentManager&& rhs) = delete;
        DocumentManager& operator =(const DocumentManager& rhs) = delete;
        DocumentManager& operator =(DocumentManager&& rhs) = delete;
        bool operator ==(DocumentManager const& rhs) const = delete;
        bool operator !=(DocumentManager const& rhs) const = delete;
    };
}

#endif // KIWI_ENGINE_DOCUMENT_MANAGER_HPP_INCLUDED
