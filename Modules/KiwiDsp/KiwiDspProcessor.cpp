//
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

#include "KiwiDspProcessor.hpp"

namespace kiwi
{
    namespace dsp
    {
        Processor::Processor(const size_t ninputs, const size_t noutputs) noexcept :
        m_ninputs(ninputs), m_noutputs(noutputs), m_used(false)
        {
            
        }
        
        Processor::~Processor() noexcept
        {
            
        }
    }
}
