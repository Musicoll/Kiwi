/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_APP_BEACON_DISPATCHER_HPP_INCLUDED
#define KIWI_APP_BEACON_DISPATCHER_HPP_INCLUDED

#include <KiwiEngine/KiwiEngine_Instance.hpp>

#include "KiwiApp_Window.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  BEACON DISPATCHER                               //
    // ================================================================================ //
    
    //! @brief A Component that allows to dispatch messages to Beacon::Castaway objects.
    class BeaconDispatcher :
    public juce::Component,
    public juce::Button::Listener
    {
    public:
        
        //! @brief Constructor.
        BeaconDispatcher(engine::Instance& instance);
        
        //! @brief Destructor.
        ~BeaconDispatcher() = default;
        
        //! @brief Called when resized.
        void resized() override;
        
        //! @brief Called when the button is clicked.
        void buttonClicked(juce::Button*) override;
    
    private: // methods
        
        //! @brief dispatch message to castaways
        void send(std::string const& name, std::vector<Atom> const& args) const;
        
    private: // members
        
        engine::Instance&                   m_instance;
        std::unique_ptr<juce::TextEditor>   m_beacon_name_editor;
        std::unique_ptr<juce::TextEditor>   m_message_editor;
        std::unique_ptr<juce::TextButton>   m_send_button;
    };
    
    // ================================================================================ //
    //                              BEACON DISPATCHER WINDOW                            //
    // ================================================================================ //
    
    //! @brief The BeaconDispatcherWindow
    class BeaconDispatcherWindow final : public AppWindow
    {
    public:
        BeaconDispatcherWindow(engine::Instance& instance);
        
        bool isMainWindow() const override;
        
        ~BeaconDispatcherWindow() = default;
    };
}


#endif // KIWI_APP_BEACON_DISPATCHER_HPP_INCLUDED
