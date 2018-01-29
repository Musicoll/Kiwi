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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Unpack.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    UNPACK                                        //
    // ================================================================================ //
    
    void Unpack::declare()
    {
        std::unique_ptr<ObjectClass> unpack_class(new ObjectClass("unpack", &Unpack::create));
        
        flip::Class<Unpack> & unpack_model = DataModel::declare<Unpack>()
                                                        .name(unpack_class->getModelName().c_str())
                                                        .inherit<Object>();
        
        Factory::add<Unpack>(std::move(unpack_class), unpack_model);
    }
    
    std::unique_ptr<Object> Unpack::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Unpack>(args);
    }
    
    Unpack::Unpack(std::vector<tool::Atom> const& args)
    {
        pushInlet({PinType::IType::Control});
        
        for(size_t i = 0; i < args.size(); ++i)
        {
            pushOutlet(PinType::IType::Control);
        }
    }
    
    std::string Unpack::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            return "list to be unpacked";
        }
        else
        {
            std::vector<tool::Atom> const& args = getArguments();
            
            const std::string type_str = [atom = args[index]]() {
                switch(atom.getType())
                {
                    case tool::Atom::Type::Int: return "int";
                    case tool::Atom::Type::Float: return "float";
                    case tool::Atom::Type::String: return "symbol";
                    default: return "null";
                }
            }();
            
            return "element " + std::to_string(index + 1) + " in list (" + type_str + ")";
        }

        
        return {};
    }
    
}}
