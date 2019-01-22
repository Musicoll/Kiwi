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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Message.h>
#include <KiwiModel/KiwiModel_ObjectClass.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT SLIDER                                   //
    // ================================================================================ //
    
    void Message::declare()
    {
        // Objectclass
        std::unique_ptr<ObjectClass> mess_class(new ObjectClass("message", &Message::create));
        
        // Parameters
        std::unique_ptr<ParameterClass> param_text(new ParameterClass(tool::Parameter::Type::String));
        
        mess_class->addAttribute("text", std::move(param_text));
        
        // Flags
        mess_class->setFlag(ObjectClass::Flag::DefinedSize);
        
        // DataModel
        flip::Class<Message> & message_model = DataModel::declare<Message>()
                                               .name(mess_class->getModelName().c_str())
                                               .inherit<Object>()
                                               .member<flip::String, &Message::m_message_text>("message_text");
        
        Factory::add<Message>(std::move(mess_class), message_model);
    }
    
    std::unique_ptr<Object> Message::create(std::vector<tool::Atom> const& args)
    {
        if (args.size() > 0)
            throw Error("message too many arguments");
            
        return std::make_unique<Message>();
    }
    
    Message::Message():
    Object(),
    m_message_text("")
    {
        setWidth(40);
        setHeight(20.);
        pushInlet({PinType::IType::Control});
        pushOutlet(PinType::IType::Control);
        addSignal<>(Signal::outputMessage, *this);
    }
    
    Message::Message(flip::Default& d):
    Object(d),
    m_message_text("")
    {
        addSignal<>(Signal::outputMessage, *this);
    }
    
    void Message::writeAttribute(std::string const& name, tool::Parameter const& parameter)
    {
        if (name == "text")
        {
            m_message_text = parameter[0].getString();
        }
    }
    
    void Message::readAttribute(std::string const& name, tool::Parameter & parameter) const
    {
        std::string const& message = m_message_text;
        
        parameter = tool::Parameter(tool::Parameter::Type::String, {message});
    }
    
    bool Message::attributeChanged(std::string const& name) const
    {
        bool changed = false;
        
        if (name == "text" && m_message_text.changed())
        {
            changed = true;
        }
        
        return changed;
    }
    
    std::string Message::getIODescription(bool is_inlet, size_t index) const
    {
        if (is_inlet && index == 0)
        {
            return "Trigger the message";
        }
        else if(!is_inlet && index == 0)
        {
            return "Message result";
        }
        else
        {
            return "";
        }
    }
}}
