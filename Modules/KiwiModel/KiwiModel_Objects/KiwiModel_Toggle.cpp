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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Toggle.h>
#include <KiwiModel/KiwiModel_Factory.h>
#include <KiwiModel/KiwiModel_DataModel.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT TOGGLE                                   //
    // ================================================================================ //
    
    void Toggle::declare()
    {
        std::unique_ptr<ObjectClass> toggle_class(new ObjectClass("toggle", &Toggle::create));
        
        toggle_class->setFlag(ObjectClass::Flag::DefinedSize);
        
        std::unique_ptr<ParameterClass> param_value(new ParameterClass(tool::Parameter::Type::Int));
        
        toggle_class->addParameter("value", std::move(param_value));
        
        flip::Class<Toggle> & toggle_model = DataModel::declare<Toggle>()
                                             .name(toggle_class->getModelName().c_str())
                                             .inherit<Object>();
        
        Factory::add<Toggle>(std::move(toggle_class), toggle_model);
    }
    
    std::unique_ptr<Object> Toggle::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Toggle>(args);
    }
    
    Toggle::Toggle(std::vector<tool::Atom> const& args):
    Object()
    {
        if (!args.empty())
        {
            throw Error("toggle too many arguments");
        }
        
        addSignal<>(Signal::OutputValue, *this);
        setWidth(20);
        pushInlet({PinType::IType::Control});
        pushOutlet(PinType::IType::Control);
    }
    
    Toggle::Toggle(flip::Default& d):
    Object(d)
    {
        addSignal<>(Signal::OutputValue, *this);
    }
    
    std::string Toggle::getIODescription(bool is_inlet, size_t index) const
    {
        if (is_inlet && index == 0)
        {
            return "Switches the toggle on or of";
        }
        else if(!is_inlet && index == 0)
        {
            return "Sends 0 or 1 when toggle is switched on or off";
        }
        else
        {
            return "";
        }
    }
    
}}
