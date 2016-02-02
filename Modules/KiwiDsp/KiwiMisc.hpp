/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef __DEF_OSP_MISC__
#define __DEF_OSP_MISC__

#if (__cplusplus <= 199711L)
#define noexcept
#define nullptr NULL
#define constexpr
#endif

#include <chrono>
#include <exception>

namespace osp
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
        virtual const char* what() const noexcept override {return "Osp::Error : problem occurs !";}
    };
    
    // ==================================================================================== //
    //                                          TIMER                                       //
    // ==================================================================================== //
    //! @brief The timer.
    //! @details The class uses high resolution clocks to estimate the time that CPU used for
    //! @details a set of processes. Given that the class does not use mutex, you should
    //! @details always ensure that start() is called before end().
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
        //! @brief Gets the lastest time computed.
        template < typename T > double get() const noexcept {
            static_assert(std::chrono::__is_duration< T >::value || 1, "Osp::Timer : The template must be a duration.");
            return std::chrono::duration_cast< T >(m_result - m_start).count();}
        //! @brief Starts the chronometer.
        inline void start() noexcept {m_start = std::chrono::high_resolution_clock::now();}
        //! @brief Stops the chronometer.
        inline void stop() noexcept {m_result = std::chrono::high_resolution_clock::now();}
    private:
        std::chrono::high_resolution_clock::time_point  m_start;
        std::chrono::high_resolution_clock::time_point  m_result;
    };
}

#if (__cplusplus <= 199711L)
#undef noexcept
#undef nullptr NULL
#undef constexpr
#endif

#endif
