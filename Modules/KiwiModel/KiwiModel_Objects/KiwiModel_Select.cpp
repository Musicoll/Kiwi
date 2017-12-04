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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Select.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    SELECT                                        //
    // ================================================================================ //
    
    void Select::declare()
    {
        std::unique_ptr<ObjectClass> select_class(new ObjectClass("select", &Select::create));
        
        flip::Class<Select> & select_model = DataModel::declare<Select>()
                                            .name(select_class->getModelName().c_str())
                                            .inherit<Object>();
        
        Factory::add<Select>(std::move(select_class), select_model);
    }
    
    std::unique_ptr<Object> Select::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Select>(args);
    }
    
    Select::Select(std::vector<tool::Atom> const& args)
    {
        pushInlet({PinType::IType::Control});
        
        for(size_t i = 0; i < args.size(); ++i)
        {
            pushOutlet(PinType::IType::Control);
            pushInlet({PinType::IType::Control});
        }
        
        pushOutlet(PinType::IType::Control);
    }
    
    std::string Select::getIODescription(bool is_inlet, size_t index) const
    {
        std::string description = "";
        
        if (is_inlet)
        {
            if (index == 0)
            {
                description = "value to be tested";
            }
            else
            {
                description = "sets value " + std::to_string(index) + " to be tested";
            }
        }
        else
        {
            if (index == getArguments().size())
            {
                description = "ouptuts bang if input doesn't match";
            }
            else
            {
                description = "outputs bang if input matches " + getArguments()[index].getString();
            }
        }
        
        return description;
    }
    
}}
