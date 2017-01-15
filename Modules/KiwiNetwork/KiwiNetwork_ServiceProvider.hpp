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

#ifndef KIWI_NETWORK_SERVICE_PROVIDER_HPP_INCLUDED
#define KIWI_NETWORK_SERVICE_PROVIDER_HPP_INCLUDED

#include <flip/Document.h>
#include <flip/contrib/MulticastServiceProvider.h>

#include <mutex>

namespace kiwi
{
    namespace network
    {
        // ================================================================================ //
        //                                 SERVICE PROVIDER                                 //
        // ================================================================================ //
        
        //! @brief The server Patcher manager.
        class ServiceProvider
        {
        public: // methods
            
            using metadata_t = std::map<std::string, std::string>;
            
            //! @brief Constructor.
            ServiceProvider(uint16_t port, metadata_t metadata);
            
            //! @brief Destructor.
            ~ServiceProvider();
            
            //! @brief Process service
            void process();
            
            //! @brief Set the metadata value for a given key.
            //! @details Call the update() method to take metadata modifications into account.
            void setMetadata(std::string const& key, std::string const& value);
            
            //! @brief Gets the metadata value for a given key.
            std::string getMetadata(std::string const& key) const;
            
            //! @brief Recreate the multicast service provider with new metadata.
            void update();
            
        private: // members
            
            uint16_t                        m_port;
            flip::Document                  m_service_document;
            metadata_t                      m_metadata;
            std::unique_ptr<flip::MulticastServiceProvider>
                                            m_service_provider;
            std::mutex                      m_mutex;
            
        private: // deleted methods
            
            ServiceProvider(ServiceProvider const& other) = delete;
            ServiceProvider(ServiceProvider && other) = delete;
            ServiceProvider &operator=(ServiceProvider const& other) = delete;
            ServiceProvider &operator=(ServiceProvider && other) = delete;
        };
    }
}

#endif //KIWI_NETWORK_SERVICE_PROVIDER_HPP_INCLUDED
