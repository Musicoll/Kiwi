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

#include "flip/Document.h"

#include "KiwiEngine_Console.hpp"
#include "KiwiEngine_Patcher.hpp"
#include "KiwiEngine_Beacon.hpp"
#include "KiwiEngine_AudioControler.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        //! @brief The Instance adds the engine objects to the engine::Factory
        //! @details The patcher document model must be initialised before creating the Instance
        //! The Instance is also the entry point to retrieve beacons and output messages to the console.
        class Instance : public Beacon::Factory
        {
        public: // methods
            
            //! @brief Constructs an Instance and adds the engine objects to the engine::Factory.
            Instance(std::unique_ptr<AudioControler> audio_controler);
            
            //! @brief Destructor.
            ~Instance();
            
            // ================================================================================ //
            //                                      CONSOLE                                     //
            // ================================================================================ //
            
            //! @brief post a log message in the Console.
            void log(std::string const& text) const;
            
            //! @brief post a message in the Console.
            void post(std::string const& text) const;
            
            //! @brief post a warning message in the Console.
            void warning(std::string const& text) const;
            
            //! @brief post an error message in the Console.
            void error(std::string const& text) const;
            
            //! @brief Adds a console listener.
            void addConsoleListener(Console::Listener& listener);
            
            //! @brief Removes a console listener.
            void removeConsoleListener(Console::Listener& listener);
            
            // ================================================================================ //
            //                              AUDIO CONTROLER                                     //
            // ================================================================================ //
            
            AudioControler& getAudioControler() const;
            
        private: // methods
            
            //! @internal Adds the engine objects to the engine::Factory
            void addObjectsToFactory();
            
        private: // members
            
            Console m_console;
            
            std::unique_ptr<AudioControler> m_audio_controler;
            
        private: // deleted methods
            
            Instance(Instance const&) = delete;
            Instance(Instance&&) = delete;
            Instance& operator=(Instance const&) = delete;
            Instance& operator=(Instance&&) = delete;
        };
    }
}
