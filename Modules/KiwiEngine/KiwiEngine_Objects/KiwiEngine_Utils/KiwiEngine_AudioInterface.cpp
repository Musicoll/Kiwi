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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Utils/KiwiEngine_AudioInterface.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                       AUDIOINTERFACE                             //
        // ================================================================================ //
        
        AudioInterfaceObject::AudioInterfaceObject(model::Object const& model,
                                                   Patcher& patcher,
                                                   std::vector<Atom> const& args):
        AudioObject(model, patcher),
        m_router(),
        m_audio_controler(patcher.getAudioControler())
        {
        }
        
        std::vector<size_t> AudioInterfaceObject::parseArgs(std::vector<Atom> const& args) const
        {
            std::vector<size_t> routes;
            
            for(Atom const& arg : args)
            {
                if (arg.isNumber())
                {
                    routes.push_back(arg.getInt() - 1);
                }
                else if(arg.isString())
                {
                    std::string inputs(arg.getString());
                    
                    int left_input = std::stoi(inputs.substr(0, inputs.find(":"))) - 1;
                    int right_input = std::stoi(inputs.substr(inputs.find(":") + 1)) - 1;
                    
                    bool rev = left_input > right_input;
                    
                    for (int channel = left_input; rev ? channel >= right_input : channel <= right_input; rev ? --channel : ++channel)
                    {
                        routes.push_back(channel);
                    }
                }
            }
            
            if (routes.empty())
            {
                routes = {0, 1};
            }
            
            return routes;
        }
        
        void AudioInterfaceObject::receive(size_t index, std::vector<Atom> const & args)
        {
            if(!args.empty())
            {
                if(args[0].isString())
                {
                    const std::string sym = args[0].getString();
                    
                    if(sym == "start")
                    {
                        m_audio_controler.startAudio();
                    }
                    else if(sym == "stop")
                    {
                        m_audio_controler.stopAudio();
                    }
                }
            }
        }
    }
}
