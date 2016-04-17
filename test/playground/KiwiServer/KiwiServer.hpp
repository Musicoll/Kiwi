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
        Server();
        
        void run();
        
    private:
        static void init(flip::DocumentServer& document, flip::BackEndIR& backend);
        
        struct Bundle
        {
            Bundle(uint64_t session_id, uint16_t port);
            
        //private:
            flip::DocumentValidatorVoid     m_validator;
            flip::DocumentServer            m_document;
            flip::PortTransportServerTcp    m_transport;
            flip::BackEndIR                 m_backend;
        };
        
        //std::vector<std::unique_ptr<Bundle>> m_bundles;
        std::unique_ptr<Bundle> m_bundle;
    };
}

#endif // KIWI_SERVER_HPP_INCLUDED
