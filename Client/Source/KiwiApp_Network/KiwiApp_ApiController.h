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

#include <KiwiEngine/KiwiEngine_Listeners.h>

#include <juce_gui_extra/juce_gui_extra.h>

#include "../KiwiApp_Network/KiwiApp_Api.h"
#include "../KiwiApp_General/KiwiApp_StoredSettings.h"
#include "../KiwiApp_Auth/KiwiApp_LoginForm.h"

namespace kiwi
{
    class ApiController
    : public Api::Controller
    , private NetworkSettings::Listener
    {
    public: // nested classes
        
        struct Listener
        {
            virtual ~Listener() {}
            virtual void userConnectedCallback(Api::AuthUser const&) = 0;
            virtual void AuthUserChanged(Api::AuthUser const&) = 0;
        };
    
    public: // methods
        
        //! @brief Constructor
        ApiController();
        
        //! @brief Destructor
        ~ApiController();
        
        //! @brief Adds a listener.
        void addListener(Listener& listener);
        
        //! @brief Removes a listener.
        void removeListener(Listener& listener);
        
        //! @brief Attempt to log the client api user in (Async).
        //! @param name_or_email The name or email of the user.
        //! @param password The user password.
        //! @see logout, isUserLoggedIn
        void login(std::string const& name_or_email,
                   std::string const& password,
                   std::function<void()> success_callback,
                   Api::ErrorCallback error_callback);
        
        //! @brief Log-out the user.
        //! @see login, isUserLoggedIn
        void logout();
        
        //! @brief Restore the controller state.
        void restoreFromXml();
        
        //! @brief Save the controller state.
        void saveToXml();
        
    private: // methods
        
        //! @brief Called when the network settings has changed.
        void networkSettingsChanged(NetworkSettings const&, juce::Identifier const&) override;
        
    private: // variables
        
        engine::Listeners<Listener> m_listeners;
    };
}