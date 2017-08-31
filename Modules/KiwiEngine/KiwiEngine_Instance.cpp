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
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Objects.h>
#include "KiwiEngine_Instance.h"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        Instance::Instance(std::unique_ptr<AudioControler> audio_controler):
        m_audio_controler(std::move(audio_controler)),
        m_scheduler(),
        m_quit(false),
        m_engine_thread(std::bind(&Instance::processScheduler, this))
        {
            addObjectsToFactory();
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
        
        void Instance::addObjectsToFactory()
        {
            engine::Factory::add<NewBox>("newbox");
            engine::Factory::add<ErrorBox>("errorbox");
            engine::Factory::add<Plus>("plus");
            engine::Factory::add<Times>("times");
            engine::Factory::add<Print>("print");
            engine::Factory::add<Receive>("receive");
            engine::Factory::add<Loadmess>("loadmess");
            engine::Factory::add<Delay>("delay");
            engine::Factory::add<Pipe>("pipe");
            engine::Factory::add<Metro>("metro");
            engine::Factory::add<OscTilde>("osc~");
            engine::Factory::add<AdcTilde>("adc~");
            engine::Factory::add<DacTilde>("dac~");
            engine::Factory::add<TimesTilde>("times~");
            engine::Factory::add<PlusTilde>("plus~");
            engine::Factory::add<SigTilde>("sig~");
            engine::Factory::add<DelaySimpleTilde>("delaysimple~");
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
        
        Scheduler<> & Instance::getScheduler()
        {
            return m_scheduler;
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
