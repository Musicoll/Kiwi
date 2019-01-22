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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_GateTilde.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    GATE~                                         //
    // ================================================================================ //
    
    void GateTilde::declare()
    {
        std::unique_ptr<ObjectClass> gatetilde_class(new ObjectClass("gate~", &GateTilde::create));
        
        flip::Class<GateTilde> & gatetilde_model = DataModel::declare<GateTilde>()
                                                   .name(gatetilde_class->getModelName().c_str())
                                                   .inherit<Object>();
        
        Factory::add<GateTilde>(std::move(gatetilde_class), gatetilde_model);
    }
    
    std::unique_ptr<Object> GateTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<GateTilde>(args);
    }
    
    GateTilde::GateTilde(std::vector<tool::Atom> const& args)
    {
        if (args.size() <= 0)
            throw Error("gate~ number of gates must be specified");
            
        if (args.size() > 2)
            throw Error("gate~ takes at most 2 arguments");
        
        if (args.size() > 1 && !args[1].isNumber())
            throw Error("gate~ 2nd argument must be a number");
        
        if (args.size() > 0)
        {
            if (!args[0].isInt())
            {
                throw Error("gate~ 1rst argument must be an integer");
            }
            else if(args[0].getInt() < 0)
            {
                throw Error("gate~ 1rst argument must be greater than 0");
            }
        }
        
        pushInlet({PinType::IType::Control, PinType::IType::Signal});
        pushInlet({PinType::IType::Signal});
        
        for(int outlet = 0; outlet < args[0].getInt(); ++outlet)
        {
            pushOutlet(PinType::IType::Signal);
        }
    }
    
    std::string GateTilde::getIODescription(bool is_inlet, size_t index) const
    {
        std::string description = "";
        
        if (is_inlet)
        {
            if (index == 0)
            {
                description = "Int Open/Close output";
            }
            else if(index == 1)
            {
                description = "Sends signal to opened output";
            }
        }
        else
        {
            description = "Output " + std::to_string(index) + " receives signal if opened";
        }
        
        return description;
    }
    
}}
