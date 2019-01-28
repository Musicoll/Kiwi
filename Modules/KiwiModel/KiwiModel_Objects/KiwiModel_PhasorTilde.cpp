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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_PhasorTilde.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                     PHASOR~                                      //
    // ================================================================================ //
    
    void PhasorTilde::declare()
    {
        std::unique_ptr<ObjectClass> phasortilde_class(new ObjectClass("phasor~", &PhasorTilde::create));
        
        flip::Class<PhasorTilde> & phasortilde_model = DataModel::declare<PhasorTilde>()
                                                       .name(phasortilde_class->getModelName().c_str())
                                                       .inherit<Object>();
        
        Factory::add<PhasorTilde>(std::move(phasortilde_class), phasortilde_model);
    }
    
    std::unique_ptr<Object> PhasorTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<PhasorTilde>(args);
    }
    
    PhasorTilde::PhasorTilde(std::vector<tool::Atom> const& args)
    {
        if (args.size() > 1)
        {
            throw Error("phasor~ too many arguments");
        }
        
        if (args.size() == 1 && !args[0].isNumber())
        {
            throw Error("phasor~ frequency must be a number");
        }
        
        pushInlet({PinType::IType::Control, PinType::IType::Signal});
        pushInlet({PinType::IType::Control});
        
        pushOutlet(PinType::IType::Signal);
    }
    
    std::string PhasorTilde::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            if(index == 0)
            {
                return "(signal/float) Set frequency";
            }
            else if(index == 1)
            {
                return "(signal/float) Set phase (0-1)";
            }
        }
        else
        {
            return "(signal) Output";
        }
        
        return {};
    }
    
}}
