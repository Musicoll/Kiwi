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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Pack.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    PACK                                          //
    // ================================================================================ //
    
    void Pack::declare()
    {
        std::unique_ptr<ObjectClass> pack_class(new ObjectClass("pack", &Pack::create));
        
        flip::Class<Pack> & pack_model = DataModel::declare<Pack>()
                                         .name(pack_class->getModelName().c_str())
                                         .inherit<Object>();
        
        Factory::add<Pack>(std::move(pack_class), pack_model);
    }
    
    std::unique_ptr<Object> Pack::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Pack>(args);
    }
    
    Pack::Pack(std::vector<tool::Atom> const& args)
    {
        pushOutlet({PinType::IType::Control});
        
        for(size_t i = 0; i < args.size(); ++i)
        {
            pushInlet({PinType::IType::Control});
        }
    }
    
    std::string Pack::getIODescription(bool is_inlet, size_t index) const
    {
        if(!is_inlet)
        {
            return "Combined list from input";
        }
        else if (is_inlet)
        {
            std::vector<tool::Atom> const& args = getArguments();
            
            std::string type = "";
            
            switch (args[index].getType())
            {
                case tool::Atom::Type::Float:
                {
                    type = "float";
                    break;
                }
                case tool::Atom::Type::Int:
                {
                    type = "int";
                    break;
                }
                case tool::Atom::Type::String:
                {
                    type = "symbol";
                    break;
                }
                case tool::Atom::Type::Null:
                {
                    type = "void";
                    break;
                }
            }
            
            return type + "to be element" + std::to_string(index + 1) + " int list";
        }
        
        return {};
    }
    
}}
