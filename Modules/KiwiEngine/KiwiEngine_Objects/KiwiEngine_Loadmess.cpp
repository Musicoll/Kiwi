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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Loadmess.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT LOADMESS                                 //
    // ================================================================================ //
    
    void Loadmess::declare()
    {
        Factory::add<Loadmess>("loadmess", &Loadmess::create);
    }
    
    std::unique_ptr<Object> Loadmess::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Loadmess>(model, patcher);
    }
    
    Loadmess::Loadmess(model::Object const& model, Patcher& patcher):
    Object(model, patcher),
    m_args(model.getArguments())
    {
    }
    
    void Loadmess::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (!args.empty() && args[0].isString() && args[0].getString() == "bang")
        {
            send(0ul, m_args);
        }
        else
        {
            warning("loadmess inlet 1 doesn't understane [" + args[0].getString() + "]");
        }
    }
    
    void Loadmess::loadbang()
    {
        defer([this]()
        {
            send(0, m_args);
        });
    }
    
}}

