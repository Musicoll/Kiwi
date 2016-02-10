//
//  KiwiDspInfos.cpp
//  KiwiTestDsp
//
//  Created by Guillot Pierre on 09/02/2016.
//  Copyright © 2016 CICM. All rights reserved.
//

#include "KiwiDspInfos.hpp"

namespace kiwi
{
    namespace dsp
    {
        Infos::Infos(const size_t samplerate, const size_t vectorsize,
                     std::vector< bool > const& inputs, std::vector< bool > const& outputs) :
        m_sample_rate(samplerate), m_vector_size(vectorsize), m_inputs(inputs), m_outputs(outputs)
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
            assert(index < m_inputs.size() && "Kiwi::Dsp::Infos : Index out of range.");
            return m_inputs[index];
        }
        
        bool Infos::isOutputConnected(const size_t index) const noexcept
        {
            assert(index < m_outputs.size() && "Kiwi::Dsp::Infos : Index out of range.");
            return m_outputs[index];
        }
    }
}
