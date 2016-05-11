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

#ifndef KIWI_JINSTANCE_HPP_INCLUDED
#define KIWI_JINSTANCE_HPP_INCLUDED

#include <KiwiEngine/KiwiInstance.hpp>

#include "flip/Document.h"

#include "jConsole.hpp"
#include "jPatcherManager.hpp"

namespace kiwi
{
    //! @brief The jInstance
    class jInstance
    {
    public:
        
        //! @brief Constructor
        jInstance();
        
        //! @brief Destructor
        ~jInstance();
        
        //! @brief create a new patcher window.
        void newPatcher();
        
        //! @brief Get the user ID of the Instance.
        uint64_t getUserId() const noexcept;
        
    private:
        
        //! @internal temporary method to pre-populate new patchers
        void populatePatcher(model::Patcher& patcher);
        
    private:
        
        const uint64_t m_user_id;
        
        std::unique_ptr<engine::Instance>   m_instance;
        
        std::unique_ptr<jPatcherManager>    m_patcher_manager;
        
        std::unique_ptr<jConsoleWindow>     m_console_window;
    };
}

#endif // KIWI_JINSTANCE_HPP_INCLUDED
