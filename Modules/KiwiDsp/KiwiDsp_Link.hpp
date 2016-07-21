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

#ifndef KIWI_DSP_LINK_HPP_INCLUDED
#define KIWI_DSP_LINK_HPP_INCLUDED

#include "KiwiDsp_Processor.hpp"

namespace kiwi
{
    namespace dsp
    {
        // ==================================================================================== //
        //                                          LINK                                        //
        // ==================================================================================== //
        //! @brief The class owns the attributes of a connection between two Processor objects.
        //! @details The class acts like a simple structure with constant attributes to connect
        //! two Processor objects in a Chain object.
        //! @see Chain and Processor
        class Link
        {
        public:
            //! @brief The constructor.
            //! @details Allocates and initializes the Link object.
            //! @param fromProcessor    The output Processor object.
            //! @param fromIndex        The index of the output.
            //! @param toProcessor      The input Processor object.
            //! @param toIndex          The index of the input.
            Link(Processor const& fromProcessor, size_t const fromIndex,
                 Processor const& toProcessor, size_t const toIndex) noexcept :
            m_from_processor(fromProcessor), m_from_index(fromIndex),
            m_to_processor(toProcessor), m_to_index(toIndex)
            {
                assert(m_from_processor.getNumberOfOutputs() > fromIndex &&
                       m_to_processor.getNumberOfInputs() > toIndex &&
                       "A link is invalid.");
            }
            
            //! @brief The destructor.
            ~Link() noexcept {};
            
            //! @brief Gets the output Processor object.
            inline Processor const& getOutputProcessor() const noexcept {return m_from_processor;}
            
            //! @brief Gets the input Processor object.
            inline Processor const& getInputProcessor() const noexcept {return m_to_processor;}
            
            //! @brief Gets the index of the output Processor object.
            inline size_t getOutputIndex() const noexcept {return m_from_index;}
            
            //! @brief Gets the index of the input Processor object.
            inline size_t getInputIndex() const noexcept {return m_to_index;}
        private:
            Processor const&    m_from_processor;
            size_t const        m_from_index;
            Processor const&    m_to_processor;
            size_t const        m_to_index;
        };
    }
}

#endif // KIWI_DSP_LINK_HPP_INCLUDED
