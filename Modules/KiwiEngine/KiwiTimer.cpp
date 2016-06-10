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

#include "KiwiTimer.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                     TIMER IMPL                                   //
        // ================================================================================ //
        
        Timer::Impl::Impl(std::function<void()> callback) : m_callback(callback)
        {
            ;
        }
        
        void Timer::Impl::tick()
        {
            if(m_callback)
            {
                m_callback();
            }
        }
        
        // ================================================================================ //
        //                                       TIMER                                      //
        // ================================================================================ //
        
        Timer::Timer(std::unique_ptr<Timer::Impl> implementation) :
        m_timer_impl(std::move(implementation))
        {
            ;
        }
        
        Timer::~Timer()
        {
            stop();
        }
        
        void Timer::start(int interval)
        {
            if(m_timer_impl)
            {
                m_timer_impl->impl_start(interval);
            }
        }
        
        void Timer::stop()
        {
            if(m_timer_impl)
            {
                m_timer_impl->impl_stop();
            }
        }
    }
}
