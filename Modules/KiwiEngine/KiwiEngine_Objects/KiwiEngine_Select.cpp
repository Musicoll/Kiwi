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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Select.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    SELECT                                        //
    // ================================================================================ //
    
    void Select::declare()
    {
        Factory::add<Select>("select", &Select::create);
    }
    
    std::unique_ptr<Object> Select::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Select>(model, patcher);
    }
    
    Select::Select(model::Object const& model, Patcher& patcher) :
    Object(model, patcher),
    m_list(model.getArguments())
    {
    }
    
    void Select::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0)
        {
            bool arg_found = false;
            
            for(size_t i = 0; i != m_list.size() && !arg_found; ++i)
            {
                if (args[0].isNumber())
                {
                    if (args[0].getFloat() == m_list[i].getFloat())
                    {
                        send(i, {"bang"});
                        arg_found = true;
                    }
                }
                else if(args[0].isString())
                {
                    if (args[0].getString() == m_list[i].getString())
                    {
                        send(i, {"bang}"});
                        arg_found = true;
                    }
                }
            }
            
            if (!arg_found)
            {
                send(m_list.size(), {"bang"});
            }
        }
        else
        {
            m_list[index - 1] = args[0];
        }
    }
    
}}
