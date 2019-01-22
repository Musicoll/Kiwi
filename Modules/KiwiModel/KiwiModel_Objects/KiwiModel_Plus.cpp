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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Plus.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    OBJECT PLUS                                   //
    // ================================================================================ //
    
    void Plus::declare()
    {
        if (!DataModel::has<model::Operator>())
        {
            Operator::declare();
        }
        
        std::unique_ptr<ObjectClass> plus_class(new ObjectClass("+",
                                                                &Plus::create));
        
        flip::Class<Plus> & plus_model = DataModel::declare<Plus>()
                                         .name(plus_class->getModelName().c_str())
                                         .inherit<Operator>();
        
        Factory::add<Plus>(std::move(plus_class), plus_model);
    }
    
    std::unique_ptr<Object> Plus::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Plus>(args);
    }
    
    Plus::Plus(std::vector<tool::Atom> const& args):
    Operator(args)
    {
    }
    
}}
