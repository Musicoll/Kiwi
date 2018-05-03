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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_SwitchTilde.h>

#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    SWITCH~                                       //
    // ================================================================================ //
    
    void SwitchTilde::declare()
    {
        Factory::add<SwitchTilde>("switch~", &SwitchTilde::create);
    }
    
    std::unique_ptr<Object> SwitchTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<SwitchTilde>(model, patcher);
    }
    
    SwitchTilde::SwitchTilde(model::Object const& model, Patcher& patcher) :
    AudioObject(model, patcher),
    m_opened_input(),
    m_num_inputs(model.getArguments()[0].getInt())
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (args.size() > 1)
        {
            openInput(args[1].getInt());
        }
    }
    
    void SwitchTilde::openInput(int input)
    {
        m_opened_input = std::max(0, std::min(input, static_cast<int>(m_num_inputs)));
    }
    
    void SwitchTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (args.size() > 0)
        {
            if (index == 0)
            {
                if (args[0].isNumber())
                {
                    openInput(args[0].getInt());
                }
                else
                {
                    warning("switch~ inlet 1 receives only numbers");
                }
            }
        }
    }
    
    void SwitchTilde::prepare(PrepareInfo const& infos)
    {
        if (infos.inputs[0])
        {
            setPerformCallBack(this, &SwitchTilde::performSig);
        }
        else
        {
            setPerformCallBack(this, &SwitchTilde::performValue);
        }
    }
    
    void SwitchTilde::performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        if (m_opened_input != 0)
        {
            output[0].copy(input[m_opened_input]);
        }
        else
        {
            output[0].fill(0);
        }
    }
    
    void SwitchTilde::performSig(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        dsp::sample_t const * route_signal = input[0].data();
        dsp::sample_t * output_signal = output[0].data();
        size_t sample_index = output[0ul].size();
        size_t opened_input = 0;
        
        while(sample_index--)
        {
            opened_input = std::max((size_t) 0, std::min(m_num_inputs, (size_t) route_signal[sample_index]));
            
            output_signal[sample_index] = opened_input ? input[opened_input][sample_index] : 0;
        }
    }
    
}}
