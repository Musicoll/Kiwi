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

#ifndef KIWI_ENGINE_AUDIO_CONTROLER_HPP_INCLUDED
#define KIWI_ENGINE_AUDIO_CONTROLER_HPP_INCLUDED

#include <KiwiDsp/KiwiDsp_Chain.hpp>
#include <KiwiDsp/KiwiDsp_Signal.hpp>

namespace kiwi
{
    namespace engine
    {
        //! @brief AudioControler is a pure interface that enable controling audio in kiwi.
        //! @details AudioControler enables the engine to control audio without knowing
        //! it's implementation depending on other libraries.
        //! @see DspDeviceManager
        class AudioControler
        {
        public: // methods
            
            //! @brief the default constructor
            AudioControler() = default;
            
            //! @brief The destuctor.
            virtual ~AudioControler() = default;
            
            //! @brief Starts the audio thread.
            virtual void startAudio() = 0;
            
            //! @brief Stops the audio thread.
            virtual void stopAudio() = 0;
            
            //! @brief Returns true if the audio is on.
            virtual bool isAudioOn() const = 0;
            
            //! @brief Adds a chain to be ticked by the audio thread.
            virtual void add(dsp::Chain& chain) = 0;
            
            //! @brief Removes a chain from the ticked chains list.
            virtual void remove(dsp::Chain& chain) = 0;
            
            //! @brief Adds a signal to the output_buffer of the AudioControler.
            virtual void addToChannel(size_t const channel, dsp::Signal const& output_signal) = 0;
            
            //! @brief Gets a signal from one of the input channels of the AudioControler.
            virtual void getFromChannel(size_t const channel, dsp::Signal & input_signal) = 0;
            
        private: // deleted methods
            
            AudioControler(AudioControler const& other) = delete;
            AudioControler(AudioControler && other) = delete;
            AudioControler& operator=(AudioControler const& other) = delete;
            AudioControler& operator=(AudioControler && other) = delete;
        };
    }
}

#endif // KIWI_ENGINE_AUDIO_CONTROLER_HPP_INCLUDED
