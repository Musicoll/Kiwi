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

#include <thread>
#include <atomic>

#include "flip/Document.h"

#include <KiwiTool/KiwiTool_Beacon.h>

#include "KiwiEngine_Console.h"
#include "KiwiEngine_Patcher.h"
#include "KiwiEngine_AudioControler.h"

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
        class Instance : public tool::Beacon::Factory
        {
        public: // methods
            
            //! @brief Constructs an Instance and adds the engine objects to the engine::Factory.
            Instance(std::unique_ptr<AudioControler> audio_controler, tool::Scheduler<> & main_scheduler);
            
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
            
            // ================================================================================ //
            //                              SCHEDULER                                           //
            // ================================================================================ //
            
            //! @brief Returns the engine's scheduler.
            tool::Scheduler<> & getScheduler();
            
            //! @brief Returns the main's scheduler.
            tool::Scheduler<> & getMainScheduler();
            
        private: // methods
            
            //! @internal Processes the scheduler to check if new messages have been added.
            void processScheduler();
            
        private: // members
            
            Console m_console;
            
            std::unique_ptr<AudioControler> m_audio_controler;
            tool::Scheduler<>               m_scheduler;
            tool::Scheduler<>&              m_main_scheduler;
            std::atomic<bool>               m_quit;
            std::thread                     m_engine_thread;
            
        private: // deleted methods
            
            Instance(Instance const&) = delete;
            Instance(Instance&&) = delete;
            Instance& operator=(Instance const&) = delete;
            Instance& operator=(Instance&&) = delete;
        };
    }
}
