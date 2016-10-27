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

#include "KiwiServer.hpp"
#include <KiwiModel/KiwiModel_DataModel.hpp>

int main(int argc, const char * argv[])
{
    using namespace kiwi;
    
    model::DataModel::init();
    
    std::unique_ptr<server::Server> server(nullptr);
    
    try
    {
        server.reset(new server::Server(9090));
    }
    catch(std::runtime_error const& e)
    {
        std::cerr << "Server already running on this machine: \nerr: " << e.what() << "\n";
        return 0;
    }
    
    if(server)
    {
        //server->setSessionsBackendDirectory("/sessions/");
        server->run();
    }
    
    return 0;
}
