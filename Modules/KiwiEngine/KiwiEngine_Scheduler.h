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

#include <cstdint>
#include <map>
#include <list>
#include <chrono>
#include <stdexcept>
#include <mutex>
#include <set>
#include <memory>

#include "KiwiEngine_ConcurrentQueue.h"

namespace kiwi
{
    namespace engine
    {
        // ==================================================================================== //
        //                                       SCHEDULER                                      //
        // ==================================================================================== //
        
        using thread_token = uint64_t;
        
        //! @brief A class designed to delay tasks' execution between threads that where previously declared.
        //! @details The scheduler is designed as a singleton that uses multiple event lists.
        //! Before processing the scheduler one should create an instance and register all threads that will
        //! use the scheduler. One can override the clock used by the scheduler to get time.
        template<class Clock = std::chrono::high_resolution_clock>
        class Scheduler final
        {
        public: // classes
            
            using clock_t = Clock;
            
            using time_point_t = typename Clock::time_point;
            
            using duration_t = typename Clock::duration;
            
            class Task;
            
            class Timer;
            
            class Lock;
            
        private: // classes
        
            class Queue;
            
            class Event;
            
        public: // methods
            
            //! @brief Creates the instance of scheduler.
            //! @details Shall be called before launching threads and processing the scheduler.
            static Scheduler& createInstance();
            
            //! @brief Retrieves the instance created with createInstance.
            //! @details Fails if createInstance was not called previously.
            static Scheduler& use();
            
            //! @brief Destroys the instance previously created.
            //! @details Call this method once caller is done using the scheduler.
            static void deleteInstance();
            
            //! @brief Adds a consumer thread to the scheduler.
            //! @details Does nothing if the consumer was already registered. Dynamicaly registering consumers
            //! is not thread safe and shall be done during initialization before launching threads.
            void registerConsumer(thread_token consumer);
            
            //! @brief Delays execution of a task. Shared ownership.
            //! @details Calling twice this method with same task will cancel the previous scheduled execution
            //! and add a new one at specified time. If you move ownership of the task here the task will be
            //! deleted at execution time.
            void schedule(std::shared_ptr<Task> const& task, duration_t delay = std::chrono::milliseconds(0));
            
            //! @brief Used to cancel the execution of a previously scheduled task.
            //! @details If the task is currently being processed by the scheduler, this method does't
            //! wait for the execution to finish but only guarantee that further execution  will no occur.
            void unschedule(std::shared_ptr<Task> const& task);
            
            //! @brief Processes events of the consumer that have reached exeuction time.
            void process(thread_token consumer);
            
        private: // methods
            
            //! @brief Constructor
            Scheduler();
            
            //! @brief Desctructor.
            ~Scheduler();
            
        private: // members
            
            std::map<thread_token, Queue>   m_queues;
            
        private: // static members
            
            static Scheduler* m_instance;
            
        private: // deleted methods
            
            Scheduler(Scheduler const& other) = delete;
            Scheduler(Scheduler && other) = delete;
            Scheduler& operator=(Scheduler const& other) = delete;
            Scheduler& operator=(Scheduler && other) = delete;
        };
        
        // ==================================================================================== //
        //                                       QUEUE                                          //
        // ==================================================================================== //
        
        //! @brief A class that holds a list of scheduled events.
        //! @details Implementation countains a list of events sorted by execution time that is updated
        //! before processing using commands. A queue is created for each consumer.
        template <class Clock>
        class Scheduler<Clock>::Queue final
        {
        public: // classes
            
            struct Command
            {
                std::shared_ptr<Task>   m_task;
                time_point_t            m_time;
            };
            
        public: // methods
            
            //! @brief Constructor.
            Queue();
            
            //! @brief Destructor
            ~Queue();
            
            //! @brief Delays the execution of a task. Shared ownership.
            void schedule(std::shared_ptr<Task> const& task, duration_t delay);
            
            //! @brief Cancels the execution of a task.
            void unschedule(std::shared_ptr<Task> const& task);
            
            //! @brief Processes all events that have reached execution time.
            void process(time_point_t process_time);
            
        private: // methods
            
            //! @internal
            void emplace(Event && event);
            
            //! @internal
            void remove(Event const& event);
            
        private: // members
            
            std::vector<Event>          m_events;
            ConcurrentQueue<Command>    m_commands;
            mutable std::mutex          m_mutex;
            
        private: // friend classes
            
            friend class Scheduler;
            
        private: // deleted methods
            
            Queue(Queue const& other) = delete;
            Queue(Queue && other) = delete;
            Queue& operator=(Queue const& other) = delete;
            Queue& operator=(Queue && other) = delete;
        };
        
