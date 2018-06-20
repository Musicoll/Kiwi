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
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                               SOUNDFILE PLAYER                                   //
    // ================================================================================ //
    
    class SoundFilePlayer
    : public juce::ChangeListener
    {
    public: // methods
        
        SoundFilePlayer();
        ~SoundFilePlayer();
        
        //! @brief Starts reading file from start pos to end pos.
        //! @details if end time is greater than starting time,
        //! file will be played from the start position to the end of the file
        bool start(const juce::File& file, double start_ms = 0., double end_ms = -1.);
        
        void stop();
        
        bool isPlaying() const;
        
        void setLoop(bool should_loop);
        
        void setNumberOfChannels(size_t channels);
        
        size_t getNumberOfChannels() const;
        
        juce::String getSupportedFormats() const;
        
        void prepare(double sample_rate, size_t vector_size);
        
        bool read(dsp::Buffer& input);
        
        void release();
        
        using playingStoppedCallback = std::function<void(void)>;
        void setPlayingStoppedCallback(playingStoppedCallback && fn);
        
        //! @brief Print file infos.
        void printInfos(juce::File file,
                        std::function<void(juce::String const&)> printer);
        
    private:
        
        enum TransportState
        {
            Stopped,
            Starting,
            Playing,
            Stopping
        };
        
        void changeListenerCallback(juce::ChangeBroadcaster* source) override;
        
        void changeState(TransportState new_state);
        
    private:
        
        size_t m_channels;
        
        playingStoppedCallback m_playing_stopped_callback {nullptr};
        
        bool is_looping {false};
        
        juce::AudioFormatManager m_format_manager;
        std::unique_ptr<juce::AudioFormatReaderSource> m_reader_source {nullptr};
        juce::AudioTransportSource m_transport_source;
        TransportState m_state {Stopped};
        
        juce::AudioBuffer<float> m_buffer;
        juce::AudioSourceChannelInfo m_audio_source_channel_info;
    };
    
    // ================================================================================ //
    //                                       SFPLAY~                                    //
    // ================================================================================ //
    
    class SfPlayTilde
    : public engine::AudioObject
    {
    public: // methods
        
        SfPlayTilde(model::Object const& model, Patcher& patcher);
        ~SfPlayTilde();
        
        void receive(size_t index, std::vector<tool::Atom> const& args) override final;
        
    private:
        
        bool openFile(juce::File file);
        
        void openFileDialog();
        void closeFileDialog();
        
        void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        void prepare(dsp::Processor::PrepareInfo const& infos) override;
        void release() override;
        
    public: // internal
        
        static void declare();
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
    private:
        
        juce::File m_file_to_read;
        std::unique_ptr<juce::FileChooser> m_file_chooser;
        
        SoundFilePlayer m_player;
    };
    
}}
