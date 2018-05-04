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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_ClipTilde.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  CLIP~                                           //
    // ================================================================================ //
    
    void ClipTilde::declare()
    {
        std::unique_ptr<ObjectClass> cliptilde_class(new ObjectClass("clip~", &ClipTilde::create));
        
        flip::Class<ClipTilde> & cliptilde_model = DataModel::declare<ClipTilde>()
        .name(cliptilde_class->getModelName().c_str())
        .inherit<Object>();
        
        Factory::add<ClipTilde>(std::move(cliptilde_class), cliptilde_model);
    }
    
    std::unique_ptr<Object> ClipTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<ClipTilde>(args);
    }
    
    ClipTilde::ClipTilde(std::vector<tool::Atom> const& args):
    Object()
    {
        if (args.size() > 2)
            throw Error("clip~ only takes two arguments");
        
        if (args.size() > 1 && !args[1].isNumber())
            throw Error("clip~ maximum must be a number");
        
        if(args.size() > 0 && !args[0].isNumber())
            throw Error("clip~ minimum value must be a number");
            
        pushInlet({PinType::IType::Signal});
        pushInlet({PinType::IType::Signal, PinType::IType::Control});
        pushInlet({PinType::IType::Signal, PinType::IType::Control});
        
        pushOutlet(PinType::IType::Signal);
    }
    
    std::string ClipTilde::getIODescription(bool is_inlet, size_t index) const
    {
        std::string description = "";
        
        if (is_inlet)
        {
            if (index == 0)
            {
                description = "clipped input";
            }
            else if (index == 1)
            {
                description = "signal or float, minimum value";
            }
            else if(index == 2)
            {
                description = "signal or float, maximum value";
            }
        }
        else
        {
            description = "clipped signal";
        }
        
        return description;
    }
    
}}
