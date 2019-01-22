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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Select.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    SELECT                                        //
    // ================================================================================ //
    
    void Select::declare()
    {
        auto kiwi_class = std::make_unique<ObjectClass>("select", &Select::create);
        
        kiwi_class->addAlias("sel");
        
        auto& flip_class = (DataModel::declare<Select>()
                            .name(kiwi_class->getModelName().c_str())
                            .inherit<Object>());
        
        Factory::add<Select>(std::move(kiwi_class), flip_class);
    }
    
    std::unique_ptr<Object> Select::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Select>(args);
    }
    
    Select::Select(std::vector<tool::Atom> const& args)
    {
        if (args.empty())
        {
            throw Error("select requires at least one argument");
        }
            
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
                description = "Value to be tested";
            }
            else
            {
                description = "Sets value " + std::to_string(index) + " to be tested";
            }
        }
        else
        {
            auto const& args = getArguments();
            
            if (index == args.size())
            {
                description = "Input if doesn't match";
            }
            else
            {
                description = "Outputs bang if input matches \"" + tool::AtomHelper::toString(args[index]) + "\"";
            }
        }
        
        return description;
    }
    
}}
