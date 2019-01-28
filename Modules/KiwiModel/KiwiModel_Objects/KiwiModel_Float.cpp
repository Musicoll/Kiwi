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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Float.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    FLOAT                                         //
    // ================================================================================ //
    
    void Float::declare()
    {
        std::unique_ptr<ObjectClass> float_class(new ObjectClass("float",
                                                                 &Float::create));
        
        float_class->addAlias("f");
        
        flip::Class<Float> & float_model = DataModel::declare<Float>()
                                           .name(float_class->getModelName().c_str())
                                           .inherit<Object>();
        
        Factory::add<Float>(std::move(float_class), float_model);
    }
    
    std::unique_ptr<Object> Float::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Float>(args);
    }
    
    Float::Float(std::vector<tool::Atom> const& args)
    {
        if (args.size() > 1)
            throw Error("float only take one argument");
        
        if (args.size() > 0 && !args[0].isNumber())
            throw Error("float initial value must be a number");
        
        pushInlet({PinType::IType::Control});
        pushInlet({PinType::IType::Control});
        
        pushOutlet(PinType::IType::Control);
    }
    
    std::string Float::getIODescription(bool is_inlet, size_t index) const
    {
        std::string description = "";
        
        if (is_inlet)
        {
            if (index == 0)
            {
                description = "Store and output value. Bang to ouput";
            }
            else if(index == 1)
            {
                description = "Store value without output";
            }
        }
        else
        {
            if (index == 0)
            {
                description = "Ouput stored value";
            }
        }
        
        return description;
    }
    
}}
