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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Message.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Message.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT MESSAGE                                  //
    // ================================================================================ //
    
    void Message::declare()
    {
        Factory::add<Message>("message", &Message::create);
    }
    
    std::unique_ptr<Object> Message::create(model::Object const& model, Patcher& patcher)
    {
        return std::make_unique<Message>(model, patcher);
    }
    
    Message::Message(model::Object const& object_model, Patcher& patcher):
    Object(object_model, patcher),
    m_text(object_model.getAttribute("text")[0].getString()),
    m_connection(object_model.getSignal<>(model::Message::Signal::outputMessage)
                 .connect(std::bind(&Message::outputMessage, this)))
    {
    }
    
    
    Message::~Message()
    {
    }
    
    void Message::outputMessage()
    {
        defer([this]()
        {
            send(0, tool::AtomHelper::parse(m_text));
        });
    }
    
    void Message::attributeChanged(std::string const& name, tool::Parameter const& parameter)
    {
        if (name == "text")
        {
            m_text = parameter[0].getString();
        }
    }
    
    void Message::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0 && !args.empty())
        {
            send(0, tool::AtomHelper::parse(m_text));;
        }
    }
    
}}

