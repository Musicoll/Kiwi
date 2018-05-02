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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Delay.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                     OBJECT DELAY                                 //
        // ================================================================================ //
        
        void Delay::declare()
        {
            Factory::add<Delay>("delay", &Delay::create);
        }
        
        std::unique_ptr<Object> Delay::create(model::Object const& object, Patcher & patcher)
        {
            return std::make_unique<Delay>(object, patcher);
        }
        
        Delay::Delay(model::Object const& model, Patcher& patcher):
        Object(model, patcher),
        m_task(new tool::Scheduler<>::CallBack(std::bind(&Delay::bang, this))),
        m_delay(std::chrono::milliseconds(0))
        {
            std::vector<tool::Atom> const& args = model.getArguments();
            
            if (!args.empty())
            {
                m_delay = std::chrono::milliseconds(args[0].getInt());
            }
        }
        
        Delay::~Delay()
        {
            getScheduler().unschedule(m_task);
        }
        
        void Delay::bang()
        {
            send(0, {"bang"});
        }
        
        void Delay::receive(size_t index, std::vector<tool::Atom> const& args)
        {
            if (!args.empty())
            {
                if (index == 0)
                {
                    if(args[0].isBang())
                    {
                        getScheduler().schedule(m_task, m_delay);
                    }
                    else if(args[0].isString() && args[0].getString() == "stop")
                    {
                        getScheduler().unschedule(m_task);
                    }
                    else
                    {
                        warning("delay inlet 1 only receives bang and stop");
                    }
                }
                else if(index == 1)
                {
                    if (args[0].isNumber())
                    {
                        m_delay = std::chrono::milliseconds(args[0].getInt());
                    }
                    else
                    {
                        warning("delay inlet 2 doesn't understand [" + args[0].getString() + "]");
                    }
                }
            }
        }
    }
}

