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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_LineTilde.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                      LINE~                                       //
    // ================================================================================ //
    
    void LineTilde::declare()
    {
        std::unique_ptr<ObjectClass> linetilde_class(new ObjectClass("line~", &LineTilde::create));
        
        flip::Class<LineTilde> & linetilde_model = DataModel::declare<LineTilde>()
                                                   .name(linetilde_class->getModelName().c_str())
                                                   .inherit<Object>();
        
        Factory::add<LineTilde>(std::move(linetilde_class), linetilde_model);
    }
    
    std::unique_ptr<Object> LineTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<LineTilde>(args);
    }
    
    LineTilde::LineTilde(std::vector<tool::Atom> const& args)
    {
        if (args.size() > 1)
        {
            throw Error("line~ too many arguments");
        }
        
        if (args.size() == 1 && !args[0].isNumber())
        {
            throw Error("line~ initial value shall be a number");
        }
        
        pushInlet({PinType::IType::Control});
        pushInlet({PinType::IType::Control});
        pushOutlet(PinType::IType::Signal);
        pushOutlet(PinType::IType::Control);
    }
    
    std::string LineTilde::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            if(index == 0)
            {
                return "(number/list) Destination value";
            }
            else if(index == 1)
            {
                return "(float) Ramp time";
            }
        }
        else
        {
            if(index == 0)
            {
                return "(signal) Output ramp";
            }
            else if(index == 1)
            {
                return "(bang) When the ramp reached its destination";
            }
        }
        
        return {};
    }
    
}}
