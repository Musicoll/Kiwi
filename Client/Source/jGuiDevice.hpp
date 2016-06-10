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

#ifndef KIWI_JUCE_GUI_DEVICE_HPP_INCLUDED
#define KIWI_JUCE_GUI_DEVICE_HPP_INCLUDED

#include <KiwiEngine/KiwiGuiDevice.hpp>

#include "../JuceLibraryCode/JuceHeader.h"

namespace kiwi
{
    class jTimer : public juce::Timer, public engine::Timer::Impl
    {
    public:
        
        //! @brief Constructor.
        jTimer(std::function<void()> callback);
        
        //! @brief Destructor.
        ~jTimer() = default;
        
        //! @brief juce::Timer callback
        void timerCallback() override;
        
        //! @brief Start timer.
        void impl_start(int interval) override;
        
        //! @brief Stop timer.
        void impl_stop() override;
    };
    
    // ================================================================================ //
    //                                  JUCE GUI DEVICE                                 //
    // ================================================================================ //
    
    //! @brief The gui device.
    class jGuiDevice : public engine::GuiDevice
    {
	public:
		
        //! @brief Constructor.
        jGuiDevice() = default;
        
        //! @brief Destructor.
        ~jGuiDevice() = default;
        
        //! @brief Creates and returns a timer implementation.
        std::unique_ptr<engine::Timer::Impl> createTimer(std::function<void()> callback) override;
    };
}

#endif // KIWI_JUCE_GUI_DEVICE_HPP_INCLUDED
