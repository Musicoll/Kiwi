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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Pack.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    PACK                                          //
    // ================================================================================ //
    
    void Pack::declare()
    {
        Factory::add<Pack>("pack", &Pack::create);
    }
    
    std::unique_ptr<Object> Pack::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Pack>(model, patcher);
    }
    
    Pack::Pack(model::Object const& model, Patcher& patcher) :
    Object(model, patcher),
    m_list(model.getArguments())
    {
    }
    
    void Pack::output_list()
    {
        send(0, m_list);
    }
    
    void Pack::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        const bool is_bang = args[0].isBang();
        if (!is_bang)
        {
            switch (m_list[index].getType())
            {
                case tool::Atom::Type::Float:
                {
                    m_list[index] = args[0].getFloat();
                    break;
                }
                case tool::Atom::Type::Int:
                {
                    m_list[index] = args[0].getInt();
                    break;
                }
                case tool::Atom::Type::String:
                {
                    m_list[index] = args[0].getString();
                    break;
                }
                default: break;
            }
        }
        
        if (index == 0 && is_bang)
        {
            output_list();
        }
    }
    
}}
