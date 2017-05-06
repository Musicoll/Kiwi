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
#include <thread>

#include "../catch.hpp"

#include <KiwiEngine/KiwiEngine_Scheduler.h>

using namespace kiwi;

template<class T>
class TTask final : public engine::Scheduler<T>::Task
{
public: // methods
    
    TTask(engine::thread_token producer_token,
          engine::thread_token consumer_token,
          std::function<void()> func,
          bool process_delete = true):
    engine::Scheduler<T>::Task(producer_token, consumer_token),
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

enum Thread : engine::thread_token
{
    Gui     = 0,
    Engine  = 1,
    Dsp     = 2,
    Network = 3
};



// ==================================================================================== //
//                                          SCHEDULER                                   //
// ==================================================================================== //

TEST_CASE("Scheduler", "[Scheduler]")
{
    Scheduler::createInstance();
    
    Scheduler& sch = Scheduler::use();
    
    sch.registerConsumer(Thread::Engine);
    
    sch.registerProducer(Thread::Gui, Thread::Engine);
    sch.registerProducer(Thread::Dsp, Thread::Engine);
    sch.registerProducer(Thread::Engine, Thread::Engine);
    
    
    SECTION("Simple add and process")
    {   
        int counter = 0;
        
        std::function<void()> func = [&counter](){++counter;};
        
        for(int i = 0 ; i < 10; ++i)
        {
            sch.schedule(new Task(Thread::Engine, Thread::Engine, func), std::chrono::milliseconds(10 * i));
        }
        
        while(counter < 10){ sch.process(Thread::Engine);}
        
        CHECK(counter == 10);
    }
    
    SECTION("Cancel/Reschedule mono thread")
    {
        int i_standard = 0;
        int i_cancel = 0;
        int i_reschedule = 0;
        
        std::function<void()> func_std = [&i_standard](){++i_standard;};
        std::function<void()> func_cancel = [&i_cancel](){++i_cancel;};
        std::function<void()> func_reschedule = [&i_reschedule](){++i_reschedule;};
        
        Scheduler::Task* standard = new Task(Thread::Engine, Thread::Engine, func_std);
        Scheduler::Task* reschedule = new Task(Thread::Engine, Thread::Engine, func_reschedule);
        Scheduler::Task* cancel = new Task(Thread::Engine, Thread::Engine, func_cancel, false);
        
        sch.schedule(standard);
        sch.schedule(reschedule);
        sch.schedule(cancel);
        
        sch.schedule(reschedule, std::chrono::milliseconds(1000 * 60 * 60));
        delete cancel;
        
        while(i_standard < 1){sch.process(Thread::Engine);};
        
        CHECK(i_standard == 1);
        CHECK(i_reschedule == 0);
        CHECK(i_cancel == 0);
        
        sch.schedule(reschedule);
        
        while(i_reschedule < 1){sch.process(Thread::Engine);};
        
        CHECK(i_reschedule == 1);
    }
    
    SECTION("Execution order and custom clock")
    {
        TickClock::start();
        
        engine::Scheduler<TickClock>::createInstance();
        engine::Scheduler<TickClock>& tick_scheduler = engine::Scheduler<TickClock>::use();
        
        tick_scheduler.registerConsumer(Thread::Engine);
        tick_scheduler.registerProducer(Thread::Engine, Thread::Engine);
        
        std::vector<int> order;
        
        std::function<void(int)> func = [&order](int number){order.push_back(number);};
        
        TTask<TickClock> * task_0 = new TTask<TickClock>(Thread::Engine, Thread::Engine, std::bind(func, 0), false);
        TTask<TickClock> * task_1 = new TTask<TickClock>(Thread::Engine, Thread::Engine, std::bind(func, 1), false);
        
        tick_scheduler.schedule(task_0, std::chrono::milliseconds(1));
        tick_scheduler.schedule(task_1, std::chrono::milliseconds(3));
        
        tick_scheduler.schedule(new TTask<TickClock>(Thread::Engine, Thread::Engine, std::bind(func, 2)),
                                std::chrono::milliseconds(2));
        tick_scheduler.schedule(new TTask<TickClock>(Thread::Engine, Thread::Engine, std::bind(func, 3)),
                                std::chrono::milliseconds(2));
        
        tick_scheduler.schedule(task_0, std::chrono::milliseconds(3));
        delete task_1;
        
        while(order.size() < 3)
        {
            TickClock::tick();
            tick_scheduler.process(Thread::Engine);
        }
        
        delete task_0;
        
        CHECK(order[0] == 2);
        CHECK(order[1] == 3);
        CHECK(order[2] == 0);
    }
    
    SECTION("Multithreading multiproducern multiconsumer")
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
                sch.schedule(new Task(Thread::Gui, Thread::Engine, func_1));
                ++count_event;
            }
        });
        
        std::thread producer_2([&count_producer_2, &sch, &func_2]()
        {
            size_t count_event = 0;
            
            while(count_event < 20)
            {
                sch.schedule(new Task(Thread::Dsp, Thread::Engine, func_2));
                ++count_event;
            }
        });
        
        while(count_producer_1 < 30 || count_producer_2 < 20)
        {
            sch.process(Thread::Engine);
        }
        
        CHECK(count_producer_1 == 30);
        CHECK(count_producer_2 == 20);
        
        producer_1.join();
        producer_2.join();
    }
}

