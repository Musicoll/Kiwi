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

#include "KiwiApp_Server.hpp"
#include <ctime>

namespace kiwi
{
    // ================================================================================ //
    //                                    EMBED SERVER                                  //
    // ================================================================================ //
    
    Server::Server(uint16_t port) : m_user_id(flip::Ref::User::Offline)
    {
        std::srand(std::time(0));
        m_user_id = std::rand();

        // initialize and run server
        try
        {
            m_server.reset(new network::Server(port, m_user_id));
        }
        catch(std::runtime_error const& e)
        {
            m_server = nullptr;
            std::cerr << "Server init failed: \nerr: " << e.what() << "\n";
        }
        
        m_browser.start();
    }
    
    Server::~Server()
    {
        stop();
    }
    
    uint64_t Server::getUserId() const noexcept
    {
        return m_user_id;
    }
    
    DocumentBrowser& Server::getBrowser()
    {
        return m_browser;
    }
    
    void Server::run()
    {
        if(m_server)
        {
            m_server_thread = std::thread(&network::Server::run, m_server.get());
        }
    }
    
    void Server::stop()
    {
        m_browser.stop();
        
        if (m_server && m_server->isRunning())
        {
            m_server->stop();
            m_server_thread.join();
        }
    }
    
    bool Server::renameDocumentSession(uint64_t session_id, std::string const& new_name)
    {
        if(m_server)
        {
            return m_server->renameDocumentSession(session_id, new_name);
        }
        
        return false;
    }
    
    bool Server::deleteDocumentSession(uint64_t session_id)
    {
        if(m_server)
        {
            return m_server->deleteDocumentSession(session_id);
        }
        
        return false;
    }
}
