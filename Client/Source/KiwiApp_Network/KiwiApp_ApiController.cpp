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

namespace kiwi
{
    ApiController::ApiController()
    {
        auto& network_settings = getAppSettings().network();
        
        setHost(network_settings.getHost());
        setPort(network_settings.getApiPort());
        
        network_settings.addListener(*this);
    }
    
    ApiController::~ApiController()
    {
        getAppSettings().network().removeListener(*this);
    }
    
    void ApiController::networkSettingsChanged(NetworkSettings const& settings,
                                               const juce::Identifier& id)
    {
        if(id == Ids::host)
        {
            setHost(settings.getHost());
        }
        else if(id == Ids::api_port)
        {
            setPort(settings.getApiPort());
        }
    }
    
    void ApiController::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void ApiController::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    void ApiController::askUserToLogin(std::string const& message)
    {
        ; // todo
    }
    
    void ApiController::closeLoginForm()
    {
        ; // todo
    }
    
    void ApiController::login(std::string const& name_or_email,
                              std::string const& password)
    {
        m_listeners.call(&ApiController::Listener::AuthUserChanged, getAuthUser());
    }
    
    void ApiController::logout()
    {
        Api::Controller::logout();
        m_listeners.call(&ApiController::Listener::AuthUserChanged, getAuthUser());
    }
    
    void ApiController::restoreFromXml()
    {
        
    }
    
    void ApiController::saveToXml()
    {
        
    }
}
