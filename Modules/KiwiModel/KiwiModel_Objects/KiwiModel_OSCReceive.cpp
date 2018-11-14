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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_OSCReceive.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                   OSC.receive                                    //
    // ================================================================================ //
    
    void OSCReceive::declare()
    {
        auto object_class = std::make_unique<ObjectClass>("OSC.receive", &OSCReceive::create);
        
        auto& model = DataModel::declare<OSCReceive>()
        .name(object_class->getModelName().c_str())
        .inherit<Object>();
        
        Factory::add<OSCReceive>(std::move(object_class), model);
    }
    
    std::unique_ptr<Object> OSCReceive::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<OSCReceive>(args);
    }
    
    OSCReceive::OSCReceive(std::vector<tool::Atom> const& args)
    {
        if(args.empty())
        {
            pushInlet({PinType::IType::Control});
        }
        
        pushOutlet(PinType::IType::Control);
    }
    
    std::string OSCReceive::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            return "port (int) Set port to listen to";
        }
        
        return "OSC messages received";
    }
    
}}
