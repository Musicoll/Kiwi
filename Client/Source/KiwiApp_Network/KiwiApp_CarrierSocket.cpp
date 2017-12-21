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

#include <cassert>

#include "KiwiApp_CarrierSocket.h"

#include <KiwiModel/KiwiModel_PatcherUser.h>

namespace kiwi
{
    // ================================================================================ //
    //                                  CARRIER SOCKET                                  //
    // ================================================================================ //
    
    CarrierSocket::CarrierSocket(flip::DocumentBase& document):
    m_transport_socket(nullptr),
    m_document(document),
    m_state(State::Disconnected),
    m_state_func(),
    m_transfer_func()
    {
    }
    
    void CarrierSocket::onStateTransition(flip::CarrierBase::Transition transition,
                                          flip::CarrierBase::Error error)
    {
        if (transition == flip::CarrierBase::Transition::Disconnected)
        {
            m_state = State::Disconnected;
        }
        else if (transition == flip::CarrierBase::Transition::Connecting)
        {
            m_state = State::Connecting;
        }
        else if (transition == flip::CarrierBase::Transition::Connected)
        {
            m_state = State::Connected;
        }
        
        if (m_state_func)
        {
            m_state_func(transition, error);
        }
    }
    
    void CarrierSocket::onTransferBackend(size_t cur, size_t total)
    {
        if (m_transfer_func)
        {
            m_transfer_func(cur, total);
        }
    }
    
    void CarrierSocket::listenStateTransition(std::function <void (flip::CarrierBase::Transition,
                                                    flip::CarrierBase::Error error)> call_back)
    {
        m_state_func = call_back;
    }
    
    
    void CarrierSocket::listenTransferBackend(std::function <void (size_t, size_t)> call_back)
    {
        m_transfer_func = call_back;
    }
    
    void CarrierSocket::process()
    {
        if (m_transport_socket != nullptr)
        {
            m_transport_socket->process();
        }
    }
    
    void CarrierSocket::disconnect()
    {
        m_transport_socket.reset();
    }
    
    bool CarrierSocket::isConnected() const
    {
        return m_transport_socket != nullptr && m_state == State::Connected;
    }
    
    void CarrierSocket::connect(std::string const& host, uint16_t port, uint64_t session_id)
    {
        m_transport_socket.reset(new flip::CarrierTransportSocketTcp(m_document, session_id, host, port));
        
        m_transport_socket->listen_state_transition(std::bind(&CarrierSocket::onStateTransition,
                                                              this,
                                                              std::placeholders::_1,
                                                              std::placeholders::_2));
        
        m_transport_socket->listen_transfer_backend(std::bind(&CarrierSocket::onTransferBackend,
                                                              this,
                                                              std::placeholders::_1,
                                                              std::placeholders::_2));
    }
    
    CarrierSocket::~CarrierSocket()
    {
        disconnect();
    }
}
