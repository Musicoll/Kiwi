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

#ifndef KIWI_ENGINE_CONSOLE_HPP_INCLUDED
#define KIWI_ENGINE_CONSOLE_HPP_INCLUDED

#include <iostream>
#include <sstream>
#include <mutex>
#include <string>
#include <set>
#include <algorithm>

#include <KiwiModel/KiwiModelAtom.hpp>
#include "KiwiEngineListeners.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      CONSOLE                                     //
        // ================================================================================ //
        
        //! @brief The console is an interface that let you print messages.
        class Console
        {
        public:
            
            //! @brief Constructor
            Console() = default;
            
            //! @brief Destructor
            ~Console() = default;
            
            //! @brief The Console Message owns the informations of a message posted via a console.
            class Message
            {
            public:
                
                //! @brief Type of message
                enum class Type
                {
                    Log = 0,
                    Normal,
                    Warning,
                    Error
                };

                std::string text;
                Type        type;
            };
            //! @brief Print a post-type message in the console.
            void post(Message const& mess);
            
            //! @brief You can inherit from this class to receive console changes.
            class Listener
            {
            public:
                virtual ~Listener() = default;
                
                //! @brief Receive the Console messages.
                virtual void newConsoleMessage(Console::Message const& message) = 0;
            };
            
            //! @brief Add an history listener.
            void addListener(Listener& listener);
            
            //! @brief Remove an history listener.
            void removeListener(Listener& listener);
            
        private:
            
            Listeners<Listener>  m_listeners;
        };
    }
}

#endif // KIWI_ENGINE_CONSOLE_HPP_INCLUDED