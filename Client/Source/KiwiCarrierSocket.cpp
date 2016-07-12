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

#include <cassert>
#include <mutex>

#include "KiwiCarrierSocket.hpp"

#include <KiwiModel/KiwiModelPatcher.hpp>

namespace kiwi
{
    // ================================================================================ //
    //                                  CARRIER SOCKET                                  //
    // ================================================================================ //
    
    CarrierSocket::CarrierSocket(flip::DocumentBase& document, std::string const& host, uint16_t port):
    m_transport_socket(document, host, port),
    m_transport_loop(),
    m_transport_running(false)
    {
        bindCallBacks();
    }
    
    void CarrierSocket::listenStateTransition(flip::CarrierBase::Transition state, flip::CarrierBase::Error error)
    {
        switch(state)
        {
            case flip::CarrierBase::Transition::Disconnected:
            {
                if (m_transport_socket.is_connected())
                {
                    stopProcess();
                    m_func_disonnected();
                }
                break;
            }
            case flip::CarrierBase::Transition::Connecting:
            {
                break;
            }
            case flip::CarrierBase::Transition::Connected:
            {
                m_func_connected();
                break;
            }
        }
    }
    
    void CarrierSocket::listenTransferBackEnd(size_t cur, size_t total)
    {
        if (cur == total)
        {
            m_func_loaded();
        }
    }
    
    void CarrierSocket::listenTransferTransaction(size_t cur, size_t total)
    {
    }
    
    void CarrierSocket::listenTransferSignal(size_t cur, size_t total)
    {
    }
    
    void CarrierSocket::bindCallBacks()
    {
        m_transport_socket.listen_state_transition(std::bind(&CarrierSocket::listenStateTransition,
                                                             this,
                                                             std::placeholders::_1,
                                                             std::placeholders::_2));
        
        m_transport_socket.listen_transfer_backend(std::bind(&CarrierSocket::listenTransferBackEnd,
                                                             this,
                                                             std::placeholders::_1,
                                                             std::placeholders::_2));
        
        m_transport_socket.listen_transfer_transaction(std::bind(&CarrierSocket::listenTransferTransaction,
                                                                 this,
                                                                 std::placeholders::_1,
                                                                 std::placeholders::_2));
        
        m_transport_socket.listen_transfer_signal(std::bind(&CarrierSocket::listenTransferSignal,
                                                            this,
                                                            std::placeholders::_1,
                                                            std::placeholders::_2));
    }
    
    void CarrierSocket::listenDisconnected(std::function<void ()> func)
    {
        m_func_disonnected = func;
    }
    
    void CarrierSocket::listenConnected(std::function<void ()> func)
    {
        m_func_connected = func;
    }
    
    void CarrierSocket::listenLoaded(std::function<void ()> func)
    {
        m_func_loaded = func;
    }
    
    void CarrierSocket::startProcess()
    {
        if (!m_transport_running.load())
        {
            m_transport_running.store(true);
            m_transport_loop = std::move(std::thread(&CarrierSocket::runProcess, this));
            m_transport_loop.detach();
        }
    }
    
    void CarrierSocket::process()
    {
        if (!m_transport_running.load())
        {
            m_transport_socket.process();
        }
    }
    
    void CarrierSocket::runProcess()
    {
        while(m_transport_running.load())
        {
            m_transport_socket.process();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    void CarrierSocket::stopProcess()
    {
        if (m_transport_running.load())
        {
            m_transport_running.store(false);
        }
    }
    
    void CarrierSocket::disconnect()
    {
        stopProcess();
        m_transport_socket.rebind("", 0);
    }
    
    bool CarrierSocket::isConnected() const
    {
        return m_transport_socket.is_connected();
    }
    
    void CarrierSocket::connect(std::string const& host, uint16_t port)
    {
        stopProcess();
        m_transport_socket.rebind(host, port);
    }
    
    CarrierSocket::~CarrierSocket()
    {
        stopProcess();
    }
}
