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

#ifndef KIWI_ENGINE_TIMER_HPP_INCLUDED
#define KIWI_ENGINE_TIMER_HPP_INCLUDED

#include <memory>
#include <functional>

namespace kiwi
{
    namespace engine
    {
        class Instance;
        
        // ================================================================================ //
        //                                       TIMER                                      //
        // ================================================================================ //
        
        //! @brief Clock on the message thread
        //! @details The timer is used to call the tick method at a regular pace on the message thread.
        class Timer final
        {
        public:
            
            //! @brief Destructor.
            virtual ~Timer();
            
            //! @brief Lets the listener callback being called
            void start(int milliseconds_interval);
            
            //! @brief Stops the listener callback from being called
            void stop();
            
            // ================================================================================ //
            //                                     TIMER IMPL                                   //
            // ================================================================================ //
            
            //! @internal Timer implementation.
            class Impl
            {
            public:
                
                //! @brief Private constructor.
                Impl(std::function<void()> callback);
                
                //! @brief Destructor.
                virtual ~Impl() = default;
                
            protected:
                
                //! @brief Lets the listener callback being called
                virtual void impl_start(int interval) = 0;
                
                //! @brief Stops the listener callback from being called
                virtual void impl_stop() = 0;
                
                //! @brief The method implementation need to call to dispatch timer event callbacks.
                void tick();
                
            private: // members
                
                std::function<void()> m_callback;
                
            private: // deleted methods
                
                Impl(Timer const& other) = delete;
                Impl& operator=(Impl const& other) = delete;
                Impl(Impl && other) = delete;
                Impl& operator=(Impl && other) = delete;
                
                friend Timer;
            };
            
        private: //methods
            
            //! @brief Constructor.
            //! @param implementation The Timer implementation.
            Timer(std::unique_ptr<Timer::Impl> implementation);
            friend engine::Instance;
            
        private: // members
            
            std::unique_ptr<Timer::Impl> m_timer_impl;
            
        private: // deleted methods
            
            Timer(Timer const& other) = delete;
            Timer& operator=(Timer const& other) = delete;
            Timer(Timer && other) = delete;
            Timer& operator=(Timer && other) = delete;
        };
    }
}

#endif //KIWI_ENGINE_TIMER_HPP_INCLUDED
