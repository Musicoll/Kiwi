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

#include <KiwiEngine/KiwiEngine_Factory.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_OSCReceive.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                   OSC.receive                                    //
    // ================================================================================ //
    
    void OSCReceive::declare()
    {
        Factory::add<OSCReceive>("OSC.receive", &OSCReceive::create);
    }
    
    std::unique_ptr<Object> OSCReceive::create(model::Object const& model, Patcher& patcher)
    {
        return std::make_unique<OSCReceive>(model, patcher);
    }
    
    OSCReceive::OSCReceive(model::Object const& model, Patcher& patcher)
    : Object(model, patcher)
    {
        const auto& args = model.getArguments();
        if(args.size() >= 1 && args[0].isInt())
        {
            setPort(args[0].getInt());
        }
    }
    
    void OSCReceive::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if(index == 0 && args.size() >= 2 && args[0].getString() == "port" && args[1].isInt())
        {
            setPort(args[1].getInt());
        }
    }
    
    bool OSCReceive::setPort(int new_port)
    {
        removeListener(this);
        disconnect();
        
        if (connect(new_port))
        {
            addListener(this);
            return true;
        }
        
        error("OSC.receive: can't connect to port " + std::to_string(new_port));
        return false;
    }
    
    tool::Atom OSCReceive::OSCArgToAtom(juce::OSCArgument const& arg)
    {
        if (arg.isFloat32()) return arg.getFloat32();
        if (arg.isInt32()) return arg.getInt32();
        if (arg.isString()) return arg.getString().toStdString();
        return {"error"};
    }
    
    void OSCReceive::oscMessageReceived(juce::OSCMessage const& msg)
    {
        std::vector<tool::Atom> msg_out {msg.getAddressPattern().toString().toStdString()};
        
        for(auto const& arg : msg)
        {
            msg_out.emplace_back(OSCArgToAtom(arg));
        }
        
        defer([this, msg_out = std::move(msg_out)]() { send(0, msg_out); });
    }
    
    void OSCReceive::oscBundleReceived(juce::OSCBundle const& bundle)
    {
        for (auto& element : bundle)
        {
            if(element.isMessage())
            {
                oscMessageReceived(element.getMessage());
            }
            else if (element.isBundle())
            {
                oscBundleReceived(element.getBundle());
            }
        }
    }
}}
