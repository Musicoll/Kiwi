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

#ifndef KIWI_DSP_MISC_HPP_INCLUDED
#define KIWI_DSP_MISC_HPP_INCLUDED

#include "KiwiDsp_def.hpp"

namespace kiwi
{
    namespace dsp
    {
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

#endif // KIWI_DSP_MISC_HPP_INCLUDED
