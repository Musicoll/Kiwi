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

#ifndef KIWI_CORE_TIMER_HPP_INCLUDED
#define KIWI_CORE_TIMER_HPP_INCLUDED

#include <memory>

#include "KiwiListeners.hpp"

namespace kiwi
{
    class Timer final
    {
    public:
        static Timer &getTimer();
        
        void tick();
        
        class Listener
        {
        public:
            virtual void tick() = 0;
            
            virtual ~Listener();
            
        protected:
            void startTimer();
            void stopTimer();
        };
        
        void addListener(Listener &listener);
        void removeListener(Listener &listener);
        
        ~Timer() = default;
        
    private:
        Timer() = default;
        
    private:
        static std::unique_ptr<Timer> m_instance;
        Listeners<Listener>  m_listeners;
        
    private:
        Timer(Timer const& other) = delete;
        Timer& operator=(Timer const& other) = delete;
        Timer(Timer && other) = delete;
        Timer& operator=(Timer && other) = delete;
    };
}

#endif //KIWI_CORE_TIMER_HPP_INCLUDED
