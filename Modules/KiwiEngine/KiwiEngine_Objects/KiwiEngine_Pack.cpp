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
        if (args[0].getString() != "bang")
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
                case tool::Atom::Type::Null:
                {
                    break;
                }
            }
        }
        
        if (index == 0 && args[0].isString() && args[0].getString() == "bang")
        {
            output_list();
        }
    }
    
}}
