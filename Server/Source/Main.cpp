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

#include <KiwiModel/KiwiModel_DataModel.hpp>
#include "KiwiServer_Server.hpp"

#include <json.hpp>

int main(int argc, const char * argv[])
{
    using namespace kiwi;
    
    model::DataModel::init();
    
    std::unique_ptr<server::Server> server(nullptr);
    
    juce::File configuration_file(argv[1]);

    try
    {
        nlohmann::json configuration = nlohmann::json::parse(configuration_file.loadFileAsString().toStdString());
        
        server.reset(new server::Server(configuration["port"], configuration["backend_directory"]));
    }
    catch(nlohmann::detail::parse_error const& e)
    {
        std::cerr << "Parsing config file failed : " << e.what() << "\n";
        return 0;
    }
    catch(nlohmann::detail::type_error const& e)
    {
        std::cerr << "Accessing element json element failed : " << e.what() << "\n";
        return 0;
    }
    catch(std::runtime_error const& e)
    {
        std::cerr << "Launching server failed: \nerr : " << e.what() << "\n";
        return 0;
    }
    
    if(server)
    {
        server->run();
    }
    
    return 0;
}
