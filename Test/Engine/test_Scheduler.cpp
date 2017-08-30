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

#include <KiwiEngine/KiwiEngine_Scheduler.h>

using namespace kiwi;

using Scheduler = engine::Scheduler<std::chrono::high_resolution_clock>;

using Task = engine::Scheduler<>::Task;

using CallBack = engine::Scheduler<>::CallBack;

static std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();

struct TickClock
{
    typedef std::chrono::high_resolution_clock::duration                 duration;
    typedef duration::rep                                                rep;
    typedef duration::period                                             period;
    typedef std::chrono::time_point<std::chrono::high_resolution_clock>  time_point;
    static const bool is_steady =                                        true;
    
    static void start() {current_time = std::chrono::high_resolution_clock::now();}
    
    static void tick(){current_time += std::chrono::milliseconds(1);};
    
    static time_point now(){return current_time;};
};


// ==================================================================================== //
//                                          SCHEDULER                                   //
// ==================================================================================== //

TEST_CASE("Scheduler", "[Scheduler]")
{
    Scheduler sch;
    
    SECTION("Simple add and process")
    {   
        int counter = 0;
        
        std::function<void()> func = [&counter](){++counter;};
        
        for(int i = 0 ; i < 10; ++i)
        {
            sch.schedule(std::shared_ptr<Task>(new CallBack(func)),
                                               std::chrono::milliseconds(10 * i));
        }
        
        while(counter < 10){ sch.process();}
        
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
            TestDestructor(int &counter):m_counter(counter){};
            
            ~TestDestructor(){++m_counter;};
            
            void execute() override final{};
            
            int& m_counter;
        };

        int shared_count = 0;
        int transfered_count = 0;
        
        std::shared_ptr<Task> shared(new TestDestructor(shared_count));
        std::shared_ptr<Task> transfered(new TestDestructor(transfered_count));
        
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
        
        std::shared_ptr<Task> standard(new CallBack(func_std));
        std::shared_ptr<Task> reschedule(new CallBack(func_reschedule));
        std::shared_ptr<Task> cancel(new CallBack(func_cancel));
        
        sch.schedule(std::move(standard));
        sch.schedule(reschedule);
        sch.schedule(cancel);
        
        sch.schedule(reschedule, std::chrono::milliseconds(1000 * 60 * 60));
        sch.unschedule(cancel);
        
        while(i_standard < 1){sch.process();};
        
        CHECK(i_standard == 1);
        CHECK(i_reschedule == 0);
        CHECK(i_cancel == 0);
        
        sch.schedule(reschedule);
        
        while(i_reschedule < 1){sch.process();};
        
        CHECK(i_reschedule == 1);
    }
    
    SECTION("Execution order and custom clock")
    {
        TickClock::start();
        
        engine::Scheduler<TickClock> tick_scheduler;
        
        std::vector<int> order;
        
        std::function<void(int)> func = [&order](int number){order.push_back(number);};
        
        std::shared_ptr<engine::Scheduler<TickClock>::Task>
                    task_0(new engine::Scheduler<TickClock>::CallBack(std::bind(func, 0)));
        
        std::shared_ptr<engine::Scheduler<TickClock>::Task>
                    task_1(new engine::Scheduler<TickClock>::CallBack(std::bind(func, 1)));
        
        tick_scheduler.schedule(task_0, std::chrono::milliseconds(1));
        tick_scheduler.schedule(task_1, std::chrono::milliseconds(3));
        
        std::shared_ptr<engine::Scheduler<TickClock>::Task>
                    task_2(new engine::Scheduler<TickClock>::CallBack(std::bind(func, 2)));
        
        std::shared_ptr<engine::Scheduler<TickClock>::Task>
                    task_3(new engine::Scheduler<TickClock>::CallBack(std::bind(func, 3)));
        
        tick_scheduler.schedule(std::move(task_2), std::chrono::milliseconds(2));
        tick_scheduler.schedule(std::move(task_3), std::chrono::milliseconds(2));
        
        tick_scheduler.schedule(task_0, std::chrono::milliseconds(3));
        tick_scheduler.unschedule(task_1);
        
        while(order.size() < 3)
        {
            TickClock::tick();
            tick_scheduler.process();
        }
        
        tick_scheduler.unschedule(task_0);
        
        CHECK(order[0] == 2);
        CHECK(order[1] == 3);
        CHECK(order[2] == 0);
    }
    
    SECTION("Multithreading multiproducer multiconsumer")
    {
        std::atomic<size_t> count_producer_1(0);
        std::atomic<size_t> count_producer_2(0);
        
        std::function<void()> func_1 = [&count_producer_1]()
        {
            ++count_producer_1;
        };
        
        std::function<void()> func_2 = [&count_producer_2]()
        {
            ++count_producer_2;
        };
        
        std::thread producer_1([&count_producer_1, &sch, &func_1]()
        {
            size_t count_event = 0;
            
            while(count_event < 30)
            {
                sch.schedule(std::shared_ptr<Task>(new CallBack(func_1)));
                ++count_event;
            }
        });
        
        std::thread producer_2([&count_producer_2, &sch, &func_2]()
        {
            size_t count_event = 0;
            
            while(count_event < 20)
            {
                sch.schedule(std::shared_ptr<Task>(new CallBack(func_2)));
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
    
    SECTION("Multithreading execution order")
    {
        std::vector<int> order;
        
        std::function<void(int)> func = [&order](int stamp){order.push_back(stamp);};
        
        // Pushing producer 1 before producer 2
        
        {
            std::thread producer_1([&sch, &func]()
            {
                sch.schedule(std::make_shared<CallBack>(std::bind(func, 1)));
            });
        
            producer_1.join();
        
            std::thread producer_2([&sch, &func]()
            {
                sch.schedule(std::make_shared<CallBack>(std::bind(func, 2)));
            });
        
            producer_2.join();
        
            while(order.size() < 2)
            {
                sch.process();
            }
        
            // Check that producer 1's task is executed first.
        
            CHECK(order[0] == 1);
            CHECK(order[1] == 2);
            
        }
        
        // Pushgin producer 2 before producer 1
        
        {
            std::thread producer_2([&sch, &func]()
            {
                sch.schedule(std::make_shared<CallBack>(std::bind(func, 2)));
            });
            
            producer_2.join();
            
            std::thread producer_1([&sch, &func]()
            {
                sch.schedule(std::make_shared<CallBack>(std::bind(func, 1)));
            });
            
            producer_1.join();
            
            while(order.size() < 4)
            {
                sch.process();
            }
            
            // Check that producer 2's task is executed first.
            
            CHECK(order[2] == 2);
            CHECK(order[3] == 1);
        }
        
        SECTION("Thread ids")
        {
            CHECK(sch.isThisConsumerThread());
            
            // Transfer consumer ownership.
            
            std::thread consumer_thread([&sch]()
            {
                sch.setThreadAsConsumer();
                CHECK(sch.isThisConsumerThread());
            });
            
            consumer_thread.join();
            
            CHECK(!sch.isThisConsumerThread());
            
            // Transfer back the ownership, to enable further test execution.
            
            sch.setThreadAsConsumer();
        }
        
        SECTION("Scheduler lock")
        {
            std::atomic<bool> quit_requested(false);
            
            std::thread consumer([&sch, &quit_requested]()
            {
                sch.setThreadAsConsumer();
                
                while(!quit_requested.load())
                {
                    sch.process();
                }
            });
            
            {
                std::unique_lock<std::mutex> lock(sch.lock());
                
                std::function<void(void)> func = [&quit_requested]()
                {
                    quit_requested.store(true);
                };
                
                sch.schedule(std::shared_ptr<Task>(new CallBack(func)));
                
                std::this_thread::sleep_for(std::chrono::seconds(1));
                
                CHECK(!quit_requested);
            }
            
            consumer.join();
            
            CHECK(quit_requested);
            
            sch.setThreadAsConsumer();
        }
        
        SECTION("defering tasks")
        {
            class DeferTask : public engine::Scheduler<>::Task
            {
            public:
                
                DeferTask(std::atomic<std::thread::id> & thread):
                m_thread(thread)
                {
                }
                
                void execute() override final
                {
                    m_thread.store(std::this_thread::get_id());
                }
                
            private:
                
                std::atomic<std::thread::id> & m_thread;
                
            };
            
            {
                // copy
                {
                    std::atomic<std::thread::id> exec_thread;
                    
                    std::shared_ptr<engine::Scheduler<>::Task> task(new DeferTask(exec_thread));
                    
                    sch.defer(task);
                    
                    CHECK(task.use_count() == 1);
                    CHECK(exec_thread == std::this_thread::get_id());
                }
                
                // move
                {
                    std::atomic<std::thread::id> exec_thread;
                    
                    std::shared_ptr<engine::Scheduler<>::Task> task(new DeferTask(exec_thread));
                    
                    sch.defer(std::move(task));
                    
                    CHECK(task.use_count() == 0);
                    CHECK(exec_thread == std::this_thread::get_id());
                }
                
                // function
                {
                    std::atomic<std::thread::id> exec_thread;
                    
                    std::shared_ptr<engine::Scheduler<>::Task> task(new DeferTask(exec_thread));
                    
                    sch.defer([&exec_thread]()
                    {
                        exec_thread.store(std::this_thread::get_id());
                    });
                    
                    CHECK(exec_thread == std::this_thread::get_id());
                }
            }
            
            {
                std::atomic<bool> quit_requested(false);
                
                std::thread consumer([&sch, &quit_requested]()
                {
                    sch.setThreadAsConsumer();
                    
                    while(!quit_requested.load())
                    {   
                        sch.process();
                    }
                });
                
                while(sch.isThisConsumerThread()){}
                
                // copy
                {
                    std::atomic<std::thread::id> exec_thread;
                    
                    std::shared_ptr<engine::Scheduler<>::Task> task(new DeferTask(exec_thread));
                    
                    sch.schedule(task);
                    
                    while(exec_thread != consumer.get_id()){}
                    
                    CHECK(task.use_count() == 1);
                    CHECK(exec_thread == consumer.get_id());
                }
                
                // move
                {
                    std::atomic<std::thread::id> exec_thread;
                    
                    std::shared_ptr<engine::Scheduler<>::Task> task(new DeferTask(exec_thread));
                    
                    sch.defer(std::move(task));
                    
                    while(exec_thread == consumer.get_id()){}
                    
                    CHECK(task.use_count() == 0);
                    CHECK(exec_thread == consumer.get_id());
                }
                
                // function
                {
                    std::atomic<std::thread::id> exec_thread;
                    
                    std::shared_ptr<engine::Scheduler<>::Task> task(new DeferTask(exec_thread));
                    
                    sch.defer([&exec_thread]()
                    {
                        exec_thread.store(std::this_thread::get_id());
                    });
                    
                    while(exec_thread != consumer.get_id()){}
                    
                    CHECK(exec_thread == consumer.get_id());
                }
                
                quit_requested.store(true);
                
                consumer.join();
            }
        }
    }
}

