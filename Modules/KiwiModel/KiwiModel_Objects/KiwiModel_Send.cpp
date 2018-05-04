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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Send.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    SEND                                          //
    // ================================================================================ //
    
    void Send::declare()
    {
        std::unique_ptr<ObjectClass> send_class(new ObjectClass("send", &Send::create));
        
        send_class->addAlias("s");
        
        flip::Class<Send> & send_model = DataModel::declare<Send>()
                                         .name(send_class->getModelName().c_str())
                                         .inherit<Object>();
        
        Factory::add<Send>(std::move(send_class), send_model);
    }
    
    std::unique_ptr<Object> Send::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Send>(args);
    }
    
    Send::Send(std::vector<tool::Atom> const& args)
    {
        if (args.empty())
            throw Error("send requires one argument");
        
        if (args.size() > 1)
            throw Error("send requires only one argument");
        
        if (args.size() == 1 && !args[0].isString())
            throw Error("send argument must be a symbol");
            
        pushInlet({PinType::IType::Control});
    }
    
    std::string Send::getIODescription(bool is_inlet, size_t index) const
    {
        std::string description = "";
        
        if (is_inlet)
        {
            if (index == 0)
            {
                description = "message to send to corresponding receive objects";
            }
        }
            
        return description;
    }
        
}}
