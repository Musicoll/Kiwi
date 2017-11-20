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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_NewBox.h>

#include <KiwiModel/KiwiModel_ObjectClass.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                       NEWBOX                                     //
    // ================================================================================ //
    
    void NewBox::declare()
    {
        std::unique_ptr<ObjectClass> object_class(new ObjectClass("newbox",
                                                                  &NewBox::create));
        
        object_class->setFlag(ObjectClass::Flag::DefinedSize);
        object_class->setFlag(ObjectClass::Flag::Internal);
        
        flip::Class<NewBox> & data_model = DataModel::declare<NewBox>()
                                           .name(object_class->getModelName().c_str())
                                           .inherit<Object>();
        
        Factory::add<NewBox>(std::move(object_class), data_model);
    }
    
    std::unique_ptr<Object> NewBox::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<NewBox>();
    }
    
    NewBox::NewBox()
    {
        setWidth(80);
        setHeight(20);
        pushInlet({PinType::IType::Control});
    }
    
    std::string NewBox::getIODescription(bool is_inlet, size_t index) const
    {
        return "(nothing here)";
    }
    
}}
