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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Time/KiwiEngine_Metro.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                  OBJECT METRO                                    //
        // ================================================================================ //
        
        Metro::Metro(model::Object const& model, Patcher& patcher, std::vector<tool::Atom> const& args):
        engine::Object(model, patcher),
        tool::Scheduler<>::Timer(patcher.getScheduler()),
        m_period(std::chrono::milliseconds(0))
        {
            if(!args.empty())
            {
                m_period = std::chrono::milliseconds(args[0].getInt());
            }
        }
        
        Metro::~Metro()
        {
        }
        
        void Metro::receive(size_t index, std::vector<tool::Atom> const& args)
        {
            if (!args.empty())
            {
                if (index == 0)
                {
                    if((args[0].isString() && args[0].getString() == "start")
                       || (args[0].isNumber() && static_cast<bool>(args[0].getInt())))
                    {
                        timerCallBack();
                        startTimer(m_period);
                    }
                    else if((args[0].isString() && args[0].getString() == "stop")
                            || (args[0].isNumber() && !static_cast<bool>(args[0].getInt())))
                    {
                        stopTimer();
                    }
                }
                else if(index == 1)
                {
                    if (args[0].isNumber())
                    {
                        m_period = std::chrono::milliseconds(args[0].getInt());
                    }
                }
            }
        }
        
        void Metro::timerCallBack()
        {
            send(0, {"bang"});
        }

    }
}
