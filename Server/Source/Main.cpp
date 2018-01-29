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

#include <csignal>
#include <atomic>

#include <flip/contrib/RunLoopTimer.h>

#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiServer/KiwiServer_Server.h>
#include "KiwiServer_CommandLineParser.h"

#include <json.hpp>

std::atomic<bool> server_stopped(false);

void showHelp()
{
    std::cout << "Usage:\n";
    std::cout << " -h shows this help message. \n";
    std::cout << " -f set the json configuration file to use (needed). \n";
}

void on_interupt(int signal)
{
    server_stopped.store(true);
}

void on_terminate(int signal)
{
    server_stopped.store(true);
}

int main(int argc, char const* argv[])
{
    using namespace kiwi;
    using nlohmann::json;
    
    std::signal(SIGINT, on_interupt); // interupt, kill -2, Ctrl + C
    
    std::signal(SIGTERM, on_terminate); // terminate, kill (-15)
    
    CommandLineParser cl_parser(argc, argv);
    
    if(cl_parser.hasOption("-h"))
    {
        showHelp();
        return 0;
    }
    
    juce::File configuration_file("./" + cl_parser.getOption("-f"));
    
    if(!configuration_file.exists())
    {
        std::cerr << "Error: Config file does not exist or is unspecified.." << std::endl;
        showHelp();
        return 0;
    }
    
    model::DataModel::init();
    
    json config;
    
    try
    {
         config = json::parse(configuration_file.loadFileAsString().toStdString());
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
    
    try
    {
        server::Server kiwi_server(config["session_port"],
                                   config["backend_directory"],
                                   config["open_token"],
                                   config["kiwi_version"]);
        
        std::cout << "[server] - running on port " << config["session_port"] << std::endl;
        
        flip::RunLoopTimer run_loop ([&kiwi_server]
        {
            kiwi_server.process();
            
            return !server_stopped.load();
        }, 0.02);
        
        run_loop.run();
        
        std::cout << "[server] - stopped" << std::endl;
    }
    catch(std::runtime_error const& e)
    {
        std::cerr << "Launching server failed: \nerr : " << e.what() << "\n";
        return 0;
    }
    
    return 0;
}
