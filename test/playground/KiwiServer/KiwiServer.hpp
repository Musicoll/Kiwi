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

#ifndef KIWI_SERVER_HPP_INCLUDED
#define KIWI_SERVER_HPP_INCLUDED

#include "../KiwiCore/KiwiConsole.hpp"
#include "../KiwiModel/KiwiModel.hpp"

#include "flip/BackEndIR.h"
#include "flip/PortDirect.h"
#include "flip/Class.h"
#include "flip/DocumentValidatorVoid.h"
#include "flip/DocumentServer.h"
#include "flip/contrib/transport_tcp/CarrierTransportSocketTcp.h"
#include "flip/contrib/transport_tcp/PortTransportServerTcp.h"

namespace kiwi
{
    class Server
    {
    public:
        Server(uint64_t session_id, uint16_t port);
        
        void run();
        
    private:
        void runProcessLoop();
        void runEventLoop();
        
        void init();
        
        // DocumentServer listener
        void on_connecting(flip::PortBase& port);
        void on_connected(flip::PortBase& port);
        void on_disconnected(flip::PortBase& port);
        
        flip::DocumentValidatorVoid     m_validator;
        flip::DocumentServer            m_document;
        flip::PortTransportServerTcp    m_transport;
        flip::BackEndIR                 m_backend;
        std::atomic_bool                m_running;
    };
}

#endif // KIWI_SERVER_HPP_INCLUDED
