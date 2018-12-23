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

#include <chrono>
#include <atomic>
#include <vector>
#include <iostream>
#include <thread>
#include <memory>

#include "../catch.hpp"

#include <KiwiTool/KiwiTool_Scheduler.h>

using namespace kiwi;
;
using Scheduler = tool::Scheduler<std::chrono::high_resolution_clock>;
using Task = Scheduler::Task;
using CallBack = Scheduler::CallBack;

// ==================================================================================== //
//                                      SCHEDULER                                       //
// ==================================================================================== //

TEST_CASE("Scheduler - mono thread", "[Scheduler]")
{
    Scheduler sch;
    
    SECTION("Simple add and process")
    {
        int counter = 0;
        
        std::function<void()> func = [&counter]() { ++counter; };
        
        for(int i = 0 ; i < 10; ++i)
        {
            sch.schedule(std::make_shared<CallBack>(func),
                         std::chrono::milliseconds(10 * i));
        }
        
        while(counter < 10) { sch.process(); }
        
        CHECK(counter == 10);
    }
    
    SECTION("Schedule lambda")
    {
        bool success = false;
        
        sch.schedule([&success]() {
            success = true;
        }, std::chrono::milliseconds(0));
        
        REQUIRE_FALSE(success);
        sch.process();
        REQUIRE(success);
    }
    
    SECTION("Schedule lambda with delay")
    {
        bool success = false;
        
        sch.schedule([&success]() {
            success = true;
        }, std::chrono::milliseconds(1000));
        
        REQUIRE_FALSE(success);
        sch.process();
        REQUIRE_FALSE(success);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        sch.process();
        REQUIRE_FALSE(success);
        
        // check with 5ms precision.
        std::this_thread::sleep_for(std::chrono::milliseconds(505));
        sch.process();
        
        REQUIRE(success);
    }
    
    SECTION("Ownership")
    {
        struct TestDestructor : public Task
        {
            TestDestructor(int &counter) : m_counter(counter) {}
            ~TestDestructor() { ++m_counter; }
            void execute() override final {}
            
        private:
            int& m_counter;
        };
        
        int shared_count = 0;
        int transfered_count = 0;
        
        auto shared = std::make_shared<TestDestructor>(shared_count);
        auto transfered = std::make_shared<TestDestructor>(transfered_count);
        
        sch.schedule(shared);
        CHECK(shared.use_count() == 2);
        
        sch.schedule(std::move(transfered));
        CHECK(transfered.use_count() == 0);
        
        sch.process();
        
        CHECK(transfered_count == 1); // Check that transfered was destroyed.
        CHECK(shared_count == 0); // Check that shared was not destroyed.
    }
    
    SECTION("Cancel/Reschedule mono thread")
    {
        int i_standard = 0;
        int i_cancel = 0;
        int i_reschedule = 0;
        
        std::function<void()> func_std = [&i_standard](){++i_standard;};
        std::function<void()> func_cancel = [&i_cancel](){++i_cancel;};
        std::function<void()> func_reschedule = [&i_reschedule](){++i_reschedule;};
        
        auto standard = std::make_shared<CallBack>(func_std);
        auto reschedule = std::make_shared<CallBack>(func_reschedule);
        auto cancel = std::make_shared<CallBack>(func_cancel);
        
        sch.schedule(std::move(standard));
        sch.schedule(reschedule);
        sch.schedule(cancel);
        
        sch.schedule(reschedule, std::chrono::milliseconds(1000 * 60 * 60));
        sch.unschedule(cancel);
        
        while(i_standard < 1) { sch.process(); }
        
        CHECK(i_standard == 1);
        CHECK(i_reschedule == 0);
        CHECK(i_cancel == 0);
        
        sch.schedule(reschedule);
        
        while(i_reschedule < 1) { sch.process(); }
        
        CHECK(i_reschedule == 1);
    }
}


// ==================================================================================== //
//                               SCHEDULER - MULTI THREAD                               //
// ==================================================================================== //

