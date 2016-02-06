/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "KiwiDspProcessor.hpp"

namespace kiwi
{
    namespace dsp
    {
        Processor::Processor() noexcept :
        m_ninputs(0ul), m_noutputs(0ul), m_inplace(false), m_valid(false), m_running(false)
        {
            
        }
        
        Processor::~Processor() noexcept
        {
            
        }
        
        void Processor::setNumberOfInlets(const size_t nins) noexcept
        {
            assert("Kiwi::Dsp::Processor : The setNumberOfInlets method shouldn't be called when the DSP is running" && !m_running);
            m_ninputs = nins;
        }
        
        void Processor::setNumberOfOutlets(const size_t nouts) noexcept
        {
            assert("Kiwi::Dsp::Processor : The setNumberOfOutlets method shouldn't be called when the DSP is running" && !m_running);
            m_noutputs = nouts;
        }
        
        void Processor::setInplace(const bool status) noexcept
        {
            assert("Kiwi::Dsp::Processor : The setInplace method shouldn't be called when the DSP is running" && !m_running);
            m_inplace = status;
        }
        
        void Processor::shouldPerform(const bool status) noexcept
        {
            assert("Kiwi::Dsp::Processor : The shouldPerform method shouldn't be called when the DSP is running" && !m_running);
            m_valid  = status;
        }
    }
}
