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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Hub.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT HUB                                      //
    // ================================================================================ //
    
    void Hub::declare()
    {
        // Objectclass
        auto kiwi_class = std::make_unique<ObjectClass>("hub", &Hub::create);
        
        // Parameters
        auto param_message = std::make_unique<ParameterClass>(tool::Parameter::Type::String);
        
        kiwi_class->addAttribute("message", std::move(param_message));
        
        // DataModel
        auto& flip_class = DataModel::declare<Hub>()
        .name(kiwi_class->getModelName().c_str())
        .inherit<Object>()
        .member<flip::Message<std::string>, &Hub::m_message>("message");
        
        Factory::add<Hub>(std::move(kiwi_class), flip_class);
    }
    
    std::unique_ptr<Object> Hub::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Hub>(args);
    }
    
    Hub::Hub(std::vector<tool::Atom> const& args)
    : Object()
    , m_message()
    {
        if (args.size() > 0)
            throw Error("message too many arguments");
        
        pushInlet({PinType::IType::Control});
        pushOutlet(PinType::IType::Control);
        
        m_message.disable_in_undo();
    }
    
    Hub::Hub(flip::Default& d)
    : Object(d)
    , m_message()
    {
        m_message.disable_in_undo();
    }
    
    void Hub::writeAttribute(std::string const& name, tool::Parameter const& parameter)
    {
        if (name == "message")
        {
            m_message.send(parameter[0].getString());
        }
    }
    
    void Hub::readAttribute(std::string const& name, tool::Parameter & parameter) const
    {
        if (name == "message")
        {
            m_message.get([&parameter](bool forward_flag, std::string received_message)
            {
                parameter = tool::Parameter(tool::Parameter::Type::String, {received_message});
            });
        }
    }
    
    bool Hub::attributeChanged(std::string const& name) const
    {
        return name == "message" && m_message.changed();
    }
    
    std::string Hub::getIODescription(bool is_inlet, size_t index) const
    {
        if (is_inlet && index == 0)
        {
            return "Sends message to all users";
        }
        else if(!is_inlet && index == 0)
        {
            return "Outputs message received from other users";
        }
        else
        {
            return "";
        }
    }
}}
