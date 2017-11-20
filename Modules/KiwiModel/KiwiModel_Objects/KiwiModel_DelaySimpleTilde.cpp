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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_DelaySimpleTilde.h>
#include <KiwiModel/KiwiModel_Factory.h>
#include <KiwiModel/KiwiModel_DataModel.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                      OBJECT DELAYSIMPLE~                         //
    // ================================================================================ //
    
    void DelaySimpleTilde::declare()
    {
        std::unique_ptr<ObjectClass> delaytilde_class(new ObjectClass("delaysimple~",
                                                                      &DelaySimpleTilde::create));
        
        flip::Class<DelaySimpleTilde> & delaytilde_model = DataModel::declare<DelaySimpleTilde>()
                                                           .name(delaytilde_class->getModelName().c_str())
                                                           .inherit<Object>();
        
        Factory::add<DelaySimpleTilde>(std::move(delaytilde_class), delaytilde_model);
    }
    
    std::unique_ptr<Object> DelaySimpleTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<DelaySimpleTilde>(args);
    }
    
    DelaySimpleTilde::DelaySimpleTilde(std::vector<tool::Atom> const& args)
    {
        if (args.size() > 2)
        {
            throw Error("delaysimple~ too many arguments");
        }
        
        if (args.size() > 0 && !args[0].isNumber())
        {
            throw Error("delaysimple~ delay argument is not a number");
        }
        
        if (args.size() >  1 && !args[1].isNumber())
        {
            throw Error("delaysimple~ feedback argument is not a number ");
        }
        
        pushInlet({PinType::IType::Control, PinType::IType::Signal});
        pushInlet({PinType::IType::Control, PinType::IType::Signal});
        pushInlet({PinType::IType::Control});
        
        pushOutlet(PinType::IType::Signal);
    }
    
    std::string DelaySimpleTilde::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            if(index == 0)
            {
                return "(signal) Input to be delayed";
            }
            else if(index == 1)
            {
                return "(signal/float) Delay time (ms)";
            }
            else if(index == 2)
            {
                return "(float) Feedback (0-1)";
            }
        }
        else
        {
            return "(signal) Delayed output signal";
        }
        
        return {};
    }
    
}}
