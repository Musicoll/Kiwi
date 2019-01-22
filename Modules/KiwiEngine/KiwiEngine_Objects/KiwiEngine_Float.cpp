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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Float.h>

#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    FLOAT                                         //
    // ================================================================================ //
    
    void Float::declare()
    {
        Factory::add<Float>("float", &Float::create);
    }
    
    std::unique_ptr<Object> Float::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Float>(model, patcher);
    }
    
    Float::Float(model::Object const& model, Patcher& patcher):
    Object(model, patcher),
    m_stored_value()
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (args.size() > 0 && args[0].isNumber())
        {
            m_stored_value = args[0].getFloat();
        }
    }
    
    void Float::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (!args.empty())
        {
            if (index == 0)
            {
                if (args[0].isNumber())
                {
                    m_stored_value = args[0].getFloat();
                    send(0, {m_stored_value});
                }
                else if(args[0].isBang())
                {
                    send(0, {m_stored_value});
                }
                else
                {
                    warning("float inlet 1 only understand numbers");
                }
            }
            else if(index == 1)
            {
                if (args[0].isNumber())
                {
                    m_stored_value = args[0].getFloat();
                }
                else
                {
                    warning("float inlet 2 only understand numbers");
                }
            }
        }
    }
    
}}
