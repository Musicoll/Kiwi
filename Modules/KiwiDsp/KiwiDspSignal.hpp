/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef KIWI_DSP_SIGNAL_H_INCLUDED
#define KIWI_DSP_SIGNAL_H_INCLUDED

#include "KiwiDspMisc.hpp"
#include "KiwiDspSamples.hpp"

namespace kiwi
{
namespace dsp
{
#ifdef KIWI_DSP_FLOAT
    typedef float sample;
#else
    typedef double sample;
#endif
    // ==================================================================================== //
    //                                          SIGNAL                                      //
    // ==================================================================================== //
    //! @brief A class that wraps a vector of samples.
    //! @details The class is a wrapper for a vector of samples that offers optimized
    //! @details operations. The class also offers static method to perform this operations
    //! @details with vectors of samples.
    class Signal
    {
    public:
        //! @brief The default constructor.
        //! @details Allocates and initializes an empty Signal.
        Signal() noexcept;
        
        //! @brief The filled constructor.
        //! @details Allocates a Signal object of a specific size and initializes it with a
        //! @details default value.
        //! @param size The number of samples.
        //! @param val  The default value of the signal.
        Signal(const size_t size, const sample val = sample(0.));
        
        //! @brief The copy constructor.
        //! @details Allocates a Signal object that is a copy of another.
        //! @param other The other Signal object.
        Signal(Signal const& other);
        
        //! @brief The move constructor.
        //! @details Moves the content of a Signal object to a new one.
        //! @param other The other Signal object.
        Signal(Signal&& other) noexcept;
        
        //! @brief The destructor.
        //! @details Frees the content of the Signal object if needed.
        ~Signal();
        
    private:
        sample* m_samples;
        size_t  m_size;
        bool    m_owner;
    };
}
}

#endif // KIWI_DSP_SIGNAL_H_INCLUDED
