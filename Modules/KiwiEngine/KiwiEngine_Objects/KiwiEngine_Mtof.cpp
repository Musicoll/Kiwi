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

#include <cmath>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Mtof.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    MTOF                                          //
    // ================================================================================ //
    
    void Mtof::declare()
    {
        Factory::add<Mtof>("mtof", &Mtof::create);
    }
    
    std::unique_ptr<Object> Mtof::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Mtof>(model, patcher);
    }
    
    Mtof::Mtof(model::Object const& model, Patcher& patcher) :
    Object(model, patcher)
    {
    }
    
    void Mtof::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0 && !args.empty())
        {
            if (args[0].isNumber())
            {
                send(0, {440. * std::pow(2., (args[0].getFloat() - 69) / 12.)});
            }
            else
            {
                warning("mtof inlet 1 doesn't understand " + args[0].getString());
            }
            
        }
    }
    
}}
