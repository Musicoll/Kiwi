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

#include <KiwiModel/KiwiModel_Factory.h>
#include <KiwiModel/KiwiModel_ObjectClass.h>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_NumberTilde.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT NUMBER TILDE                             //
    // ================================================================================ //
    
    void NumberTilde::declare()
    {   
        std::unique_ptr<ObjectClass> numbertilde_class(new ObjectClass("number~",
                                                                  &NumberTilde::create));
        // parameters
        std::unique_ptr<ParameterClass> param_value(new ParameterClass(tool::Parameter::Type::Float));
        
        numbertilde_class->addParameter("value", std::move(param_value));
        
        
        // flags
        numbertilde_class->setFlag(ObjectClass::Flag::DefinedSize);
        
        // data model
        
        flip::Class<NumberTilde> & numbertilde_model = DataModel::declare<NumberTilde>()
                                                       .name(numbertilde_class->getModelName().c_str())
                                                       .inherit<Object>();
        
        Factory::add<NumberTilde>(std::move(numbertilde_class), numbertilde_model);
    }
    
    std::unique_ptr<Object> NumberTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<NumberTilde>(args);
    }
    
    NumberTilde::NumberTilde(std::vector<tool::Atom> const& args):
    Object()
    {
        if (!args.empty())
        {
            throw Error("number tilde doesn't take any arguments");
        }
        
        setWidth(50.);
        setHeight(20.);
        pushInlet({PinType::IType::Signal});
        pushOutlet(PinType::IType::Control);
    }
    
    NumberTilde::NumberTilde(flip::Default& d):
    Object(d)
    {
    }
    
    std::string NumberTilde::getIODescription(bool is_inlet, size_t index) const
    {
        if (is_inlet && index == 0)
        {
            return "Input signal";
        }
        else if(!is_inlet && index == 0)
        {
            return "Ouputs input signal value every interval";
        }
        else
        {
            return "";
        }
    }
}}
