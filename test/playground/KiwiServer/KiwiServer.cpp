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
#include <atomic>
#include <string>
#include <thread>

#include "flip/Document.h"
#include "flip/DataConsumerMemory.h"
#include "flip/BackendMl.h"

#include "KiwiServer.hpp"

namespace kiwi
{
    Server::Server(uint64_t session_id, uint16_t port) :
    m_validator(),
    m_document(Model::use(), m_validator, session_id),
    m_transport(m_document, port),
    m_backend(),
    m_running(false)
    {
        {
            flip::Document doc_temp(Model::use(), 123UL, 'appl', 'gui ');
            Root& root = doc_temp.root<Root>();
            root.m_value = 99999;
            
            // load backend
            // m_backend.write(doc_temp);
        }
        
        init();
    }
    
    void Server::run()
    {
        m_running.store(true);
        
        std::thread process_loop(&Server::runProcessLoop, this);
        std::thread event_loop(&Server::runEventLoop, this);
        
        process_loop.join();
        event_loop.join();
    }
    
    void Server::runProcessLoop()
    {
        while(m_running.load())
        {
            m_transport.process();
            m_document.pull();
            m_document.push();
            m_transport.process();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    void Server::runEventLoop()
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
            }
            else if (mystr == "save")
            {
                std::vector <uint8_t> data;
                flip::DataConsumerMemory consumer(data);
                
                flip::BackEndIR backup = m_document.write();
                backup.write<flip::BackEndMl>(consumer);
                
                std::string backup_str(data.begin (), data.end ());
                Console::log(backup_str);
                // save to file here
            }
            else
            {
                Console::error("\"", mystr, "\" is not a valid command.");
            }
        }
    }
    
    void Server::on_connecting(flip::PortBase& port)
    {
        Console::log("client [", port.user(), "] -> connecting");
    }
    
    void Server::on_connected(flip::PortBase& port)
    {
        Console::log("client [", port.user(), "] -> connected");
    }
    
    void Server::on_disconnected(flip::PortBase& port)
    {
        Console::log("client [", port.user(), "] -> disconnected");
    }
    
    void Server::init()
    {
        using namespace std::placeholders;
        m_document.listen_connecting    (std::bind(&Server::on_connecting, this, _1));
        m_document.listen_connected     (std::bind(&Server::on_connected, this, _1));
        m_document.listen_disconnected  (std::bind(&Server::on_disconnected, this, _1));

        if(m_backend.empty())
        {
            Root& root = m_document.root<Root>();
            root.m_value = 42;
            
            m_document.commit();
        }
        else
        {
            m_document.read(m_backend);
        }
    }
}
