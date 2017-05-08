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

#include <KiwiDsp/KiwiDsp_Signal.hpp>
#include <KiwiDsp/KiwiDsp_Chain.hpp>
#include <KiwiEngine/KiwiEngine_AudioControler.hpp>

#include <juce_audio_devices/juce_audio_devices.h>

#include "../KiwiApp_Components/KiwiApp_Window.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                               DSP DEVICE MANAGER                                 //
    // ================================================================================ //
 
    class DspDeviceManager : public juce::AudioIODeviceCallback,
                             public juce::AudioDeviceManager,
                             public engine::AudioControler
    {
    public: // methods
        
        //! @brief Constructor.
        //! @details Initialized with saved audio settings if previously stored and with
        //! @details default settings if not.
        DspDeviceManager();
        
        //! @brief Destructor.
        ~DspDeviceManager();
        
        // ================================================================================ //
        //                               AudioControler                                     //
        // ================================================================================ //
        
        //! @brief Adds a chain to the dsp device manager
        //! @details If the device is playing the chain will prepared before it's added
        void add(dsp::Chain& chain) override;
        
        //! @brief Removes a chain from the device manager.
        //! @details The chain is removed and released since is tick method is no longer called.
        void remove(dsp::Chain& chain) override;
        
        //! @brief Starts the device.
        void startAudio() override;
        
        //! @brief Stops the device.
        void stopAudio() override;
        
        //! @brief Returns true if the audio is on.
        bool isAudioOn() const override;
        
        //! @brief Adds a buffer to the output matrix of signal.
        void addToChannel(size_t const channel, dsp::Signal const& output_buffer) override;
        
        //! @brief Gets a buffer from the input matrix signal.
        void getFromChannel(size_t const channel, dsp::Signal & input_signal) override;
        
    private: // methods
        
        // ================================================================================ //
        //                               AudioIODeviceCallback                              //
        // ================================================================================ //
        
        //! @internal Processes a block of incoming and outgoing audio data.
        void audioDeviceIOCallback(const float** inputChannelData,
                                   int numInputChannels,
                                   float** outputChannelData,
                                   int numOutputChannels,
                                   int numSamples) override;
        
        //! @internal Called to indicate that the device is about to start calling back.
        void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
        
        //! @internal Called to indicate that the device has stopped.
        void audioDeviceStopped() override;
        
        // ================================================================================ //
        //                                    DSP DEVICE                                    //
        // ================================================================================ //
        
        //! @brief Ticks all the chains.
        //! @details Called at each dsp cycle.
        void tick() const noexcept;
        
    private: // members
        
        std::unique_ptr<dsp::Buffer>                m_input_matrix;
        std::unique_ptr<dsp::Buffer>                m_output_matrix;
        std::vector<dsp::Chain*>                    m_chains;
        bool                                        m_is_playing;
        mutable std::mutex                          m_mutex;
    };
}
