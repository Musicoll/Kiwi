/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiApp_DspDeviceManager.hpp"
#include "KiwiApp_StoredSettings.hpp"
#include "KiwiApp.hpp"

#include <juce_audio_utils/juce_audio_utils.h>

namespace kiwi
{
    
    // ================================================================================ //
    //                               DSP DEVICE MANAGER                                 //
    // ================================================================================ //
    
    DspDeviceManager::DspDeviceManager() :
    m_input_matrix(nullptr),
    m_output_matrix(nullptr)
    {
        juce::ScopedPointer<juce::XmlElement> previous_settings(getGlobalProperties().getXmlValue("Audio Settings"));
        
        if (previous_settings)
        {
            initialise(2, 2, previous_settings, false);
        }
        else
        {
            initialiseWithDefaultDevices(2, 2);
        }
        
        //! @todo Find a way to initilise setups without launching audio device.
        stopAudio();
    }
    
    DspDeviceManager::~DspDeviceManager()
    {
        juce::ScopedPointer<juce::XmlElement> data(createStateXml());
        getGlobalProperties().setValue("Audio Settings", data);
        
        stopAudio();
    }
    
    void DspDeviceManager::add(dsp::Chain& chain)
    {
        if(std::find(m_chains.begin(), m_chains.end(), &chain) == m_chains.cend())
        {
            juce::AudioIODevice* device = getCurrentAudioDevice();
            
            if (device && device->isPlaying())
            {
                chain.prepare(device->getCurrentSampleRate(), device->getCurrentBufferSizeSamples());
            }
            
            {
                juce::GenericScopedLock<juce::CriticalSection> lock(getAudioCallbackLock());
                m_chains.push_back(&chain);
            }
        }
    }
    
    void DspDeviceManager::remove(dsp::Chain& chain)
    {
        const auto it = std::find(m_chains.begin(), m_chains.end(), &chain);
        
        if(it != m_chains.cend())
        {
            (*it)->release();
            
            {
                juce::GenericScopedLock<juce::CriticalSection> lock(getAudioCallbackLock());
                m_chains.erase(it);
            }
        }
    }
    
    void DspDeviceManager::startAudio()
    {
        addAudioCallback(this);
        
        closeAudioDevice();
        restartLastAudioDevice();
        
        KiwiApp::commandStatusChanged();
    }
    
    void DspDeviceManager::stopAudio()
    {
        getCurrentAudioDevice()->stop();
        removeAudioCallback(this);
        KiwiApp::commandStatusChanged();
    }
    
    bool DspDeviceManager::isAudioOn() const
    {
        return getCurrentAudioDevice() ? getCurrentAudioDevice()->isPlaying() : false;
    };
    
    void DspDeviceManager::addSignal(dsp::Buffer const& output_buffer)
    {
        if (output_buffer.getNumberOfChannels() == m_output_matrix->getNumberOfChannels()
            && output_buffer.getVectorSize() == m_output_matrix->getVectorSize())
        {
            for(int channel_index = 0; channel_index < m_output_matrix->getNumberOfChannels(); ++channel_index)
            {
                (*m_output_matrix)[channel_index].add(output_buffer[channel_index]);
            }
        }
    }
    
    
    void DspDeviceManager::tick() const noexcept
    {
        for(dsp::Chain* chain : m_chains)
        {
            chain->tick();
        }
    }
    
    void DspDeviceManager::audioDeviceAboutToStart(juce::AudioIODevice *device)
    {
        
        size_t sample_rate = device->getCurrentSampleRate();
        size_t buffer_size = device->getCurrentBufferSizeSamples();
        
        for(dsp::Chain * chain : m_chains)
        {
            chain->prepare(sample_rate, buffer_size);
        }
        
        // allocate input matrix
        m_input_matrix.reset(new dsp::Buffer(device->getActiveInputChannels().getHighestBit() + 1,
                                             device->getCurrentBufferSizeSamples()));
        
        // allocate output matrix
        m_output_matrix.reset(new dsp::Buffer(device->getActiveOutputChannels().getHighestBit() + 1,
                                              device->getCurrentBufferSizeSamples()));
    }
    
    void DspDeviceManager::audioDeviceStopped()
    {
        for(dsp::Chain * chain : m_chains)
        {
            chain->release();
        }
        
        // clear input matrix
        m_input_matrix.reset();
        
        // clear output matrix
        m_output_matrix.reset();
    }
    
    void DspDeviceManager::audioDeviceIOCallback(float const** inputs, int numins,
                                                 float** outputs, int numouts,
                                                 int vector_size)
    {
        //@todo may be pointing to same samples instead of copying them
        
        for(int i = 0; i < numins; ++i)
        {
            dsp::Signal& channel = (*m_input_matrix)[i];
            float const* const input_channel = inputs[i];
            
            for(int j = 0; j < vector_size; ++j)
            {
                channel[j] = input_channel[j];
            }
        }
        
        tick();
        
        for(int i = 0; i < numouts; ++i)
        {
            dsp::Signal const& channel = (*m_output_matrix)[i];
            float* const output_channel = outputs[i];
            
            for(int j = 0; j < vector_size; ++j)
            {
                output_channel[j] = channel[j];
            }
        }
        
        for (int i = 0; i < numouts; ++i)
        {
            (*m_output_matrix)[i].fill(0);
        }
    }
    
    // ================================================================================ //
    //                               AUDIO SETTING WINDOW                               //
    // ================================================================================ //
    
    AudioSettingWindow::AudioSettingWindow(DspDeviceManager& device_manager):
    Window("Audio Settings", juce::Colours::white, allButtons, false)
    {
        juce::AudioDeviceSelectorComponent* audio_settings =
        new juce::AudioDeviceSelectorComponent(device_manager, 1, 20, 1, 20, false, false, false, true);
        
        setContentOwned(audio_settings, false);
        setSize(300, 440);
        setTopLeftPosition(10, 10);
        setResizable(false, false);
    }
    
    void AudioSettingWindow::closeButtonPressed()
    {
        setVisible(false);
    }
}
