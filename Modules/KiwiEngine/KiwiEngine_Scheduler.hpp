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

#include <algorithm>

namespace kiwi
{
    namespace engine
    {
        // ==================================================================================== //
        //                                       SCHEDULER                                      //
        // ==================================================================================== //
        
        template<class Clock>
        Scheduler<Clock>::Scheduler():
        m_producers(),
        m_next_token(0)
        {
        }
        
        template<class Clock>
        Scheduler<Clock>::~Scheduler()
        {
        }

        template<class Clock>
        typename Scheduler<Clock>::token_t Scheduler<Clock>::createProducerToken()
        {
            m_producers[m_next_token];
            
            return m_next_token++;
        }

        template<class Clock>
        void Scheduler<Clock>::schedule(Task * task, duration_t delay)
        {
            assert(task && &task->m_scheduler == this);
            m_producers[task->m_token].schedule(task, delay);
        }

        template<class Clock>
        void Scheduler<Clock>::unschedule(Task * task)
        {
            assert(task && &task->m_scheduler == this);
            m_producers[task->m_token].unschedule(task);
        }
        
        template<class Clock>
        void Scheduler<Clock>::process()
        {
            time_point_t process_time = clock_t::now();
            
            for(typename std::map<token_t, Producer>::iterator producer = m_producers.begin();
                producer != m_producers.end();
                ++producer)
            {
                producer->second.process(process_time);
            }
        }
        
        // ==================================================================================== //
        //                                       PRODUCER                                       //
        // ==================================================================================== //
        
        template<class Clock>
        Scheduler<Clock>::Producer::Producer():
        m_head(),
        m_pool(),
        m_commands(1024, 256)
        {
        }
        
        template<class Clock>
        Scheduler<Clock>::Producer::~Producer()
        {
        }
        
        template<class Clock>
        std::shared_ptr<typename Scheduler<Clock>::Event> Scheduler<Clock>::Producer::createEvent(Task * task)
        {
            // clean up pool before inserting new elements
            m_pool.erase(std::remove_if(m_pool.begin(), m_pool.end(),
                                        [](auto & event){return event.use_count() <= 1;}),
                         m_pool.end());
            
            // Create new element in event pool.
            m_pool.emplace_back(std::make_shared<Event>(task));
            
            return m_pool.back();
        }
        
        template<class Clock>
        void Scheduler<Clock>::Producer::schedule(Task * task, duration_t delay)
        {
            Command command {task->m_event, clock_t::now() + delay };
            m_commands.push(command);
        }
        
        template<class Clock>
        void Scheduler<Clock>::Producer::unschedule(Task * task)
        {
            Command command {task->m_event, clock_t::time_point::max()};
            m_commands.push(command);
        }
        
        template<class Clock>
        void Scheduler<Clock>::Producer::remove(std::shared_ptr<Event> const& event)
        {
            if (m_head)
            {
                if(m_head == event)
                {
                    m_head = event->m_next;
                }
                else
                {
                    Event* current = m_head->m_next.get();
                    Event* previous = m_head.get();
                    
                    while(current)
                    {
                        if(current == event.get())
                        {
                            previous->m_next = current->m_next;
                            return;
                        }
                        previous = current;
                        current = current->m_next.get();
                    }
                }
            }
        }
    
        template<class Clock>
        void Scheduler<Clock>::Producer::insert(std::shared_ptr<Event> const& event)
        {
            remove(event);
            
            if(m_head)
            {
                if(m_head->m_time > event->m_time)
                {
                    event->m_next = m_head;
                    m_head = event;
                    return;
                }
                
                Event* previous = m_head.get();
                Event* current = previous->m_next.get();
                
                while(current)
                {
                    if(current->m_time > event->m_time)
                    {
                        event->m_next = previous->m_next;
                        previous->m_next = event;
                        return;
                    }
                    previous = current;
                    current = current->m_next.get();
                }
                
                previous->m_next = event;
                event->m_next = nullptr;
            }
            else
            {
                m_head = event;
                event->m_next = nullptr;
            }
        }
    
        template<class Clock>
        void Scheduler<Clock>::Producer::process(time_point_t process_time)
        {
            size_t command_size = m_commands.load_size();
            
            for (size_t i = 0; i < command_size; ++i)
            {
                Command command;
                
                if (m_commands.pop(command))
                {
                    if (command.m_event)
                    {
                        if (command.m_time != clock_t::time_point::max())
                        {
                            command.m_event->m_time = command.m_time;
                            insert(command.m_event);
                        }
                        else
                        {
                            remove(command.m_event);
                        }
                    }
                }
            }
            
            Event* event = nullptr;
            
            bool time_reached = false;
            
            while((event = m_head.get()) != nullptr && !time_reached)
            {
                if (event->m_time <= process_time)
                {
                    event->execute();
                    remove(m_head);
                    
                }
                else
                {
                    time_reached = true;
                }
            }
        }
        
        // ==================================================================================== //
        //                                       TASK                                           //
        // ==================================================================================== //
        
        template<class Clock>
        Scheduler<Clock>::Task::Task(Scheduler & scheduler, token_t token):
        m_scheduler(scheduler),
        m_token(token),
        m_event(scheduler.m_producers[m_token].createEvent(this))
        {
            assert(token < m_scheduler.m_next_token);
        }
        
        template<class Clock>
        Scheduler<Clock>::Task::~Task()
        {
            m_event->removeTask();
        }
        
        // ==================================================================================== //
        //                                       EVENT                                          //
        // ==================================================================================== //
        
        template<class Clock>
        Scheduler<Clock>::Event::Event(Task * task):
        m_task(task),
        m_time(clock_t::time_point::max()),
        m_next()
        {
            assert(task != nullptr);
        }
        
        template<class Clock>
        Scheduler<Clock>::Event::~Event()
        {
        }
        
        template<class Clock>
        void Scheduler<Clock>::Event::removeTask()
        {
            m_task = nullptr;
        }
        
        template<class Clock>
        void Scheduler<Clock>::Event::execute()
        {
            if (m_task)
            {
                m_task->execute();
            }
        }
    }
}
