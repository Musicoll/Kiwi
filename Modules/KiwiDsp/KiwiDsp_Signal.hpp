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

#include "KiwiDsp_misc.hpp"
#include "KiwiDsp_Samples.hpp"

namespace kiwi
{
    namespace dsp
    {        
        // ==================================================================================== //
        //                                          SIGNAL                                      //
        // ==================================================================================== //
        //! @brief A class that wraps a vector of samples.
        //! @details The class is a wrapper for a vector of samples that offers optimized
        //! operations. The class also offers static method to perform this operations
        //! with vectors of samples.
        class Signal
        {
        public: // methods
            
            typedef std::shared_ptr<Signal>         sPtr;
            typedef std::shared_ptr<const Signal>   scPtr;
            typedef std::unique_ptr<Signal>         uPtr;
            
            //! @brief Default ctor.
            //! @details Creates an empty Signal object.
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
            //! The size of the other signal is set to 0 and its data are reset to nullptr.
            //! @param other The other Signal object.
            Signal(Signal&& other) noexcept;
            
            //! @brief Signal assignment operator.
            //! @details Copies a Signal object via the "copy and swap" idiom.
            Signal& operator=(Signal other);
            
            //! @brief The destructor.
            //! @details Frees the content of the Signal object if needed.
            ~Signal();
            
            //! @brief Exchanges the contents of two Signal objects.
            friend void swap(Signal& first, Signal& second);
            
            //! @brief Gets the number of samples the Signal object currently holds.
            size_t size() const noexcept;
            
            //! @brief Returns true if the Signal object contains no data.
            bool empty() const noexcept;
            
            //! @brief Returns a pointer to the samples data.
            sample_t const* data() const noexcept;
            
            //! @brief Returns a pointer to the samples data.
            sample_t* data() noexcept;
            
            //! @brief Gets the sample_t for a given index.
            sample_t const& operator[](const size_t index) const;
            
            //! @brief Gets the sample_t for a given index.
            sample_t& operator[](const size_t index);
            
            //! @brief Clears this signal.
            //! @details Sets all values to zero.
            void clear() noexcept;
            
            //! @brief Fill this Signal with a new value.
            void fillWith(sample_t const& value) noexcept;
            
            //! @brief Copies the samples of another signal into it.
            void copy(Signal const& other_signal) noexcept;
            
            //! @brief Adds a Signal to this one.
            void add(Signal const& other_signal) noexcept;
            
            //! @brief Adds two Signal together and returns the resulting Signal.
            static Signal add(Signal const& signal_1, Signal const& signal_2);
            
        private: // members
            
            bool        m_owner;
            size_t      m_size;
            sample_t*   m_samples;
        };
    }
}

#endif // KIWI_DSP_SIGNAL_HPP_INCLUDED
