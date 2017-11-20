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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_SigTilde.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT SIG~                                     //
    // ================================================================================ //
    
    void SigTilde::declare()
    {
        std::unique_ptr<ObjectClass> sigtilde_class(new ObjectClass("sig~", &SigTilde::create));
        
        flip::Class<SigTilde> & sigtilde_model = DataModel::declare<SigTilde>()
                                                 .name(sigtilde_class->getModelName().c_str())
                                                 .inherit<Object>();
        
        Factory::add<SigTilde>(std::move(sigtilde_class), sigtilde_model);
    }
    
    std::unique_ptr<Object> SigTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<SigTilde>(args);
    }
    
    
    SigTilde::SigTilde(std::vector<tool::Atom> const& args)
    {
        if (args.empty() || !args[0].isNumber())
        {
            pushInlet({PinType::IType::Control});
        }
        
        pushOutlet(PinType::IType::Signal);
    }
    
    std::string SigTilde::getIODescription(bool is_inlet, size_t index) const
    {
        return is_inlet ? "(float/int) Set signal value" : "(signal) Output value";
    }
    
}}
