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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_SahTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi{ namespace engine {
    
    // ================================================================================ //
    //                                      SAH~                                        //
    // ================================================================================ //
    
    void SahTilde::declare()
    {
        Factory::add<SahTilde>("sah~", &SahTilde::create);
    }
    
    std::unique_ptr<Object> SahTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<SahTilde>(model, patcher);
    }
    
    SahTilde::SahTilde(model::Object const& model, Patcher& patcher)
    : AudioObject(model, patcher)
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (!args.empty() && args[0].isNumber())
        {
            setThreshold(args[0].getFloat());
        }
    }
    
    void SahTilde::setThreshold(dsp::sample_t const& value) noexcept
    {
        m_threshold.store(value);
    }
    
    void SahTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 1 && !args.empty())
        {
            if (args[0].isNumber())
            {
                setThreshold(args[0].getFloat());
            }
            else
            {
                warning("sah~ inlet 1 only takes numbers");
            }
        }
    }
    
    void SahTilde::prepare(PrepareInfo const& infos)
    {
        setPerformCallBack(this, &SahTilde::perform);
    }
    
    void SahTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        size_t sampleframes = output[0ul].size();
        dsp::sample_t const* in1 = input[0ul].data();
        dsp::sample_t const* in2 = input[1ul].data();
        dsp::sample_t* out = output[0ul].data();
        
        const dsp::sample_t threshold = m_threshold.load();
        
        while(sampleframes--)
        {
            dsp::sample_t sample = *in1++;
            dsp::sample_t ctrl_sample = *in2++;
            
            if(m_last_ctrl_sample <= threshold
               && ctrl_sample > threshold)
            {
                m_hold_value = sample;
            }
            
            *out++ = m_hold_value;
            m_last_ctrl_sample = ctrl_sample;
        }
    }
    
}}
