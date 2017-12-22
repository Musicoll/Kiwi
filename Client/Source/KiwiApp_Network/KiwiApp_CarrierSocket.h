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

#include <functional>

#include "flip/contrib/transport_tcp/CarrierTransportSocketTcp.h"

namespace kiwi
{
    // ================================================================================ //
    //                                  CARRIER SOCKET                                  //
    // ================================================================================ //
    
    //! @brief Class that encapsulate a TCP socket
    class CarrierSocket
    {
    public: // classes
        
        using state_func_t = std::function <void (flip::CarrierBase::Transition, flip::CarrierBase::Error error)>;
        
        using transfer_func_t = std::function <void (size_t, size_t)>;
        
    private: // classes
        
        enum class State
        {
            Disconnected,
            Connecting,
            Connected
        };
        
    public: // methods
        
        //! @brief Constructor
        CarrierSocket(flip::DocumentBase& document);
        
        // @brief Connects the socket to a remote socket
        void connect(std::string const& host, uint16_t port, uint64_t session_id, std::string & metadata);
        
        //! @brief Stop the socket from processing and disconnect
        void disconnect();
        
        //! @brief Returns true if the socket is connected, false otherwise
        bool isConnected() const;
        
        //! @brief Process the socket once
        void process();
        
        //! @brief Callback called when transition changes.
        void listenStateTransition(state_func_t call_back);
        
        //! @brief Callback called receiving backend informations.
        void listenTransferBackend(transfer_func_t call_back);
        
        //! @brief Stops processing
        ~CarrierSocket();
        
    private: // methods
        
        //! @brief Called when sockets state changed.
        void onStateTransition(flip::CarrierBase::Transition transition,
                               flip::CarrierBase::Error error);
        
        //! @brief Called when the socket receives the backend.
        //! @details cur represent the portion that has been downloaded yet. total is the total
        //! amount of data that needs to be received.
        void onTransferBackend(size_t cur, size_t total);
        
    private: // members
        
        std::unique_ptr<flip::CarrierTransportSocketTcp>    m_transport_socket;
        flip::DocumentBase &                                m_document;
        State                                               m_state;
        state_func_t                                        m_state_func;
        transfer_func_t                                     m_transfer_func;
        
        
    private: // deleted methods
        
        CarrierSocket(CarrierSocket const& other) = delete;
        CarrierSocket(CarrierSocket && other) = delete;
        CarrierSocket& operator=(CarrierSocket const& other) = delete;
        CarrierSocket& operator=(CarrierSocket && other) = delete;
    };
}
