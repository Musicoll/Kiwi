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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_SahTilde.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                      SAH~                                        //
    // ================================================================================ //
    
    void SahTilde::declare()
    {
        std::unique_ptr<ObjectClass> sahtilde_class(new ObjectClass("sah~", &SahTilde::create));
        
        flip::Class<SahTilde> & sahtilde_model = DataModel::declare<SahTilde>().name(sahtilde_class->getModelName().c_str()).inherit<Object>();
        
        Factory::add<SahTilde>(std::move(sahtilde_class), sahtilde_model);
    }
    
    
    std::unique_ptr<Object> SahTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<SahTilde>(args);
    }
    
    SahTilde::SahTilde(std::vector<tool::Atom> const& args)
    {
        if (args.size() > 1)
        {
            throw Error("sah~ too many arguments");
        }
        
        if (args.size() == 1 && !args[0].isNumber())
        {
            throw Error("sah~ threshold shall be a number");
        }
        
        pushInlet({PinType::IType::Signal});
        pushInlet({PinType::IType::Control, PinType::IType::Signal});
        
        pushOutlet(PinType::IType::Signal);
    }
    
    std::string SahTilde::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            if(index == 0)
            {
                return "(signal) Values to sample";
            }
            else if(index == 1)
            {
                return "(signal/control) Trigger input";
            }
        }
        else
        {
            return "(signal) Output sampled value";
        }
        
        return {};
    }
    
}}
