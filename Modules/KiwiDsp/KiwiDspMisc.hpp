//
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

#ifndef KIWI_DSP_MISC_H_INCLUDED
#define KIWI_DSP_MISC_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <exception>
#include <vector>
#include <set>
#include <memory>
#include <atomic>
#include <mutex>

#ifndef __APPLE__
#include "malloc.h"
#endif

namespace kiwi
{
namespace dsp
{
#ifdef KIWI_DSP_FLOAT
    typedef float sample_t;
#elif KIWI_DSP_DOUBLE
    typedef double sample_t;
#endif
    // ==================================================================================== //
    //                                          ERROR                                       //
    // ==================================================================================== //
    //! @brief The exception.
    //! @details The class defines the std::exception objects that can be trown during of by
    //! @details DSP objects during the compilation of a Chain object.
    class Error : public std::runtime_error
    {
    public:
        //! @brief The std::string constructor.
        //! @param message The message of the error
        explicit Error(const std::string& message) :
        std::runtime_error(std::string("kiwi::dsp : ") + message) {}
        //! @brief The const char* constructor.
        //! @param message The message of the error
        explicit Error(const char* message) :
        std::runtime_error(std::string("kiwi::dsp : ") + std::string(message)) {}
        //! @brief The destructor.
        virtual inline ~Error() noexcept {}
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
        template < typename Dur > inline double get(const bool reset) noexcept
        {
            static_assert(std::chrono::__is_duration< Dur >::value, "Kiwi::Dsp::Timer : The template must be a duration.");
            const std::chrono::high_resolution_clock::time_point next = std::chrono::high_resolution_clock::now();
            const double time = std::chrono::duration_cast< Dur >(next - m_start).count();
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
