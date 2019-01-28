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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Loadmess.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT LOADMESS                                 //
    // ================================================================================ //
    
    void Loadmess::declare()
    {
        std::unique_ptr<ObjectClass> loadmess_class(new ObjectClass("loadmess", &Loadmess::create));
        
        flip::Class<Loadmess> & loadmess_model = DataModel::declare<Loadmess>()
                                                 .name(loadmess_class->getModelName().c_str())
                                                 .inherit<Object>();
        
        Factory::add<Loadmess>(std::move(loadmess_class), loadmess_model);
    }
    
    std::unique_ptr<Object> Loadmess::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Loadmess>(args);
    }
    
    Loadmess::Loadmess(std::vector<tool::Atom> const& args)
    {
        if (args.empty())
            throw Error("loadmess argument is required");
            
        pushInlet({PinType::IType::Control});
        pushOutlet(PinType::IType::Control);
    }
    
    std::string Loadmess::getIODescription(bool is_inlet, size_t index) const
    {
        if(!is_inlet && index == 0)
        {
            return "Sends message when patch is loaded";
        }
        else if(is_inlet && index == 0)
        {
            return "Bang to retrigger message";
        }
        
        return {};
    }
    
}}
