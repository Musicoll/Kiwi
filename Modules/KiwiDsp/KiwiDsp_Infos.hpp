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

#ifndef KIWI_DSP_INFOS_HPP_INCLUDED
#define KIWI_DSP_INFOS_HPP_INCLUDED

#include "KiwiDsp_Signal.hpp"

namespace kiwi
{
    namespace dsp
    {
        class Infos
        {
        public:
            //! @brief The destructor.
            //! @details Frees the content of the Infos object if needed.
            ~Infos();
            
            //! @brief Gets the sample rate of the DSP.
            //! @details Gets the sample rate of the DSP used during the perform method of the
            //! Processor object.
            //! @return The vector size.
            //! @see getSampleRate()
            size_t getSampleRate() const noexcept;
            
            //! @brief Gets the vector size of the samples vectors.
            //! @details Gets the vectors size of the samples vectors used during the perform
            //! method of the Processor object.
            //! @return The vector size.
            //! @see getSampleRate()
            size_t getVectorSize() const noexcept;
            
            //! @brief Checks if an inlet is connected to any other Processor object.
            //! @details This method can be used to check if an inlet is connected to any other
            //! Processor object and to adapt the behavior of the Processor.
            //! @return true if the outlet is connected, otherwise it returns false.
            //! @see isOutputConnected()
            bool isInputConnected(const size_t index) const noexcept;
            
            //! @brief Checks if an outlet is connected to any other Processor object.
            //! @details This method can be used to check if an outlet is connected to any other
            //! Processor object and to adapt the behavior of the Processor.
            //! @return true if the outlet is connected, otherwise it returns false.
            //! @see isInputConnected()
            bool isOutputConnected(const size_t index) const noexcept;
            
            //! @brief Checks if the processing should use inplace buffers.
            //! @details This method can be used to check if the buffers owns the same matrices of
            //! samples during the call of the perform method of the Processor object.
            //! @return true if the processing should be inplace, otherwise false.
            //! @see shouldBeInplace() and isAligned()
            bool isInplace() const noexcept;
            
            //! @brief Checks if the processing should use aligned buffers.
            //! @details This method can be used to check if the buffers owns the aligned matrices
            //! of samples during the call of the perform method of the Processor object.
            //! @return true if the processing should be inplace, otherwise false.
            //! @see shouldBeAligned() and isInplace()
            bool isAligned() const noexcept;
            
            //! @brief Sets if the processing should use inplace buffers.
            //! @details This method enables or disables if the buffers owns the same matrices of
            //! samples during the call of the perform method of the Processor object. By default
            //! the buffers are inplace if the number of inputs of the Processor object is
            //! superior or equal to its number of outputs.
            //! @param state True if the processing should be inplace, otherwise false.
            //! @see shouldBeAligned()
            void shouldBeInplace(const bool state);
            
            //! @brief Sets if the processing should use aligned buffers.
            //! @details This method enables or disables if the buffers owns the aligned matrices
            //! of samples during the call of the perform method of the Processor object. By
            //! default the buffers are not necessarily aligned. The aligned option should be
            //! activated only when the processing really needs it to avoid unnecessary copy of
            //! matrices.
            //! @param state True if the processing should be inplace, otherwise false.
            //! @see shouldBeInplace()
            void shouldBeAligned(const bool state);
            
        private:
            
            //! @brief The constructor.
            //! @details Allocates a Infos object that describes the behavior of the DSP of a
            //! Processor object.
            //! @param samplerate The sample rate.
            //! @param vectorsize The vector size.
            //! @param inputs     The inputs connection states.
            //! @param outputs    The outputs connection states.
            Infos(const size_t samplerate, const size_t vectorsize,
                  std::vector< bool >&& inputs, std::vector< bool >&& outputs);
            
            size_t const              m_sample_rate;
            size_t const              m_vector_size;
            std::vector< bool > const m_inputs;
            std::vector< bool > const m_outputs;
            bool                      m_inplace;
            bool                      m_aligned;
            friend class Chain;
        };
    }
}

#endif // KIWI_DSP_INFOS_HPP_INCLUDED
