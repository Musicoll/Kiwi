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
        m_events(),
        m_commands(1024, 256)
        {
        }
        
        template<class Clock>
        Scheduler<Clock>::Producer::~Producer()
        {
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
        void Scheduler<Clock>::Producer::remove(std::shared_ptr<Event> event)
        {
            typename std::list<std::weak_ptr<Event>>::iterator event_it = m_events.begin();
            
            bool removed = false;
            
            while(event_it != m_events.end() && !removed)
            {
                std::shared_ptr<Event> list_event = (*event_it).lock();
                
                if (list_event == nullptr || list_event == event)
                {
                    m_events.erase(event_it);
                    removed = true;
                }
                
                ++event_it;
            }
        }
        
        template<class Clock>
        void Scheduler<Clock>::Producer::insert(std::shared_ptr<Event> event)
        {
            remove(event);
            
            typename std::list<std::weak_ptr<Event>>::iterator event_it = m_events.begin();
            
            bool inserted = false;
            
            while(event_it != m_events.end() && !inserted)
            {
                std::shared_ptr<Event> list_event = (*event_it).lock();
                
                if (list_event && (event->m_time < list_event->m_time))
                {
                    m_events.insert(event_it, event);
                    inserted = true;
                }
                
                ++event_it;
            }
            
            if (!inserted) {m_events.push_back(event);}
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
                    std::shared_ptr<Event> event = command.m_event.lock();
                    
                    if (event)
                    {
                        if (command.m_time != clock_t::time_point::max())
                        {
                            event->m_time = command.m_time;
                            insert(event);
                        }
                        else
                        {
                            remove(event);
                        }
                    }
                }
            }
            
            typename std::list<std::weak_ptr<Event>>::iterator event_it = m_events.begin();
            
            bool time_reached = false;
            
            while(event_it != m_events.end() && !time_reached)
            {
                std::shared_ptr<Event> event = event_it->lock();
                
                if (event)
                {
                    if (event->m_time <= process_time)
                    {
                        event->execute();
                    }
                    else
                    {
                        time_reached = true;
                    }
                }
                
                ++event_it;
            }
        }
        
        // ==================================================================================== //
        //                                       TASK                                           //
        // ==================================================================================== //
        
        template<class Clock>
        Scheduler<Clock>::Task::Task(Scheduler & scheduler, token_t token):
        m_scheduler(scheduler),
        m_token(token),
        m_event(std::make_shared<Event>(*this))
        {
            assert(token < m_scheduler.m_next_token);
        }
        
        template<class Clock>
        Scheduler<Clock>::Task::~Task()
        {
        }
        
        // ==================================================================================== //
        //                                       EVENT                                          //
        // ==================================================================================== //
        
        template<class Clock>
        Scheduler<Clock>::Event::Event(Task & task):
        m_task(task),
        m_time(clock_t::time_point::max())
        {
        }
        
        template<class Clock>
        Scheduler<Clock>::Event::~Event()
        {
        }
        
        template<class Clock>
        void Scheduler<Clock>::Event::execute()
        {
            m_task.execute();
        }
    }
}
