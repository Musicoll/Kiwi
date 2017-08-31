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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Time/KiwiEngine_Delay.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                     OBJECT DELAY                                 //
        // ================================================================================ //
        
        Delay::Task::Task(Delay& object):
        Scheduler<>::Task(),
        m_object(object)
        {
        }
        
        void Delay::Task::execute()
        {
            m_object.send(0, {"bang"});
        }
        
        Delay::Delay(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args):
        Object(model, patcher),
        m_task(new Task(*this)),
        m_delay(std::chrono::milliseconds(0))
        {
            if (!args.empty())
            {
                m_delay = std::chrono::milliseconds(args[0].getInt());
            }
        }
        
        Delay::~Delay()
        {
            getScheduler().unschedule(m_task);
        }
        
        void Delay::receive(size_t index, std::vector<Atom> const& args)
        {
            if (!args.empty())
            {
                if (index == 0)
                {
                    if(args[0].isString() && args[0].getString() == "bang")
                    {
                        getScheduler().schedule(m_task, m_delay);
                    }
                    else if(args[0].isString() && args[0].getString() == "stop")
                    {
                        getScheduler().unschedule(m_task);
                    }
                }
                else if(index == 1)
                {
                    if (args[0].isNumber())
                    {
                        m_delay = std::chrono::milliseconds(args[0].getInt());
                    }
                }
            }
        }
    }
}

