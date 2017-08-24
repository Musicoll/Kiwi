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
    
    void ApiController::login(std::string const& name_or_email,
                              std::string const& password,
                              std::function<void()> success_callback,
                              Api::ErrorCallback error_callback)
    {
        auto success_cb = [this, success_callback = std::move(success_callback)](Api::AuthUser user)
        {
            std::cout << "User " << user._id << " Authenticated !\n";
            
            auto& scheduler = KiwiApp::useInstance().useScheduler();
            scheduler.schedule([this, success_callback = std::move(success_callback), user = std::move(user)]() {
                
                m_auth_user = std::move(user);
                m_listeners.call(&ApiController::Listener::AuthUserChanged, m_auth_user);
                
                KiwiApp::commandStatusChanged();
                
                success_callback();
            });
        };
        
        auto& api = KiwiApp::useApi();
        api.login(name_or_email, password,
                  std::move(success_cb),
                  std::move(error_callback));
    }
    
    void ApiController::logout()
    {
        Api::Controller::clearToken();
        m_listeners.call(&ApiController::Listener::AuthUserChanged, getAuthUser());
    }
    
    void ApiController::restoreFromXml()
    {
        
    }
    
    void ApiController::saveToXml()
    {
        
    }
}
