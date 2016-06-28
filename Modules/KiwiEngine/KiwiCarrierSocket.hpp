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

#ifndef KIWI_ENGINE_CARRIER_SOCKET_HPP_INCLUDED
#define KIWI_ENGINE_CARRIER_SOCKET_HPP_INCLUDED

#include <thread>
#include <functional>
#include <atomic>

#include "flip/contrib/transport_tcp/CarrierTransportSocketTcp.h"

namespace flip
{
    class DocumentBase;
    class Object;
}

namespace kiwi
{
    // ================================================================================ //
    //                                  CARRIER SOCKET                                  //
    // ================================================================================ //
    
    //! @biref Class that encapsulate a TCP socket
    class CarrierSocket
    {
    public:
        CarrierSocket(flip::DocumentBase& document, std::string const& host, uint16_t port);
        
        // @brief Connects the socket to a remote socket
        void connect(std::string const& host, uint16_t port);
        
        //! @brief Stop the socket from processing and disconnect
        void disconnect();
        
        //! @brief Returns true if the socket is connected, false otherwise
        bool isConnected() const;
        
        //! @brief Starts a thread that continuously process the socket
        void startProcess();
        
        //! @biref Process the socket once
        void process();
        
        //! @brief Stops the thread that processes the socket
        void stopProcess();
        
        //! @brief Add a callback to be called once disconnected
        void listenDisconnected(std::function<void ()> func);
        
        //! @brief Add a callback to be called on connected
        void listenConnected(std::function<void ()> func);
        
        //! @brief Add a callback to be called once first load terminated
        void listenLoaded(std::function<void ()> func);
        
        //! @brief Stops processing
        ~CarrierSocket();
        
    private:
        //! @brief Function called on the processing thread
        void runProcess();
        
        //! @brief Binds the encapsulated socket callbacks
        void bindCallBacks();
        
        //! @brief Called when connection transition happen
        void listenStateTransition(flip::CarrierBase::Transition state, flip::CarrierBase::Error error);
        
        //! @brief Called during loading process
        void listenTransferBackEnd(size_t cur, size_t total);
        
        //! @brief Called when receiving a transaction
        void listenTransferTransaction(size_t cur, size_t total);
        
        //! @brief Called when receiving a signal
        void listenTransferSignal(size_t cur, size_t total);
        
    private:
        flip::CarrierTransportSocketTcp m_transport_socket;
        std::thread                     m_transport_loop;
        std::atomic_bool                m_transport_running;
        
        std::function<void ()> m_func_disonnected;
        std::function<void ()> m_func_connected;
        std::function<void ()> m_func_loaded;
        
    private:
        CarrierSocket(CarrierSocket const& other) = delete;
        CarrierSocket(CarrierSocket && other) = delete;
        CarrierSocket& operator=(CarrierSocket const& other) = delete;
        CarrierSocket& operator=(CarrierSocket && other) = delete;
    };
}

#endif // KIWI_ENGINE_CARRIER_SOCKET_HPP_INCLUDED