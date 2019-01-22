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

#include <string>

#include <KiwiModel/KiwiModel_Factory.h>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Switch.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    SWITCH                                        //
    // ================================================================================ //
    
    void Switch::declare()
    {
        std::unique_ptr<ObjectClass> switch_class(new ObjectClass("switch", &Switch::create));
        
        flip::Class<Switch> & switch_model = DataModel::declare<Switch>()
                                             .name(switch_class->getModelName().c_str())
                                             .inherit<Object>();
        
        Factory::add<Switch>(std::move(switch_class), switch_model);
    }
    
    std::unique_ptr<Object> Switch::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Switch>(args);
    }
    
    Switch::Switch(std::vector<tool::Atom> const& args)
    {
        if (args.size() <= 0)
            throw Error("switch number of inputs must be specified");
            
        if (args.size() > 2)
            throw Error("switch takes at most 2 arguments");
        
        if (args.size() > 1 && !args[1].isNumber())
            throw Error("switch 2nd argument must be a number");
        
        if (args.size() > 0)
        {
            if (!args[0].isInt())
            {
                throw Error("switch 1rst argument must be an integer");
            }
            else if(args[0].getInt() < 0)
            {
                throw Error("switch 1rst argument must be greater than 0");
            }
        }
        
        pushInlet({PinType::IType::Control});
        
        pushOutlet(PinType::IType::Control);
        
        if (args.empty())
        {
            pushInlet({PinType::IType::Control});
        }
        else
        {
            for(int inlet = 0; inlet < args[0].getInt(); ++inlet)
            {
                pushInlet({PinType::IType::Control});
            }
        }
    }
    
    std::string Switch::getIODescription(bool is_inlet, size_t index) const
    {
        std::string description = "";
        
        if (is_inlet)
        {
            if (index == 0)
            {
                description = "Integer opens inlet. 0 closes all inlets";
            }
            else if(index < getNumberOfInlets())
            {
                description = "Input " + std::to_string(index) + " sends message when inlet is opened";
            }
        }
        else
        {
            description = "Sends message received in opened inlet";
        }
        
        return description;
    }
    
}}