        // ==================================================================================== //
        //                                       TASK                                           //
        // ==================================================================================== //
        
        //! @brief The abstract class that the scheduler executes. Caller must override execute function
        //! to specify the callback behavior.
        template <class Clock>
        class Scheduler<Clock>::Task
        {
        public: // methods
            
            //! @brief Constructor.
            //! @details A certain task is designed to be scheduled on only one consumer.
            Task(thread_token consumer);
            
            //! @brief Destructor.
            //! @details It is not safe to destroy a task from another thread than the consumer because it can be
            //! concurrent to its execution. One can create a task that deletes itself at exeuction time or lock
            //! a consumer using Scheduler::Lock before deleting the task.
            virtual ~Task();
            
        private: // methods

            //! @brief The pure virtual execution method. Called by the scheduler when execution time
            //! is reached.
            virtual void execute() = 0;
            
        private: // methods
            
            thread_token            m_consumer;
            
        private: // friends
            
            friend class Scheduler;
            
        private: // deleted methods
            
            Task(Task const& other) = delete;
            Task(Task && other) = delete;
            Task& operator=(Task const& other) = delete;
            Task& operator=(Task && other) = delete;
        };
        
        // ==================================================================================== //
        //                                       TIMER                                          //
        // ==================================================================================== //
        
        //! @brief An abstract class designed to repetedly call a method at a specified intervall of time.
        //! Overriding timerCallBack and calling startTimer will start repetdly calling method.
        template<class Clock>
        class Scheduler<Clock>::Timer
        {
        private: // classes
            
            class Task;
            
        public: // methods
            
            //! @brief Constructor.
            //! @details A timer can only be created for a certain consumer.
            Timer(thread_token consumer_token);
            
            //! @brief Destructor.
            //! @details It is not safe to destroy a timer in another thread than the consumer. If intended
            //! one shall lock the consumer before destroying the timer.
            ~Timer();
            
            //! @brief Starts the timer.
            //! @details Will cause timerCallBack to be called at a specified rate by the right consumer.
            void startTimer(duration_t period);
            
            //! @brief Stops the timer.
            //! @details If called when timerCallBack is being processed, stopTimer will not wait for the execution
            //! to finish but will only guarantee that further execution will not occur.
            void stopTimer();
            
        private: // methods
            
            //! @brief The pure virtual call back function.
            virtual void timerCallBack() = 0;
            
            //! @brief Reschedule task before calling virtual method timerCallBack.
            void callBackInternal();
            
        private: // members
            
            std::shared_ptr<Task>   m_task;
            duration_t              m_period;
            
        private: // deleted methods
            
            Timer() = delete;
            Timer(Timer const& other) = delete;
            Timer(Timer && other) = delete;
            Timer& operator=(Timer const& other) = delete;
            Timer& operator=(Timer && other) = delete;
        };
        
        // ==================================================================================== //
        //                                       LOCK                                           //
        // ==================================================================================== //
        
        //! @brief This class is intended to lock the execution of a certain consumer.
        //! @details Beware that critical sections shall not take long since it blocks a certain thread
        //! from processing events.
        template<class Clock>
        class Scheduler<Clock>::Lock final
        {
        public: // methods
            
            //! @brief Constructor.
            Lock(thread_token consumer);
            
            //! @brief Destructor.
            ~Lock();
            
        private: // members
            
            thread_token m_consumer;
            
        private: // deleted methods
            
            Lock() = delete;
            Lock(Lock const& other) = delete;
            Lock(Lock && other) = delete;
            Lock& operator=(Lock const& other) = delete;
            Lock& operator=(Lock && other) = delete;
        };
        
        // ==================================================================================== //
        //                                       EVENT                                          //
        // ==================================================================================== //
        
        //! @brief An event that associates a task and a execution time.
        template<class Clock>
        class Scheduler<Clock>::Event final
        {
        public: // methods
            
            //! @brief Constructor.
            Event(std::shared_ptr<Task> const& task, time_point_t time);
            
            //! @brief Copy Constructor.
            Event(Event const& other);
            
            //! @brief Assignement operator
            Event& operator=(Event const& other);
            
            //! @brief Moove constructor
            Event(Event && other);
            
            //! @brief Moove assignment operator.
            Event& operator=(Event && other);
            
            //! @brief Destructor.
            ~Event();
            
            //! @brief Called by the scheduler to execute a the task.
            void execute();
            
        private: // friends
            
            friend class Scheduler;
            
        private: // members
            
            std::shared_ptr<Task>       m_task;
            time_point_t                m_time;
            
        private: // deleted methods
            
            Event() = delete;
        };
    }
}

#include "KiwiEngine_Scheduler.hpp"
