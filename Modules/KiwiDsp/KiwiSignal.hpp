/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef __DEF_OSP_SIGNAL__
#define __DEF_OSP_SIGNAL__

#if (__cplusplus <= 199711L)
#define noexcept
#define nullptr NULL
#define constexpr
#endif

#include <cmath>
#include "OspVec.hpp"

namespace osp
{
    // ==================================================================================== //
    //                                          SIGNAL                                      //
    // ==================================================================================== //
    //! @brief The signal.
    //! @details The class is a wrapper for a vector of samples that offers optimized
    //! @details operations. The class also offers static method to perform this operations
    //! @details with vectors of samples.
    template < typename T > class Signal
    {
    public:
        //! @brief The default constructor.
        //! @details Allocates and initializes an empty Signal.
        inline constexpr Signal() noexcept :
        m_samples(nullptr), m_size(0ul), m_owner(true) {}
        
        //! @brief The filled constructor.
        //! @details Allocates a signal of a specific size and initializes the Signal with a
        //! @details default value.
        inline Signal(const size_t size, T val = T(0.)) noexcept :
        m_samples(nullptr), m_size(0ul), m_owner(true)
        {
            ;
        }
        
        inline Signal(const Signal& other) noexcept : m_samples(nullptr), m_size(0ul), m_owner(true)
        {
            ;
        }
        
        inline Signal(Signal&& other) noexcept : m_samples(nullptr), m_size(0ul), m_owner(true)
        {
            other.m_owner = false;
            std::swap(m_samples, other.m_samples);
            std::swap(m_size, other.m_size);
        }
        
        inline ~Signal() noexcept {}
        
        inline size_t alignment() const noexcept {return (size_t)pow(2ul, (size_t)sizeof(T));}
        
        inline bool valid() const noexcept {return !m_samples;}
        
        inline size_t size() const noexcept {return m_size;}
        
        //! Sets all the samples to zero.
        inline void clear() noexcept
        {
           Vec<T>::clear(m_size, m_samples);
        }
        
        //! Fills all the samples with a value.
        /**
         @param value The value.
         */
        inline void fill(T const& value) noexcept
        {
            Vec<T>::fill(m_size, value, m_samples);
        }
        
    private:
        T*      m_samples;
        size_t  m_size;
        bool    m_owner;
    };
}

#endif
