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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Number.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT NUMBER                                   //
    // ================================================================================ //
    
    void Number::declare()
    {   
        std::unique_ptr<ObjectClass> number_class(new ObjectClass("number",
                                                                  &Number::create));
        // parameters
        std::unique_ptr<ParameterClass> param_value(new ParameterClass(tool::Parameter::Type::Float));
        
        number_class->addParameter("value", std::move(param_value));
        
        
        // flags
        number_class->setFlag(ObjectClass::Flag::ResizeWidth);
        number_class->setFlag(ObjectClass::Flag::DefinedSize);
        
        // data model
        
        flip::Class<Number> & number_model = DataModel::declare<Number>()
                                             .name(number_class->getModelName().c_str())
                                             .inherit<Object>();
        
        Factory::add<Number>(std::move(number_class), number_model);
    }
    
    std::unique_ptr<Object> Number::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Number>(args);
    }
    
    Number::Number(std::vector<tool::Atom> const& args):
    Object()
    {
        if (!args.empty())
        {
            throw Error("number doesn't take any arguments");
        }
        
        addSignal<>(Signal::OutputValue, *this);
        
        setMinWidth(20.);
        setMinHeight(20.);
        setWidth(50.);
        setHeight(20.);
        pushInlet({PinType::IType::Control});
        pushOutlet(PinType::IType::Control);
    }
    
    Number::Number(flip::Default& d):
    Object(d)
    {
        addSignal<>(Signal::OutputValue, *this);
    }
    
    std::string Number::getIODescription(bool is_inlet, size_t index) const
    {
        if (is_inlet && index == 0)
        {
            return "Sets number value.";
        }
        else if(!is_inlet && index == 0)
        {
            return "Outputs number value";
        }
        else
        {
            return "";
        }
    }
}}
