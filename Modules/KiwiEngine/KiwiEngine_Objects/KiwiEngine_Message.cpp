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
    
    Message::Message(model::Object const& object_model, Patcher& patcher)
    : Object(object_model, patcher)
    , m_connection(object_model.getSignal<>(model::Message::Signal::outputMessage)
                   .connect(std::bind(&Message::outputMessage, this)))
    {
        attributeChanged("text", object_model.getAttribute("text"));
    }
    
    
    Message::~Message()
    {
    }
    
    void Message::outputMessage()
    {
        defer([this]()
        {
            sendMessages();
        });
    }
    
    void Message::attributeChanged(std::string const& name, tool::Parameter const& parameter)
    {
        if (name == "text")
        {
            tool::Atom null;
            for(auto& input_args : m_input_args)
            {
                input_args = null;
            }
            
            prepareMessagesForText(parameter[0].getString());
        }
    }
    
    void Message::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0 && !args.empty())
        {
            if(!args[0].isBang())
            {
                for(size_t i = 0; i <= 9; i++)
                {
                    if(i < args.size())
                    {
                        m_input_args[i] = args[i];
                    }
                }
            }
            
            sendMessages();
        }
    }
    
    void Message::prepareMessagesForText(std::string const& text)
    {
        static const int flags = (0
                                  | tool::AtomHelper::ParsingFlags::Comma
                                  | tool::AtomHelper::ParsingFlags::Dollar);
        
        auto atoms = tool::AtomHelper::parse(text, flags);
        
        m_messages.clear();
        if(!atoms.empty())
        {
            std::vector<tool::Atom> atom_sequence;
            bool dollar_flag = false;
            
            for(auto&& atom : atoms)
            {
                if(atom.isComma())
                {
                    if(!atom_sequence.empty())
                    {
                        m_messages.emplace_back(std::move(atom_sequence),
                                                dollar_flag);
                        atom_sequence.clear();
                        dollar_flag = false;
                    }
                }
                else
                {
                    if(atom.isDollar())
                    {
                        dollar_flag = true;
                    }
                    
                    atom_sequence.emplace_back(std::move(atom));
                }
            }
            
            if(!atom_sequence.empty())
            {
                m_messages.emplace_back(std::move(atom_sequence),
                                        dollar_flag);
            }
        }
    }
    
    void Message::sendMessages()
    {
        for(auto const& message : m_messages)
        {
            if(message.has_dollar)
            {
                std::vector<tool::Atom> atoms = message.atoms;
                
                for(auto& atom : atoms)
                {
                    if(atom.isDollar())
                    {
                        const size_t index = atom.getDollarIndex() - 1;
                        auto& atom_arg = m_input_args[index];
                        atom = !atom_arg.isNull() ? atom_arg : 0;
                    }
                }
                
                send(0, atoms);
            }
            else
            {
                send(0, message.atoms);
            }
        }
    }
    
}}

