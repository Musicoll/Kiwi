/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Send.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    SEND                                          //
    // ================================================================================ //
    
    void Send::declare()
    {
        Factory::add<Send>("send", &Send::create);
    }
    
    std::unique_ptr<Object> Send::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Send>(model, patcher);
    }
    
    Send::Send(model::Object const& model, Patcher& patcher) :
    Object(model, patcher),
    m_beacon(patcher.getBeacon(model.getArguments()[0].getString()))
    {
    }
    
    void Send::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0 && !args.empty())
        {
            m_beacon.dispatch(args);
        }
    }
    
}}
