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

#include "KiwiDsp_Infos.hpp"

namespace kiwi
{
    namespace dsp
    {
        Infos::Infos(const size_t samplerate, const size_t vectorsize,
                     std::vector< bool >&& inputs, std::vector< bool >&& outputs) :
        m_sample_rate(samplerate), m_vector_size(vectorsize),
        m_inputs(std::forward< std::vector< bool > >(inputs)),
        m_outputs(std::forward< std::vector< bool > >(outputs)),
        m_inplace(true), m_aligned(false)
        {
            
        }
        
        Infos::~Infos()
        {
            
        }
        
        size_t Infos::getSampleRate() const noexcept
        {
            return m_sample_rate;
        }
        
        size_t Infos::getVectorSize() const noexcept
        {
            return m_vector_size;
        }
        
        bool Infos::isInputConnected(const size_t index) const noexcept
        {
            assert(index < static_cast< size_t >(m_inputs.size()) && "Index out of range");
            return m_inputs[index];
        }
        
        bool Infos::isOutputConnected(const size_t index) const noexcept
        {
            assert(index < static_cast< size_t >(m_outputs.size()) && "Index out of range");
            return m_outputs[index];
        }
        
        bool Infos::isInplace() const noexcept
        {
            return m_inplace;
        }
        
        bool Infos::isAligned() const noexcept
        {
            return m_aligned;
        }
        
        void Infos::shouldBeInplace(const bool state)
        {
            m_inplace = state;
        }
        
        void Infos::shouldBeAligned(const bool state)
        {
            m_aligned = state;
        }
    }
}
