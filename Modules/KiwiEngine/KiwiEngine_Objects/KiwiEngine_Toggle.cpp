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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Toggle.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Toggle.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT TOGGLE                                   //
    // ================================================================================ //
    
    void Toggle::declare()
    {
        Factory::add<Toggle>("toggle", &Toggle::create);
    }
    
    std::unique_ptr<Object> Toggle::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Toggle>(model, patcher);
    }
    
    Toggle::Toggle(model::Object const& model, Patcher& patcher):
    Object(model, patcher),
    m_connection(model.getSignal<>(model::Toggle::Signal::OutputValue)
                                  .connect(std::bind(&Toggle::outputValue, this))),
    m_is_on(false)
    {
    }
    
    Toggle::~Toggle()
    {
    }
    
    void Toggle::parameterChanged(std::string const& name, tool::Parameter const& param)
    {
        if (name == "value")
        {
            m_is_on = static_cast<bool>(param[0].getInt());
        }
    }
    
    void Toggle::outputValue()
    {
        defer([this]()
        {
            send(0, {m_is_on});
        });
    }
    
    void Toggle::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (!args.empty())
        {
            if (index == 0)
            {
                if(args[0].isString())
                {
                    if (args[0].getString() == "bang")
                    {
                        send(0, {!m_is_on});
                        setParameter("value", tool::Parameter(tool::Parameter::Type::Int, {!m_is_on}));
                    }
                    else if(args.size() == 2 && args[0].getString() == "set")
                    {
                        if (args[1].isNumber())
                        {
                            if (args[1].getFloat() != 0)
                            {
                                setParameter("value", tool::Parameter(tool::Parameter::Type::Int, {1}));
                            }
                            else
                            {
                                setParameter("value", tool::Parameter(tool::Parameter::Type::Int, {0}));
                            }
                        }
                        else
                        {
                            warning("toggle set requires number value");
                        }
                    }
                    else
                    {
                        warning("toggle doesn't understand message [" + args[0].getString() + "]");
                    }
                }
                else if(args[0].isNumber())
                {
                    if (args[0].getFloat() != 0)
                    {
                        send(0, {true});
                        setParameter("value", tool::Parameter(tool::Parameter::Type::Int, {1}));
                    }
                    else
                    {
                        send(0, {false});
                        setParameter("value", tool::Parameter(tool::Parameter::Type::Int, {0}));
                    }
                }
            }
        }
    }
    
}}