TEST_CASE("Scheduler - Multithread", "[Scheduler]")
{
    Scheduler sch;
    
    SECTION("multiproducer - multiconsumer")
    {
        std::atomic<size_t> count_producer_1(0);
        std::atomic<size_t> count_producer_2(0);
        
        std::function<void()> func_1 = [&count_producer_1]() {
            ++count_producer_1;
        };
        
        std::function<void()> func_2 = [&count_producer_2]() {
            ++count_producer_2;
        };
        
        std::thread producer_1([&sch, &func_1]() {
            size_t count_event = 0;
            
            while(count_event < 30)
            {
                sch.schedule(std::make_shared<CallBack>(func_1));
                ++count_event;
            }
        });
        
        std::thread producer_2([&sch, &func_2]() {
            size_t count_event = 0;
            
            while(count_event < 20)
            {
                sch.schedule(std::make_shared<CallBack>(func_2));
                ++count_event;
            }
        });
        
        while(count_producer_1 < 30 || count_producer_2 < 20)
        {
            sch.process();
        }
        
        CHECK(count_producer_1 == 30);
        CHECK(count_producer_2 == 20);
        
        producer_1.join();
        producer_2.join();
    }
    
    
    SECTION("Execution Order")
    {
        std::vector<int> order;
        std::function<void(int)> func = [&order](int stamp) {order.push_back(stamp);};
        
        SECTION("Pushing producer 1 before producer 2")
        {
            std::thread producer_1([&sch, &func]() {
                sch.schedule(std::make_shared<CallBack>(std::bind(func, 1)));
            });
            
            producer_1.join();
            
            std::thread producer_2([&sch, &func]() {
                sch.schedule(std::make_shared<CallBack>(std::bind(func, 2)));
            });
            
            producer_2.join();
            
            while(order.size() < 2) { sch.process(); }
            
            // Check that producer 1's task is executed first.
            
            CHECK(order[0] == 1);
            CHECK(order[1] == 2);
        }
        
        SECTION("Pushing producer 2 before producer 1")
        {
            std::thread producer_2([&sch, &func]() {
                sch.schedule(std::make_shared<CallBack>(std::bind(func, 2)));
            });
            
            producer_2.join();
            
            std::thread producer_1([&sch, &func]() {
                sch.schedule(std::make_shared<CallBack>(std::bind(func, 1)));
            });
            
            producer_1.join();
            
            while(order.size() < 2) { sch.process(); }
            
            // Check that producer 2's task is executed first.
            
            CHECK(order[0] == 2);
            CHECK(order[1] == 1);
        }
    }
}


// ==================================================================================== //
//                             SCHEDULER - OWNERSHIP TRANSFER                           //
// ==================================================================================== //

TEST_CASE("Scheduler - Thread ids", "[Scheduler]")
{
    Scheduler sch;

    SECTION("Consumer ownership transfer")
    {
        REQUIRE(sch.isThisConsumerThread());
        
        // Transfer consumer ownership.
        
        std::thread consumer_thread([&sch]() {
            sch.setThreadAsConsumer();
            REQUIRE(sch.isThisConsumerThread());
        });
        
        consumer_thread.join();
        
        CHECK(!sch.isThisConsumerThread());
    }
}


// ==================================================================================== //
//                                  SCHEDULER - LOCK                                    //
// ==================================================================================== //

TEST_CASE("Scheduler - Lock", "[Scheduler]")
{
    Scheduler sch;
    
    SECTION("Scheduler lock")
    {
        std::atomic<bool> quit_requested(false);
        
        std::thread consumer([&sch, &quit_requested]() {
            sch.setThreadAsConsumer();
            
            while(!quit_requested.load())
            {
                sch.process();
            }
        });
        
        {
            std::unique_lock<std::mutex> lock(sch.lock());
            
            sch.schedule(std::make_shared<CallBack>([&quit_requested]() {
                quit_requested.store(true);
            }));
            
            std::this_thread::sleep_for(std::chrono::seconds(1));
            
            REQUIRE(!quit_requested);
        }
        
        consumer.join();
        
        CHECK(quit_requested);
    }
}


// ==================================================================================== //
//                               SCHEDULER - DEFERING TASKS                             //
// ==================================================================================== //

