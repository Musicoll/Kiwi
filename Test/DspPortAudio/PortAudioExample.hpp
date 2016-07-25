/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#include <portaudio.h>
#include <cstdio>

#include <KiwiDsp/KiwiDsp_Chain.hpp>

namespace kiwi
{
    namespace dsp
    {
        // ==================================================================================== //
        //                                   DSP PORTAUDIO EXAMPLE                              //
        // ==================================================================================== //
        
        class PortAudioExample
        {
        public: // methods
            
            //! @brief Constructor
            PortAudioExample(const size_t samplerate, const size_t vectorsize);
            
            //! @brief Destructor.
            virtual ~PortAudioExample();
            
            //! @brief Return the sample rate.
            size_t getSampleRate() const noexcept;
            
            //! @brief Return the signal vector size.
            size_t getVectorSize() const noexcept;
            
            //! @brief Start signal processing.
            void start();
            
            //! @brief Stop signal processing.
            void stop();
            
        private: // methods
            
            struct DeviceInfos
            {
                PortAudioExample&   device;
                const size_t        nins;
                sample_t *const     inputs;
                const size_t        nouts;
                sample_t *const     outputs;
                const size_t        samplerate;
                const size_t        vectorsize;
                
                DeviceInfos(PortAudioExample& device);
            };
            
            //! @brief called by the portaudio callback method.
            virtual void perform(float*	out, unsigned long vectorsize) = 0;
            
            //! @internal portaudio callback method.
            static int callback(void const* input_buffer,
                                void* output_buffer,
                                unsigned long vectorsize,
                                PaStreamCallbackTimeInfo const* time_info,
                                PaStreamCallbackFlags flags,
                                void* user_data);
            
        private: // members
            
            const size_t        m_samplerate;
            const size_t        m_vectorsize;
            PaStream*           m_stream;
            PaStreamParameters  m_in_params;
            PaStreamParameters  m_out_params;
            Signal::uPtr        m_input_signal;
            Signal::uPtr        m_output_signal;
            
            static size_t s_instances;
        };
    }
}
