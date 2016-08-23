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

#include "PortAudioExample.hpp"

namespace kiwi
{
    namespace dsp
    {
        size_t PortAudioExample::s_instances = 0;
        
        PortAudioExample::DeviceInfos::DeviceInfos(PortAudioExample& _device) :
        device(_device),
        nins(device.m_in_params.channelCount),
        inputs(device.m_input_signal ? device.m_input_signal->data() : nullptr),
        nouts(device.m_out_params.channelCount),
        outputs(device.m_output_signal ? device.m_output_signal->data() : nullptr),
        samplerate(device.m_samplerate),
        vectorsize(device.m_vectorsize)
        {
            ;
        }
        
        // ==================================================================================== //
        //                                   DSP PORTAUDIO EXAMPLE                              //
        // ==================================================================================== //
        
        PortAudioExample::PortAudioExample(const size_t samplerate, const size_t vectorsize) :
        m_samplerate(samplerate),
        m_vectorsize(vectorsize),
        m_stream(nullptr)
        {
            if(!s_instances)
            {
                PaError err = Pa_Initialize();
                if(err != paNoError)
                {
                    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << '\n';
                }
            }
            s_instances++;
        }
        
        PortAudioExample::~PortAudioExample()
        {
            stop();
            if(s_instances == 1)
            {
                PaError err = Pa_Terminate();
                if(err != paNoError)
                {
                    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << '\n';
                }
            }
            s_instances--;
        }
        
        size_t PortAudioExample::getSampleRate() const noexcept
        {
            return m_samplerate;
        }
        
        size_t PortAudioExample::getVectorSize() const noexcept
        {
            return m_vectorsize;
        }
        
        void PortAudioExample::start()
        {
            m_in_params.device = Pa_GetDefaultInputDevice();
            if(m_in_params.device == paNoDevice)
            {
                fprintf(stderr, "Error: No default input device.\n");
                return;
            }
            
            m_out_params.device = Pa_GetDefaultOutputDevice();
            if(m_out_params.device == paNoDevice)
            {
                fprintf(stderr, "Error: No default output device.\n");
                return;
            }
            
            m_in_params.channelCount = 2;
            m_out_params.channelCount = 2;
            m_in_params.hostApiSpecificStreamInfo = nullptr;
            m_out_params.hostApiSpecificStreamInfo = nullptr;
            m_in_params.sampleFormat = paFloat32;
            m_out_params.sampleFormat = paFloat32;
            m_in_params.suggestedLatency = Pa_GetDeviceInfo(m_in_params.device)->defaultLowInputLatency;
            m_out_params.suggestedLatency = Pa_GetDeviceInfo(m_out_params.device)->defaultLowOutputLatency;
            
            DeviceInfos device_infos(*this);
            
            PaError err = Pa_OpenStream(&m_stream, &m_in_params, &m_out_params,
                                m_samplerate, m_vectorsize, paClipOff,
                                (PaStreamCallback*)&callback, &device_infos);
            
            if(err != paNoError)
            {
                std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << '\n';
                return;
            }
            
            err = Pa_StartStream(m_stream);
            if (err != paNoError)
            {
                std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << '\n';
                return;
            }
            
            // Loop until the callback returns non-zero
            while((err = Pa_IsStreamActive(m_stream)) == 1)
            {
                //Pa_Sleep(100);
            }
        }
        
        void PortAudioExample::stop()
        {
            if(m_stream)
            {
                if(Pa_IsStreamActive(m_stream))
                {
                    PaError err = Pa_StopStream(m_stream);
                    if(err != paNoError)
                    {
                        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << '\n';
                        err = Pa_AbortStream(m_stream);
                        if(err != paNoError)
                        {
                            std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << '\n';
                        }
                    }
                    
                    while(!Pa_IsStreamStopped(m_stream))
                    {
                        ;
                    }
                    
                    err = Pa_CloseStream(m_stream);
                    m_stream = nullptr;
                }
            }
            
            // destroy IO buffers
            m_input_signal.reset();
            m_output_signal.reset();
        }
        
        int PortAudioExample::callback(void const* input_buffer,
                                       void* output_buffer,
                                       unsigned long vectorsize,
                                       PaStreamCallbackTimeInfo const* time_info,
                                       PaStreamCallbackFlags flags,
                                       void* user_data)
        {
            DeviceInfos* infos = (DeviceInfos*)(user_data);
            
            PortAudioExample& device = infos->device;
            
            device.perform(static_cast<float*>(output_buffer), vectorsize);
            
            return PaStreamCallbackResult::paContinue; // run forever
        }
    }
}
