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

#include <functional>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Toggle.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Controller/KiwiEngine_Toggle.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                  OBJECT BANG                                     //
        // ================================================================================ //
        
        class Toggle::Task final : public tool::Scheduler<>::Task
        {
        public: // methods
            
            Task(Toggle & object, model::Toggle::Request request, bool shall_output):
            tool::Scheduler<>::Task(),
            m_object(object),
            m_request(request),
            m_shall_output(shall_output)
            {
            }
            
            ~Task()
            {
            }
            
            void execute()
            {
                switch(m_request)
                {
                    case model::Toggle::Request::Switch:
                    {
                        m_object.m_is_on = !m_object.m_is_on;
                        break;
                    }
                    case model::Toggle::Request::SwitchOn:
                    {
                        m_object.m_is_on = true;
                        break;
                    }
                    case model::Toggle::Request::SwitchOff:
                    {
                        m_object.m_is_on = false;
                        break;
                    }
                }
                
                if (m_shall_output)
                {
                    if (m_object.m_is_on)
                    {
                        m_object.send(0, {1});
                    }
                    else
                    {
                        m_object.send(0, {0});
                    }
                }
                
                std::shared_ptr<Task> task;
                m_object.m_tasks.pop(task);
            }
            
        private: // members
            
            Toggle&                 m_object;
            model::Toggle::Request  m_request;
            bool                    m_shall_output;
        };
        
        Toggle::Toggle(model::Object const& model, Patcher& patcher, std::vector<tool::Atom> const& args):
        Object(model, patcher),
        m_signal(model.getSignal<model::Toggle::Request, bool>(model::Toggle::Signal::Switch)),
        m_connection(m_signal.connect(std::bind(&Toggle::toggleSwitched,
                                                this,
                                                std::placeholders::_1,
                                                std::placeholders::_2))),
        m_is_on(false),
        m_tasks(20)
        {
        }
        
        Toggle::~Toggle()
        {
            while(m_tasks.load_size() > 0)
            {
                std::shared_ptr<Task> task;
                m_tasks.pop(task);
                getScheduler().unschedule(task);
            }
        }
        
        void Toggle::toggleSwitched(model::Toggle::Request request, bool shall_output)
        {
            std::shared_ptr<Task> task(new Task(*this, request, shall_output));
            m_tasks.push(task);
            getScheduler().schedule(task);
        }
        
        void Toggle::receive(size_t index, std::vector<tool::Atom> const& args)
        {
            if (!args.empty())
            {
                if (index == 0)
                {
                    if(args[0].isString())
                    {
                        if (args[0].getString() == "bang")
                        {
                            m_signal(model::Toggle::Request::Switch, true);
                        }
                        else if(args.size() == 2 && args[0].getString() == "set" && args[1].isNumber())
                        {
                            if (args[1].getFloat() != 0)
                            {
                                m_signal(model::Toggle::Request::SwitchOn, false);
                            }
                            else
                            {
                                m_signal(model::Toggle::Request::SwitchOff, false);
                            }
                        }
                    }
                    else if(args[0].isNumber())
                    {
                        if (args[0].getFloat() != 0)
                        {
                            m_signal(model::Toggle::Request::SwitchOn, true);
                        }
                        else
                        {
                            m_signal(model::Toggle::Request::SwitchOff, true);
                        }
                    }
                }
            }
        }
    }
}

