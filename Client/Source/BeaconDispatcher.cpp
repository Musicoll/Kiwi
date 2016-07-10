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

#include "BeaconDispatcher.hpp"
#include "StoredSettings.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  BEACON DISPATCHER                               //
    // ================================================================================ //
    
    BeaconDispatcher::BeaconDispatcher(engine::Instance& instance) : m_instance(instance)
    {
        ;
    }
    
    void BeaconDispatcher::resized()
    {
        ;
    }
    
    void BeaconDispatcher::buttonClicked(juce::Button* btn)
    {
        if(btn == m_send_button.get())
        {
            ;
        }
    }
    
    // ================================================================================ //
    //                               BEACON DISPATCHER WINDOW                           //
    // ================================================================================ //
    
    BeaconDispatcherWindow::BeaconDispatcherWindow(engine::Instance& instance) :
    jWindow("Beacon dispatcher", juce::Colours::white, minimiseButton | closeButton, true)
    {
        setContentOwned(new BeaconDispatcher(instance), false);
        setResizable(true, false);
        setResizeLimits(50, 50, 32000, 32000);
        
        const juce::String windowState(getGlobalProperties().getValue("beacon_dispatcher_window"));
        
        if(windowState.isNotEmpty())
        {
            restoreWindowStateFromString(windowState);
        }
        else
        {
            setTopLeftPosition(10, 10);
            setSize(300, 300);
            setVisible(true);
        }
    }
    
    BeaconDispatcherWindow::~BeaconDispatcherWindow()
    {
        getGlobalProperties().setValue("beacon_dispatcher_window", getWindowStateAsString());
    }
    
    void BeaconDispatcherWindow::closeButtonPressed()
    {
        setVisible(false);
    }
}