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

#include <KiwiServer/KiwiServer_Server.h>

#include <json.hpp>

#include <flip/BackEndBinary.h>

#include <flip/contrib/DataProviderFile.h>
#include <flip/contrib/DataConsumerFile.h>
#include <flip/contrib/RunLoopTimer.h>

#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Def.h>
#include <KiwiModel/KiwiModel_Converters/KiwiModel_Converter.h>

#include <iomanip>

namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                      SERVER                                      //
        // ================================================================================ //
        
        using json = nlohmann::json;
        
        std::string hexadecimal_convert(uint64_t hexa_decimal)
        {
            std::stringstream converter;
            converter << std::setfill('0') << std::setw(16) << std::hex << hexa_decimal;
            
            return converter.str();
        }
        
        const char* Server::kiwi_file_extension = "kiwi";
        
        Server::Server(uint16_t port,
                       juce::File backend_directory,
                       std::string const& open_token,
                       std::string const& kiwi_version)
        : m_backend_directory(std::move(backend_directory))
        , m_open_token(open_token)
        , m_kiwi_version(kiwi_version)
        , m_sessions()
        , m_socket(*this, port)
        , m_ports()
        , m_logger(m_backend_directory.getChildFile("log.txt"),
                 "server port: " + std::to_string(port)
                 + " server model version: " + KIWI_MODEL_VERSION_STRING
                 + " server kiwi version: " + m_kiwi_version)
        {
            if (m_backend_directory.exists() && !m_backend_directory.isDirectory())
            {
                throw std::runtime_error("Specified backend directory is a file");
            }
            else if(!m_backend_directory.exists())
            {
                m_backend_directory.createDirectory();
            }
            
            if(!createEmptyDocument())
            {
                throw std::runtime_error("Failed to create empty document");
            }
        }
        
        Server::~Server()
        {
            m_socket.reset();
        }
        
        void Server::process()
        {
            m_socket.process();
        }
        
        std::set<uint64_t> Server::getSessions() const
        {
            std::set<uint64_t> sessions;
            
            for(auto & session : m_sessions)
            {
                sessions.insert(session.first);
            }
            
            return sessions;
        }
        
        std::set<uint64_t> Server::getConnectedUsers(uint64_t session_id) const
        {
            auto session = m_sessions.find(session_id);
            return session != m_sessions.end() ? session->second.getConnectedUsers() : std::set<uint64_t>();
        }
        
        bool Server::createEmptyDocument()
        {
            juce::File empty_file = m_backend_directory.getChildFile("empty").withFileExtension(".kiwi");
            
            if (!empty_file.exists())
            {
                if (empty_file.create().wasOk())
                {
                    model::PatcherValidator validator;
                    flip::DocumentServer empty_document(model::DataModel::use(), validator, 0);
                    
                    empty_document.commit();
                    
                    flip::BackEndIR backend(empty_document.write());
                    flip::DataConsumerFile consumer(empty_file.getFullPathName().toStdString().c_str());
                    
                    backend.write<flip::BackEndBinary>(consumer);
                }
            }
            
            return empty_file.exists();
        }
        
        juce::File Server::getSessionFile(uint64_t session_id) const
        {
            return m_backend_directory.getChildFile(juce::String(hexadecimal_convert(session_id)))
            .withFileExtension(kiwi_file_extension);
        }
        
        void Server::onConnected(flip::PortBase & port)
        {
            const auto session_id = port.session();
            const auto session = m_sessions.find(session_id);
            if(session == m_sessions.end())
            {
                juce::File session_file = getSessionFile(session_id);
                
                const auto session_hex_id = hexadecimal_convert(session_id);
                
                m_logger.log("Creating new session (" + session_hex_id + ")");
                
                auto session = m_sessions
                .insert(std::make_pair(session_id,
                                       Session(session_id, session_file, m_open_token, m_kiwi_version, m_logger)));
                
                if (session_file.exists())
                {
                    //m_logger.log("loading session file for session_id : " + session_hex_id);
                    
                    if (!(*session.first).second.load())
                    {
                        m_logger.log("opening document session : "
                                     + session_hex_id + " failed");
                        
                        m_sessions.erase(session_id);
                        
                        throw std::runtime_error("loading session failed.");
                    }
                }
                
                (*session.first).second.bind(port);
            }
            else
            {
                session->second.bind(port);
            }
        }
        
        void Server::onDisconnected(flip::PortBase & port)
        {
            port.impl_activate(false);
            
            auto session = m_sessions.find(port.session());
            
            if (session != m_sessions.end())
            {
                session->second.unbind(port);
                
                if (session->second.getConnectedUsers().empty())
                {
                    m_sessions.erase(session);
                }
            }
        }
        
        void Server::port_factory_add(flip::PortBase& port)
        {
            port.impl_bind (*this);
            
            m_ports.insert (&port);
        }
        
        void Server::port_factory_remove(flip::PortBase& port)
        {
            if (m_ports.erase (&port)) {port.impl_unbind (*this);}
            
            onDisconnected(port);
        }
        
        void Server::port_greet(flip::PortBase& port)
        {
            if (m_ports.erase (&port)) { port.impl_unbind (*this); }
            
            onConnected(port);
        }
        
        void Server::port_commit(flip::PortBase& /* port */, const flip::Transaction& /* tx */)
        {
            // nothing
        }
        
        void Server::port_squash(flip::PortBase& /* port */, const flip::TxIdRange& /* range */, const flip::Transaction& /* tx */)
        {
            // nothing
        }
        
        void Server::port_push(flip::PortBase& /* port */, const flip::Transaction& /* tx */)
        {
            // port was rebound in greet
            flip_FATAL;
        }
        
        void Server::port_signal(flip::PortBase& /* port */, const flip::SignalData& /* data */)
        {
            // port was rebound in greet
            flip_FATAL;
        }
        
        // ================================================================================ //
        //                                   SERVER LOGGER                                  //
        // ================================================================================ //
        
        Server::Logger::Logger(juce::File const& file, juce::String const& welcome):
        m_limit(10 * 1000 * 1000), // 10 Mo
        m_jlogger(file, welcome, m_limit)
        {
        }
        
        Server::Logger::~Logger()
        {
        }
        
        void Server::Logger::log(juce::String const& message)
        {
            if (m_jlogger.getLogFile().getSize() > m_limit)
            {
                juce::FileLogger::trimFileSize(m_jlogger.getLogFile(), m_limit);
            }
            
            auto time = juce::Time::Time::getCurrentTime().toString(true, true, true);
            
            juce::String log = "[server] - ";
            log << time << juce::newLine;
            log << message;
            
            m_jlogger.logMessage(log);
        }
        
        // ================================================================================ //
        //                                   SERVER SESSION                                 //
        // ================================================================================ //
        
        Server::Session::Session(Session && other)
        : m_identifier(other.m_identifier)
        , m_hex_id(other.m_hex_id)
        , m_validator(std::move(other.m_validator))
        , m_document(std::move(other.m_document))
        , m_signal_connections(std::move(other.m_signal_connections))
        , m_backend_file(std::move(other.m_backend_file))
        , m_token(std::move(other.m_token))
        , m_kiwi_version(std::move(other.m_kiwi_version))
        , m_logger(other.m_logger)
        {
            ;
        }
        
        Server::Session::Session(uint64_t identifier,
                                 juce::File const& backend_file,
                                 std::string const& token,
                                 std::string const& kiwi_version,
                                 Server::Logger & logger)
        : m_identifier(identifier)
        , m_hex_id(hexadecimal_convert(identifier))
        , m_validator(new model::PatcherValidator())
        , m_document(new flip::DocumentServer(model::DataModel::use(), *m_validator, m_identifier))
        , m_signal_connections()
        , m_backend_file(backend_file)
        , m_token(token)
        , m_kiwi_version(kiwi_version)
        , m_logger(logger)
        {
            model::Patcher& patcher = m_document->root<model::Patcher>();
            
            auto cnx = patcher.signal_get_connected_users.connect(std::bind(&Server::Session::sendConnectedUsers, this));
            
            m_signal_connections.emplace_back(std::move(cnx));
        }
        
        Server::Session::~Session()
        {
            if (m_document)
            {
                for (auto* port : m_document->ports())
                {
                    m_document->port_factory_remove(*port);
                }
            }
        }
        
        uint64_t Server::Session::getId() const
        {
            return m_identifier;
        }
        
        bool Server::Session::save() const
        {   
            flip::BackEndIR backend(m_document->write());
            
            flip::DataConsumerFile consumer(m_backend_file.getFullPathName().toStdString().c_str());
            
            try
            {
                backend.write<flip::BackEndBinary>(consumer);
            }
            catch(...)
            {
                return false;
            }
            
            return true;
        }
        
        bool Server::Session::load()
        {
            bool success = false;
            
            flip::BackEndIR backend;
            backend.register_backend<flip::BackEndBinary>();
            
            flip::DataProviderFile provider(m_backend_file.getFullPathName().toStdString().c_str());
            
            try
            {
                backend.read(provider);
            }
            catch (...)
            {
                m_logger.log("Fail to read " + m_backend_file.getFileName().toStdString());
                return false;
            }
            
            const auto current_version = model::Converter::getLatestVersion();
            const auto backend_version = backend.version;
            
            if(!model::Converter::canConvertToLatestFrom(backend_version))
            {
                m_logger.log("Bad document version: no conversion available from "
                             + backend_version + " to " + current_version);
                return false;
            }
            
            if (model::Converter::process(backend))
            {
                try
                {
                    m_document->read(backend);
                    m_document->commit();
                    success = true;
                }
                catch(...)
                {
                    m_logger.log("Failed to read " + m_backend_file.getFileName().toStdString());
                    return false;
                }
            }
            else
            {
                m_logger.log("Failed to convert document from version "
                             + backend_version + " to " + current_version);
                return false;
            }
            
            return success;
        }
        
        bool Server::Session::authenticateUser(uint64_t user, std::string metadata) const
        {
            json j;
            
            try
            {
                j = json::parse(metadata);
            }
            catch (json::parse_error& e)
            {
                std::cout << "message: " << e.what() << '\n';
                return false;
            }
            
            static const auto model_version = "model_version";
            static const auto open_token = "open_token";
            static const auto kiwi_version = "kiwi_version";
            
            return ((j.count(model_version) && j[model_version] == KIWI_MODEL_VERSION_STRING)
                    && (j.count(open_token) && j[open_token] == m_token)
                    && (j.count(kiwi_version) && j[kiwi_version] == m_kiwi_version));
        }
        
        void Server::Session::bind(flip::PortBase & port)
        {
            m_logger.log("user (" + std::to_string(port.user())
                                + ") connecting to session (" + hexadecimal_convert(m_identifier) + ")");
            
            if (authenticateUser(port.user(), port.metadata()))
            {
                // disconnect client if already connected.
                
                std::set<flip::PortBase*> ports = m_document->ports();
                
                std::set<flip::PortBase*>::iterator port_user = std::find_if(ports.begin(),
                                                                             ports.end(),
                                                                             [&port](flip::PortBase * const document_port)
                {
                    return document_port->user() == port.user();
                });
                
                if (port_user != ports.end())
                {
                    m_document->port_factory_remove(**port_user);
                }
                
                m_document->port_factory_add(port);
                
                m_document->port_greet(port);
                
                model::Patcher& patcher = m_document->root<model::Patcher>();
                
                std::set<uint64_t> user_lit = getConnectedUsers();
                std::vector<uint64_t> users(user_lit.begin(), user_lit.end());
                
                // send a list of connected users to the user that is connecting.
                m_document->send_signal_if(patcher.signal_receive_connected_users.make(users),
                                           [&port](flip::PortBase& current_port)
                                           {
                                               return port.user() == current_port.user();
                                           });
                
                // Notify other users that this one is connected.
                m_document->send_signal_if(patcher.signal_user_connect.make(port.user()),
                                           [&port](flip::PortBase& current_port)
                                           {
                                               return port.user() != current_port.user();
                                           });
            }
            else
            {
                m_logger.log("user (" + std::to_string(port.user())
                                    + ") failed to authenticate \n"
                                    + "with metadata : " + port.metadata());
                
                throw std::runtime_error("authentication failed");
            }
        }
        
        void Server::Session::unbind(flip::PortBase & port)
        {
            m_logger.log("disconnecting user (" + std::to_string(port.user())
                                + ") from session (" + m_hex_id + ')');
            
            std::set<flip::PortBase*> ports = m_document->ports();
            
            if (ports.find(&port) != ports.end())
            {
                model::Patcher& patcher = m_document->root<model::Patcher>();
                
                m_document->send_signal_if(patcher.signal_user_disconnect.make(port.user()),
                                           [](flip::PortBase& port)
                                           {
                                               return true;
                                           });
                
                m_document->port_factory_remove(port);
                
                if (m_document->ports().empty())
                {
                    if(!m_backend_file.exists())
                    {
                        m_backend_file.create();
                    }
                    
                    m_logger.log("saving session : " + m_hex_id
                                 + " in file : " + m_backend_file.getFileName().toStdString());
                    
                    if (!save())
                    {
                        m_logger.log("saving session to "
                                     + m_backend_file.getFileName() + " failed");
                    }
                }
            }
        }
        
        std::set<uint64_t> Server::Session::getConnectedUsers() const
        {
            std::set<uint64_t> users;
            
            auto const& ports = m_document->ports();
            
            for(auto const& port : ports)
            {
                users.emplace(port->user());
            }
            
            return users;
        }
        
        void Server::Session::sendConnectedUsers() const
        {
            model::Patcher& patcher = m_document->root<model::Patcher>();
            
            std::set<uint64_t> user_list = getConnectedUsers();
            std::vector<uint64_t> users(user_list.begin(), user_list.end());

            m_document->reply_signal(patcher.signal_receive_connected_users.make(users));
        }
    }
}
