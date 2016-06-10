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

#include "jGuiDevice.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                    JUCE TIMER                                    //
    // ================================================================================ //
    
    jTimer::jTimer(std::function<void()> callback) : engine::Timer::Impl(callback)
    {
        ;
    }
    
    void jTimer::timerCallback()
    {
        engine::Timer::Impl::tick();
    }
    
    void jTimer::impl_start(int interval)
    {
        startTimer(interval);
    }
    
    void jTimer::impl_stop()
    {
        stopTimer();
    }
    
    // ================================================================================ //
    //                                  JUCE GUI DEVICE                                 //
    // ================================================================================ //
    
    std::unique_ptr<engine::Timer::Impl> jGuiDevice::createTimer(std::function<void()> callback)
    {
        return std::unique_ptr<engine::Timer::Impl>(new jTimer(callback));
    }
}
