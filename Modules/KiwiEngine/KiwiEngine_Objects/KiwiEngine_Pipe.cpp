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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Pipe.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT PIPE                                     //
    // ================================================================================ //
    
    void Pipe::declare()
    {
        Factory::add<Pipe>("pipe", &Pipe::create);
    }
    
    std::unique_ptr<Object> Pipe::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Pipe>(model, patcher);
    }
    
    Pipe::Pipe(model::Object const& model, Patcher& patcher):
    Object(model, patcher),
    m_delay(std::chrono::milliseconds(0))
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (!args.empty())
        {
            m_delay = std::chrono::milliseconds(args[0].getInt());
        }
    }
    
    Pipe::~Pipe()
    {
    }
    
    void Pipe::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (!args.empty())
        {
            if (index == 0)
            {
                schedule([this, args](){send(0, args);}, m_delay);
            }
            else if(index == 1)
            {
                if (args[0].isNumber())
                {
                    m_delay = std::chrono::milliseconds(args[0].getInt());
                }
                else
                {
                    warning("pipe inlet 2 doesn't understand [" + args[0].getString() + "]");
                }
            }
        }
    }
    
}}

