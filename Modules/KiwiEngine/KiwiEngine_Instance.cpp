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

#include "KiwiEngine_Factory.h"
#include "KiwiEngine_Instance.h"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        Instance::Instance(std::unique_ptr<AudioControler> audio_controler, tool::Scheduler<> & main_scheduler):
        m_audio_controler(std::move(audio_controler)),
        m_scheduler(),
        m_main_scheduler(main_scheduler),
        m_quit(false),
        m_engine_thread(std::bind(&Instance::processScheduler, this))
        {
        }
        
        Instance::~Instance()
        {
            m_quit.store(true);
            m_engine_thread.join();
        }
        
        // ================================================================================ //
        //                                      CONSOLE                                     //
        // ================================================================================ //
        
        void Instance::log(std::string const& text) const
        {
            m_console.post({text, Console::Message::Type::Log});
        }
        
        void Instance::post(std::string const& text) const
        {
            m_console.post({text, Console::Message::Type::Normal});
        }
        
        void Instance::warning(std::string const& text) const
        {
            m_console.post({text, Console::Message::Type::Warning});
        }
        
        void Instance::error(std::string const& text) const
        {
            m_console.post({text, Console::Message::Type::Error});
        }

        void Instance::addConsoleListener(Console::Listener& listener)
        {
            m_console.addListener(listener);
        }

        void Instance::removeConsoleListener(Console::Listener& listener)
        {
            m_console.removeListener(listener);
        }
        
        // ================================================================================ //
        //                              AUDIO CONTROLER                                     //
        // ================================================================================ //
        
        AudioControler& Instance::getAudioControler() const
        {
            return *m_audio_controler.get();
        }
        
        // ================================================================================ //
        //                                  SCHEDULER                                       //
        // ================================================================================ //
        
        tool::Scheduler<> & Instance::getScheduler()
        {
            return m_scheduler;
        }
        
        tool::Scheduler<> & Instance::getMainScheduler()
        {
            return m_main_scheduler;
        }
        
        void Instance::processScheduler()
        {
            m_scheduler.setThreadAsConsumer();
            
            while(!m_quit.load())
            {
                m_scheduler.process();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));

            }
        }
    }
}
