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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Metro.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT METRO                                    //
    // ================================================================================ //
    
    void Metro::declare()
    {
        Factory::add<Metro>("metro", &Metro::create);
    }
    
    std::unique_ptr<Object> Metro::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Metro>(model, patcher);
    }
    
    Metro::Metro(model::Object const& model, Patcher& patcher):
    engine::Object(model, patcher),
    m_period(std::chrono::milliseconds(1000)),
    m_task(new tool::Scheduler<>::CallBack(std::bind(&Metro::timerCallBack, this)))
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if(!args.empty())
        {
            m_period = std::chrono::milliseconds(args[0].getInt());
        }
    }
    
    Metro::~Metro()
    {
        getScheduler().unschedule(m_task);
    }
    
    void Metro::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (!args.empty())
        {
            if (index == 0)
            {
                if (args[0].isNumber())
                {
                    if (static_cast<bool>(args[0].getFloat()))
                    {
                        getScheduler().unschedule(m_task);
                        timerCallBack();
                        getScheduler().schedule(m_task, m_period);
                    }
                    else
                    {
                        getScheduler().unschedule(m_task);
                    }
                }
                else
                {
                    warning("metro inlet 1 only take numbers");
                }
            }
            else if(index == 1)
            {
                if (args[0].isNumber())
                {
                    m_period = std::chrono::milliseconds(args[0].getInt());
                }
                else
                {
                    warning("metro inlet 2 doesn't understand [" + args[0].getString() + "]");
                }
            }
        }
    }
    
    void Metro::timerCallBack()
    {
        send(0, {"bang"});
        getScheduler().schedule(m_task, m_period);
    }
}}
