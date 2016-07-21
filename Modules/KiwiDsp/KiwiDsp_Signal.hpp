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

#ifndef KIWI_DSP_SIGNAL_HPP_INCLUDED
#define KIWI_DSP_SIGNAL_HPP_INCLUDED

#include "KiwiDsp_Misc.hpp"
#include "KiwiDsp_Samples.hpp"

namespace kiwi
{
    namespace dsp
    {
        class Signal;
        
        // ==================================================================================== //
        //                                          BUFFER                                      //
        // ==================================================================================== //
        //! @brief A class that wraps a matrix of sample_t values.
        //! @details The class is a wrapper for a matrix of sample_t values that offers optimized
        //! @details operations.
        class Buffer
        {
        public:
            //! @brief The default constructor.
            //! @details Allocates and initializes an empty Buffer object.
            Buffer() noexcept;
            
            //! @brief The filled constructor.
            //! @details Allocates a Buffer object with a specific number of channels and number
            //! @details of samples per channels. The matrix of sample_t can also be initialized
            //! @details with a default value.
            //! @param nchannels    The number of channels of the buffer.
            //! @param nsamples     The number of samples per channels.
            //! @param val          The default value of the signal.
            Buffer(const size_t nchannels, const size_t nsamples, const sample_t val = sample_t(0.));
            
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
            
            //! @brief Gets the sample rate of the Buffer object.
            inline size_t getSampleRate() const noexcept {return m_sample_rate;}
            
            //! @brief Gets the vector size of the Buffer object.
            inline size_t getVectorSize() const noexcept {return m_vector_size;}
            
            //! @brief Gets the number of channels of the Buffer object.
            inline size_t getNumberOfChannels() const noexcept {return m_nchannels;}
            
            //! @brief Gets the sample_t pointer of a channel.
            inline sample_t const* operator[](const size_t /*index*/) const noexcept {return nullptr;}
            
            //! @brief Gets the sample_t pointer of a channel.
            inline sample_t* operator[](const size_t /*index*/) noexcept {return nullptr;}
            
        private:
            size_t              m_sample_rate;
            size_t              m_vector_size;
            size_t              m_nchannels;
            std::vector<Signal> m_signals;
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
        public: // methods
            
            //! @brief Constructor.
            //! @details Allocates and initializes an empty Signal object.
            Signal() noexcept;
            
            //! @brief Constructs and fill a Signal object.
            //! @details Allocates a Signal object of a specific size and initializes it with a default value.
            //! @param size The number of samples.
            //! @param val  The default value of the signal.
            Signal(const size_t size, const sample_t val = sample_t(0.));
            
            //! @brief Copy constructor.
            //! @details Allocates a Signal object that is a copy of another.
            //! @param other The other Signal object.
            Signal(Signal const& other);
            
            //! @brief Move constructor.
            //! @details Moves the content of a Signal object to a new one.
            //! @param other The other Signal object.
            Signal(Signal&& other) noexcept;
            
            //! @brief The destructor.
            //! @details Frees the content of the Signal object if needed.
            ~Signal();
            
        private: // members
            
            sample_t*   m_samples;
            size_t      m_size;
            bool        m_owner;
            //bool        m_borrowed;
        };
    }
}

#endif // KIWI_DSP_SIGNAL_HPP_INCLUDED