TEST_CASE("Scheduler - Defering tasks", "[Scheduler]")
{
    Scheduler sch;
    
    struct DeferTask : public Scheduler::Task
    {
        DeferTask(std::atomic<std::thread::id>& thread_id, std::atomic_bool& executed_flag)
        : m_thread_id(thread_id)
        , m_executed_flag(executed_flag) {}
        
        void execute() override
        {
            m_executed_flag.store(true);
            m_thread_id.store(std::this_thread::get_id());
        }
        
    private:
        std::atomic<std::thread::id>&   m_thread_id;
        std::atomic_bool&               m_executed_flag;
    };
    
    SECTION("Defering task on the same thread execute task directly")
    {
        std::atomic<std::thread::id> exec_thread;
        std::atomic_bool executed_flag(false);
        auto task = std::make_shared<DeferTask>(exec_thread, executed_flag);
        
        REQUIRE(!executed_flag);
        
        SECTION("task copy")
        {
            sch.defer(task);
            CHECK(task.use_count() == 1);
        }
        
        SECTION("task move")
        {
            sch.defer(std::move(task));
            CHECK(task.use_count() == 0);
        }
        
        SECTION("task lambda")
        {
            sch.defer([&exec_thread, &executed_flag]() {
                executed_flag.store(true);
                exec_thread.store(std::this_thread::get_id());
            });
        }
        
        CHECK(executed_flag);
        CHECK(exec_thread == std::this_thread::get_id());
    }
    
    SECTION("Defering task on another thread schedule the task")
    {
        std::atomic_bool quit_requested(false);
        
        std::thread consumer([&sch, &quit_requested]() {
            
            sch.setThreadAsConsumer();
            
            while(!quit_requested.load())
            {
                sch.process();
            }
        });
        
        std::atomic<std::thread::id> exec_thread(std::this_thread::get_id());
        std::atomic_bool executed_flag(false);
        auto task = std::make_shared<DeferTask>(exec_thread, executed_flag);
        
        while(sch.isThisConsumerThread()){}
        
        REQUIRE(!executed_flag);
        
        SECTION("task copy")
        {
            sch.defer(task);
            
            while(exec_thread != consumer.get_id()){}
            
            CHECK(task.use_count() == 1);
        }
        
        SECTION("task move")
        {
            sch.defer(std::move(task));
            
            while(exec_thread != consumer.get_id()){}
            
            CHECK(task.use_count() == 0);
        }
        
        SECTION("task lambda")
        {
            sch.defer([&exec_thread, &executed_flag]() {
                executed_flag.store(true);
                exec_thread.store(std::this_thread::get_id());
            });
            
            while(exec_thread != consumer.get_id()){}
        }
        
        CHECK(executed_flag);
        CHECK(exec_thread == consumer.get_id());
        
        quit_requested.store(true);
        consumer.join();
    }
}

// ==================================================================================== //
//                              SCHEDULER - CUSTOM CLOCK                                //
// ==================================================================================== //

struct TickClock
{
    using clock_t = std::chrono::high_resolution_clock;
    using duration = clock_t::duration;
    using time_point = clock_t::time_point;
    
    static void start() {current_time = clock_t::now();}
    
    static void tick() {current_time += std::chrono::milliseconds(1);};
    
    static time_point now() {return current_time;};
    
private:
    static time_point current_time;
};

TickClock::time_point TickClock::current_time = TickClock::clock_t::now();

TEST_CASE("Scheduler - custom clock", "[Scheduler]")
{
    using TickScheduler = tool::Scheduler<TickClock>;
    
    SECTION("Execution order and custom clock")
    {
        TickClock::start();
        
        TickScheduler scheduler;
        
        std::vector<int> order;
        
        std::function<void(int)> func = [&order](int number){order.push_back(number);};
        
        auto task_0 = std::make_shared<TickScheduler::CallBack>(std::bind(func, 0));
        auto task_1 = std::make_shared<TickScheduler::CallBack>(std::bind(func, 1));
        auto task_2 = std::make_shared<TickScheduler::CallBack>(std::bind(func, 2));
        auto task_3 = std::make_shared<TickScheduler::CallBack>(std::bind(func, 3));
        
        scheduler.schedule(task_0, std::chrono::milliseconds(1));
        scheduler.schedule(task_1, std::chrono::milliseconds(3));
        scheduler.schedule(std::move(task_2), std::chrono::milliseconds(2));
        scheduler.schedule(std::move(task_3), std::chrono::milliseconds(2));
        
        scheduler.schedule(task_0, std::chrono::milliseconds(3));
        scheduler.unschedule(task_1);
        
        while(order.size() < 3)
        {
            TickClock::tick();
            scheduler.process();
        }
        
        scheduler.unschedule(task_0);
        
        CHECK(order[0] == 2);
        CHECK(order[1] == 3);
        CHECK(order[2] == 0);
    }
}
