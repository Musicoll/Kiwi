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
#include "KiwiClient.hpp"

int main(int argc, const char * argv[])
{
    using namespace kiwi;
    
    Model::init("test.server");
    
    std::string ip = (argc > 1) ? argv[1] : "localhost";
    
    std::cout << "ip : " << ip << std::endl;
    
    uint16_t cin_user_id;
    std::cout << "Enter client user ID : ";
    std::cin >> cin_user_id;
    
    Client client(ip, 9090, cin_user_id);
    client.run();
    
    return 0;
}
