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

#include "KiwiPatcherManager.hpp"

#include <KiwiModel/KiwiPatcherModel.hpp>

namespace kiwi
{
    namespace server
    {
        PatcherManager::PatcherManager(uint64_t document_id): m_validator(),
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
            
            populatePatcher();
            
            m_document.commit();
            m_document.push();
            
            launchTransport();
        }
        
        model::Patcher &PatcherManager::getPatcher()
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
            while(m_transport_running.load())
            {
                m_transport.process();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
        
        void PatcherManager::populatePatcher()
        {
            model::Patcher & patcher = getPatcher();
            
            {
                // simple print
                auto& plus = patcher.addObject("plus 44");
                plus.setPosition(50, 50);
                auto& print = patcher.addObject("print");
                print.setPosition(50, 100);
                patcher.addLink(plus, 0, print, 0);
            }
            
            {
                // set rhs value
                auto& plus_1 = patcher.addObject("plus 1");
                plus_1.setPosition(150, 50);
                
                auto& plus_2 = patcher.addObject("plus 10");
                plus_2.setPosition(220, 50);
                
                auto& plus_3 = patcher.addObject("plus");
                plus_3.setPosition(150, 100);
                
                auto& print = patcher.addObject("print");
                print.setPosition(150, 150);
                
                patcher.addLink(plus_1, 0, plus_3, 0);
                patcher.addLink(plus_2, 0, plus_3, 1);
                patcher.addLink(plus_3, 0, print, 0);
            }
            
            {
                // basic counter
                auto& plus_1 = patcher.addObject("plus");
                plus_1.setPosition(350, 100);
                
                auto& plus_2 = patcher.addObject("plus");
                plus_2.setPosition(405, 70);
                
                auto& plus_3 = patcher.addObject("plus 10");
                plus_3.setPosition(300, 20);
                
                auto& plus_4 = patcher.addObject("plus -10");
                plus_4.setPosition(380, 20);
                
                auto& print = patcher.addObject("print zozo");
                print.setPosition(350, 150);
                
                patcher.addLink(plus_1, 0, plus_2, 0);
                patcher.addLink(plus_2, 0, plus_1, 1);
                patcher.addLink(plus_1, 0, print, 0);
                
                patcher.addLink(plus_3, 0, plus_1, 0);
                patcher.addLink(plus_4, 0, plus_1, 0);
            }
            
            {
                // stack overflow
                auto& plus_1 = patcher.addObject("plus");
                plus_1.setPosition(550, 100);
                
                auto& plus_2 = patcher.addObject("plus");
                plus_2.setPosition(605, 70);
                
                auto& plus_3 = patcher.addObject("plus 10");
                plus_3.setPosition(500, 20);
                
                auto& plus_4 = patcher.addObject("plus -10");
                plus_4.setPosition(580, 20);
                
                auto& print = patcher.addObject("print zozo");
                print.setPosition(550, 150);
                
                patcher.addLink(plus_1, 0, plus_2, 0);
                patcher.addLink(plus_2, 0, plus_1, 0);
                patcher.addLink(plus_1, 0, print, 0);
                
                patcher.addLink(plus_3, 0, plus_1, 0);
                patcher.addLink(plus_4, 0, plus_1, 0);
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