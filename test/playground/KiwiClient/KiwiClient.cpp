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

#include "KiwiClient.hpp"

#include <iostream>
#include <atomic>
#include <string>
#include <thread>
#include <mutex>

namespace kiwi
{
    Client::Client(const std::string & host, uint16_t port, uint16_t client_id) :
    m_document(Model::use(), *this, client_id, 'appl', 'gui '),
    m_transport(m_document, host, port),
    m_running(false)
    {
        ;
    }
    
    void Client::document_changed(Root& root)
    {
        if(root.changed())
        {
            if(root.m_value.changed())
            {
                std::cout << "-> value changed : " << root.m_value << '\n';
            }
        }
    }
    
    void Client::run()
    {
        init();
        
        m_running.store(true);
        
        std::thread process_loop(&Client::runProcessLoop, this);
        std::thread event_loop(&Client::runEventLoop, this);
        
        process_loop.join();
        event_loop.join();
    }
    
    void Client::runProcessLoop()
    {
        while(m_running.load())
        {
            m_transport.process();
            
            if(m_transport.is_connected())
            {
                m_document.pull();
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    void Client::runEventLoop()
    {
        while(m_running.load())
        {
            std::string mystr;
            std::getline (std::cin, mystr);
            
            if(mystr == "quit")
            {
                m_running.store(false);
                break;
            }
            else if(mystr == "getvalue")
            {
                Root& root = m_document.root<Root>();
                std::cout << "value : " << root.m_value << "\n";
            }
            else if(mystr == "setvalue")
            {
                Root& root = m_document.root<Root>();
                
                int val;
                std::cout << "enter a new value : ";
                std::cin >> val;
                root.m_value = val;
                
                m_document.commit();
                m_document.push();
            }
            else
            {
                std::cout << mystr << " is not a valid command. \n";
            }
        }
    }
    
    void Client::init()
    {
        flip::CarrierBase::Transition transition = flip::CarrierBase::Transition::Disconnected;
        
        m_transport.listen_state_transition([&transition](flip::CarrierBase::Transition sub_transition, int err){
            if(transition == flip::CarrierBase::Transition::Disconnected)
            {
                std::cout << "client disconnected" << '\n';
                transition = sub_transition;
            }
            else if(transition == flip::CarrierBase::Transition::Connecting)
            {
                std::cout << "client connecting" << '\n';
                transition = sub_transition;
            }
            else if(transition == flip::CarrierBase::Transition::Connected)
            {
                std::cout << "client connected" << '\n';
                transition = sub_transition;
            }
        });
    }
}
