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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_GateTilde.h>

#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    GATE~                                         //
    // ================================================================================ //
    
    void GateTilde::declare()
    {
        Factory::add<GateTilde>("gate~", &GateTilde::create);
    }
    
    std::unique_ptr<Object> GateTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<GateTilde>(model, patcher);
    }
    
    GateTilde::GateTilde(model::Object const& model, Patcher& patcher) :
    AudioObject(model, patcher),
    m_opened_output(),
    m_num_outputs(model.getArguments()[0].getInt())
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (args.size() > 1)
        {
            openOutput(args[1].getInt());
        }
    }
    
    void GateTilde::openOutput(int output)
    {
        m_opened_output = std::max(0, std::min(output, static_cast<int>(m_num_outputs)));
    }
    
    void GateTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (args.size() > 0)
        {
            if (index == 0)
            {
                if (args[0].isNumber())
                {
                    openOutput(args[0].getInt());
                }
                else
                {
                    warning("gate~ inlet 1 receives only numbers");
                }
            }
        }
    }
    
    void GateTilde::prepare(PrepareInfo const& infos)
    {
        if (infos.inputs[0])
        {
            setPerformCallBack(this, &GateTilde::performSig);
        }
        else
        {
            setPerformCallBack(this, &GateTilde::performValue);
        }
    }
    
    void GateTilde::performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        size_t output_number = output.getNumberOfChannels();
        
        for(size_t outlet = 0; outlet < output_number; ++outlet)
        {
            output[outlet].fill(0);
        }
        
        if (m_opened_output != 0)
        {
            output[m_opened_output - 1].copy(input[1]);
        }
    }
    
    void GateTilde::performSig(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        dsp::sample_t const * route_signal = input[0].data();
        dsp::sample_t const * input_signal = input[1].data();
        size_t sample_index = input[0ul].size();
        size_t output_number = output.getNumberOfChannels();
        size_t opened_output = 0;
        
        for(size_t outlet = 0; outlet < output_number; ++outlet)
        {
            output[outlet].fill(0);
        }
        
        while(sample_index--)
        {
            opened_output = std::max((size_t) 0, std::min(m_num_outputs, (size_t) route_signal[sample_index]));
            
            if (opened_output)
            {
                output[opened_output - 1][sample_index] = input_signal[sample_index];
            }
        }
    }
    
}}
