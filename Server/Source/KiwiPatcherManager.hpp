/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_SERVER_PATCHER_MANAGER_HPP_INCLUDED
#define KIWI_SERVER_PATCHER_MANAGER_HPP_INCLUDED

#include <atomic>
#include <thread>

#include "KiwiPatcherValidator.hpp"

#include <KiwiModel/KiwiPatcher.hpp>

#include <flip/DocumentServer.h>
#include <flip/contrib/transport_tcp/PortTransportServerTcp.h>
#include <flip/detail/PortBase.h>
#include <flip/DocumentObserver.h>

namespace kiwi
{
    namespace server
    {
        //! @brief The server Patcher manager.
        class PatcherManager
        {
        public:
            
            //! @brief Constructor.
            PatcherManager(uint64_t document_id);
            
            //! @brief Destructor.
            ~PatcherManager();
            
        private: // methods
            
            //! @brief Launches a thread that continuously processes the document's socket.
            void launchTransport();
            
            //! @brief Functions called by the processing thread to process the socket.
            void runTransport();
            
            //! @brief Joins the thread processing the socket.
            void stopTransport();
            
            //! @brief Returns the Patcher model.
            model::Patcher& getPatcher();
            
            //! @brief Called when a client is connecting to the document.
            void on_connecting(flip::PortBase& port);
            
            //! @brief Called once a client is connected to the document.
            void on_connected(flip::PortBase& port);
            
            //! @brief Called once a client is disconnected.
            void on_disconnected(flip::PortBase& port);
            
        private: // members
            
            PatcherValidator                m_validator;
            flip::DocumentServer            m_document;
            flip::PortTransportServerTcp    m_transport;
            std::thread                     m_transport_loop;
            std::atomic_bool                m_transport_running;
            
        private: // deleted methods
            
            PatcherManager(PatcherManager const& other) = delete;
            PatcherManager(PatcherManager && other) = delete;
            PatcherManager &operator=(PatcherManager const& other) = delete;
            PatcherManager &operator=(PatcherManager && other) = delete;
        };
    }
}

#endif //KIWI_SERVER_PATCHER_MANAGER_HPP_INCLUDED
