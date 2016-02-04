/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef KIWI_DSP_SIGNAL_H_INCLUDED
#define KIWI_DSP_SIGNAL_H_INCLUDED

#include "KiwiDspSamples.hpp"

namespace kiwi
{
namespace dsp
{
    // ==================================================================================== //
    //                                          SIGNAL                                      //
    // ==================================================================================== //
    //! @brief A class that wraps a vector of samples.
    //! @details The class is a wrapper for a vector of samples that offers optimized
    //! @details operations. The class also offers static method to perform this operations
    //! @details with vectors of samples.
    template < typename Type > class Signal
    {
        static_assert(std::is_floating_point< Type >::value, "The type of signal must be floating point.");
        static_assert(!std::is_const< Type >::value, "The type of signal can't be constant.");
    public:
        //! @brief The default constructor.
        //! @details Allocates and initializes an empty Signal.
        inline constexpr Signal() noexcept :
        m_samples(nullptr),
        m_size(0ul),
        m_owner(true)
        {
            
        }
        
        //! @brief The filled constructor.
        //! @details Allocates a signal of a specific size and initializes the Signal with a
        //! @details default value.
        Signal(const size_t size, Type val = Type(0.)) : m_samples(nullptr), m_size(0ul), m_owner(true)
        {
            //m_samples = Samples<Type>::allocate(
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
        
        ~Signal() noexcept
        {
            if(m_owner)
            {
                 Samples<Type>::release(m_samples);
            }
            m_samples = nullptr;
            m_size    = 0ul;
        }
        
        inline size_t alignment() const noexcept {return Samples<Type>::getAlignment();}
        
        inline bool valid() const noexcept {return !m_samples;}
        
        inline size_t size() const noexcept {return m_size;}
        
        //! Sets all the samples to zero.
        inline void clear() noexcept
        {
            Samples<Type>::clear(m_size, m_samples);
        }
        
        //! Fills all the samples with a value.
        inline void fill(Type const& value) noexcept
        {
            Samples<Type>::fill(m_size, value, m_samples);
        }
    private:
        Type*   m_samples;
        size_t  m_size;
        bool    m_owner;
    };
}
}

#endif // KIWI_DSP_SIGNAL_H_INCLUDED
