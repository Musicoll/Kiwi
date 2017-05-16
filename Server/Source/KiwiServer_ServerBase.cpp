/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot& Eliott Paris.
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

#include "KiwiServer_ServerBase.h"

#include "flip/DocumentValidatorVoid.h"
#include "flip/detail/def.h"
#include "flip/detail/fnc.h"

#include <cassert>

namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                   SERVER SESSION                                 //
        // ================================================================================ //
        
        ServerBase::Session::Session(Session && rhs)
        : m_parent(rhs.m_parent)
        , m_identifier(std::move(rhs.m_identifier))
        , m_validator_uptr(std::move(rhs.m_validator_uptr))
        , m_document_uptr(std::move(rhs.m_document_uptr))
        {
            ;
        }
        
        ServerBase::Session::Session(ServerBase& parent,
                                     uint64_t identifier,
                                     std::unique_ptr<flip::DocumentServer> && document_uptr,
                                     std::unique_ptr<flip::DocumentValidatorBase> && validator_uptr)
        : m_parent(parent)
        , m_identifier(identifier)
        , m_validator_uptr(std::move(validator_uptr))
        , m_document_uptr(std::move(document_uptr))
        {
            ;
        }
        
        uint64_t ServerBase::Session::identifier() const
        {
            return m_identifier;
        }

        flip::DocumentServer& ServerBase::Session::document()
        {
            return *m_document_uptr;
        }
        
        void ServerBase::Session::write()
        {
            m_parent.writeSession(document(), identifier());
        }
        
        // ================================================================================ //
        //                                      SERVER                                      //
        // ================================================================================ //
        
        ServerBase::ServerBase(flip::DataModelBase const& data_model, uint16_t port)
        : m_data_model(data_model)
        , m_port(*this, port)
        , m_sessions()
        {
        }

        ServerBase::~ServerBase()
        {
            for(auto& pair : m_sessions)
            {
                auto& server = pair.second.document();
                
                auto ports = server.ports();
                
                for(auto* port_ptr : ports)
                {
                    server.port_factory_remove(*port_ptr);
                    port_ptr->impl_bind(*this);
                }
            }
            
            m_sessions.clear();
            
            m_port.reset();
        }
        
        void ServerBase::process()
        {
            m_port.process();
        }
        
        size_t ServerBase::getNumberOfSessions() const
        {
            return m_sessions.size();
        }
        
        void ServerBase::writeSession(flip::DocumentServer& server, uint64_t session_id)
        {
            auto backend = server.write();
            writeSessionBackend(session_id, backend);
        }
        
        void ServerBase::port_factory_add(flip::PortBase& port)
        {
            port.impl_bind(*this);
        }
        
        void ServerBase::port_factory_remove(flip::PortBase& port)
        {
            port.impl_activate(false);
            
            bool was_attached_flag = false;
            
            auto it = m_sessions.begin();
            const auto it_end = m_sessions.end();
            
            for(; it != it_end ;)
            {
                auto it_next = it;
                ++it_next;
                
                auto session_id = it->first;
                auto& document_server = it->second.document();
                const auto& ports = document_server.ports();
                
                if(ports.find(&port) != ports.end())
                {
                    was_attached_flag = true;
                    document_server.port_factory_remove(port);
                    
                    if(ports.empty())
                    {
                        writeSession(document_server, session_id);
                        
                        m_sessions.erase(it);
                    }
                    
                    break;
                }
                
                it = it_next;
            }
            
            if(!was_attached_flag)
            {
                port.impl_unbind(*this);
            }
        }
        
        void ServerBase::port_greet(flip::PortBase& from)
        {
            if(!authenticateUser(from.user(), from.session(), from.metadata()))
            {
                throw std::runtime_error("authentication failed");
            }
            
            auto session_id = from.session();
            auto it = m_sessions.find(session_id);
            
            if(it == m_sessions.end())
            {
                it = create_session(session_id);
            }
            
            assert(it != m_sessions.end());
            
            auto& document_server = it->second.document();
            
            // rebind
            from.impl_unbind(*this);
            document_server.port_factory_add(from);
            
            // forward
            document_server.port_greet(from);
        }
        
        void ServerBase::port_commit(flip::PortBase& /* from */, const flip::Transaction& /* tx */)
        {
            // nothing
        }
        
        void ServerBase::port_squash(flip::PortBase& /* from */, const flip::TxIdRange& /* range */, const flip::Transaction& /* tx */)
        {
            // nothing
        }

        void ServerBase::port_push(flip::PortBase& /* from */, const flip::Transaction& /* tx */)
        {
            // port was rebound in greet
            flip_FATAL;
        }

        void ServerBase::port_signal(flip::PortBase& /* from */, const flip::SignalData& /* data */)
        {
            // port was rebound in greet
            flip_FATAL;
        }
        
        ServerBase::SessionMap::iterator ServerBase::create_session(uint64_t session_id)
        {
            auto validator_uptr = createValidator(session_id);
            
            auto document_uptr = std::make_unique<flip::DocumentServer>(m_data_model,
                                                                        *validator_uptr,
                                                                        session_id);
            
            load_session(*document_uptr, session_id);
            
            auto && pair = std::make_pair(session_id, Session(*this, session_id,
                                                              std::move(document_uptr),
                                                              std::move(validator_uptr)));
            
            return m_sessions.insert(std::move(pair)).first;
        }
        
        void ServerBase::load_session(flip::DocumentServer& server, uint64_t session_id)
        {
            flip::BackEndIR backend(readSessionBackend(session_id));
            
            if(backend.empty())
            {
                initEmptyDocument(session_id, server);
            }
            else
            {
                server.read(backend);
            }
            
            server.commit();
        }
    }
}
