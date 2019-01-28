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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_DacTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       DAC~                                       //
    // ================================================================================ //
    
    void DacTilde::declare()
    {
        Factory::add<DacTilde>("dac~", &DacTilde::create);
    }
    
    std::unique_ptr<Object> DacTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<DacTilde>(model, patcher);
    }
    
    DacTilde::DacTilde(model::Object const& model, Patcher& patcher):
    AudioInterfaceObject(model, patcher)
    {
    }
    
    void DacTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        for (size_t inlet = 0; inlet < m_routes.size(); ++inlet)
        {
            m_audio_controler.addToChannel(m_routes[inlet], input[inlet]);
        }
    }
    
    void DacTilde::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        setPerformCallBack(this, &DacTilde::perform);
    }
    
}}
