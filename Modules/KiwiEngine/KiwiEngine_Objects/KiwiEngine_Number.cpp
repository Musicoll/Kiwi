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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Number.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Number.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT NUMBER                                   //
    // ================================================================================ //
    
    void Number::declare()
    {
        Factory::add<Number>("number", &Number::create);
    }
    
    std::unique_ptr<Object> Number::create(model::Object const& model, Patcher& patcher)
    {
        return std::make_unique<Number>(model, patcher);
    }
    
    Number::Number(model::Object const& object_model, Patcher& patcher):
    Object(object_model, patcher),
    m_value(object_model.getParameter("value")[0].getFloat()),
    m_connection(object_model.getSignal<>(model::Number::Signal::OutputValue)
                 .connect(std::bind(&Number::outputValue, this)))
    {
    }
    
    void Number::parameterChanged(std::string const& name, tool::Parameter const& parameter)
    {
        if (name == "value")
        {
            m_value = parameter[0].getFloat();
        }
    }
    
    void Number::outputValue()
    {
        defer([this]()
        {
            send(0, {m_value});
            
        });
    }
    
    void Number::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0 && !args.empty())
        {
            if (args[0].isNumber())
            {
                send(0, {args[0].getFloat()});
                setParameter("value", tool::Parameter(tool::Parameter::Type::Float, {args[0].getFloat()}));
            }
            else if (args[0].isString())
            {
                if (args[0].getString() == "bang")
                {
                    outputValue();
                }
                else if(args[0].getString() == "set")
                {
                    if (args.size() > 1 && args[1].isNumber())
                    {
                        setParameter("value", tool::Parameter(tool::Parameter::Type::Float, {args[1].getFloat()}));
                    }
                    else
                    {
                        warning("number, \"set\" must be followed by a number");
                    }
                }
                else
                {
                    warning("number doesn't understand symbol " + args[0].getString());
                }
            }
            else
            {
                warning("number doesn't understand " + args[0].getString());
            }
        }
    }
    
}}

