/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef KIWI_DSP_MISC_H_INCLUDED
#define KIWI_DSP_MISC_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <exception>

#ifndef __APPLE__
#include "malloc.h"
#endif

namespace kiwi
{
namespace dsp
{
    // ==================================================================================== //
    //                                          ERROR                                       //
    // ==================================================================================== //
    //! @brief The exception.
    //! @details The class defines the exceptions that can occur during of the creation or
    //! @details the compilation of the dsp chain.
    class Error : public std::exception
    {
    public:
        //! @brief The constructor.
        inline Error() noexcept {}        
        //! @brief The destructor.
        virtual inline ~Error() noexcept {}
        //! @brief Retrieves a textual information about the error.
        virtual const char* what() const noexcept override {return "Kiwi::Dsp::Error : problem occurs !";}
    };
    
    // ==================================================================================== //
    //                                          TIMER                                       //
    // ==================================================================================== //
    //! @brief The timer.
    //! @details The class uses high resolution clocks to estimate the time that the CPU uses
    //! @details for a set of processes.
    class Timer
    {
    public:
        //! @brief The nanosecond precision.
        typedef std::chrono::nanoseconds nanoseconds;
        //! @brief The microseconds precision.
        typedef std::chrono::microseconds microseconds;
        //! @brief The milliseconds precision.
        typedef std::chrono::milliseconds milliseconds;
        //! @brief The seconds precision.
        typedef std::chrono::seconds seconds;
        //! @brief The minutes precision.
        typedef std::chrono::minutes minutes;
        //! @brief The hours precision.
        typedef std::chrono::hours hours;
        //! @brief The constructor.
        inline Timer() {};
        //! @brief The destrcutor.
        inline ~Timer() {};
        //! @brief Gets the ellapsed time.
        template <typename Dur> inline double get(const bool reset) noexcept
        {
            static_assert(std::chrono::__is_duration<Dur>::value, "Kiwi::Dsp::Timer : The template must be a duration.");
            const std::chrono::high_resolution_clock::time_point next = std::chrono::high_resolution_clock::now();
            const double time = std::chrono::duration_cast<Dur>(next - m_start).count();
            if(reset)
            {
                m_start = next;
            }
            return time;
        }
        //! @brief Starts the timer.
        inline void start() noexcept {m_start = std::chrono::high_resolution_clock::now();}
    private:
        std::chrono::high_resolution_clock::time_point  m_start;
    };
}
}

#endif // KIWI_DSP_MISC_H_INCLUDED
