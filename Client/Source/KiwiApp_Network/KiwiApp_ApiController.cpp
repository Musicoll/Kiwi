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

#include "KiwiApp_ApiController.h"

#include "../KiwiApp_General/KiwiApp_IDs.h"

#include "../KiwiApp.h"

namespace kiwi
{
    ApiController::ApiController()
    {
        auto& settings = getAppSettings().network();
        
        setHost(settings.getHost());
        setPort(settings.getApiPort());
        
        restoreAuthUserProfile();
    }
    
    ApiController::~ApiController()
    {
        auto& settings = getAppSettings().network();
        
        if(!settings.getRememberUserFlag())
        {
            clearToken();
        }
        
        saveAuthUserProfile();
    }
    
    bool ApiController::saveAuthUserProfile() const
    {
        return saveJsonToFile("UserProfile", {{"user", getAuthUser()}});
    }
    
    bool ApiController::restoreAuthUserProfile()
    {
        auto j = getJsonFromFile("UserProfile");
        if(j.count("user"))
        {
            from_json(j["user"], m_auth_user);
            return true;
        }
        
        return false;
    }
    
    void ApiController::setAuthUser(Api::AuthUser const& auth_user)
    {
        m_auth_user.resetWith(auth_user);
    }
    
    void ApiController::logout()
    {
        Api::Controller::clearToken();
    }
}
