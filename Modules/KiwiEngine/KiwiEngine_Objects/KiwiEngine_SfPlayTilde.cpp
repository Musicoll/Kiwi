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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_SfPlayTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                SOUNDFILE PLAYER                                  //
    // ================================================================================ //
    
    SoundFilePlayer::SoundFilePlayer()
    : m_buffer()
    , m_audio_source_channel_info(m_buffer)
    {
        m_format_manager.registerBasicFormats();
        m_transport_source.addChangeListener(this);
    }
    
    SoundFilePlayer::~SoundFilePlayer()
    {
        stop();
    }
    
    bool SoundFilePlayer::start(const juce::File& file, double start_ms, double end_ms)
    {
        stop();
        
        std::unique_ptr<juce::AudioFormatReader> reader (m_format_manager.createReaderFor(file));
        
        if (reader)
        {
            const auto sf_frames = reader->lengthInSamples;
            const auto sf_samplerate = reader->sampleRate;
            
            const int64_t start_sample = (start_ms * 0.001 * sf_samplerate);
            
            if(start_sample >= 0 && start_sample < sf_frames)
            {
                int64_t end_sample = sf_frames;
                
                if(start_ms < end_ms)
                {
                    end_sample = std::min(static_cast<int64_t>(end_ms * 0.001 * sf_samplerate),
                                          sf_frames);
                }
                
                int64_t length = end_sample - start_sample;
                auto susection_reader = new juce::AudioSubsectionReader(reader.release(), start_sample, length, true);
                
                auto new_source = std::make_unique<juce::AudioFormatReaderSource>(susection_reader, true);
                m_transport_source.setSource(new_source.get(), 0, nullptr, susection_reader->sampleRate);
                m_reader_source.reset(new_source.release());
                setLoop(is_looping);
                changeState(Starting);
            }
        }
        
        return false;
    }
    
    void SoundFilePlayer::stop()
    {
        changeState(Stopping);
    }
    
    bool SoundFilePlayer::isPlaying() const
    {
        return m_transport_source.isPlaying();
    }
    
    void SoundFilePlayer::setLoop(bool should_loop)
    {
        is_looping = should_loop;
        if (m_reader_source != nullptr)
        {
            m_reader_source->setLooping(should_loop);
        }
    }
    
    void SoundFilePlayer::setNumberOfChannels(size_t channels)
    {
        m_channels = channels > 0 ? channels : m_channels;
    }
    
    size_t SoundFilePlayer::getNumberOfChannels() const
    {
        return m_channels;
    }
    
    juce::String SoundFilePlayer::getSupportedFormats() const
    {
        return m_format_manager.getWildcardForAllFormats();
    }
    
    void SoundFilePlayer::printInfos(juce::File file,
                                     std::function<void(juce::String const&)> post)
    {
        std::unique_ptr<juce::AudioFormatReader> reader (m_format_manager.createReaderFor(file));
        
        if (reader)
        {
            post("- file: " + file.getFullPathName());
            post("- format: " + reader->getFormatName());
            post("- sampling rate: " + juce::String(reader->sampleRate));
            post("- channels: " + juce::String(reader->numChannels));
            post("- duration: " + juce::RelativeTime::seconds(reader->lengthInSamples / reader->sampleRate).getDescription());
            post("- bits per sample: " + juce::String(reader->bitsPerSample));
            
            // other infos;
            const auto speaker_arr = reader->getChannelLayout().getSpeakerArrangementAsString();
            if(!speaker_arr.isEmpty())
            {
                post("- speaker arrangement: " + speaker_arr + "\n");
            }
            
            const auto metadata = reader->metadataValues.getDescription();
            if(!metadata.isEmpty())
            {
                post("- metadata: " + metadata + "\n");
            }
        }
    }
    
    void SoundFilePlayer::setPlayingStoppedCallback(playingStoppedCallback && fn)
    {
        m_playing_stopped_callback = std::move(fn);
    }
    
    void SoundFilePlayer::prepare(double sample_rate, size_t vector_size)
    {
        m_buffer.setSize(getNumberOfChannels(), vector_size);
        m_audio_source_channel_info.startSample = 0.;
        m_audio_source_channel_info.numSamples = vector_size;
        
        m_transport_source.prepareToPlay(vector_size, sample_rate);
    }
    
    bool SoundFilePlayer::read(dsp::Buffer& outputs)
    {
        if (m_reader_source == nullptr)
        {
            const auto channels = outputs.getNumberOfChannels();
            for(auto channel = 0; channel < channels; ++channel)
            {
                outputs[channel].fill(0.);
            }
            
            return false;
        }
        
        m_transport_source.getNextAudioBlock(m_audio_source_channel_info);
        
        // recopy samples
        
        const auto channels = outputs.getNumberOfChannels();
        for(auto channel = 0; channel < channels; ++channel)
        {
            auto& output = outputs[channel];
            if(channel < m_buffer.getNumChannels()
               && output.size() <= m_buffer.getNumSamples())
            {
                for(auto i = 0; i < output.size(); ++i)
                {
                    output[i] = m_buffer.getReadPointer(channel)[i];
                }
            }
        }
        
        return true;
    }
    
    void SoundFilePlayer::release()
    {
        m_transport_source.releaseResources();
    }
    
    void SoundFilePlayer::changeState (TransportState new_state)
    {
        if (m_state != new_state)
        {
            m_state = new_state;
            
            switch (m_state)
            {
                case Stopped:
                    if(m_playing_stopped_callback)
                    {
                        m_playing_stopped_callback();
                    }
                    break;
                    
                case Starting:
                    m_transport_source.start();
                    break;
                    
                case Playing:
                    break;
                    
                case Stopping:
                    m_transport_source.stop();
                    break;
            }
        }
    }
    
    void SoundFilePlayer::changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        if (source == &m_transport_source)
        {
            if (m_transport_source.isPlaying())
                changeState (Playing);
            else
                changeState (Stopped);
        }
    }
    
    // ================================================================================ //
    //                                       SFPLAY~                                    //
    // ================================================================================ //
    
    void SfPlayTilde::declare()
    {
        Factory::add<SfPlayTilde>("sfplay~", &SfPlayTilde::create);
    }
    
    std::unique_ptr<Object> SfPlayTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<SfPlayTilde>(model, patcher);
    }
    
    SfPlayTilde::SfPlayTilde(model::Object const& model, Patcher& patcher)
    : AudioObject(model, patcher)
    {
        const auto& args = model.getArguments();
        const auto channels = !args.empty() && args[0].getInt() > 0 ? args[0].getInt() : 2;
        m_player.setNumberOfChannels(channels);
        
        m_player.setPlayingStoppedCallback([this](){
            defer([this](){
                send(getNumberOfOutputs() - 1, {"bang"});
            });
        });
    }
    
    SfPlayTilde::~SfPlayTilde()
    {
        closeFileDialog();
    }
    
    void SfPlayTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (!args.empty() && index == 0)
        {
            if (args[0].isString() && args[0].getString() == "open")
            {
                if(args.size() > 1)
                {
                    if(args[1].isString())
                    {
                        openFile(juce::File(args[1].getString()));
                    }
                }
                else
                {
                    openFileDialog();
                }
            }
            else if ((args[0].isNumber() && args[0].getInt() == 1)
                     || args[0].getString() == "start")
            {
                m_player.start(m_file_to_read);
            }
            else if ((args[0].isNumber() && args[0].getInt() == 0)
                     || args[0].getString() == "stop")
            {
                m_player.stop();
            }
            else if (args[0].getString() == "seek")
            {
                if(args.size() > 1 && args[1].isNumber())
                {
                    double start_ms = args[1].getFloat();
                    double end_ms = ((args.size() > 2 && args[2].isNumber())
                                     ? args[2].getFloat()
                                     : -1.);
                                     
                    m_player.start(m_file_to_read, start_ms, end_ms);
                }
            }
            else if (args[0].getString() == "loop")
            {
                if(args.size() == 2 && args[1].isNumber())
                {
                    m_player.setLoop(args[1].getInt());
                }
                else
                {
                    warning("sfplay~: loop message must be followed by 0 or 1");
                }
            }
            else if (args[0].getString() == "print")
            {
                post("*sfplay~ infos*");
                const auto filepath = m_file_to_read.getFullPathName();
                if(filepath.isNotEmpty())
                {
                    m_player.printInfos(m_file_to_read, [this](juce::String const& line){
                        post(line.toStdString());
                    });
                }
                else
                {
                    post("- no file opened");
                }
            }
        }
    }
    
    bool SfPlayTilde::openFile(juce::File file)
    {
        const auto file_path = file.getFullPathName();
        if(!juce::File::isAbsolutePath(file_path))
        {
            warning("sfplay~: is not an absolute path");
            return false;
        }
        
        if(file.isDirectory())
        {
            warning("sfplay~: invalid file path");
            return false;
        }
        
        if(!file.exists())
        {
            warning("sfplay~: file doesn't exist \""
                    + file_path.toStdString() + "\"");
            return false;
        }
        
        // that really exist.
        
        m_player.stop();
        m_file_to_read = file;
        return true;
    }
    
    void SfPlayTilde::openFileDialog()
    {
        const auto default_dir = juce::File::getSpecialLocation(juce::File::userMusicDirectory);
        
        auto dir = (!m_file_to_read.getFullPathName().isEmpty()
                    ? m_file_to_read.getParentDirectory()
                    : default_dir);
        
        m_file_chooser = std::make_unique<juce::FileChooser>("Choose a file to read...",
                                                             dir, m_player.getSupportedFormats(), true);
        
        deferMain([this, fc = m_file_chooser.get()]() {
            
            const int fc_flags = (juce::FileBrowserComponent::openMode
                                  | juce::FileBrowserComponent::canSelectFiles);

            fc->launchAsync(fc_flags, [this](juce::FileChooser const& chooser) {
                auto file = chooser.getResult();
                if(file.getFullPathName().isNotEmpty())
                {
                    openFile(file);
                }
            });
        });
    }
    
    void SfPlayTilde::closeFileDialog()
    {
        deferMain([this]() {
            
            m_file_chooser.reset();
            
        });
    }
    
    void SfPlayTilde::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        m_player.prepare(infos.sample_rate, infos.vector_size);
        setPerformCallBack(this, &SfPlayTilde::perform);
    }
    
    void SfPlayTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        m_player.read(output);
    }
    
    void SfPlayTilde::release()
    {
        m_player.release();
    }
    
}}
