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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Dsp/KiwiEngine_DacTilde.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                       DAC~                                       //
        // ================================================================================ //
        
        DacTilde::DacTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args):
        AudioInterfaceObject(model, patcher, args)
        {
            std::vector<size_t> routes = parseArgs(args);
            
            for (size_t index = 0; index < routes.size(); ++index)
            {
                m_router.connect(index, routes[index]);
            }
        }
        
        void DacTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            std::set<Router::Cnx> const& connections = m_router.getConnections();
            
            for(Router::Cnx const& connection : connections)
            {
                m_audio_controler.addToChannel(connection.m_output, input[connection.m_input]);
            }
        }
        
        void DacTilde::prepare(dsp::Processor::PrepareInfo const& infos)
        {
            setPerformCallBack(this, &DacTilde::perform);
        }
    }
}
