/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
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
#include "KiwiServer_CommandLineParser.hpp"

#include <json.hpp>

void showHelp()
{
    std::cout << "Usage:\n";
    std::cout << " -h shows this help message. \n";
    std::cout << " -f set the json configuration file to use (needed). \n";
}

int main(int argc, char const* argv[])
{
    using namespace kiwi;
    using nlohmann::json;
    
    CommandLineParser cl_parser(argc, argv);
    
    if(cl_parser.hasOption("-h"))
    {
        showHelp();
        return 0;
    }
    
    juce::File configuration_file(cl_parser.getOption("-f"));
    
    if(!configuration_file.exists())
    {
        std::cerr << "Error: Config file does not exist or is unspecified.." << std::endl;
        showHelp();
        return 0;
    }
    
    model::DataModel::init();
    std::unique_ptr<server::Server> server(nullptr);
    
    try
    {
        json config = json::parse(configuration_file.loadFileAsString().toStdString());
        server.reset(new server::Server(config["port"], config["backend_directory"]));
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
