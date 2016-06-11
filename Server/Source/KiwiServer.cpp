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

#include <string>

#include "KiwiServer.hpp"


namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                      SERVER                                      //
        // ================================================================================ //
        
        Server::~Server()
        {
            m_patchers.clear();
        }
        
        void Server::openPatcher(uint64_t patcher_id)
        {
            m_patchers.emplace(std::piecewise_construct,
                               std::forward_as_tuple(patcher_id),
                               std::forward_as_tuple(patcher_id));
        }
        
        void Server::closePatcher(uint64_t patcher_id)
        {
            m_patchers.erase(patcher_id);
        }
        
        void Server::runCommand()
        {
            bool run_cmd = true;
            
            while(run_cmd)
            {
                std::cout << "> ";
                
                std::string command;
                std::cin >> command;
                
                if (command == "open")
                {
                    openPatcher(10);
                    std::cout << "> ";
                    std::cout << "patcher 10 opened and running" << std::endl;
                    
                }
                else if(command == "close")
                {
                    closePatcher(10);
                    std::cout << "> ";
                    std::cout << "patcher 10 closed" << std::endl;
                    run_cmd = false;
                }
                else
                {
                    std::cout << "invalid command" << std::endl;
                }
            }
        }
    }
}