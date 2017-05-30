/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include <memory>

#include "KiwiDsp_Def.h"

namespace kiwi
{
    namespace dsp
    {        
        // ==================================================================================== //
        //                                          SIGNAL                                      //
        // ==================================================================================== //
        //! @brief A class that wraps a vector of sample_t.
        //! @details The class is a wrapper for a vector of sample_t values that offers optimized
        //! operations. The class also offers static methods to perform these operations
        //! with other Signal objects.
        class Signal
        {
        public: // methods
            
            typedef std::shared_ptr<Signal>         sPtr;
            typedef std::shared_ptr<const Signal>   scPtr;
            typedef std::unique_ptr<Signal>         uPtr;
            
            //! @brief Constructs and fill a Signal object.
            //! @details Allocates a Signal object of a specific size and initializes it with a default value.
            //! @param size The number of samples.
            //! @param val  The default value of the signal.
            Signal(const size_t size, const sample_t val = sample_t(0.));
            
            //! @brief Move constructor.
            //! @details Moves the content of a Signal object to a new one.
            //! The size of the other signal is set to 0 and its data are reset to nullptr.
            //! @param other The other Signal object.
            Signal(Signal&& other) noexcept;
            
            //! @brief Move assignment operator
            //! @details Moves the content of a Signal to this signal.
            //! The size of the other signal is set to 0 and its data are reset to nullptr.
            //! @param other The other Signal object.
            Signal& operator=(Signal&& other) noexcept;
            
            //! @brief The destructor.
            //! @details Frees the content of the Signal object if needed.
            ~Signal();
            
            //! @brief Gets the number of samples that the Signal object currently holds.
            size_t size() const noexcept;
            
            //! @brief Returns a read pointer to the samples data.
            sample_t const* data() const noexcept;
            
            //! @brief Returns a write pointer to the samples data.
            sample_t* data() noexcept;
            
            //! @brief Gets a read sample_t for a given index.
            //! @param index of the sample requested. Must be lower than signal size.
            sample_t const& operator[](const size_t index) const;
            
            //! @brief Gets a write sample_t for a given index.
            //! @param index of the sample requested. Must be lower than signal size.
            sample_t& operator[](const size_t index);
            
            //! @brief Fill this Signal with a new value.
            //! @param value The value for every signal sample.
            void fill(sample_t const& value) noexcept;
            
            //! @brief Copies the samples of another signal into it.
            void copy(Signal const& other_signal) noexcept;
            
            //! @brief Adds a Signal to this one.
            void add(Signal const& other_signal) noexcept;
            
            //! @brief Adds two Signal together and returns the resulting Signal.
            static void add(Signal const& signal_1, Signal const& signal_2, Signal& result);
            
        private: // members
            
            size_t      m_size;
            sample_t*   m_samples;
            
        private: // deleted methods
            
            Signal() = delete;
            Signal(Signal const& other) = delete;
            Signal& operator=(Signal& other) = delete;
        };
        
        // ==================================================================================== //
        //                                          BUFFER                                      //
        // ==================================================================================== //
        
        //! @brief A class that wraps a vector of Signal objects.
        //! @details The class is a wrapper for a vector of Signal objects that offers optimized
        //! operations. It can either own its own memory or share it with the outside.
        class Buffer
        {
        public: // methods
            
            //! @brief Default constructor. Creates an empty buffer.
            //! @details SetChannels can the be used to feed signals
            Buffer();
            
            //! @brief A constructor that aggregates signals
            //! @details Beware that the buffer now share the ownership with caller.
            //! Clean has to be called afterwards to release memory.
            //! signals must have the same size to be inserted in the Buffer.
            //! @param signals A vector of signals. Shall have same vectorsize.
            Buffer(std::vector<Signal::sPtr> signals);
            
            //! @brief The filled constructor.
            //! @details Allocates a Buffer object with a specific number of channels and number
            //! of samples per channels. The matrix of sample_t can also be initialized
            //! with a default value.
            //! @param nchannels    The number of channels of the buffer.
            //! @param nsamples     The number of samples per channels.
            //! @param val          The default value of the signal.
            Buffer(const size_t nchannels, const size_t nsamples, const sample_t val = 0.);
            
            //! @brief The move constructor.
            //! @details Moves the content of a Buffer object to a new one.
            //! The other Buffer will be reset to an empty state.
            //! @param other The Buffer object you want to move.
            Buffer(Buffer&& other) noexcept;
            
            //! @brief The move assignment operator.
            //! @details Moves the content of a Buffer object to a new one.
            //! The other Buffer will be reset to an empty state.
            //! @param other The Buffer object you want to move.
            Buffer& operator=(Buffer&& other) noexcept;
            
            //! @brief The destructor.
            //! @details Frees the content of the Buffer object if needed.
            ~Buffer();
            
            //! @brief Resets the entire buffer with new signals fed to him.
            //! @details If the buffer was not empty it will be cleared.
            //! Channels will then share ownership with caller.
            //! Clean has to be called to release them.
            //! signals must have the same size to be inserted in the Buffer.
            //! @param signals A vector of aggregated signals. Shall have same vectorsize.
            void setChannels(std::vector<Signal::sPtr> signals);
            
            //! @brief Clears buffers data. The resulting buffer will be empty.
            //! @brief If signal ownership was shared this method will release signal.
            void clear();
            
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
            
            Buffer(Buffer const& other) = delete;
            Buffer& operator=(Buffer const& other) = delete;
        };
    }
}
