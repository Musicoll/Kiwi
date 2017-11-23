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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Scale.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                     SCALE                                        //
    // ================================================================================ //
    
    void Scale::declare()
    {
        std::unique_ptr<ObjectClass> scale_class(new ObjectClass("scale", &Scale::create));
        
        flip::Class<Scale> & scale_model = DataModel::declare<Scale>().name(scale_class->getModelName().c_str()).inherit<Object>();
        
        Factory::add<Scale>(std::move(scale_class), scale_model);
    }
    
    std::unique_ptr<Object> Scale::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Scale>(args);
    }
    
    Scale::Scale(std::vector<tool::Atom> const& args)
    {
        if (args.size() > 4)
        {
            throw Error("scale too manu arguments");
        }
        
        if (args.size() > 3 && !args[3].isNumber())
        {
            throw Error("scale output high must be a number");
        }
        
        if (args.size() > 2 && !args[2].isNumber())
        {
            throw Error("scale output low must be a number");
        }
        
        if (args.size() > 1 && !args[1].isNumber())
        {
            throw Error("scale intput high must be a number ");
        }
        
        if (args.size() > 0 && !args[0].isNumber())
        {
            throw Error("scale input low must be a number");
        }
        
        pushInlet({PinType::IType::Control});
        pushInlet({PinType::IType::Control});
        pushInlet({PinType::IType::Control});
        pushInlet({PinType::IType::Control});
        pushInlet({PinType::IType::Control});
        
        pushOutlet(PinType::IType::Control);
    }
    
    std::string Scale::getIODescription(bool is_inlet, size_t index) const
    {
        if (is_inlet)
        {
            if (index == 0)
            {
                return "Number to be scaled";
            }
            else if (index == 1)
            {
                return "Sets input low value";
            }
            else if (index == 2)
            {
                return "Sets input high value";
            }
            else if(index == 3)
            {
                return "Sets output low value";
            }
            else if (index == 4)
            {
                return "Sets output high value";
            }
        }
        else
        {
            return "Ouputs the scaling result";
        }
     
        return "";
    }
    
}}
