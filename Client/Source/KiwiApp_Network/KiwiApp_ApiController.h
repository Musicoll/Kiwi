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

#include <KiwiTool/KiwiTool_Listeners.h>

#include "../KiwiApp_Network/KiwiApp_Api.h"
#include "../KiwiApp_General/KiwiApp_StoredSettings.h"

namespace kiwi
{
    
    class ApiController
    : public Api::Controller
    {
    public: // methods
        
        //! @brief Constructor
        ApiController();
        
        //! @brief Destructor
        //! @details If the "remember me" flag is true, the user profile will be saved when the object is destroyed.
        //! If false, the user profile will be saved whithout the token.
        ~ApiController();
        
        //! @brief Attempt to log the client api user in (Async).
        //! @param name_or_email The name or email of the user.
        //! @param password The user password.
        //! @see logout, isUserLoggedIn
        void login(std::string const& name_or_email,
                   std::string const& password,
                   std::function<void()> success_callback,
                   Api::ErrorCallback error_callback);
        
        //! @brief Attempt to register/signup the user.
        //! @param username user name
        //! @param email email address
        //! @param password password
        void signup(std::string const& username,
                    std::string const& email,
                    std::string const& password,
                    std::function<void(std::string)> success_cb,
                    Api::ErrorCallback error_cb);
        
        //! @brief Log-out the user.
        //! @see login, isUserLoggedIn
        void logout();
        
    private: // methods
        
        bool saveAuthUserProfile() const;
        bool restoreAuthUserProfile();
    };
}
