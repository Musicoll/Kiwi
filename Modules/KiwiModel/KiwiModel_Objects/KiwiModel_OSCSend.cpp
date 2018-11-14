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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_OSCSend.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                   OSC.send                                       //
    // ================================================================================ //
    
    void OSCSend::declare()
    {
        auto object_class = std::make_unique<ObjectClass>("OSC.send", &OSCSend::create);
        
        auto& model = DataModel::declare<OSCSend>()
        .name(object_class->getModelName().c_str())
        .inherit<Object>();
        
        Factory::add<OSCSend>(std::move(object_class), model);
    }
    
    std::unique_ptr<Object> OSCSend::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<OSCSend>(args);
    }
    
    OSCSend::OSCSend(std::vector<tool::Atom> const& args)
    {
        pushInlet({PinType::IType::Control});
    }
    
    std::string OSCSend::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet && index == 0)
        {
            return "(atoms) send messages, connect msg to set host and port to listen to";
        }
        
        return {};
    }
    
}}
