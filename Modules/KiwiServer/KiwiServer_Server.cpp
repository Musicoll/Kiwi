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

#include <flip/contrib/DataProviderFile.h>
#include <flip/contrib/DataConsumerFile.h>

#include <KiwiModel/KiwiModel_DataModel.hpp>
#include <KiwiModel/KiwiModel_PatcherUser.hpp>
#include <KiwiModel/KiwiModel_PatcherValidator.hpp>

#include "KiwiServer_Server.hpp"

namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                      SERVER                                      //
        // ================================================================================ //
        
        const char* Server::kiwi_file_extension = "kiwi";
        
        Server::Server(uint16_t port, std::string const& backend_directory) :
        m_port(port),
        m_server(model::DataModel::use(), m_port),
        m_running(false),
        m_backend_directory(backend_directory)
        {
            using namespace std::placeholders; // for _1, _2 etc.
            
            m_server.bind_validator_factory(std::bind(&Server::createValidator, this, _1));
            m_server.bind_init(std::bind(&Server::initEmptyDocument, this, _1, _2));
            m_server.bind_read(std::bind(&Server::readSessionBackend, this, _1));
            m_server.bind_write(std::bind(&Server::writeSessionBackend, this, _1, _2));
            m_server.bind_authenticate(std::bind(&Server::authenticateUser, this, _1, _2, _3));
            
            if (m_backend_directory.exists() && !m_backend_directory.isDirectory())
            {
                throw std::runtime_error("Specified backend directory is a file");
            }
            else if(!m_backend_directory.exists())
            {
                m_backend_directory.createDirectory();
            }
        }
        
        Server::~Server()
        {
            ;
        }
        
        void Server::run()
        {
            if(!m_running)
            {
                flip::RunLoopTimer run_loop ([this](){
                    process();
                    return m_running.load();
                }, 0.1);
                
                std::cout << "[server] - running on port " << getPort() << std::endl;;
                
                m_running.store(true);
                run_loop.run();
            }
        }
        
        void Server::stop()
        {
            m_running.store(false);
            std::cout << "[server] - stopped" << std::endl;
        }
        
        bool Server::isRunning() const noexcept
        {
            return m_running;
        }
        
        uint64_t Server::getNewSessionId() const
        {
            uint64_t new_session_id = 0;
            while(m_files.find(new_session_id++) != m_files.end()) {}
            return new_session_id - 1;
        }
        
        juce::File Server::getSessionFile(uint64_t session_id)
        {
            if(m_files.find(session_id) != m_files.end())
            {
                return m_files[session_id];
            }
            else
            {
                juce::File file = m_backend_directory.getChildFile("Document_" + juce::String(session_id))
                .withFileExtension(kiwi_file_extension);
                
                m_files[session_id] = file;
                
                return file;
            }
        }
        
        uint16_t Server::getPort() const noexcept
        {
            return m_port;
        }
        
        uint16_t Server::getNumberOfActiveSessions() const noexcept
        {
            return m_server.nbr_sessions();
        }
        
        void Server::process()
        {
            m_server.process();
        }
        
        std::unique_ptr<flip::DocumentValidatorBase> Server::createValidator(uint64_t session_id)
        {
            DBG("[server] - createValidator for session_id : " << std::to_string(session_id));
            
            return std::make_unique<model::PatcherValidator>();
        }
        
        void Server::initEmptyDocument(uint64_t session_id, flip::DocumentBase& document)
        {
            DBG("[server] - initEmptyDocument for session_id : " << std::to_string(session_id));
            
            // init default patcher here.
            model::Patcher& patcher = document.root<model::Patcher>();
            patcher.setName("Document_" + std::to_string(session_id));
        }
        
        flip::BackEndIR Server::readSessionBackend(uint64_t session_id)
        {
            DBG("[server] - readSessionBackend for session_id : " << std::to_string(session_id));
            
            flip::BackEndIR backend;
            
            const auto session_file = getSessionFile(session_id);
            
            if (session_file.exists())
            {
                flip::DataProviderFile provider(session_file.getFullPathName().toStdString().c_str());
                
                backend.register_backend<flip::BackEndBinary>();
                backend.read(provider);
            }
            
            return backend;
        }
        
        void Server::writeSessionBackend(uint64_t session_id, flip::BackEndIR const& backend)
        {
            const auto session_file = getSessionFile(session_id);
            
            if(!session_file.exists())
            {
                session_file.create();
            }
            
            DBG("[server] - Saving session : " << std::to_string(session_id) << " in file : " << session_file.getFileName());
            
            flip::DataConsumerFile consumer(session_file.getFullPathName().toStdString().c_str());
            backend.write<flip::BackEndBinary>(consumer);
        }
        
        bool Server::authenticateUser(uint64_t user_id, uint64_t session_id, std::string metadata)
        {
            // @todo do something here
            
            DBG("[server] - Authenticate user: " << std::to_string(user_id) << " for session: " << std::to_string(session_id));
            
            return true;
        }
    }
}
