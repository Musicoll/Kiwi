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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Trigger.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    TRIGGER                                       //
    // ================================================================================ //
    
    void Trigger::declare()
    {
        std::unique_ptr<ObjectClass> trigger_class(new ObjectClass("trigger", &Trigger::create));
        
        trigger_class->addAlias("t");
        
        flip::Class<Trigger> & trigger_model = DataModel::declare<Trigger>()
                                               .name(trigger_class->getModelName().c_str())
                                               .inherit<Object>();
        
        Factory::add<Trigger>(std::move(trigger_class), trigger_model);
    }
    
    std::unique_ptr<Object> Trigger::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Trigger>(args);
    }
    
    Trigger::Trigger(std::vector<tool::Atom> const& args)
    {
        pushInlet({PinType::IType::Control});
        
        for(size_t i = 0; i < args.size(); ++i)
        {
            pushOutlet(PinType::IType::Control);
        }
    }
    
    std::string Trigger::getTypeDescription(tool::Atom const& atom)
    {
        if(atom.isInt())
            return std::to_string(atom.getInt());
        
        if(atom.isFloat())
            return std::to_string(atom.getFloat());
        
        if(atom.isString())
        {
            const std::string str = atom.getString();
            
            if(str == "b") return "bang";
            if(str == "i") return "int";
            if(str == "f") return "float";
            if(str == "s") return "symbol";
            if(str == "l") return "list";
            
            return str;
        }
        
        return {"??"};
    }
    
    std::string Trigger::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            return "(anything) Message to be fanned to multiple outputs";
        }
        
        auto args = tool::AtomHelper::parse(getText());
        args.erase(args.cbegin());
        
        const auto size = args.size();
        if(!args.empty() && index < size)
        {
            return ("Output order " + std::to_string(size - index)
                    + " (" + getTypeDescription(args[index]) + ")");
        }
        
        return {};
    }
    
}}
