/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#include <KiwiDsp/KiwiDsp_Processor.hpp>

// ==================================================================================== //
//                                   SIMPLE PROCESSORS                                  //
// ==================================================================================== //

using namespace kiwi;
using namespace dsp;

// ==================================================================================== //
//                                          OSC                                         //
// ==================================================================================== //

//This produces a floating point linear ramp between 0 and 1 at the desired frequency
class Phasor : public Processor
{
public:
    Phasor(sample_t frequency) noexcept : Processor(0ul, 1ul), m_freq(frequency) {}
    ~Phasor() = default;
private:
    bool prepare(Infos& infos)
    {
        m_sr = static_cast<sample_t>(infos.getSampleRate());
        return true;
    }
    
    void perform(Buffer const& /*input*/, Buffer& output) noexcept final
    {
        Signal& sig = output[0ul];
        sample_t* sig_data = sig.data();
        size_t framesize = sig.size();
        while(framesize--)
        {
            *sig_data++ = m_phase;
            if(m_phase >= 1.0 ) m_phase -= 1.0;
            m_phase += (1./(m_sr/(m_freq)));
        }
    }
    
private: // members
    
    sample_t m_sr;
    sample_t m_freq;
    sample_t m_phase;
};

// ==================================================================================== //
//                                          OSC                                         //
// ==================================================================================== //

//This produces a floating point cosine at the desired frequency
class Osc : public Processor
{
public:
    Osc(sample_t frequency) noexcept : Processor(0ul, 1ul), m_freq(frequency)
    {
        ;
    }
    
    ~Osc() = default;
    
    void setFrequency(sample_t const& freq) noexcept
    {
        m_freq = freq;
        m_phase_inc = (freq != 0.) ? (1./(m_sr/(m_freq))) : 0.;
    }
    
private:
    
    bool prepare(Infos& infos)
    {
        m_sr = static_cast<sample_t>(infos.getSampleRate());
        setFrequency(m_freq);
        return true;
    }
    
    void perform(Buffer const& /*input*/, Buffer& output) noexcept final
    {
        Signal& sig = output[0ul];
        sample_t* sig_data = sig.data();
        size_t framesize = sig.size();
        while(framesize--)
        {
            *sig_data++ = std::cos(m_phase * 6.283185307179586476925286766559);
            if(m_phase >= 1.) m_phase -= 1.;
            m_phase += m_phase_inc;
        }
    }
    
private: // members
    
    sample_t m_sr;
    sample_t m_freq;
    sample_t m_phase;
    sample_t m_phase_inc;
};

// ==================================================================================== //
//                                      *~ SIGNAL                                       //
// ==================================================================================== //

class MultiplySignal : public Processor
{
public:
    MultiplySignal() noexcept : Processor(2ul, 1ul) {}
    ~MultiplySignal()  noexcept {}
private:
    
    void perform(Buffer const& input, Buffer& output) noexcept final
    {
        // this should be inplace
        Samples<sample_t>::mul(input[1ul].size(), input[1ul].data(), output[0ul].data());
    }
};

// ==================================================================================== //
//                                          DAC (fake)                                  //
// ==================================================================================== //

//This is a simple class to transmit signal out of the chain.
class Dac : public Processor
{
public:
    Dac(Buffer& buffer) noexcept : Processor(2ul, 0ul), m_buffer(buffer) {}
    ~Dac() = default;
private:
    
    void perform(Buffer const& input, Buffer&) noexcept final
    {
        Samples<sample_t>::copy(input[0].size(), input[0].data(), m_buffer[0].data());
        Samples<sample_t>::copy(input[1].size(), input[1].data(), m_buffer[1].data());
    }
    
private: // members
    
    Buffer& m_buffer;
};
