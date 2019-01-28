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

#include <functional>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_NoiseTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                     NOISE~                                       //
    // ================================================================================ //
    
    void NoiseTilde::declare()
    {
        Factory::add<NoiseTilde>("noise~", &NoiseTilde::create);
    }
    
    std::unique_ptr<Object> NoiseTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<NoiseTilde>(model, patcher);
    }
    
    NoiseTilde::NoiseTilde(model::Object const& model,
                           Patcher& patcher)
    : AudioObject(model, patcher)
    , m_random_devive()
    , m_random_generator(m_random_devive())
    , m_random_distribution(-1., 1.)
    {
        ;
    }
    
    void NoiseTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        ;
    }
    
    void NoiseTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        size_t frames = output[0].size();
        dsp::sample_t* out = output[0].data();
        
        while(frames--)
        {
            *out++ = m_random_distribution(m_random_generator);
        }
    }
    
    void NoiseTilde::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        setPerformCallBack(this, &NoiseTilde::perform);
    }
    
}}
