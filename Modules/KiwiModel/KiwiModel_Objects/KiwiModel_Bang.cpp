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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Bang.h>
#include <KiwiModel/KiwiModel_Factory.h>
#include <KiwiModel/KiwiModel_DataModel.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT BANG                                     //
    // ================================================================================ //
    
    void Bang::declare()
    {
        std::unique_ptr<ObjectClass> bang_class(new ObjectClass("bang", &Bang::create));
        
        bang_class->setFlag(ObjectClass::Flag::DefinedSize);
        
        flip::Class<Bang> & bang_model = DataModel::declare<Bang>().name(bang_class->getModelName().c_str()).inherit<Object>();
        
        Factory::add<Bang>(std::move(bang_class), bang_model);
    }
    
    std::unique_ptr<Object> Bang::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Bang>(args);
    }
    
    Bang::Bang(std::vector<tool::Atom> const& args):
    Object()
    {
        if (!args.empty())
        {
            throw Error("bang too many arguments");
        }
        
        addSignal<>(Signal::TriggerBang, *this);
        addSignal<>(Signal::FlashBang, *this);
        setWidth(20);
        pushInlet({PinType::IType::Control});
        pushOutlet(PinType::IType::Control);
    }
    
    Bang::Bang(flip::Default& d):
    Object(d)
    {
        addSignal<>(Signal::TriggerBang, *this);
        addSignal<>(Signal::FlashBang, *this);
    }
    
    std::string Bang::getIODescription(bool is_inlet, size_t index) const
    {
        if (is_inlet && index == 0)
        {
            return "Makes the object flash and sends bang through first outlet";
        }
        else if(!is_inlet && index == 0)
        {
            return "Sends bang";
        }
        else
        {
            return "";
        }
    }
    
}}
