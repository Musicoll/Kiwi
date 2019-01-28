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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_SwitchTilde.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    SWITCH~                                       //
    // ================================================================================ //
    
    void SwitchTilde::declare()
    {
        std::unique_ptr<ObjectClass> switchtilde_class(new ObjectClass("switch~", &SwitchTilde::create));
        
        flip::Class<SwitchTilde> & switchtilde_model = DataModel::declare<SwitchTilde>()
                                                      .name(switchtilde_class->getModelName().c_str())
                                                      .inherit<Object>();
        
        Factory::add<SwitchTilde>(std::move(switchtilde_class), switchtilde_model);
    }
    
    std::unique_ptr<Object> SwitchTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<SwitchTilde>(args);
    }
    
    SwitchTilde::SwitchTilde(std::vector<tool::Atom> const& args)
    {
        if (args.size() <= 0)
            throw Error("switch~ number of inputs must be specified");
            
        if (args.size() > 2)
            throw Error("switch~ takes at most 2 arguments");
        
        if (args.size() > 1 && !args[1].isNumber())
            throw Error("switch~ 2nd argument must be a number");
        
        if (args.size() > 0)
        {
            if (!args[0].isInt())
            {
                throw Error("switch~ 1rst argument must be an integer");
            }
            else if(args[0].getInt() < 0)
            {
                throw Error("switch~ 1rst argument must be greater than 0");
            }
        }
        
        pushInlet({PinType::IType::Control, PinType::IType::Signal});
        pushOutlet(PinType::IType::Signal);
        
        for(int inlet = 0; inlet < args[0].getInt(); ++inlet)
        {
            pushInlet({PinType::IType::Signal});
        }
    }
    
    std::string SwitchTilde::getIODescription(bool is_inlet, size_t index) const
    {
        std::string description = "";
        
        if (is_inlet)
        {
            if (index == 0)
            {
                description = "Int Open/Close input";
            }
            else
            {
                description = "Input " + std::to_string(index) + " sends signal if opened";
            }
        }
        else
        {
            if (index == 0)
            {
                description = "Output sends signal from opened input";
            }
        }
        
        return description;
    }
    
}}
