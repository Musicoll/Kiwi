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

#pragma once

#include <algorithm>

namespace kiwi { namespace tool {
    
    // ==================================================================================== //
    //                                       SCHEDULER                                      //
    // ==================================================================================== //
    
    template<class Clock>
    Scheduler<Clock>::Scheduler():
    m_queue(),
    m_mutex(),
    m_consumer_id(std::this_thread::get_id())
    {
    }
    
    template<class Clock>
    Scheduler<Clock>::~Scheduler()
    {
    }
    
    template<class Clock>
    void Scheduler<Clock>::setThreadAsConsumer()
    {
        m_consumer_id = std::this_thread::get_id();
    }
    
    template<class Clock>
    bool Scheduler<Clock>::isThisConsumerThread() const
    {
        return m_consumer_id == std::this_thread::get_id();
    }
    
    template<class Clock>
    void Scheduler<Clock>::schedule(std::shared_ptr<Task> const& task, duration_t delay)
    {
        assert(task);
        m_queue.schedule(task, delay);
    }
    
    template<class Clock>
    void Scheduler<Clock>::schedule(std::shared_ptr<Task> && task, duration_t delay)
    {
        assert(task);
        m_queue.schedule(std::move(task), delay);
    }
    
    template<class Clock>
    void Scheduler<Clock>::schedule(std::function<void(void)> && func, duration_t delay)
    {
        schedule(std::make_shared<CallBack>(func), delay);
    }
    
    template<class Clock>
    void Scheduler<Clock>::defer(std::shared_ptr<Task> const& task)
    {
        assert(task);
        
        if (!isThisConsumerThread())
        {
            schedule(task);
        }
        else
        {
            task->execute();
        }
    }
    
    template<class Clock>
    void Scheduler<Clock>::defer(std::shared_ptr<Task> && task)
    {
        assert(task);
        
        if (!isThisConsumerThread())
        {
            schedule(std::move(task));
        }
        else
        {
            task->execute();
            task.reset();
        }
    }
    
    template<class Clock>
    void Scheduler<Clock>::defer(std::function<void(void)> && func)
    {
        if (!isThisConsumerThread())
        {
            schedule(std::move(func));
        }
        else
        {
            func();
        }
    }
    
    template<class Clock>
    void Scheduler<Clock>::unschedule(std::shared_ptr<Task> const& task)
    {
        assert(task);
        m_queue.unschedule(task);
    }
    
    template<class Clock>
    void Scheduler<Clock>::process()
    {
        assert(std::this_thread::get_id() == m_consumer_id);
        
        std::lock_guard<std::mutex> lock(m_mutex);
        
        time_point_t process_time = clock_t::now();
        
        m_queue.process(process_time);
    }
    
    template<class Clock>
    std::unique_lock<std::mutex> Scheduler<Clock>::lock() const
    {
        std::unique_lock<std::mutex> head_lock(m_mutex);
        
        return head_lock;
    }
    
    // ==================================================================================== //
    //                                       QUEUE                                          //
    // ==================================================================================== //
    
    template<class Clock>
    Scheduler<Clock>::Queue::Queue():
    m_events(),
    m_commands(1024)
    {
    }
    
    template<class Clock>
    Scheduler<Clock>::Queue::~Queue()
    {
    }
    
    template<class Clock>
    void Scheduler<Clock>::Queue::schedule(std::shared_ptr<Task> const& task, duration_t delay)
    {
        assert(task);
        m_commands.push({task, clock_t::now() + delay });
    }
    
    template<class Clock>
    void Scheduler<Clock>::Queue::schedule(std::shared_ptr<Task> && task, duration_t delay)
    {
        assert(task);
        m_commands.push({std::move(task), clock_t::now() + delay});
    }
    
    template<class Clock>
    void Scheduler<Clock>::Queue::unschedule(std::shared_ptr<Task> const& task)
    {
        m_commands.push({task, clock_t::time_point::max()});
    }
    
    template<class Clock>
    void Scheduler<Clock>::Queue::remove(Event const& event)
    {
        m_events.erase(std::remove_if(m_events.begin(), m_events.end(), [&event](Event const& e) {
            
            return e.m_task == event.m_task;
            
        }), m_events.end());
    }
    
