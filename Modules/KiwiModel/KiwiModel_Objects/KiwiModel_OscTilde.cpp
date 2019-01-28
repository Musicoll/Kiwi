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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_OscTilde.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT OSC~                                     //
    // ================================================================================ //
    
    void OscTilde::declare()
    {
        std::unique_ptr<ObjectClass> osctilde_class(new ObjectClass("osc~", &OscTilde::create));
        
        flip::Class<OscTilde> & osctilde_model = DataModel::declare<OscTilde>()
                                                 .name(osctilde_class->getModelName().c_str())
                                                 .inherit<Object>();
        
        Factory::add<OscTilde>(std::move(osctilde_class), osctilde_model);
    }
    
    std::unique_ptr<Object> OscTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<OscTilde>(args);
    }
    
    OscTilde::OscTilde(std::vector<tool::Atom> const& args):
    model::Object()
    {
        if (args.size() > 1)
            throw Error("osc~ too many arguments");
        
        if (args.size() == 1 && !args[0].isNumber())
            throw Error("osc~ wrong arguments [" + args[0].getString() + "]");
            
        pushInlet({PinType::IType::Control, PinType::IType::Signal});
        pushInlet({PinType::IType::Control, PinType::IType::Signal});
        
        pushOutlet(PinType::IType::Signal);
    }
    
    std::string OscTilde::getIODescription(bool is_inlet, size_t index) const
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
