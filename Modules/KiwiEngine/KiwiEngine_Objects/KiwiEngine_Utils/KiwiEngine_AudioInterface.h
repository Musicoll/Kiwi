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

#include <KiwiEngine/KiwiEngine_AudioControler.h>
#include <KiwiEngine/KiwiEngine_Object.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Utils/KiwiEngine_Router.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                       AUDIO_INTERFACE                            //
        // ================================================================================ //
        
        class AudioInterfaceObject : public AudioObject
        {
        public: // methods
            
            AudioInterfaceObject(model::Object const& model, Patcher& patcher, std::vector<tool::Atom> const& args);
            
            void receive(size_t index, std::vector<tool::Atom> const & args) override final;
            
            std::vector<size_t> parseArgs(std::vector<tool::Atom> const& args) const;
            
            virtual ~AudioInterfaceObject() = default;
            
        protected: // members
            
            Router                      m_router;
            engine::AudioControler&     m_audio_controler;
        };
    }
}
