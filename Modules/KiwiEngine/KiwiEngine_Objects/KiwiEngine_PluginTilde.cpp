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

    class PluginTilde::PluginWrapper
    {
    public:
        
        inline bool isValid()
        {
            return static_cast<bool>(m_plugin);
        }
        
        void load(const std::string& path, const std::string& filepath)
        {
            juce::File const file(filepath);
            juce::OwnedArray<juce::PluginDescription> results;
            std::unique_ptr<juce::AudioPluginFormat> plugin_format;
            if(file.hasFileExtension(".vst3"))
            {
                plugin_format = std::make_unique<juce::VST3PluginFormat>();
            }
            else
            {
                plugin_format = std::make_unique<juce::VSTPluginFormat>();
            }
            
            plugin_format->findAllTypesForFile(results, filepath);
            if(!results.isEmpty())
            {
                m_plugin = plugin_format->createInstanceFromDescription(*results[0], 44100, 64);
                if(m_plugin)
                {
                    m_plugin->enableAllBuses();
                    return;
                }
                throw std::string("can't create plugin intance for ") + filepath;
                return;
            }
            throw std::string("can't find ") + filepath;
        }
        
        void setParameter(const std::string& name, const float value)
        {
            const int strsize = static_cast<int>(name.size());
            auto const& params = m_plugin->getParameters();
            for(int i = 0; i < params.size(); ++i)
            {
                if(!params[i]->getName(strsize).compare(name))
                {
                    params[i]->setValue(value);
                }
            }
            throw std::string("unknown parameter \'") + name + std::string("'");
        }
        
        void setParameter(const int index, const float value)
        {
            auto const& params = m_plugin->getParameters();
            if(params.size() > index)
            {
                params[index]->setValue(value);
                return;
            }
            throw std::string("index out of range");
        }
        
        void prepare(const int nins, const int nouts, PrepareInfo const& infos)
        {
            if(m_plugin->setBusesLayout(BusesLayout{AudioChannelSet::canonicalChannelSet(nins), AudioChannelSet::canonicalChannelSet(nouts)}))
            {
                m_audio_buffer.setSize(std::max(nins, nouts), infos.vector_size);
                m_plugin->prepareToPlay(infos.sample_rate, infos.vector_size);
                return;
            }
            throw std::string("default audio buses layout is not supported");
        }
        
        void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            const size_t nsamples = input.getVectorSize();
            const size_t ninputs  = input.getNumberOfChannels();
            const size_t noutputs = output.getNumberOfChannels() - 1;
            for(size_t i = 0; i < ninputs; ++i)
            {
                std::copy_n(input[i].data(), nsamples, m_audio_buffer.getWritePointer(i));
            }
            m_plugin->processBlock(m_audio_buffer, m_midi_buffer);
            for(size_t i = 0; i < noutputs; ++i)
            {
                std::copy_n(m_audio_buffer.getReadPointer(i), nsamples, output[i].data());
            }
        }
        
    private:
            
        using BusesLayout = juce::AudioProcessor::BusesLayout;
        using AudioChannelSet = juce::AudioChannelSet;
            
        juce::ScopedPointer<juce::AudioPluginInstance> m_plugin = nullptr;
        juce::AudioBuffer<dsp::sample_t>               m_audio_buffer;
        juce::MidiBuffer                               m_midi_buffer;
    };
    
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
    m_wrapper(std::make_unique<PluginWrapper>()),
    m_plugin_file(parsePluginFile(model.getArguments()))
    {
        load();
    }
    
    PluginTilde::~PluginTilde()
    {
        
    }
    
    void PluginTilde::load()
    {
        try
        {
            m_wrapper->load("", m_plugin_file);
        }
        catch (std::string& e)
        {
            error(e);
        }
    }
    
    void PluginTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if(!m_wrapper->isValid())
            return;
        if(!args.empty() && args[0].isString())
        {
            std::string const method = args[0].getString();
            if(method == "param")
            {
                if(args.size() > 2 && args[1].isInt() && args[2].isFloat())
                {
                    try
                    {
                        m_wrapper->setParameter(args[1].getInt(), args[2].getFloat());
                    }
                    catch (std::string& e)
                    {
                        error(e);
                        return;
                    }
                    if(args.size() > 3)
                        warning("param method too many arguments");
                    return;
                }
                warning("param method wrong arguments");
                return;
            }
            else if(args.size() > 1 && args[1].isFloat())
            {
                try
                {
                    m_wrapper->setParameter(method, args[1].getFloat());
                }
                catch (std::string& e)
                {
                    error(e);
                    return;
                }
                if(args.size() > 2)
                    warning(method + " method too many arguments");
                return;
            }
            warning("wrong arguments");
            return;
        }
    }
    
    void PluginTilde::prepare(PrepareInfo const& infos)
    {
        if(!m_wrapper->isValid())
            return;
        try
        {
            // See for inputs = 0...
            m_wrapper->prepare(static_cast<int>(getNumberOfInputs()),
                               static_cast<int>(getNumberOfOutputs() - 1),
                               infos);
        }
        catch (std::string& e)
        {
            warning(e);
            return;
        }
        setPerformCallBack(m_wrapper.get(), &PluginWrapper::perform);
    }
    
}}
