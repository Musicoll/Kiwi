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

#include <functional>
#include <algorithm>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Slider.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT SLIDER                                   //
    // ================================================================================ //
    
    void Slider::declare()
    {
        Factory::add<Slider>("slider", &Slider::create);
    }
    
    std::unique_ptr<Object> Slider::create(model::Object const& model, Patcher& patcher)
    {
        return std::make_unique<Slider>(model, patcher);
    }
    
    Slider::Slider(model::Object const& object_model, Patcher& patcher):
    Object(object_model, patcher),
    m_value(object_model.getParameter("value")[0].getFloat())
    {
    }
    
    
    Slider::~Slider()
    {
    }
    
    void Slider::parameterChanged(std::string const& name, tool::Parameter const& parameter)
    {
        if (name == "value")
        {
            m_value = parameter[0].getFloat();
            send(0, {m_value});
        }
    }
    
    void Slider::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0 && !args.empty())
        {
            if (args[0].isNumber())
            {
                setParameter("value", tool::Parameter(tool::Parameter::Type::Float, {args[0].getFloat()}));
            }
            else if (args[0].isString() && args[0].getString() == "bang")
            {
                send(0, {m_value});
            }
            else
            {
                warning("slider inlet 1 doesn't understand [" + args[0].getString() + "]");
            }
        }
    }
    
}}

