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

#ifndef KIWI_SERVER_SERVICE_PROVIDER_HPP_INCLUDED
#define KIWI_SERVER_SERVICE_PROVIDER_HPP_INCLUDED

#include <flip/Document.h>
#include <flip/contrib/MulticastServiceProvider.h>

namespace kiwi
{
    namespace server
    {
        class Server;
        
        // ================================================================================ //
        //                                  PATCHER MANAGER                                 //
        // ================================================================================ //
        
        //! @brief The server Patcher manager.
        class ServiceProvider
        {
        public:
            
            //! @brief Constructor.
            ServiceProvider(Server& server, std::map<std::string, std::string> metadata);
            
            //! @brief Destructor.
            ~ServiceProvider();
            
            //! @brief Process service
            void process();
            
        private: // members
            
            Server&                         m_server;
            flip::Document                  m_service_document;
            flip::MulticastServiceProvider  m_service_provider;
            
        private: // deleted methods
            
            ServiceProvider(ServiceProvider const& other) = delete;
            ServiceProvider(ServiceProvider && other) = delete;
            ServiceProvider &operator=(ServiceProvider const& other) = delete;
            ServiceProvider &operator=(ServiceProvider && other) = delete;
        };
    }
}

#endif //KIWI_SERVER_SERVICE_PROVIDER_HPP_INCLUDED
