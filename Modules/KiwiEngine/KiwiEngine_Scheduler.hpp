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
        
        template <class Clock>
        std::unique_ptr<Scheduler<Clock>> Scheduler<Clock>::m_instance = nullptr;
        
        template<class Clock>
        Scheduler<Clock>::Scheduler():
        m_queues()
        {
        }
        
        template<class Clock>
        Scheduler<Clock>::~Scheduler()
        {
        }
        
        template<class Clock>
        void Scheduler<Clock>::createInstance()
        {
            if (m_instance == nullptr)
            {
                m_instance.reset(new Scheduler());
            }
        }
        
        template<class Clock>
        Scheduler<Clock>& Scheduler<Clock>::getInstance()
        {
            assert(m_instance != nullptr);
            return *m_instance;
        }
        
        template<class Clock>
        void Scheduler<Clock>::registerConsumer(thread_token consumer)
        {
            m_queues[consumer];
        }
        
        template<class Clock>
        void Scheduler<Clock>::registerProducer(thread_token producer, thread_token consumer)
        {
            assert(m_queues.find(consumer) != m_queues.end());
            m_queues[consumer][producer];
        }

        template<class Clock>
        void Scheduler<Clock>::schedule(Task * task, duration_t delay)
        {
            m_queues[task->m_consumer][task->m_producer].schedule(task, delay);
        }

        template<class Clock>
        void Scheduler<Clock>::unschedule(Task * task)
        {
            assert(task);
            m_queues[task->m_consumer][task->m_producer].unschedule(task);
        }
        
        template<class Clock>
        void Scheduler<Clock>::process(thread_token consumer)
        {
            assert(m_queues.find(consumer) != m_queues.end());
            
            time_point_t process_time = clock_t::now();
            
            for(typename std::map<thread_token, Queue>::iterator queue = m_queues[consumer].begin();
                queue != m_queues[consumer].end();
                ++queue)
            {
                queue->second.process(process_time);
            }
        }
        
        // ==================================================================================== //
        //                                       Queue                                          //
        // ==================================================================================== //
        
        template<class Clock>
        Scheduler<Clock>::Queue::Queue():
        m_commands(1024)
        {
        }
        
        template<class Clock>
        Scheduler<Clock>::Queue::~Queue()
        {
        }
        
        template<class Clock>
        void Scheduler<Clock>::Queue::schedule(Task * task, duration_t delay)
        {
            Command command {task->m_event, clock_t::now() + delay };
            m_commands.push(command);
        }
        
        template<class Clock>
        void Scheduler<Clock>::Queue::unschedule(Task * task)
        {
            Command command {task->m_event, clock_t::time_point::max()};
            m_commands.push(command);
        }
        
        template<class Clock>
        void Scheduler<Clock>::deleteTask(Task * task)
        {
            task->disable();
            delete task;
        }
        
        template<class Clock>
        void Scheduler<Clock>::Queue::remove(std::shared_ptr<Event> const& event)
        {
            typename std::vector<std::shared_ptr<Event>>::iterator event_it = m_events.begin();
            
            while(event_it != m_events.end())
            {
                if ((*event_it) == event)
                {
                    m_events.erase(event_it);
                    break;
                }
                
                ++event_it;
            }
        }
    
        template<class Clock>
        void Scheduler<Clock>::Queue::insert(std::shared_ptr<Event> const& event)
        {
            remove(event);
            
            typename std::vector<std::shared_ptr<Event>>::iterator event_it = m_events.begin();
            
            while(event_it != m_events.end())
            {
                if (event->m_time < (*event_it)->m_time)
                {
                    m_events.insert(event_it, event);
                    break;
                }
                
                ++event_it;
            }
            
            if (event_it == m_events.end()){m_events.insert(m_events.end(), event);}
        }
    
        template<class Clock>
        void Scheduler<Clock>::Queue::process(time_point_t process_time)
        {
            size_t command_size = m_commands.load_size();
            
            for (size_t i = 0; i < command_size; ++i)
            {
                Command command;
                
                if (m_commands.pop(command))
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
            
            typename std::vector<std::shared_ptr<Event>>::iterator event = m_events.begin();
            
            while(event != m_events.end())
            {
                if ((*event)->m_time <= process_time)
                {
                    (*event)->execute();
                    m_events.erase(event);
                }
                else
                {
                    break;
                }
            }
        }
        
        // ==================================================================================== //
        //                                       TASK                                           //
        // ==================================================================================== //
        
        template<class Clock>
        Scheduler<Clock>::Task::Task(thread_token producer, thread_token consumer):
        m_producer(producer),
        m_consumer(consumer),
        m_event(new Event(this, time_point_t::max()))
        {
            Scheduler& scheduler = Scheduler::getInstance();
            
            assert(scheduler.m_queues.find(consumer) != scheduler.m_queues.end() &&
                   scheduler.m_queues[consumer].find(producer) != scheduler.m_queues[consumer].end());
        }
        
        template<class Clock>
        Scheduler<Clock>::Task::~Task()
        {
        }
        
        template<class Clock>
        void Scheduler<Clock>::Task::disable()
        {
            m_event->disable();
        }
        
        // ==================================================================================== //
        //                                       EVENT                                          //
        // ==================================================================================== //
        
        template<class Clock>
        Scheduler<Clock>::Event::Event(Task * task, time_point_t time):
        m_task(task),
        m_time(time)
        {
        }
        
        template<class Clock>
        void Scheduler<Clock>::Event::disable()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_task = nullptr;
        }
        
        template<class Clock>
        Scheduler<Clock>::Event::~Event()
        {
        }
        
        template<class Clock>
        bool Scheduler<Clock>::Event::operator==(Event const& other) const
        {
            if (this != &other)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::lock_guard<std::mutex> other_lock(other.m_mutex);
                return m_task == other.m_task;
            }
            else
            {
                return true;
            }
        }
        
        template<class Clock>
        void Scheduler<Clock>::Event::execute()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            
            if (m_task){m_task->execute();}
        }
    }
}
