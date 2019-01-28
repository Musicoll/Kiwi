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

#include <KiwiEngine/KiwiEngine_Factory.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_OSCSend.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                   OSC.receive                                    //
    // ================================================================================ //
    
    void OSCSend::declare()
    {
        Factory::add<OSCSend>("OSC.send", &OSCSend::create);
    }
    
    std::unique_ptr<Object> OSCSend::create(model::Object const& model, Patcher& patcher)
    {
        return std::make_unique<OSCSend>(model, patcher);
    }
    
    OSCSend::OSCSend(model::Object const& model, Patcher& patcher)
    : Object(model, patcher)
    {
        const auto& args = model.getArguments();
        if(args.size() >= 2)
        {
            if(args[0].isString())
            {
                m_host = args[0].getString();
            }
            
            if(args[1].isInt())
            {
                m_port = args[1].getInt();
            }
        }
        
        connectToHostAndPort(m_host, m_port);
    }
    
    void OSCSend::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if(args.size() >= 1 && args[0].isString() && args[0].getString() == "connect")
        {
            if(args.size() >= 3 && args[1].isString() && args[2].isInt())
            {
                m_host = args[1].getString();
                m_port = args[2].getInt();
                connectToHostAndPort(m_host, m_port);
            }
        }
        else if(!args.empty())
        {
            // send message
            auto arg = args.begin();
            
            if(arg->isString() && arg->getString().front() == '/')
            {
                juce::OSCAddressPattern ap(arg->getString());
                juce::OSCMessage message(ap);
                
                for(arg++; arg != args.end(); arg++)
                {
                    if(arg->isInt()) {message.addInt32(arg->getInt());}
                    else if(arg->isFloat()) {message.addFloat32(arg->getFloat());}
                    else if(arg->isString()) {message.addString(arg->getString());}
                }
                
                if(!m_sender.sendToIPAddress(m_host, m_port, message))
                {
                    error("OSC.send: can't bind to host " + m_host + " on port " + std::to_string(m_port));
                }
            }
            else
            {
                error("bad OSC message format");
            }
        }
    }
    
    bool OSCSend::connectToHostAndPort(std::string host, int new_port)
    {
        m_sender.disconnect();
        
        if(m_sender.connect(host, new_port))
        {
            return true;
        }
        
        error("OSC.send: can't connect to host " + host + " on port " + std::to_string(new_port));
        return false;
    }
}}
