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

#include <KiwiTool/KiwiTool_Atom.h>

#include <KiwiEngine/KiwiEngine_Factory.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Print.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    OBJECT PRINT                                  //
    // ================================================================================ //
    
    void Print::declare()
    {
        Factory::add<Print>("print", &Print::create);
    }
    
    std::unique_ptr<Object> Print::create(model::Object const& model, Patcher& patcher)
    {
        return std::make_unique<Print>(model, patcher);
    }
    
    Print::Print(model::Object const& model, Patcher& patcher)
    : Object(model, patcher)
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (!args.empty())
        {
            if (args[0].isFloat())
            {
                m_name = std::to_string(args[0].getFloat());
            }
            else if(args[0].isInt())
            {
                m_name = std::to_string(args[0].getInt());
            }
            else
            {
                m_name = args[0].getString();
            }
        }
        else
        {
            m_name = "print";
        }
    }
    
    void Print::receive(size_t, std::vector<tool::Atom> const& args)
    {
        if(!args.empty())
        {
            post(m_name + " \xe2\x80\xa2 " + tool::AtomHelper::toString(args));
        }
    }
    
}}

