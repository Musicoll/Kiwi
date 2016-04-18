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

#ifndef KIWI_CLIENT_HPP_INCLUDED
#define KIWI_CLIENT_HPP_INCLUDED

#include <iostream>
#include <mutex>
#include "../KiwiModel/KiwiModel.hpp"

#include "flip/CarrierDirect.h"
#include "flip/Class.h"
#include "flip/Document.h"
#include "flip/DocumentObserver.h"
#include "flip/contrib/transport_tcp/CarrierTransportSocketTcp.h"

namespace kiwi
{
    class Client : public flip::DocumentObserver<Root>
    {
    public:
        
        Client(const std::string & host, uint16_t port, uint16_t client_id);
        
        void run();
        
    private:
        void runProcessLoop();
        void runEventLoop();
        
        void document_changed(Root& root) override;
        
        void init();
        void waitTransferBackend();
                
        flip::Document                  m_document;
        flip::CarrierTransportSocketTcp m_transport;
        std::atomic_bool                m_running;        
    };
}

#endif // KIWI_CLIENT_HPP_INCLUDED