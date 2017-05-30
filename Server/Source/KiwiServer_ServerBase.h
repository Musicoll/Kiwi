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

#pragma once

#include "flip/config.h"
#include "flip/contrib/transport_tcp/PortTransportServerTcp.h"
#include "flip/DocumentServer.h"

#include "flip/detail/DocumentValidatorBase.h"
#include "flip/detail/BackEndBase.h"
#include "flip/Signal.h"

#include <map>
#include <memory>

namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                      SERVER                                      //
        // ================================================================================ //
        
        class ServerBase : public flip::PortFactoryListener, public flip::PortListener
        {
        public: // methods
            
            class Session;
            using SessionMap = std::map<uint64_t, Session>;
            
            //! @brief Constructor.
            ServerBase(flip::DataModelBase const& data_model, uint16_t port);
            
            //! @brief Destructor.
            virtual ~ServerBase();
            
            //! @brief Server process
            //! @details Must be called regularly.
            void process();
            
            //! @brief Get the number of sessions currently running.
            size_t getNumberOfSessions() const;
            
            //! @brief Write the Session
            void writeSession(flip::DocumentServer& server, uint64_t session_id);
            
        protected: // methods
            
            //! @brief Create a document validator.
            virtual std::unique_ptr<flip::DocumentValidatorBase> createValidator(uint64_t session_id) = 0;
            
            //! @brief Initialise a new empty document.
            virtual void initEmptyDocument(uint64_t session_id, flip::DocumentBase & document) = 0;
            
            //! @brief read a session backend.
            virtual flip::BackEndIR readSessionBackend(uint64_t session_id) = 0;
            
            //! @brief write a session backend.
            virtual void writeSessionBackend(uint64_t session_id, flip::BackEndIR const& backend) = 0;
            
            //! @brief Authenticate a user.
            virtual bool authenticateUser(uint64_t user_id, uint64_t session_id, std::string metadata) = 0;
            
            //! @brief Called when a new Session has been created.
            virtual void sessionCreated(Session& session) {};
            
            //! @brief Called when a user connects to a document.
            virtual void onConnected(Session& session, uint64_t user_id) {};
            
            //! @brief Called when a user has been disconnected from a document.
            virtual void onDisconnected(Session& session, uint64_t user_id) {};
            
        private: // methods
            
            SessionMap::iterator create_session(uint64_t session_id);
            void load_session(flip::DocumentServer & server, uint64_t session_id);
            
            // PortFactoryListener
            virtual void port_factory_add(flip::PortBase & port) override;
            virtual void port_factory_remove(flip::PortBase & port) override;
            
            // PortListener
            virtual void port_greet(flip::PortBase& from) override;
            
            virtual void port_commit(flip::PortBase& from,
                                     flip::Transaction const & tx) override;
            
            virtual void port_squash(flip::PortBase& from,
                                     flip::TxIdRange const& range,
                                     flip::Transaction const& tx) override;
            
            virtual void port_push(flip::PortBase& from,
                                   flip::Transaction const& tx) override;
            
            virtual void port_signal(flip::PortBase& from,
                                     const flip::SignalData & data) override;
            
        private: // variables
            
            const flip::DataModelBase&      m_data_model;
            flip::PortTransportServerTcp    m_port;
            SessionMap                      m_sessions;
            
        private: // deleted methods
            
            ServerBase() = delete;
            ServerBase(const ServerBase & rhs) = delete;
            ServerBase(ServerBase && rhs) = delete;
            ServerBase& operator =(const ServerBase & rhs) = delete;
            ServerBase& operator =(ServerBase && rhs) = delete;
            bool operator ==(const ServerBase & rhs) const = delete;
            bool operator !=(const ServerBase & rhs) const = delete;
        };
        
        // ================================================================================ //
        //                                   SERVER SESSION                                 //
        // ================================================================================ //
        
        class ServerBase::Session
        {
        public: // methods
            
            //! @brief Constructor.
            //! @details Create a Session by moving another Session.
            Session(Session && rhs);
            
            //! @brief Constructor.
            Session(ServerBase& parent,
                    uint64_t session_id,
                    std::unique_ptr<flip::DocumentServer> && document_uptr,
                    std::unique_ptr<flip::DocumentValidatorBase> && validator_uptr);
            
            //! @brief Destructor.
            virtual ~Session() = default;
            
            //! @brief Get the Session identifier.
            uint64_t identifier() const;
            
            //! @brief Get the document.
            flip::DocumentServer& document();
            
            //! @brief Write the session.
            void write();
            
            //! @brief Returns a list of connected users.
            std::vector<uint64_t> getConnectedUsers();
            
            //! @brief Store the signal connection in the session.
            void storeSignalConnection(flip::SignalConnection && cnx);
            
        private: // variables
            
            ServerBase&                                     m_parent;
            const uint64_t                                  m_identifier;
            std::unique_ptr<flip::DocumentValidatorBase>    m_validator_uptr;
            std::unique_ptr<flip::DocumentServer>           m_document_uptr;
            
            std::vector<flip::SignalConnection>             m_signal_connections;
            
        private: // variables
            
            Session() = delete;
            Session(const Session & rhs) = delete;
            Session& operator = (const Session & rhs) = delete;
            Session& operator = (Session && rhs) = delete;
        };
    }
}
