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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Operator.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    OBJECT OPERATOR                               //
    // ================================================================================ //
    
    Operator::Operator(model::Object const& model, Patcher& patcher):
    Object(model, patcher),
    m_lhs(),
    m_rhs()
    {
        
        std::vector<tool::Atom> const& args = model.getArguments();
        
        m_lhs = 0.;
        
        if(!args.empty() && args[0].isNumber())
        {
            m_rhs = args[0].getFloat();
        }
    }
    
    void Operator::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if(!args.empty())
        {
            if(index == 0)
            {
                if (args[0].isNumber())
                {
                    m_lhs = args[0].getFloat();
                    bang();
                }
                else if(args[0].isBang())
                {
                    bang();
                }
                else
                {
                    warning("operator inlet 1 parameter must be a number or bang");
                }
            }
            else if(index == 1)
            {
                if (args[0].isNumber())
                {
                    m_rhs = args[0].getFloat();
                }
                else
                {
                    warning("operator inlet 2 parameter must be a number");
                }
            }
        }
    }
    
    void Operator::bang()
    {
        send(0, {compute(m_lhs, m_rhs)});
    }
    
}}
