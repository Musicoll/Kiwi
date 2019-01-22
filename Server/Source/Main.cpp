/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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
    std::cout << " -f set the json configuration file to use (required). \n";
    std::cout << '\n';
    std::cout << "ex: ./Server -f prod.json" << std::endl;
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
    
    const auto config_filepath = cl_parser.getOption("-f");
    if(config_filepath.empty())
    {
        std::cerr << "Error: Server need a configuration file:\n" << std::endl;
        showHelp();
        return 0;
    }
    
    juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
    .setAsCurrentWorkingDirectory();
    
    const auto config_file = juce::File::getCurrentWorkingDirectory().getChildFile(config_filepath);
    
    if(!config_file.exists())
    {
        std::cerr << "Error: Config file: \""
        << config_file.getFullPathName()
        << "\" not found !" << std::endl;
        
        showHelp();
        return 0;
    }
    
    model::DataModel::init();
    
    json config;
    
    try
    {
         config = json::parse(config_file.loadFileAsString().toStdString());
    }
    catch(json::parse_error& e)
    {
        std::cerr << "Parsing config file failed : " << e.what() << '\n';
        return 0;
    }
    
    // check config entries:
    const std::vector<std::string> required_config_entries {
      "backend_directory", "session_port", "open_token", "kiwi_version"
    };
    
    for (auto const& entry : required_config_entries)
    {
        if(! (config.find(entry) != config.end()) )
        {
            std::cerr << entry << " entry needed in config file\n";
            return 0;
        }
    }
    
    const std::string backend_path = config["backend_directory"];
    const juce::File backend_dir {config_file.getParentDirectory().getChildFile(backend_path)};
    const uint16_t session_port = config["session_port"];
    const std::string token = config["open_token"];
    const std::string kiwi_client_version = config["kiwi_version"];
    
    try
    {
        server::Server kiwi_server(session_port, backend_dir, token, kiwi_client_version);
        
        flip::RunLoopTimer run_loop ([&kiwi_server]
        {
            kiwi_server.process();
            
            return !server_stopped.load();
        }, 0.02);
        
        std::cout << "[server] - running on port: " << std::to_string(session_port) << std::endl;
        std::cout << "[server] - backend_directory: " << backend_dir.getFullPathName().toStdString() << std::endl;
        
        run_loop.run();
        
        std::cout << "[server] - stopped" << std::endl;
    }
    catch(std::runtime_error const& e)
    {
        std::cerr << "Launching server failed: \nerr : " << e.what() << "\n";
        std::cerr << "Maybe someone is already listening on port " << std::to_string(session_port) << "\n";
        return 0;
    }
    
    return 0;
}
