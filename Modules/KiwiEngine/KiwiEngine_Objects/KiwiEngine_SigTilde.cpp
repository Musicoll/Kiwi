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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_SigTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       SIG~                                       //
    // ================================================================================ //
    
    void SigTilde::declare()
    {
        Factory::add<SigTilde>("sig~", &SigTilde::create);
    }
    
    std::unique_ptr<Object> SigTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<SigTilde>(model, patcher);
    }
    
    SigTilde::SigTilde(model::Object const& model, Patcher& patcher):
    AudioObject(model, patcher)
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (!args.empty() && args[0].isNumber())
        {
            m_value = args[0].getFloat();
        }
    }
    
    void SigTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0)
        {
            if (args[0].isNumber())
            {
                m_value = args[0].getFloat();
            }
            else
            {
                warning("sig~ inlet 1 must receive a number");
            }
        }
    }
    
    void SigTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        size_t sample_index = output[0].size();
        dsp::sample_t* output_sig = output[0].data();
        dsp::sample_t const value = m_value;
        
        while(sample_index--)
        {
            *output_sig++  = value;
        }
    }
    
    void SigTilde::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        setPerformCallBack(this, &SigTilde::perform);
    }
    
}}
