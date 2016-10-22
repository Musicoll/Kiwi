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

#ifndef KIWI_SERVER_HPP_INCLUDED
#define KIWI_SERVER_HPP_INCLUDED

#include "flip/Document.h"
#include "flip/contrib/ServerSimple.h"
#include "flip/contrib/MulticastServiceProvider.h"
#include "flip/contrib/DataProviderFile.h"
#include "flip/contrib/DataConsumerFile.h"
#include "flip/BackEndBinary.h"

#include <KiwiModel/KiwiModel_DataModel.hpp>
#include "KiwiPatcherManager.hpp"

#include "KiwiPatcherValidator.hpp"

#include <juce_core/juce_core.h>

namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                      SERVER                                      //
        // ================================================================================ //
        
        //! @brief The Server class.
        class Server
        {
        public:
            
            //! @brief Constructor.
            Server(uint16_t port);
            
            //! @brief Destructor.
            ~Server();
            
            //! @brief Server process
            void process();
            
            //! @brief Loop that retrieves user input to manager server.
            void run();
            
            //! @brief Get the server running port
            uint16_t getPort() const noexcept;
            
            //! @brief Get the number of sessions currently running.
            uint16_t getNumberOfActiveSessions() const noexcept;
            
            //! @brief set the sessions' backend files directory.
            void setSessionsBackendDirectory(std::string const& directory);
            
        private: // methods
            
            //! @brief Initialise a new empty patcher
            std::unique_ptr<flip::DocumentValidatorBase> createValidator(uint64_t session_id);
            
            //! @brief Initialise a new empty patcher
            void initEmptyDocument(uint64_t session_id, flip::DocumentBase & document);
            
            //! @brief read a session backend.
            flip::BackEndIR readSessionBackend(uint64_t session_id);
            
            //! @brief write a session backend.
            void writeSessionBackend(uint64_t session_id, flip::BackEndIR const& backend);
            
            //! @brief Authenticate a user.
            bool authenticateUser(uint64_t user_id, uint64_t session_id, std::string metadata);
            
            //! @brief Get the path for a given session.
            juce::File getSessionFile(uint64_t session_id);
            
            //! create a backend directory
            bool initBackendDirectory(char const* name);
            
        private: // members
            
            using metadata_t = std::map<std::string, std::string>;
            using service_t = std::map<std::string, std::string>;
            
            const uint16_t                  m_port;
            flip::ServerSimple              m_server;
            flip::Document                  m_service_document;
            flip::MulticastServiceProvider  m_service_provider;
            
            juce::File                      m_backend_files_path;
            
            static const char* kiwi_file_extension;
            
        private: // deleted methods
            
            Server(Server const& other) = delete;
            Server(Server &&other) = delete;
            Server &operator=(Server const& other) = delete;
            Server &operator=(Server &&other) = delete;
        };
    }
}

#endif // KIWI_SERVER_HPP_INCLUDED
