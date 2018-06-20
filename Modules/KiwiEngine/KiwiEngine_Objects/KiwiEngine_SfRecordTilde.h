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

#pragma once

#include <KiwiEngine/KiwiEngine_Object.h>
#include <juce_core/juce_core.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                               SOUNDFILE RECORDER                                 //
    // ================================================================================ //
    
    class SoundFileRecorder
    {
    public: // methods
        
        SoundFileRecorder();
        
        ~SoundFileRecorder();
        
        bool start(const juce::File& file);
        
        void stop();
        
        bool isRecording() const;
        
        bool write(dsp::Buffer const& input);
        
        void setNumberOfChannels(size_t channels);
        
        size_t getNumberOfChannels() const;
        
        void prepare(double sample_rate, size_t vector_size);
        
    private:
        
        size_t m_channels;
        
        std::vector<float const*> m_buffer_ref;
        
        juce::TimeSliceThread m_background_thread;
        std::unique_ptr<juce::AudioFormatWriter::ThreadedWriter> m_threaded_writer = nullptr;
        
        double m_sample_rate = 0.;
        size_t m_vector_size = 0;
        
        std::mutex m_writer_lock;
        juce::AudioFormatWriter::ThreadedWriter* m_active_writer {nullptr};
    };
    
    // ================================================================================ //
    //                                     SFRECORD~                                    //
    // ================================================================================ //
    
    class SfRecordTilde
    : public AudioObject
    {
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
        SfRecordTilde(model::Object const& model, Patcher& patcher);
        
        ~SfRecordTilde();
        
        void receive(size_t index, std::vector<tool::Atom> const& args) override final;
        
    private:
        
        void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        void prepare(dsp::Processor::PrepareInfo const& infos) override;
        void release() override;
        
        bool openFile(juce::File file);
        void openFileDialog();
        void closeFileDialog();
        
        void record(double duration_ms = -1);
        void stop();
        
    private:
        
        const std::string m_extension = "wav";
        
        juce::File m_file_to_write;
        std::unique_ptr<juce::FileChooser> m_file_chooser;
        
        SoundFileRecorder m_recorder;
        double m_sample_rate = 0.;
        long m_writer_count {0};
        double m_time_to_stop_ms {-1};
    };
    
}}
