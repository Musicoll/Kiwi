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

#include "KiwiServer.hpp"

namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                      SERVER                                      //
        // ================================================================================ //
        
        const char* Server::kiwi_file_extension = "kiwi";
        
        Server::Server(uint16_t port) :
        m_port(port),
        m_server(model::DataModel::use(), m_port)
        {
            using namespace std::placeholders; // for _1, _2 etc.
            
            m_server.bind_validator_factory(std::bind(&Server::createValidator, this, _1));
            m_server.bind_init(std::bind(&Server::initEmptyDocument, this, _1, _2));
            m_server.bind_read(std::bind(&Server::readSessionBackend, this, _1));
            m_server.bind_write(std::bind(&Server::writeSessionBackend, this, _1, _2));
            m_server.bind_authenticate(std::bind(&Server::authenticateUser, this, _1, _2, _3));
            
            initBackendDirectory("server_backend");
            initService();
        }
        
        Server::~Server()
        {
            ;
        }
        
        void Server::run()
        {
            flip::RunLoopTimer run_loop ([this](){
                process();
                return true;
            }, 0.1);
            
            std::cout << "- KiwiServer running on port " << getPort() << '\n';
            
            run_loop.run(); // never returns
        }
        
        bool Server::initBackendDirectory(char const* name)
        {
            m_backend_files_path = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile).getSiblingFile(name);
            
            return juce::Result::ok() == m_backend_files_path.createDirectory();
        }
        
        void Server::initService()
        {
            ServiceProvider::metadata_t metadata;
            metadata["computer_name"] = juce::SystemStats::getComputerName().toStdString();
            
            juce::Array<juce::File> files;
            juce::String wild_card_pattern("*.kiwi");
            m_backend_files_path.findChildFiles(files, juce::File::findFiles, false, wild_card_pattern);
            
            if(!files.isEmpty())
            {
                juce::var var;
                std::vector<juce::DynamicObject::Ptr> file_properties;
                
                for(auto file : files)
                {
                    juce::DynamicObject::Ptr file_prop = new juce::DynamicObject();
                    
                    file_prop->setProperty("name", file.getFileNameWithoutExtension());
                    file_prop->setProperty("session_id", static_cast<long long>(m_files.size()));
                    var.append(file_prop.get());
                    file_properties.push_back(file_prop);
                    
                    m_files[m_files.size()] = file;
                }
                
                std::string documents = juce::JSON::toString(var).toStdString();
                
                file_properties.clear();
                
                metadata["backend_files_list"] = documents;
                //std::cout << "serving kiwi files : " << documents << '\n';
            }
            
            metadata["new_session_id"] = std::to_string(m_files.size()+1);
            
            m_service.reset(new ServiceProvider(*this, metadata));
        }
        
        void Server::updateMetadata()
        {
            if(!m_files.empty())
            {
                juce::var var;
                std::vector<juce::DynamicObject::Ptr> file_properties;
                
                for(auto file_pair : m_files)
                {
                    juce::DynamicObject::Ptr file_prop = new juce::DynamicObject();
                    
                    const auto file = file_pair.second;
                    
                    file_prop->setProperty("name", file.getFileNameWithoutExtension());
                    file_prop->setProperty("session_id", static_cast<long long>(file_pair.first));
                    var.append(file_prop.get());
                    file_properties.push_back(file_prop);
                }
                
                std::string documents = juce::JSON::toString(var).toStdString();
                
                file_properties.clear();
                
                (*m_service)["backend_files_list"] = documents;
                (*m_service)["new_session_id"] = std::to_string(m_files.size()+1);
                m_service->update();
            }
        }
        
        void Server::setSessionsBackendDirectory(std::string const& directory)
        {
            m_backend_files_path = directory;
        }
        
        juce::File Server::getSessionFile(uint64_t session_id)
        {
            if(session_id < m_files.size())
            {
                return m_files[session_id];
            }
            else
            {
                juce::File file = m_backend_files_path.getChildFile("Document_" + juce::String(session_id))
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
            m_service->process();
        }
        
        std::unique_ptr<flip::DocumentValidatorBase> Server::createValidator(uint64_t session_id)
        {
            std::cout << "createValidator - session_id : " << std::to_string(session_id) << '\n';
            
            return std::make_unique<PatcherValidator>();
        }
        
        void Server::initEmptyDocument(uint64_t session_id, flip::DocumentBase& document)
        {
            std::cout << "initEmptyDocument - session_id : " << std::to_string(session_id) << '\n';
            
            // init default patcher here.
            model::Patcher& patcher = document.root<model::Patcher>();
            patcher.setName("Document " + std::to_string(session_id));
        }
        
        flip::BackEndIR Server::readSessionBackend(uint64_t session_id)
        {
            std::cout << "readSessionBackend - session_id : " << std::to_string(session_id) << '\n';
            
            flip::BackEndIR backend;
            
            const auto session_file = getSessionFile(session_id);
            
            if (session_file.exists())
            {
                flip::DataProviderFile provider(session_file.getFullPathName().toStdString().c_str());
                
                backend.register_backend<flip::BackEndBinary>();
                backend.read(provider);
            }
            
            updateMetadata();
            
            return backend;
        }
        
        void Server::writeSessionBackend(uint64_t session_id, flip::BackEndIR const& backend)
        {
            std::cout << "Saving session [ " << std::to_string(session_id) << " ]";
            
            const auto session_file = getSessionFile(session_id);
            
            if(!session_file.exists())
            {
                session_file.create();
            }
            
            std::cout << " in file : " << session_file.getFileName() << "\n";
            
            flip::DataConsumerFile consumer(session_file.getFullPathName().toStdString().c_str());
            backend.write<flip::BackEndBinary>(consumer);
        }
        
        bool Server::authenticateUser(uint64_t user_id, uint64_t session_id, std::string metadata)
        {
            // @todo do something here
            
            std::cout << "Authenticate user [ " << std::to_string(user_id) << " ] \n";
            
            return true;
        }
    }
}
