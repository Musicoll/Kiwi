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

#include "flip/contrib/MulticastServiceProvider.h"

#include "KiwiPatcherManager.hpp"

#include <KiwiModel/KiwiPatcherModel.hpp>

namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                  PATCHER MANAGER                                 //
        // ================================================================================ //
        
        PatcherManager::PatcherManager(uint64_t document_id) :
        m_validator(),
        m_document(model::PatcherModel::use(),
                   m_validator,
                   document_id),
        m_transport(m_document, 9090),
        m_transport_loop(),
        m_transport_running(false)
        {
            m_document.listen_connecting
                       (std::bind(&PatcherManager::on_connecting, this, std::placeholders::_1));
            
            m_document.listen_connected
                       (std::bind(&PatcherManager::on_connected, this, std::placeholders::_1));
            
            m_document.listen_disconnected
                       (std::bind(&PatcherManager::on_disconnected, this, std::placeholders::_1));
            
            m_document.commit();
            m_document.push(); // needed ?
            
            launchTransport();
        }
        
        model::Patcher& PatcherManager::getPatcher()
        {
            return m_document.root<model::Patcher>();
        }
        
        void PatcherManager::launchTransport()
        {
            if (!m_transport_running.load())
            {
                m_transport_running.store(true);
                m_transport_loop = std::move(std::thread(&PatcherManager::runTransport, this));
            }
        }
        
        void PatcherManager::runTransport()
        {
            model::Patcher& patcher = getPatcher();
            
            std::map<std::string, std::string> metadata;
            
            std::string name = patcher.getName();
            metadata["name"] = !name.empty() ? name : "Untitled";
            
            flip::MulticastServiceProvider provider(9090, m_document, metadata);
            
            while(m_transport_running.load())
            {
                m_transport.process();
                provider.process();
                
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }
        
        void PatcherManager::stopTransport()
        {
            if (m_transport_running.load())
            {
                m_transport_running.store(false);
                m_transport_loop.join();
            }
        }
        
        PatcherManager::~PatcherManager()
        {
            stopTransport();
        }
        
        void PatcherManager::on_connecting(flip::PortBase& port)
        {
            std::cout << "client [" + std::to_string(port.user()) +"] -> connecting" << std::endl << "> ";
        }
        
        void PatcherManager::on_connected(flip::PortBase& port)
        {
            std::cout << "client [" + std::to_string(port.user()) +"] -> connected" << std::endl << "> ";;
        }
        
        void PatcherManager::on_disconnected(flip::PortBase& port)
        {
            std::cout << "client [" + std::to_string(port.user()) +"] -> disconnected" << std::endl << "> ";;
        }
    }
}