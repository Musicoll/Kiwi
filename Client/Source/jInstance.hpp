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

#include <string>

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
        
        //! @brief Create a new remote patcher
        void openRemotePatcher();
        
        //! @brief Opens the settings
        void openSettings();
        
        //! @brief Get the user ID of the Instance.
        uint64_t getUserId() const noexcept;
        
        //! Brings the Console to front.
        void showConsoleWindow();
        
        //! @brief Returns the engine::Instance
        engine::Instance& getEngineInstance();
        
        //! @brief Returns the engine::Instance
        engine::Instance const& getEngineInstance() const;
        
    private:
        //! @brief Open a dialog box used to enter server infos
        void openRemoteDialogBox(std::string & host, uint16_t & port);
        
        //! @brief Sets the instance user id
        void setUserId(uint64_t user_id);
        
    private:
        uint64_t m_user_id;
        
        std::unique_ptr<engine::Instance>   m_instance;
        
        std::unique_ptr<jPatcherManager>    m_patcher_manager;
        
        std::unique_ptr<jConsoleWindow>     m_console_window;
    };
}

#endif // KIWI_JINSTANCE_HPP_INCLUDED
