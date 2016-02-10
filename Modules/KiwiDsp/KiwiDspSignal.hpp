//
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

#ifndef KIWI_DSP_SIGNAL_H_INCLUDED
#define KIWI_DSP_SIGNAL_H_INCLUDED

#include "KiwiDspMisc.hpp"
#include "KiwiDspSamples.hpp"

namespace kiwi
{
namespace dsp
{
    // ==================================================================================== //
    //                                          BUFFER                                      //
    // ==================================================================================== //
    //! @brief A class that wraps a matrix of samples.
    //! @details The class is a wrapper for a matrix of samples that offers optimized
    //! @details operations.
    class Buffer
    {
    public:
        //! @brief The default constructor.
        //! @details Allocates and initializes an empty Signal object.
        Buffer() noexcept;
        
        //! @brief The filled constructor.
        //! @details Allocates a Signal object of a specific size and initializes it with a
        //! @details default value.
        //! @param size The number of samples.
        //! @param val  The default value of the signal.
        Buffer(const size_t size, const sample_t val = sample_t(0.));
        
        //! @brief The copy constructor.
        //! @details Allocates a Signal object that is a copy of another.
        //! @param other The other Signal object.
        Buffer(Buffer const& other);
        
        //! @brief The move constructor.
        //! @details Moves the content of a Signal object to a new one.
        //! @param other The other Signal object.
        Buffer(Buffer&& other) noexcept;
        
        //! @brief The destructor.
        //! @details Frees the content of the Signal object if needed.
        ~Buffer();
        
        //! @brief Gets the current sample rate of the chain.
        inline size_t getSampleRate() const noexcept {return m_sample_rate;}
        
        //! @brief Gets the current vector size of the chain.
        inline size_t getVectorSize() const noexcept {return m_vector_size;}
        
        //! @brief Gets the vector of samples of an input.
        inline sample_t const* getInputSamples(const size_t index) const noexcept
        {
            assert(index < m_ninputs && "Kiwi::Dsp::Buffer : Index out of range.");
            return m_inputs[index];
        }
        
        //! @brief Gets the vector of samples of the inputs.
        inline sample_t const* const* getInputsSamples() const noexcept {return m_inputs;}
        
        //! @brief Gets the vector of samples of an output.
        inline sample_t* getOutputSamples(const size_t index) const noexcept
        {
            assert(index < m_noutputs && "Kiwi::Dsp::Buffer : Index out of range.");
            return m_outputs[index];
        }
        
        //! @brief Gets the vector of samples of the inputs.
        inline sample_t* const* getOutputsSamples() const noexcept {return m_outputs;}
        
    private:
        const size_t            m_sample_rate;
        const size_t            m_vector_size;
        const size_t            m_ninputs;
        const size_t            m_noutputs;
        sample_t const* const*  m_inputs;
        sample_t* const*        m_outputs;
    };
    
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
        //! @details Allocates and initializes an empty Signal object.
        Signal() noexcept;
        
        //! @brief The filled constructor.
        //! @details Allocates a Signal object of a specific size and initializes it with a
        //! @details default value.
        //! @param size The number of samples.
        //! @param val  The default value of the signal.
        Signal(const size_t size, const sample_t val = sample_t(0.));
        
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
        sample_t*   m_samples;
        size_t      m_size;
        bool        m_owner;
        bool        m_borrowed;
    };
}
}

#endif // KIWI_DSP_SIGNAL_H_INCLUDED
