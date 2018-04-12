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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_PluginTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

#include <juce_audio_processors/juce_audio_processors.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       PLUGIN WRAPPER                             //
    // ================================================================================ //
    
    class PluginTilde::PluginWrapper : private juce::AudioPluginFormatManager
    {
    public:
        PluginWrapper()
        {
            addDefaultFormats();
        }
    };
    // ================================================================================ //
    //                                       PLUGIN~                                    //
    // ================================================================================ //
    
    void PluginTilde::declare()
    {
        Factory::add<PluginTilde>("plugin~", &PluginTilde::create);
    }
    
    std::unique_ptr<Object> PluginTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<PluginTilde>(model, patcher);
    }
    
    PluginTilde::PluginTilde(model::Object const& model, Patcher& patcher):
    AudioObject(model, patcher), m_plugin_wrapper(std::make_unique<PluginWrapper>())
    {
        
    }
    
    PluginTilde::~PluginTilde()
    {
        
    }
    
    void PluginTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        
    }
    
    void PluginTilde::prepare(PrepareInfo const& infos)
    {
        
    }
    
}}
