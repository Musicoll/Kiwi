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

#ifndef KIWI_DSP_PROCESSOR_HPP_INCLUDED
#define KIWI_DSP_PROCESSOR_HPP_INCLUDED

#include "KiwiDsp_Infos.hpp"

namespace kiwi
{
    namespace dsp
    {
        class Chain;
        // ==================================================================================== //
        //                                      PROCESSOR                                       //
        // ==================================================================================== //
        //! @brief The pure virtual class that processes digital signal in a Chain object.
        //! @details The class is pure virtual and allows to implement digital signal processing.
        //! You should implement the pure virtual methods prepare, perform and release.
        //! @see Buffer and Infos
        //! @todo see if the processor really need m_used...
        class Processor
        {
        public: // methods
            
            //! @brief The constructor.
            //! @details Allocates and initializes a default Processor object.
            //! @param ninputs The number of inputs.
            //! @param noutputs The number of outputs.
            Processor(const size_t ninputs, const size_t noutputs) noexcept :
            m_ninputs(ninputs), m_noutputs(noutputs), m_used(false) {}
            
            //! @brief The destructor.
            virtual ~Processor() = default;
            
            //! @brief Gets the current number of inputs.
            //! @return The number of inputs of the Processor object.
            //! @see getNumberOfOutputs()
            inline size_t getNumberOfInputs() const noexcept {return m_ninputs;}
            
            //! @brief Gets the current number of outputs.
            //! @return The number of outputs of the Processor object.
            //! @see getNumberOfInputs()
            inline size_t getNumberOfOutputs() const noexcept {return m_noutputs;}
            
        private:
            //! @brief Prepares everything for the perform method.
            //! @details This is a pure virtual method. You should use this method to check the
            //! vector size, the sample rate, the connected inputs and outputs and to allocate
            //! memory if needed. The method should return true if the perform method of the
            //! Processor object can be called, otherwise it should return false.
            //! @param infos The DSP informations.
            //! @return true if the perform method can be called, otherwise false.
            //! @see perform() and release()
            virtual bool prepare(Infos& infos) = 0;
            
            //! @brief Performs the digital signal processing.
            //! @details This is a pure virtual method. You should use this method to perform the
            //! digital signal processing but you should avoid to allocate memory or do it
            //! asynchronously.
            //! @param input    The input Buffer object.
            //! @param output   The output Buffer object.
            //! @see prepare() and release()
            virtual void perform(Buffer const& input, Buffer& output) noexcept = 0;
            
            //! @brief Releases everything after the digital signal processing.
            //! @details You can use this method to free the memory allocated during the call of
            //! the prepare method for example.
            //! @see prepare() and perform()
            virtual void release() {};
            
            size_t const        m_ninputs;
            size_t const        m_noutputs;
            std::atomic< bool > m_used;
            friend class Chain;
        };
    }
}

#endif // KIWI_DSP_PROCESSOR_HPP_INCLUDED