    template<class Clock>
    void Scheduler<Clock>::Queue::emplace(Event && event)
    {
        remove(event);
        
        auto event_it = m_events.begin();
        
        while(event_it != m_events.end())
        {
            if (event.m_time < event_it->m_time)
            {
                m_events.insert(event_it, std::move(event));
                break;
            }
            
            ++event_it;
        }
        
        if (event_it == m_events.end())
        {
            m_events.emplace_back(std::move(event));
        }
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
                Event event(std::move(command.m_task), command.m_time);
                
                if (event.m_time != clock_t::time_point::max())
                {
                    emplace(std::move(event));
                }
                else
                {
                    remove(event);
                }
            }
        }
        
        m_events.erase(std::remove_if(m_events.begin(), m_events.end(), [&process_time](auto& event) {
            
            if (event.m_time <= process_time)
            {
                event.execute();
                return true;
            }
            
            return false;
            
        }), m_events.end());
    }
    
    // ==================================================================================== //
    //                                       TASK                                           //
    // ==================================================================================== //
    
    template<class Clock>
    Scheduler<Clock>::Task::Task()
    {
    }
    
    template<class Clock>
    Scheduler<Clock>::Task::~Task()
    {
    }
    
    // ==================================================================================== //
    //                                      CALLBACK                                        //
    // ==================================================================================== //
    
    template<class Clock>
    Scheduler<Clock>::CallBack::CallBack(std::function<void(void)> func):
    Task(),
    m_func(func)
    {
    }
    
    template<class Clock>
    Scheduler<Clock>::CallBack::~CallBack()
    {
    }
    
    template<class Clock>
    void Scheduler<Clock>::CallBack::execute()
    {
        m_func();
    }
    
    // ==================================================================================== //
    //                                       TIMER                                          //
    // ==================================================================================== //
    
    template<class Clock>
    class Scheduler<Clock>::Timer::Task final : public Scheduler<Clock>::Task
    {
    public: // methods
        
        Task(Timer& timer):
        m_timer(timer)
        {
        }
        
        ~Task()
        {
        }
        
        void execute() override
        {
            m_timer.callBackInternal();
        }
        
    private: // members
        
        Timer&          m_timer;
        
    };
    
    template<class Clock>
    Scheduler<Clock>::Timer::Timer(Scheduler & scheduler):
    m_scheduler(scheduler),
    m_task(new Task(*this)),
    m_period()
    {
    }
    
    template<class Clock>
    Scheduler<Clock>::Timer::~Timer()
    {
        stopTimer();
    }
    
    template<class Clock>
    void Scheduler<Clock>::Timer::startTimer(duration_t period)
    {
        stopTimer();
        
        m_period = period;
        
        m_scheduler.schedule(m_task, m_period);
    }
    
    template<class Clock>
    void Scheduler<Clock>::Timer::callBackInternal()
    {
        timerCallBack();
        
        m_scheduler.schedule(m_task, m_period);
    }
    
    template<class Clock>
    void Scheduler<Clock>::Timer::stopTimer()
    {
        m_scheduler.unschedule(m_task);
    }
    
    // ==================================================================================== //
    //                                       EVENT                                          //
    // ==================================================================================== //
    
    template<class Clock>
    Scheduler<Clock>::Event::Event(std::shared_ptr<Task> && task, time_point_t time):
    m_task(std::move(task)),
    m_time(time)
    {
    }
    
    template<class Clock>
    Scheduler<Clock>::Event::Event(Event && other):
    m_task(std::move(other.m_task)),
    m_time(std::move(other.m_time))
    {
    }
    
    template<class Clock>
    typename Scheduler<Clock>::Event& Scheduler<Clock>::Event::operator=(Event && other)
    {
        m_task = std::move(other.m_task);
        m_time = std::move(other.m_time);
        
        return *this;
    }
    
    
    template<class Clock>
    Scheduler<Clock>::Event::~Event()
    {
    }
    
    template<class Clock>
    void Scheduler<Clock>::Event::execute()
    {
        if (m_task){m_task->execute();}
    }
}}
