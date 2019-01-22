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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_AdcTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       ADC~                                       //
    // ================================================================================ //
    
    void AdcTilde::declare()
    {
        Factory::add<AdcTilde>("adc~", &AdcTilde::create);
    }
    
    std::unique_ptr<Object> AdcTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<AdcTilde>(model, patcher);
    }
    
    AdcTilde::AdcTilde(model::Object const& model, Patcher& patcher):
    AudioInterfaceObject(model, patcher)
    {
    }
    
    void AdcTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        for (size_t outlet = 0; outlet < m_routes.size(); ++outlet)
        {
            m_audio_controler.getFromChannel(m_routes[outlet], output[outlet]);
        }
    }
    
    void AdcTilde::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        setPerformCallBack(this, &AdcTilde::perform);
    }
    
}}
