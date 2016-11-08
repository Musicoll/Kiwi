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

#ifndef KIWI_APP_SERVER_HPP_INCLUDED
#define KIWI_APP_SERVER_HPP_INCLUDED

#include "flip/contrib/MulticastServiceExplorer.h"
#include <KiwiEngine/KiwiEngine_Listeners.hpp>

#include <juce_core/juce_core.h>

#include <KiwiServer/KiwiServer_Server.hpp>
#include <thread>

#include "KiwiApp_DocumentBrowser.hpp"

namespace kiwi
{
    class Instance;
    
    // ================================================================================ //
    //                                    EMBED SERVER                                  //
    // ================================================================================ //
    
    //! @brief An emmbed Server.
    class Server
    {   
    public: // methods
        
        //! @brief Constructor
        Server(uint16_t port = 9090);
        
        //! @brief Destructor
        ~Server();
        
        //! @brief Returns the user id.
        uint64_t getUserId() const noexcept;
        
        //! @brief Get the document browser.
        DocumentBrowser& getBrowser();
        
        //! @brief Scan the LAN to find a service provider.
        void run();
        
        //! @brief Stop the server.
        void stop();
        
        //! @brief Rename a given document session.
        bool renameDocumentSession(uint64_t session_id, std::string const& new_name);
        
        //! @brief Deletes a given document session.
        bool deleteDocumentSession(uint64_t session_id);
        
    private: // members
        
        uint64_t                            m_user_id;
        std::unique_ptr<server::Server>     m_server;
        std::thread                         m_server_thread;
        
        DocumentBrowser                     m_browser;
    };
}


#endif // KIWI_APP_SERVER_HPP_INCLUDED
