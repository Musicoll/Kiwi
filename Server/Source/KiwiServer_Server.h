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

#include <flip/contrib/ServerSimple.h>
#include <flip/BackEndBinary.h>

#include <KiwiModel/KiwiModel_PatcherUser.h>

#include <juce_core/juce_core.h>

#include <atomic>

#include "KiwiServer_ServerBase.h"

namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                      SERVER                                      //
        // ================================================================================ //
        
        //! @brief The Server class.
        class Server : public ServerBase
        {
        public: // methods
            
            //! @brief Constructor.
            Server(uint16_t port, std::string const& backend_directory);
            
            //! @brief Destructor.
            ~Server();
            
            //! @brief Run the server.
            //! @details This will run a loop that call ServerBase::process regularly.
            //! @see stop
            void run();
            
            //! @brief Stops the server.
            //! @see Run, isRunning
            void stop();
            
            //! @brief Returns true if the server is running.
            bool isRunning() const noexcept;
            
            //! @brief Get the server running port
            uint16_t getPort() const noexcept;
            
        private: // methods
            
            //! @brief Initialise a new empty patcher
            std::unique_ptr<flip::DocumentValidatorBase> createValidator(uint64_t session_id) override;
            
            //! @brief Initialise a new empty patcher
            void initEmptyDocument(uint64_t session_id, flip::DocumentBase & document) override;
            
            //! @brief read a session backend.
            flip::BackEndIR readSessionBackend(uint64_t session_id) override;
            
            //! @brief write a session backend.
            void writeSessionBackend(uint64_t session_id, flip::BackEndIR const& backend) override;
            
            //! @brief Authenticate a user.
            bool authenticateUser(uint64_t user_id, uint64_t session_id, std::string metadata) override;
            
            //! @brief Get the path for a given session.
            juce::File getSessionFile(uint64_t session_id);
            
        private: // methods
            
            //static uint64_t hexadecimal_convert(std::string const& hexa_string);
            static std::string hexadecimal_convert(uint64_t hexa_decimal);
            
        private: // variables
            
            const uint16_t                      m_port;
            std::atomic_bool                    m_running;
            juce::File                          m_backend_directory;
            
            static const char*  kiwi_file_extension;
            
        private: // deleted methods
            
            Server(Server const& other) = delete;
            Server(Server &&other) = delete;
            Server &operator=(Server const& other) = delete;
            Server &operator=(Server &&other) = delete;
        };
    }
}
