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

#include "flip/contrib/MulticastServiceProvider.h"

#include "KiwiServer_ServiceProvider.hpp"

#include <KiwiModel/KiwiModel_DataModel.hpp>

namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                 SERVICE PROVIDER                                 //
        // ================================================================================ //
        
        ServiceProvider::ServiceProvider(uint16_t port, metadata_t metadata) :
        m_port(port),
        m_service_document(model::DataModel::use(), 1, 'serv', 'serv'),
        m_metadata(metadata),
        m_service_provider(new flip::MulticastServiceProvider(m_port, m_service_document, m_metadata))
        {
            ;
        }
        
        ServiceProvider::~ServiceProvider()
        {
            ;
        }
        
        void ServiceProvider::setMetadata(std::string const& key, std::string const& value)
        {
            m_metadata[key] = value;
        }
        
        std::string ServiceProvider::getMetadata(std::string const& key) const
        {
            return m_metadata.at(key);
        }
        
        void ServiceProvider::update()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            
            m_service_provider.reset(new flip::MulticastServiceProvider(m_port,
                                                                        m_service_document,
                                                                        m_metadata));
        }
        
        void ServiceProvider::process()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            
            if(m_service_provider)
            {
                m_service_provider->process();
            }
        }
    }
}
