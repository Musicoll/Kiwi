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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_DivideTilde.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT /~                                       //
    // ================================================================================ //
    
    void DivideTilde::declare()
    {
        if (!DataModel::has<model::OperatorTilde>())
        {
            OperatorTilde::declare();
        }
        
        std::unique_ptr<ObjectClass> dividetilde_class(new ObjectClass("/~", &DivideTilde::create));
        
        flip::Class<DivideTilde> & dividetilde_model = DataModel::declare<DivideTilde>()
                                                                .name(dividetilde_class->getModelName().c_str())
                                                                .inherit<OperatorTilde>();
        
        Factory::add<DivideTilde>(std::move(dividetilde_class), dividetilde_model);
    }
    
    std::unique_ptr<Object> DivideTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<DivideTilde>(args);
    }
    
    DivideTilde::DivideTilde(std::vector<tool::Atom> const& args):
    OperatorTilde(args)
    {
    }
    
}}
