/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_PhasorTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi{ namespace engine {
    
    // ================================================================================ //
    //                                     PHASOR~                                      //
    // ================================================================================ //
    
    void PhasorTilde::declare()
    {
        Factory::add<PhasorTilde>("phasor~", &PhasorTilde::create);
    }
    
    std::unique_ptr<Object> PhasorTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<PhasorTilde>(model, patcher);
    }
    
    PhasorTilde::PhasorTilde(model::Object const& model, Patcher& patcher):
    AudioObject(model, patcher)
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (!args.empty() && args[0].isNumber())
        {
            setFrequency(args[0].getFloat());
        }
    }
    
    void PhasorTilde::setFrequency(dsp::sample_t const& freq) noexcept
    {
        m_freq = freq;
        m_phase_inc.store(m_freq / m_sr);
    }
    
    void PhasorTilde::setSampleRate(dsp::sample_t const& sample_rate)
    {
        m_sr = sample_rate;
        m_phase_inc.store(m_freq / m_sr);
    }
    
    void PhasorTilde::setPhase(dsp::sample_t const& phase) noexcept
    {
        auto new_phase = phase;
        while(new_phase > 1.f) { new_phase -= 1.f; }
        while(new_phase < 0.f) { new_phase += 1.f; }
        m_phase.store(new_phase);
    }
    
    void PhasorTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0)
        {
            if (args[0].isNumber())
            {
                setFrequency(args[0].getFloat());
            }
            else
            {
                warning("phasor~ inlet 1 only take numbers");
            }
        }
        else if(index == 1)
        {
            if (args[0].isNumber())
            {
                setPhase(args[0].getFloat());
            }
            else
            {
                warning("phasor~ inlet 2 only takes numbers");
            }
        }
    }
    
    void PhasorTilde::prepare(PrepareInfo const& infos)
    {
        setSampleRate(static_cast<dsp::sample_t>(infos.sample_rate));
        
        setPerformCallBack(this, (infos.inputs[0]
                                  ? &PhasorTilde::performSignal
                                  : &PhasorTilde::performValue));
    }
    
    dsp::sample_t PhasorTilde::getAndIncrementPhase(const dsp::sample_t inc)
    {
        dsp::sample_t old_phase = m_phase.load();
        dsp::sample_t phase = old_phase;
        
        if(phase > 1.f) { phase -= 1.f; }
        else if(phase < 0.f) { phase += 1.f; }

        m_phase.compare_exchange_strong(old_phase, phase += inc);
        return phase;
    }
    
    void PhasorTilde::performSignal(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        size_t sampleframes = output[0ul].size();
        dsp::sample_t const* in = input[0ul].data();
        dsp::sample_t* out = output[0ul].data();
        dsp::sample_t freq = 0.f;
        
        while(sampleframes--)
        {
            freq = *in++;
            *out++ = getAndIncrementPhase(freq / m_sr);
        }
    }
    
    void PhasorTilde::performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        size_t sampleframes = output[0ul].size();
        dsp::sample_t *out = output[0ul].data();
        
        while(sampleframes--)
        {
            *out++ = getAndIncrementPhase(m_phase_inc.load());
        }
    }
    
}}
