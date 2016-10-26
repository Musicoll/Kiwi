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
    
    void prepare(PrepareInfo const& infos) override final
    {
        m_sr = static_cast<sample_t>(infos.sample_rate);
        setPerformCallBack(this, &Phasor::perform);
    }
    
    void perform(Buffer const& /*input*/, Buffer& output) noexcept
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
        //m_phase = 0.;
    }
    
private:
    
    void prepare(PrepareInfo const& infos) override final
    {
        m_sr = static_cast<sample_t>(infos.sample_rate);
        setFrequency(m_freq);
        setPerformCallBack(this, &Osc::perform);
    }
    
    void perform(Buffer const& /*input*/, Buffer& output) noexcept
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
//                                          DAC (fake)                                  //
// ==================================================================================== //

//This is a simple class to transmit signal out of the chain.
class Dac : public Processor
{
public:
    Dac(Buffer& buffer) noexcept : Processor(2ul, 0ul), m_buffer(buffer) {}
    ~Dac() = default;
    
private:
    
    void prepare(PrepareInfo const& infos) override final
    {
        setPerformCallBack(this, &Dac::perform);
    }
    
    void perform(Buffer const& input, Buffer&) noexcept
    {
        m_buffer[0].copy(input[0]);
        m_buffer[1].copy(input[1]);
    }
    
private: // members
    
    Buffer& m_buffer;
};
