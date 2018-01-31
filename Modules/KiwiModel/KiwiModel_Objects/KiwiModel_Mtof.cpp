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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Mtof.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    MTOF                                          //
    // ================================================================================ //
    
    void Mtof::declare()
    {
        std::unique_ptr<ObjectClass> mtof_class(new ObjectClass("mtof", &Mtof::create));
        
        flip::Class<Mtof> & mtof_model = DataModel::declare<Mtof>()
                                         .name(mtof_class->getModelName().c_str())
                                         .inherit<Object>();
        
        Factory::add<Mtof>(std::move(mtof_class), mtof_model);
    }
    
    std::unique_ptr<Object> Mtof::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Mtof>(args);
    }
    
    Mtof::Mtof(std::vector<tool::Atom> const& args)
    {
        if (!args.empty())
            throw Error("mtof doesn't take any argument");
            
        pushInlet({PinType::IType::Control});
        pushOutlet(PinType::IType::Control);
    }
    
    std::string Mtof::getIODescription(bool is_inlet, size_t index) const
    {
        std::string description = "";
        
        if (is_inlet)
        {
            if (index == 0)
            {
                description = "midi note to convert";
            }
        }
        else
        {
            if (index == 0)
            {
                description = "ouputs converted midi note";
            }
        }
            
        return description;
    }
        
}}
