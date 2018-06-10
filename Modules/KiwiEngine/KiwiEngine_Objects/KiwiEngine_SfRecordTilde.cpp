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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_SfRecordTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                               SOUNDFILE RECORDER                                 //
    // ================================================================================ //
    
    SoundFileRecorder::SoundFileRecorder()
    : m_background_thread("SoundFile Recorder Thread")
    {
        m_background_thread.startThread();
    }
    
    SoundFileRecorder::~SoundFileRecorder()
    {
        stop();
    }
    
    bool SoundFileRecorder::start(const juce::File& file)
    {
        stop();
        
        if (m_sample_rate <= 0)
            return false; // abort
        
        // Create an OutputStream to write to our destination file...
        file.deleteFile();
        std::unique_ptr<juce::FileOutputStream> file_stream (file.createOutputStream());
        
        if (file_stream == nullptr)
            return false; // abort
        
        // Now create a WAV writer object that writes to our output stream...
        juce::WavAudioFormat wav_format;
        juce::AudioFormatWriter* writer = wav_format.createWriterFor(file_stream.get(), m_sample_rate, getNumberOfChannels(), 16, {}, 0);
        
        if (writer == nullptr)
            return false; // abort
        
        // (passes responsibility for deleting the stream to the writer object that is now using it)
        file_stream.release();
        
        // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
        // write the data to disk on our background thread.
        m_threaded_writer = std::make_unique<juce::AudioFormatWriter::ThreadedWriter>(writer,
                                                                                      m_background_thread,
                                                                                      32768);
        
        // And now, swap over our active writer pointer so that the audio callback will start using it..
        const std::lock_guard<std::mutex> lock (m_writer_lock);
        m_active_writer = m_threaded_writer.get();
        
        return true;
    }
    
    void SoundFileRecorder::stop()
    {
        // First, clear this pointer to stop the audio callback from using our writer object..
        {
            const std::lock_guard<std::mutex> lock (m_writer_lock);
            m_active_writer = nullptr;
        }
        
        // Now we can delete the writer object. It's done in this order because the deletion could
        // take a little time while remaining data gets flushed to disk, so it's best to avoid blocking
        // the audio callback while this happens.
        m_threaded_writer.reset();
    }
    
    bool SoundFileRecorder::isRecording() const
    {
        return m_active_writer != nullptr;
    }
    
    void SoundFileRecorder::setNumberOfChannels(size_t channels)
    {
        m_channels = channels > 0 ? channels : m_channels;
        m_buffer_ref.resize(m_channels);
    }
    
    size_t SoundFileRecorder::getNumberOfChannels() const
    {
        return m_channels;
    }
    
    void SoundFileRecorder::prepare(double sample_rate, size_t vector_size)
    {
        m_sample_rate = sample_rate;
        m_vector_size = vector_size;
    }
    
    bool SoundFileRecorder::write(dsp::Buffer const& input)
    {
        const std::lock_guard<std::mutex> lock (m_writer_lock);
        
        if (m_active_writer != nullptr)
        {
            const auto channels = input.getNumberOfChannels();
            for(auto channel = 0; channel < channels; ++channel)
            {
                m_buffer_ref[channel] = input[channel].data();
            }
            
            return m_active_writer->write(m_buffer_ref.data(), m_vector_size);
        }
        
        return false;
    }
    
    // ================================================================================ //
    //                                     SFRECORD~                                    //
    // ================================================================================ //
    
    void SfRecordTilde::declare()
    {
        Factory::add<SfRecordTilde>("sfrecord~", &SfRecordTilde::create);
    }
    
    std::unique_ptr<Object> SfRecordTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<SfRecordTilde>(model, patcher);
    }
    
    SfRecordTilde::SfRecordTilde(model::Object const& model, Patcher& patcher)
    : AudioObject(model, patcher)
    {
        m_recorder.setNumberOfChannels(getNumberOfInputs());
    }
    
    SfRecordTilde::~SfRecordTilde()
    {
        closeFileDialog();
    }
    
    void SfRecordTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (!args.empty() && index == 0)
        {
            if (args[0].isString() && args[0].getString() == "open")
            {
                if(args.size() > 1)
                {
                    if(args[0].isString())
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
                record();
            }
            else if ((args[0].isNumber() && args[0].getInt() == 0)
                     || args[0].getString() == "stop")
            {
                stop();
            }
            else if (args[0].getString() == "record")
            {
                double duration_ms = -1;
                
                if(args.size() > 1 && args[1].isNumber())
                {
                    duration_ms = args[1].getFloat();
                }
                
                record(duration_ms);
            }
        }
    }
    
    bool SfRecordTilde::openFile(juce::File file)
    {
        const auto path = file.getFullPathName();
        if(!juce::File::isAbsolutePath(path))
        {
            warning("sfrecord~: is not an absolute path");
            return false;
        }
        
        if(file.isDirectory())
        {
            warning("sfrecord~: invalid file path");
            return false;
        }
        
        // is a file
        
        if(!file.hasWriteAccess())
        {
            warning("sfrecord~: no write access to file \""
                    + path.toStdString() + "\"");
            return false;
        }
        
        // that has write access
        
        if(!file.hasFileExtension(m_extension))
        {
            file = file.withFileExtension(m_extension);
        }
        
        // and valid extension
        
        if(!file.exists())
        {
            if(!file.create())
            {
                warning("sfrecord~: can't create file \"" + path.toStdString() + "\"");
                return false;
            }
        }
        else
        {
            warning("sfrecord~: file will be overwritten \""
                    + path.toStdString() + "\"");
        }
        
        // that really exist.
        
        m_file_to_write = path;
        return true;
    }
    
    void SfRecordTilde::openFileDialog()
    {
        const auto default_dir = juce::File::getSpecialLocation(juce::File::userMusicDirectory);
        
        auto dir = (!m_file_to_write.getFullPathName().isEmpty()
                    ? m_file_to_write.getParentDirectory()
                    : default_dir);
        
        if (dir.createDirectory().wasOk())
        {
            dir = dir.getChildFile("Untitled.wav");
        }
        
        m_file_chooser = std::make_unique<juce::FileChooser>("Choose a file to save...",
                                                             dir, "*.wav;", true);
        deferMain([this, fc = m_file_chooser.get()]() {
            
            const int fc_flags = (juce::FileBrowserComponent::saveMode
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
    
    void SfRecordTilde::closeFileDialog()
    {
        deferMain([this]() {
            
            m_file_chooser.reset();
            
        });
    }
    
    void SfRecordTilde::stop()
    {
        defer([this]{
            m_writer_count = 0;
            m_recorder.stop();
        });
    }
    
    void SfRecordTilde::record(double duration_ms)
    {
        m_writer_count = 0;
        m_time_to_stop_ms = duration_ms > 0 ? duration_ms : -1;
        m_recorder.start(m_file_to_write);
        m_file_to_write = juce::File(); // consumed
    }
    
    void SfRecordTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        if(m_recorder.write(input))
        {
            size_t sampleframe = output[0].size();
            dsp::sample_t* output_sig = output[0].data();
            
            while(sampleframe--)
            {
                *output_sig++ = (m_writer_count++ / m_sample_rate * 1000.);
            }
            
            if((m_time_to_stop_ms > 0) && m_time_to_stop_ms < output_sig[sampleframe-1])
            {
                // defered stop
                stop();
                m_time_to_stop_ms = 0;
            }
        }
        else
        {
            output[0].fill(m_writer_count / m_sample_rate * 1000.);
        }
    }

    void SfRecordTilde::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        m_sample_rate = infos.sample_rate;
        
        m_recorder.prepare(infos.sample_rate, infos.vector_size);
        setPerformCallBack(this, &SfRecordTilde::perform);
    }
    
    void SfRecordTilde::release()
    {
        m_sample_rate = 0.;
    }
    
}}
