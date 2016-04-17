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

#include <iostream>
#include "KiwiServer.hpp"

namespace kiwi
{
    Server::Server()
    {
        Model::version ("test.server");
        
        Model::declare<Root>()
        .name ("Root")
        .member <flip::Int, &Root::m_value>("value");
        
        m_bundle = std::unique_ptr<Bundle>(new Bundle(123456789ULL, 9090));
    }
    
    void Server::run()
    {
        for (;;)
        {
            m_bundle->m_transport.process();
            m_bundle->m_document.pull();
            m_bundle->m_document.push();
            m_bundle->m_transport.process();
        }
    }
    
    void Server::init(flip::DocumentServer& document, flip::BackEndIR& backend)
    {
        document.listen_connecting([](flip::PortBase& port){
            std::cout << "client [" << port.user() << "] -> connecting" << '\n';
        });
        
        document.listen_connected([](flip::PortBase& port){
            std::cout << "client [" << port.user() << "] -> connected" << '\n';
        });
        
        document.listen_disconnected([](flip::PortBase& port){
            std::cout << "client [" << port.user() << "] -> disconnected" << '\n';
        });
        
        if(backend.empty())
        {
            Root & root = document.root<Root>();
            root.m_value = 42;
            
            document.commit();
        }
        else
        {
            document.read(backend);
        }
    }
    
    Server::Bundle::Bundle(uint64_t session_id, uint16_t port) :
    m_validator(),
    m_document(Model::use(), m_validator, session_id),
    m_transport(m_document, port),
    m_backend()
    {
        Server::init(m_document, m_backend);
    }
}
