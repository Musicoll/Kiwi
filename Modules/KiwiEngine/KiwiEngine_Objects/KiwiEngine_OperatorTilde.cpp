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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_OperatorTilde.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    OPERATOR TILDE                                //
    // ================================================================================ //
    
    OperatorTilde::OperatorTilde(model::Object const& model, Patcher& patcher):
    AudioObject(model, patcher),
    m_rhs()
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (!args.empty() && args[0].isNumber())
        {
            m_rhs = args[0].getFloat();
        }
    }
    
    void OperatorTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if(!args.empty())
        {
            if(args[0].isNumber() && index == 1)
            {
                m_rhs = args[0].getFloat();
            }
        }
    }
    
    void OperatorTilde::performVec(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        dsp::Signal const& in = input[0];
        const size_t size = in.size();
        dsp::sample_t const* in1 = in.data();
        dsp::sample_t const* in2 = input[1].data();
        dsp::sample_t* out = output[0].data();
        
        for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8, out += 8)
        {
            compute(out[0], in1[0], in2[0]);
            compute(out[1], in1[1], in2[1]);
            compute(out[2], in1[2], in2[2]);
            compute(out[3], in1[3], in2[3]);
            compute(out[4], in1[4], in2[4]);
            compute(out[5], in1[5], in2[5]);
            compute(out[6], in1[6], in2[6]);
            compute(out[7], in1[7], in2[7]);
        }
        for(size_t i = size&7; i; --i, in1++, in2++, out++)
        {
            compute(out[0], in1[0], in2[0]);
        }
    }
    
    void OperatorTilde::performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        dsp::Signal const& in = input[0];
        const size_t size = in.size();
        dsp::sample_t const* in1 = in.data();
        dsp::sample_t* out = output[0].data();
        
        dsp::sample_t const value = m_rhs;
        
        for(size_t i = size>>3; i; --i, in1 += 8, out += 8)
        {
            compute(out[0], in1[0], value);
            compute(out[1], in1[1], value);
            compute(out[2], in1[2], value);
            compute(out[3], in1[3], value);
            compute(out[4], in1[4], value);
            compute(out[5], in1[5], value);
            compute(out[6], in1[6], value);
            compute(out[7], in1[7], value);
        }
        
        for(size_t i = size&7; i; --i, in1++, out++)
        {
            compute(out[0], in1[0], value);
        }
    }
    
    void OperatorTilde::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        if (infos.inputs.size() > 1 && infos.inputs[1])
        {
            setPerformCallBack(this, &OperatorTilde::performVec);
        }
        else
        {
            setPerformCallBack(this, &OperatorTilde::performValue);
        }
    }
    
}}
