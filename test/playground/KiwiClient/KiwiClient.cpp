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
#include <future>

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
                Console::post("-> value changed : ", root.m_value);
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
            std::string mystr = Console::getInput<std::string>();
            
            if(mystr == "quit")
            {
                m_running.store(false);
                break;
            }
            else if(mystr == "getvalue")
            {
                Root& root = m_document.root<Root>();
                Console::post("value = ", root.m_value);
            }
            else if(mystr == "setvalue")
            {
                Console::post("Enter a new integer value :");
                
                int64_t val = Console::getInput<int64_t>();
                
                Root& root = m_document.root<Root>();
                root.m_value = val;
                
                m_document.commit();
                m_document.push();
                m_transport.process();
            }
            else
            {
                Console::error("\"", mystr, "\" is not a valid command.");
            }
        }
    }
    
    void Client::init()
    {
        flip::CarrierBase::Transition transition = flip::CarrierBase::Transition::Disconnected;
        
        m_transport.listen_state_transition([&transition](flip::CarrierBase::Transition sub_transition, int err){
            if(transition == flip::CarrierBase::Transition::Disconnected)
            {
                Console::log("client disconnected");
                transition = sub_transition;
            }
            else if(transition == flip::CarrierBase::Transition::Connecting)
            {
                Console::log("client connecting");
                transition = sub_transition;
            }
            else if(transition == flip::CarrierBase::Transition::Connected)
            {
                Console::log("client connected");
                transition = sub_transition;
            }
        });
    }
}
