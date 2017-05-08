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

#include <juce_audio_utils/juce_audio_utils.h>

#include "KiwiApp_DspDeviceManager.hpp"

#include "../KiwiApp_General/KiwiApp_StoredSettings.hpp"
#include "../KiwiApp.hpp"

namespace kiwi
{
    
    // ================================================================================ //
    //                               DSP DEVICE MANAGER                                 //
    // ================================================================================ //
    
    DspDeviceManager::DspDeviceManager() :
    m_input_matrix(nullptr),
    m_output_matrix(nullptr),
    m_chains(),
    m_is_playing(false),
    m_mutex()
    {
        juce::ScopedPointer<juce::XmlElement> previous_settings(getGlobalProperties().getXmlValue("Audio Settings"));
        
        if (previous_settings)
        {
            initialise(256, 256, previous_settings, false);
        }
        else
        {
            initialiseWithDefaultDevices(256, 256);
        }
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
            if (m_is_playing)
            {
                juce::AudioIODevice * const device = getCurrentAudioDevice();
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
        m_is_playing = true;
        KiwiApp::commandStatusChanged();
    }
    
    void DspDeviceManager::stopAudio()
    {
        removeAudioCallback(this);
        m_is_playing = false;
        KiwiApp::commandStatusChanged();
    }
    
    bool DspDeviceManager::isAudioOn() const
    {
        return m_is_playing;
    };
    
    void DspDeviceManager::addToChannel(size_t const channel, dsp::Signal const& output_signal)
    {
        if (channel < m_output_matrix->getNumberOfChannels() && output_signal.size() == m_output_matrix->getVectorSize())
        {
            (*m_output_matrix)[channel].add(output_signal);
        }
    }
    
    void DspDeviceManager::getFromChannel(size_t const channel, dsp::Signal & input_signal)
    {
        if (channel < m_input_matrix->getNumberOfChannels() && input_signal.size() == m_input_matrix->getVectorSize())
        {
            input_signal.copy((*m_input_matrix)[channel]);
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
            try
            {
                chain->prepare(sample_rate, buffer_size);
            }
            catch(dsp::LoopError & e)
            {
                KiwiApp::use().error(e.what());
            }
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
}
