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
    std::unique_ptr<Timer> Timer::m_instance;
    
    Timer& Timer::getTimer()
    {
        if (m_instance == nullptr)
        {
            m_instance.reset(new Timer());
        }
        
        return *m_instance;
    }
    
    void Timer::tick()
    {
        m_listeners.call(&Listener::tick);
    }
    
    void Timer::addListener(Listener &listener)
    {
        m_listeners.add(listener);
    }
    
    void Timer::removeListener(Listener &listener)
    {
        m_listeners.remove(listener);
    }
    
    void Timer::Listener::startTimer()
    {
        Timer::getTimer().addListener(*this);
    }
    
    void Timer::Listener::stopTimer()
    {
        Timer::getTimer().removeListener(*this);
    }
    
    Timer::Listener::~Listener()
    {
        stopTimer();
    }
}