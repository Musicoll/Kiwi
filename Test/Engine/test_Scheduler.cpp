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

#include <chrono>
#include <atomic>
#include <vector>
#include <iostream>

#include "../catch.hpp"

#include <KiwiEngine/KiwiEngine_Scheduler.h>

using namespace kiwi;

template<class T>
class TTask final : public engine::Scheduler<T>::Task
{
public: // methods
    
    TTask(engine::Scheduler<T>& sch,
         typename engine::Scheduler<T>::token_t producer_token,
         std::function<void()> func,
         bool process_delete = true):
    engine::Scheduler<T>::Task(sch, producer_token),
    m_func(func),
    m_process_delete(process_delete){};
    
    void execute() override
    {
        m_func.operator()();
        
        if (m_process_delete)
        {
            delete this;
        }
    }
    
    ~TTask() = default;
    
private: // members
    
    std::function<void()>   m_func;
    bool                    m_process_delete;
};

using Scheduler = engine::Scheduler<std::chrono::high_resolution_clock>;

using Task = TTask<std::chrono::high_resolution_clock>;

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
    SECTION("Simple add and process")
    {
        Scheduler sch;
        
        Scheduler::token_t token = sch.createProducerToken();
        
        int counter = 0;
        
        std::function<void()> func = [&counter](){++counter;};
        
        for(int i = 0 ; i < 10; ++i)
        {
            sch.schedule(new Task(sch, token, func), std::chrono::milliseconds(10 * i));
        }
        
        while(counter < 10){ sch.process();}
        
        CHECK(counter == 10);
    }
    
    SECTION("Cancel/Reschedule mono thread")
    {
        Scheduler sch;
        
        Scheduler::token_t token = sch.createProducerToken();
        
        int i_standard = 0;
        int i_cancel = 0;
        int i_reschedule = 0;
        
        std::function<void()> func_std = [&i_standard](){++i_standard;};
        std::function<void()> func_cancel = [&i_cancel](){++i_cancel;};
        std::function<void()> func_reschedule = [&i_reschedule](){++i_reschedule;};
        
        std::unique_ptr<Scheduler::Task> standard(new Task(sch, token, func_std, false));
        std::unique_ptr<Scheduler::Task> reschedule(new Task(sch, token, func_reschedule, false));
        std::unique_ptr<Scheduler::Task> cancel(new Task(sch, token, func_cancel, false));
        
        sch.schedule(standard.get());
        sch.schedule(reschedule.get());
        sch.schedule(cancel.get());
        
        sch.schedule(reschedule.get(), std::chrono::milliseconds(1000 * 60 * 60));
        sch.unschedule(cancel.get());
        
        while(i_standard < 1){sch.process();};
        
        CHECK(i_standard == 1);
        CHECK(i_reschedule == 0);
        CHECK(i_cancel == 0);
        
        sch.schedule(reschedule.get());
        
        while(i_reschedule < 1){sch.process();};
        
        CHECK(i_reschedule == 1);
    }
    
    SECTION("Execution order and custom clock")
    {
        engine::Scheduler<TickClock> sch;
        
        engine::Scheduler<TickClock>::token_t token = sch.createProducerToken();
        
        TickClock::start();
        
        std::vector<int> order;
        
        std::function<void(int)> func = [&order](int number){order.push_back(number);};
        
        TTask<TickClock> * task_0 = new TTask<TickClock>(sch, token, std::bind(func, 0), false);
        TTask<TickClock> * task_1 = new TTask<TickClock>(sch, token, std::bind(func, 1), false);
        
        sch.schedule(task_0, std::chrono::milliseconds(1));
        sch.schedule(task_1, std::chrono::milliseconds(3));
        
        sch.schedule(new TTask<TickClock>(sch, token, std::bind(func, 2)), std::chrono::milliseconds(2));
        sch.schedule(new TTask<TickClock>(sch, token, std::bind(func, 3)), std::chrono::milliseconds(2));
        
        sch.schedule(task_0, std::chrono::milliseconds(3));
        delete task_1;
        
        while(order.size() < 3)
        {
            TickClock::tick();
            sch.process();
        }
        
        CHECK(order[0] == 2);
        CHECK(order[1] == 3);
        CHECK(order[2] == 0);
    }
    
    SECTION("Multithreading multiproducer")
    {
        Scheduler sch;
        
        Scheduler::token_t token_1 = sch.createProducerToken();
        Scheduler::token_t token_2 = sch.createProducerToken();
        
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
        
        std::thread producer_1([&count_producer_1, &sch, &func_1, &token_1]()
        {
            size_t count_event = 0;
            
            while(count_event < 30)
            {
                sch.schedule(new Task(sch, token_1, func_1));
                ++count_event;
            }
        });
        
        std::thread producer_2([&count_producer_2, &sch, &func_2, &token_2]()
        {
            size_t count_event = 0;
            
            while(count_event < 20)
            {
                sch.schedule(new Task(sch, token_2, func_2));
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
    
    SECTION("Exceeding capacity multithreading")
    {
        Scheduler sch;
        
        Scheduler::token_t token = sch.createProducerToken();
        
        std::atomic<int> process_counter(0);
        std::atomic<int> push_counter(0);
        
        std::function<void()> func = [&process_counter]()
        {
            ++process_counter;
        };
        
        std::thread producer([&push_counter, &sch, &func, &token]()
        {
            while(push_counter < 1500)
            {
                sch.schedule(new Task(sch, token, func));
                std::atomic_fetch_add(&push_counter, 1);
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
        });
        
        
        std::thread consumer([&process_counter, &push_counter, &sch]()
        {
            while(push_counter.load() < 512){}
            
            while(process_counter.load() < 1500)
            {
                sch.process();
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        });
        
        producer.join();
        consumer.join();
        
        CHECK(process_counter.load() == 1500);
    }
}
