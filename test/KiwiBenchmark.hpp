/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_BENCHMARK_HPP_INCLUDED
#define KIWI_BENCHMARK_HPP_INCLUDED

#include <chrono>
#include <iostream>

class Benchmark
{
public:
    using clock_t = std::chrono::system_clock;
    using duration_t = std::chrono::duration<double>;
    using timestamp_t = std::chrono::time_point<clock_t>;
    
    //! brief Sorting methods
    enum class Sort : uint8_t
    {
        None        = 0,
        ByPerfAsc   = 1,
        ByPerfDesc  = 2
    };
    
    //! brief Starts a test case.
    void startTestCase(const std::string& message, const Sort& sort_method = Sort::None)
    {
        m_test_case_message = message;
        m_sort_method = sort_method;
    }
    
    //! brief Ends a test case.
    void endTestCase()
    {
        printResult();
        m_units.clear();
    }
    
    //! brief Starts a unit.
    void startUnit(const std::string& name)
    {
        Unit unit(name);
        unit.start();
        m_units.push_back(unit);
    }
    
    //! brief Ends a unit.
    void endUnit()
    {
        if(!m_units.empty())
        {
            auto& last_unit = m_units.at(m_units.size() - 1);
            
            if(!last_unit.isCompleted())
            {
                last_unit.end();
            }
        }
    }
    
private:
    
    class Unit
    {
    public:
        Unit(const std::string& name) : m_name(name) {}
        inline std::string getName() const noexcept { return m_name; }
        inline double getResult() const noexcept    { return m_result_ms; }
        inline bool isRunning() const noexcept      { return m_completed; }
        inline bool isCompleted() const noexcept    { return m_completed; }
        
        void start()
        {
            if(m_completed || m_running) return;
            
            m_running = true;
            m_start = clock_t::now();
        }
        
        void end()
        {
            if(!m_running || m_completed) return;
            
            m_end = clock_t::now();
            duration_t time = (m_end - m_start);
            m_result_ms = time.count() * 1000.;
            m_completed = true;
            m_running = false;
        }
        
    private:
        std::string m_name;
        timestamp_t m_start;
        timestamp_t m_end;
        bool m_running = false;
        bool m_completed = false;
        double m_result_ms;
    };
    
    class Print
    {
    public:
        Print(const std::string& message) {std::cout << message;}
        
        static void newLine() {std::cout << '\n';}
        static void DashedLine_1()
        {
            for(int i = 0; i < 20; ++i)
            {
                std::cout << '-';
            }
        }
        static void DashedLine()
        {
            for(int i = 0; i < 80; ++i)
            {
                std::cout << ((i % 2) ? '=' : '-');
            }
        }
    };
    
    void sortResult() noexcept
    {
        if(m_sort_method != Sort::None)
        {
            std::sort(m_units.begin(), m_units.end(), [&](const Unit& lhs, const Unit& rhs)
            {
                if(m_sort_method == Sort::ByPerfAsc)
                {
                    return (lhs.getResult() > rhs.getResult());
                }
                else if(m_sort_method == Sort::ByPerfDesc)
                {
                    return (lhs.getResult() < rhs.getResult());
                }
                
                return true;
            });
        }
    }
        
    void printResult()
    {
        sortResult();
        
        Print::DashedLine();
        Print::newLine();
        std::cout << "Test : " << m_test_case_message;
        Print::newLine();
        Print::DashedLine_1();
        Print::newLine();
        
        double total = 0.;
        
        for(auto& unit : m_units)
        {
            auto name = unit.getName();
            double result = unit.getResult();
            std::cout << "|" << result << " ms| \t=> " << name;
            Print::newLine();
            total += result;
        }
        
        Print::newLine();
        
        std::cout << "Total : " << total << " ms";
        
        Print::newLine();
        Print::newLine();
    }
    
    std::vector<Unit>   m_units;
    Sort                m_sort_method;
    std::string         m_test_case_message;
};

#endif /* KIWI_TEST_UTILITY_HPP_INCLUDED */
