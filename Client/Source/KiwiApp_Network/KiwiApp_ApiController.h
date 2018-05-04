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
        
        //! @brief Changes the API's authenticated user.
        //! @details All authenticated will now use this user to
        //! query the server.
        void setAuthUser(Api::AuthUser const& auth_user);
        
        //! @brief Log-out the user.
        //! @see login, isUserLoggedIn
        void logout();
        
    private: // methods
        
        bool saveAuthUserProfile() const;
        bool restoreAuthUserProfile();
    };
}