// ==================================================================================== //
//                                          SCHEDULER BENCHMARK                         //
// ==================================================================================== //

void check_precision(std::vector<Scheduler::duration_t> * precision, Scheduler::time_point_t expected_time)
{
    precision->push_back(Scheduler::clock_t::now() - expected_time);
}

size_t mean(std::vector<Scheduler::duration_t> &duration_list)
{
    size_t mean = 0;
    
    for (int i = 0; i < duration_list.size(); ++i)
    {
        mean = mean + std::chrono::duration_cast<std::chrono::nanoseconds>(duration_list[i]).count();
    }
    
    return mean / duration_list.size();
}

void construct_delay_list(std::vector<size_t> &delay_list, size_t size)
{
    std::vector<size_t> increment_list = {11, 56, 12, 70, 2, 92, 32, 66, 3, 102};
    
    size_t cursor = 0;
    size_t delay = 0;
    
    for (int i = 0; i < size; ++i)
    {
        delay_list.push_back(delay);
        
        delay  = (delay + increment_list[cursor])  % 100;
        
        cursor = cursor = increment_list.size() ? 0 : cursor + 1;
    }
}

TEST_CASE("Scheduler Benchmark", "[Scheduler]")
{
    Scheduler& sch = Scheduler::use();
    
    SECTION("Benchmark no delay MonoProducer")
    {
        // vector to check the mean of insertion time
        std::vector<Scheduler::duration_t> insert;
        insert.reserve(2048);
        
        // vector to check the execution precision of the callback
        std::vector<Scheduler::duration_t> precision;
        precision.reserve(2048);
        
        Scheduler::time_point_t before_launch = Scheduler::clock_t::now();
        
        std::thread producer([&sch, &insert, &precision]()
        {
            size_t counter = 0;
            
            while(counter < 2048)
            {
                Scheduler::duration_t delay = std::chrono::milliseconds(0);
                
                Scheduler::Task* task = new Task(Thread::Gui,
                                                 Thread::Engine,
                                                 std::bind(check_precision, &precision, Scheduler::clock_t::now()));
                                     
                                     
                Scheduler::time_point_t before = Scheduler::clock_t::now();
                
                sch.schedule(task, delay);
                
                insert.push_back(Scheduler::clock_t::now() - before);
                ++counter;
            }
        });
        
        std::thread consumer([&sch, &precision]()
        {
            while(precision.size() < 2048)
            {
                sch.process(Thread::Engine);
            }
        });
        
        producer.join();
        consumer.join();
        
        size_t exec_time = std::chrono::duration_cast<std::chrono::microseconds>(Scheduler::clock_t::now() - before_launch).count();
        
        std::cout << "Benchmark no delay results ------------" << std::endl;
        std::cout << "Global Time : " << exec_time << " microseconds" << std::endl;
        std::cout << "Mean insert time : " << mean(insert) << " nanoseconds" << std::endl;
        std::cout << "Mean precision : " << mean(precision) << " nanoseconds" << std::endl << std::endl;
    }
    
    SECTION("Benchmark random delay MonoProducer")
    {
        // vector to check the mean of insertion time
        std::vector<Scheduler::duration_t> insert;
        insert.reserve(2048);
        
        // vector to check the execution precision of the callback
        std::vector<Scheduler::duration_t> precision;
        precision.reserve(2048);
        
        // vector of pseudo random delay time between 0 and 100
        std::vector<size_t> delay_list;
        construct_delay_list(delay_list, 2048);
        
        Scheduler::time_point_t before_launch = Scheduler::clock_t::now();
        
        std::thread producer([&sch, &insert, &precision, &delay_list]()
        {
            size_t counter = 0;
            
            while(counter < 2048)
            {
                Scheduler::duration_t delay = std::chrono::milliseconds(delay_list[counter]);
                
                std::function<void(void)> func =
                    std::bind(check_precision, &precision, Scheduler::clock_t::now() + delay);
                
                Task* task = new Task(Thread::Gui, Thread::Engine, func);
                
                Scheduler::time_point_t before = Scheduler::clock_t::now();
                
                sch.schedule(task, delay);
                
                insert.push_back(Scheduler::clock_t::now() - before);
                ++counter;
            }
        });
        
        std::thread consumer([&sch, &precision]()
        {
            while(precision.size() < 2048)
            {
                sch.process(Thread::Engine);
            }
        });
        
        consumer.join();
        producer.join();
        
        size_t exec_time =
        std::chrono::duration_cast<std::chrono::microseconds>(Scheduler::clock_t::now() - before_launch).count();
        
        std::cout << "Benchmark delay results -------------------" << std::endl;
        std::cout << "Global Time : " << exec_time << " microseconds" << std::endl;
        std::cout << "Mean insert time : " << mean(insert) << " nanoseconds" << std::endl;
        std::cout << "Mean precision nanoseconds: " << mean(precision) << " nanoseconds" << std::endl << std::endl;
    }
}

