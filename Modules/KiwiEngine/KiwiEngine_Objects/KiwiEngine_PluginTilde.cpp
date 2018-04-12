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
    //                                       PLUGIN~                                    //
    // ================================================================================ //
    
    std::string PluginTilde::parsePluginFile(std::vector<tool::Atom> const& args)
    {
#ifdef JUCE_MAC
        const std::string vst_ext(".vst");
#elif JUCE_WINDOWS
        const std::string vst_ext(".dll");
#elif JUCE_LINUX
        const std::string vst_ext(".so");
#endif
        juce::File const folder(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
                                .getChildFile("Kiwi Media")
                                .getChildFile("Plugins"));
        
        if(args[3].getString() == std::string("vst2"))
        {
            return folder.getChildFile("VST2")
            .getChildFile(juce::String(args[2].getString() + vst_ext))
            .getFullPathName().toStdString();
        }
        return folder.getChildFile("VST3")
        .getChildFile(juce::String(args[2].getString() + ".vst3"))
        .getFullPathName().toStdString();
    }
    
    void PluginTilde::declare()
    {
        Factory::add<PluginTilde>("plugin~", &PluginTilde::create);
    }
    
    std::unique_ptr<Object> PluginTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<PluginTilde>(model, patcher);
    }
    
    PluginTilde::PluginTilde(model::Object const& model, Patcher& patcher):
    AudioObject(model, patcher),
    m_plugin_file(parsePluginFile(model.getArguments()))
    {
        load();
    }
    
    PluginTilde::~PluginTilde()
    {
        
    }
    
    bool PluginTilde::isVST2()
    {
        return m_plugin_file.find("VST2");
    }
    
    bool PluginTilde::isVST3()
    {
        return m_plugin_file.find("VST3");
    }
    
    void PluginTilde::load()
    {
        std::unique_ptr<juce::AudioPluginFormat> plugin_format;
        if(isVST2())
        {
            plugin_format = std::make_unique<juce::VSTPluginFormat>();
        }
        else
        {
            plugin_format = std::make_unique<juce::VST3PluginFormat>();
        }
        
        if(plugin_format->fileMightContainThisPluginType(m_plugin_file))
        {
            juce::OwnedArray<juce::PluginDescription> results;
            plugin_format->findAllTypesForFile(results, m_plugin_file);
            if(!results.isEmpty())
            {
                m_plugin = std::unique_ptr<juce::AudioPluginInstance>(plugin_format->createInstanceFromDescription(*results[0], 44100, 64));
                if(m_plugin)
                {
                    post("plugin~ " + m_plugin_file + " has been loaded");
                }
                else
                {
                    error("plugin~ can't allocate " + m_plugin_file);
                }
            }
            else
            {
                error("plugin~ can't find type " + m_plugin_file);
            }
        }
        else
        {
            error("plugin~ can't find " + m_plugin_file);
        }
        
        
        juce::PluginDescription descrition;
    }
    
    void PluginTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        
    }
    
    void PluginTilde::prepare(PrepareInfo const& infos)
    {
        
    }
    
}}
