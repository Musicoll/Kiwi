/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_SERVER_HPP_INCLUDED
#define KIWI_SERVER_HPP_INCLUDED

#include <map>

#include "KiwiPatcherManager.hpp"

namespace kiwi
{
    namespace server
    {
        //! @brief The Server class.
        class Server
        {
        public:
            
            //! @brief Constructor.
            Server() = default;
            
            //! @brief Destructor.
            ~Server() = default;
            
            //! @brief Loop that retrieves user input to manager server.
            void runCommand();
            
        private: // methods
            
            //! @brief Open the patcher that has document_id as id.
            void openPatcher(uint64_t document_id);
            
            //! @brief Close the patcher that has document_id as id.
            void closePatcher(uint64_t patcher_id);
            
        private: // members
            
            std::map<uint64_t, PatcherManager>        m_patchers;
            
        private: // deleted methods
            
            Server(Server const& other) = delete;
            Server(Server &&other) = delete;
            Server &operator=(Server const& other) = delete;
            Server &operator=(Server &&other) = delete;
        };
    }
}

#endif // KIWI_SERVER_HPP_INCLUDED
