/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Dsp/KiwiEngine_OscTilde.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                       OSC~                                       //
        // ================================================================================ //
        
        OscTilde::OscTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : AudioObject(model, patcher)
        {
            if (!args.empty() && args[0].isNumber())
            {
                setFrequency(args[0].getFloat());
            }
        }
        
        void OscTilde::setFrequency(dsp::sample_t const& freq) noexcept
        {
            m_freq = freq;
        }
        
        void OscTilde::setSampleRate(dsp::sample_t const& sample_rate)
        {
            m_sr = sample_rate;
        }
        
        void OscTilde::setOffset(dsp::sample_t const& offset) noexcept
        {
            m_offset = fmodf(offset, 1.f);
        }
        
        void OscTilde::receive(size_t index, std::vector<Atom> const& args)
        {
            if (index == 0 && args[0].isNumber())
            {
                setFrequency(args[0].getFloat());
            }
            else if(index == 1 && args[0].isNumber())
            {
                setOffset(args[0].getFloat());
            }
        }
        
        void OscTilde::prepare(PrepareInfo const& infos)
        {
            setSampleRate(static_cast<dsp::sample_t>(infos.sample_rate));
            
            if (infos.inputs[0] && infos.inputs[1])
            {
                setPerformCallBack(this, &OscTilde::performPhaseAndFreq);
            }
            else if(infos.inputs[0])
            {
                setPerformCallBack(this, &OscTilde::performFreq);
            }
            else if(infos.inputs[1])
            {
                setPerformCallBack(this, &OscTilde::performPhase);
            }
            else
            {
                setPerformCallBack(this, &OscTilde::performValue);
            }
        }
        
        void OscTilde::performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            dsp::sample_t *sig_data = output[0ul].data();
            size_t sample_index = output[0ul].size();
            dsp::sample_t const time_inc = m_freq/m_sr;
            dsp::sample_t const offset = m_offset;
            
            while(sample_index--)
            {
                *sig_data++ = std::cos(2.f * dsp::pi * (m_time + offset));
                m_time += time_inc;
            }
            
            m_time = fmodf(m_time, 1.f);
        }
        
        void OscTilde::performFreq(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            size_t sample_index = output[0ul].size();
            dsp::sample_t* output_sig = output[0ul].data();
            dsp::sample_t const* freq = input[0ul].data();
            dsp::sample_t const offset = m_offset;
            
            while(sample_index--)
            {
                *output_sig++ = std::cos(2.f *dsp::pi * (m_time + offset));
                m_time += (*freq++ / m_sr);
            }
            
            m_time = fmodf(m_time, 1.f);
        }
        
        void OscTilde::performPhase(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            dsp::sample_t* output_sig = output[0ul].data();
            size_t sample_index = output[0ul].size();
            dsp::sample_t const* phase = input[1ul].data();
            dsp::sample_t const time_inc = m_freq/m_sr;
            
            while(sample_index--)
            {
                *output_sig++  = std::cos(2.f * dsp::pi * (m_time + fmodf(*phase++, 1.f)));
                m_time += time_inc;
            }
            
            m_time = fmodf(m_time, 1.f);
        }
        
        void OscTilde::performPhaseAndFreq(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            size_t sample_index = output[0].size();
            dsp::sample_t* output_sig = output[0].data();
            dsp::sample_t const* freq = input[0].data();
            dsp::sample_t const* phase = input[1].data();
            
            while(sample_index--)
            {
                *output_sig++  = std::cos(2.f * dsp::pi * (m_time + fmodf(*phase++, 1.f)));
                m_time += (*freq++ / m_sr);
            }
            
            m_time = fmodf(m_time, 1.f);
        }
    }
}
