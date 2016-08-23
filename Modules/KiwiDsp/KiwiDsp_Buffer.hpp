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

#ifndef KIWI_DSP_BUFFER_HPP_INCLUDED
#define KIWI_DSP_BUFFER_HPP_INCLUDED

#include "KiwiDsp_Signal.hpp"

namespace kiwi
{
    namespace dsp
    {
        class Node;
        
        // ==================================================================================== //
        //                                          BUFFER                                      //
        // ==================================================================================== //
        
        //! @brief A class that wraps a matrix of sample_t values.
        //! @details The class is a wrapper for a matrix of sample_t values that offers optimized
        //! operations.
        class Buffer
        {
        public: // methods
            
            //! @brief A constructor that aggregates signals
            //! @details Beware that the buffer now share the ownership with caller.
            //! @param signals A list of aggregated signals. Shall have same vectorsize.
            Buffer(std::vector<Signal::sPtr> signals);
            
            //! @brief The filled constructor.
            //! @details Allocates a Buffer object with a specific number of channels and number
            //! of samples per channels. The matrix of sample_t can also be initialized
            //! with a default value.
            //! @param nchannels    The number of channels of the buffer.
            //! @param nsamples     The number of samples per channels.
            //! @param val          The default value of the signal.
            Buffer(const size_t nchannels, const size_t nsamples, const sample_t val = 0.);
            
            //! @brief The deep copy constructor.
            //! @details Allocates a Buffer object that is a copy of another.
            //! @param other The other Buffer object.
            Buffer(Buffer const& other);
            
            //! @brief The move constructor.
            //! @details Moves the content of a Buffer object to a new one.
            //! The other Buffer will be reset to an empty state.
            //! @param other The Buffer object you want to move.
            Buffer(Buffer&& other) noexcept;
            
            //! @brief The destructor.
            //! @details Frees the content of the Buffer object if needed.
            ~Buffer();
            
            //! @brief Gets the vector size of the Buffer object.
            size_t getVectorSize() const noexcept;
            
            //! @brief Gets the number of channels of the Buffer object.
            size_t getNumberOfChannels() const noexcept;
            
            //! @brief Returns true if the Buffer object has no Signal.
            bool empty() const noexcept;
            
            //! @brief Gets the Signal object for a given channel.
            Signal const& operator[](const size_t index) const;
            
            //! @brief Gets the Signal object for a given channel.
            Signal& operator[](const size_t index);
            
        private: // members
            
            size_t                      m_vectorsize;
            std::vector<Signal::sPtr>   m_signals;
            
        private: // deleted methods
            Buffer() = delete;
        };
    }
}

#endif // KIWI_DSP_BUFFER_HPP_INCLUDED
