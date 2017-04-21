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

#ifndef KIWI_ENGINE_SCHEDULER_HPP_INCLUDED
#define KIWI_ENGINE_SCHEDULER_HPP_INCLUDED

#include <cstdint>
#include <map>
#include <list>
#include <chrono>
#include <stdexcept>
#include <mutex>
#include <set>

#include "KiwiEngine_ConcurrentQueue.hpp"

namespace kiwi
{
    namespace engine
    {
        // ==================================================================================== //
        //                                       SCHEDULER                                      //
        // ==================================================================================== //
        
        using thread_token = uint64_t;
        
        template<class Clock>
        class Scheduler final
        {
        public: // classes
            
            using clock_t = Clock;
            using time_point_t = typename Clock::time_point;
            using duration_t = typename Clock::duration;
            
            class Task;
            
        private: // classes
        
            class Queue;
            
            class Event;
            
        public: // methods
            
            static void createInstance();
            
            static Scheduler& getInstance();
            
            ~Scheduler();
            
            void registerConsumer(thread_token consumer);
            
            void registerProducer(thread_token producer, thread_token consumer);
            
            void schedule(Task * task, duration_t delay = std::chrono::milliseconds(0));
            
            void unschedule(Task * task);
            
            void deleteTask(Task * task);
            
            void process(thread_token consumer);
            
        private: // methods
            
            Scheduler();
            
        private: // members
            
            std::map<thread_token, std::map<thread_token, Queue>>   m_queues;
            
        private: // static members
            
            static std::unique_ptr<Scheduler> m_instance;
            
        private: // deleted methods
            
            Scheduler(Scheduler const& other) = delete;
            Scheduler(Scheduler && other) = delete;
            Scheduler& operator=(Scheduler const& other) = delete;
            Scheduler& operator=(Scheduler && other) = delete;
        };
        
        // ==================================================================================== //
        //                                       PRODUCER                                       //
        // ==================================================================================== //
        
        template <class Clock>
        class Scheduler<Clock>::Queue final
        {
        public: // classes
            
            struct Command
            {
                std::shared_ptr<Event>  m_event;
                time_point_t            m_time;
            };
            
        public: // methods
            
            Queue();
            
            ~Queue();
            
            void schedule(Task * task, duration_t delay);

            void unschedule(Task * task);
            
            void process(time_point_t process_time);
            
        private: // methods
            
            void remove(std::shared_ptr<Event> const& event);
            
            void insert(std::shared_ptr<Event> const& event);
            
        private: // members
            
            std::vector<std::shared_ptr<Event>> m_events;
            ConcurrentQueue<Command>            m_commands;
            
            
        private: // deleted methods
            
            Queue(Queue const& other) = delete;
            Queue(Queue && other) = delete;
            Queue& operator=(Queue const& other) = delete;
            Queue& operator=(Queue && other) = delete;
        };
        
        // ==================================================================================== //
        //                                       TASK                                           //
        // ==================================================================================== //
        
        template <class Clock>
        class Scheduler<Clock>::Task
        {
        protected: // methods
            
            Task(thread_token producer, thread_token consumer);
            
            virtual ~Task();
            
            void disable();
            
        private: // methods

            virtual void execute() = 0;
            
        private: // methods
            
            thread_token            m_producer;
            thread_token            m_consumer;
            std::shared_ptr<Event>  m_event;
            mutable std::mutex      m_mutex;
            
        private: // friends
            
            friend class Scheduler;
            
        private: // deleted methods
            
            Task(Task const& other) = delete;
            Task(Task && other) = delete;
            Task& operator=(Task const& other) = delete;
            Task& operator=(Task && other) = delete;
        };
        
        // ==================================================================================== //
        //                                       EVENT                                          //
        // ==================================================================================== //
        
        template<class Clock>
        class Scheduler<Clock>::Event final
        {
        public: // methods
            
            Event(Task * task, time_point_t time);
            
            ~Event();
            
            bool operator==(Event const& other) const;
            
            void execute();
            
        private: // methods
            
            void disable();
            
        private: // friends
            
            friend class Scheduler;
            
        private: // members
            
            Task *                      m_task;
            time_point_t                m_time;
            mutable std::mutex          m_mutex;
            
        private: // deleted methods
            
            Event() = delete;
            Event(Event const& other) = delete;
            Event(Event && other) = delete;;
            Event& operator=(Event && other);
            Event& operator=(Event const& other) = delete;
        };
    }
}

#include "KiwiEngine_Scheduler.hpp"


#endif // KIWI_ENGINE_SCHEDULER_HPP_INCLUDED
