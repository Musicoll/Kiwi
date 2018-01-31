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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Operator.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    OPERATOR                                      //
    // ================================================================================ //
    
    void Operator::declare()
    {
        DataModel::declare<Operator>()
                   .name("cicm.kiwi.object.operator")
                   .inherit<Object>();
    }
    
    Operator::Operator(std::vector<tool::Atom> const& args):
    Object()
    {
        if (args.size() > 1)
        {
            throw Error("operator too many arguments");
        }
        
        if (args.size() == 1 && !args[0].isNumber())
        {
            throw Error("operator argument must be a number");
        }
        
        pushInlet({PinType::IType::Control});
        
        if (args.empty())
        {
            pushInlet({PinType::IType::Control});
        }
        
        pushOutlet(PinType::IType::Control);
    }
    
    std::string Operator::getIODescription(bool is_inlet, size_t index) const
    {
        return is_inlet ? (index == 0) ? "Left operand" : "Right operand" : "Result";
    }
}}
