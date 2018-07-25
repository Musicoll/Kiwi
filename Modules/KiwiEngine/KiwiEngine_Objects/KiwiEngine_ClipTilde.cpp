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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_ClipTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>
#include <algorithm>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       CLIP~                                      //
    // ================================================================================ //
    
    void ClipTilde::declare()
    {
        Factory::add<ClipTilde>("clip~", &ClipTilde::create);
    }
    
    std::unique_ptr<Object> ClipTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<ClipTilde>(model, patcher);
    }
    
    ClipTilde::ClipTilde(model::Object const& model, Patcher& patcher):
    AudioObject(model, patcher),
    m_minimum(0.),
    m_maximum(0.)
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (args.size() > 0)
        {
            m_minimum.store(args[0].getFloat());
            
            if (args.size() > 1)
            {
                m_maximum.store(args[1].getFloat());
            }
        }
    }
    
    void ClipTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (!args.empty())
        {
            if (index == 1)
            {
                if (args[0].isNumber())
                {
                    m_minimum.store(args[0].getFloat());
                    
                    if (m_minimum.load() > m_maximum.load())
                    {
                        warning("clip~ minimum is higher than maximum");
                    }
                }
                else
                {
                    warning("clip~ inlet 2 only take numbers");
                }
            }
            else if(index == 2)
            {
                if (args[0].isNumber())
                {
                    m_maximum.store(args[0].getFloat());
                    
                    if (m_minimum.load() > m_maximum.load())
                    {
                        warning("clip~ maximum is lower than minimum");
                    }
                }
                else
                {
                    warning("clip~ inlet 3 only take numbers");
                }
            }
        }
    }
    
    void ClipTilde::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        if (infos.inputs[1] && infos.inputs[2])
        {
            setPerformCallBack(this, &ClipTilde::performMinMax);
        }
        else if(infos.inputs[2])
        {
            setPerformCallBack(this, &ClipTilde::performMax);
        }
        else if(infos.inputs[1])
        {
            setPerformCallBack(this, &ClipTilde::performMin);
        }
        else
        {
            setPerformCallBack(this, &ClipTilde::perform);
        }
    }
    
    void ClipTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        dsp::sample_t *output_data = output[0ul].data();
        dsp::sample_t const *input_data = input[0].data();
        size_t sample_index = output[0ul].size();
        
        while(sample_index--)
        {
            *output_data++ = std::max(static_cast<kiwi::dsp::sample_t>(m_minimum.load()), std::min(*input_data++, static_cast<kiwi::dsp::sample_t>(m_maximum.load())));
        }
    }
    
    void ClipTilde::performMinMax(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        dsp::sample_t *output_data = output[0ul].data();
        dsp::sample_t const *input_data = input[0].data();
        dsp::sample_t const *input_min = input[1].data();
        dsp::sample_t const *input_max = input[2].data();
        size_t sample_index = output[0ul].size();
        
        while(sample_index--)
        {
            *output_data++ = std::max(*input_min++, std::min(*input_data++, *input_max++));
        }
    }
    
    void ClipTilde::performMin(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        dsp::sample_t *output_data = output[0ul].data();
        dsp::sample_t const *input_data = input[0].data();
        dsp::sample_t const *input_min = input[1].data();
        size_t sample_index = output[0ul].size();
        
        while(sample_index--)
        {
            *output_data++ = std::max(*input_min++, std::min(*input_data++, static_cast<kiwi::dsp::sample_t>(m_maximum.load())));
        }
    }
    
    void ClipTilde::performMax(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        dsp::sample_t *output_data = output[0ul].data();
        dsp::sample_t const *input_data = input[0].data();
        dsp::sample_t const *input_max = input[2].data();
        size_t sample_index = output[0ul].size();
        
        while(sample_index--)
        {
            *output_data++ = std::max(static_cast<kiwi::dsp::sample_t>(m_minimum.load()), std::min(*input_data++, *input_max++));
        }
    }
}}
