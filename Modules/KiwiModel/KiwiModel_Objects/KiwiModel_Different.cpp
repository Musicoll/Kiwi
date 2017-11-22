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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Different.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    OBJECT DIFFERENT                              //
    // ================================================================================ //
    
    void Different::declare()
    {
        if (!DataModel::has<model::Operator>())
        {
            Operator::declare();
        }
        
        std::unique_ptr<ObjectClass> different_class(new ObjectClass("different",
                                                                     &Different::create));
        
        different_class->addAlias("!=");
        
        flip::Class<Different> & different_model = DataModel::declare<Different>()
                                                   .name(different_class->getModelName().c_str())
                                                   .inherit<Operator>();
        
        Factory::add<Different>(std::move(different_class), different_model);
    }
    
    std::unique_ptr<Object> Different::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Different>(args);
    }
    
    Different::Different(std::vector<tool::Atom> const& args):
    Operator(args)
    {
    }
    
}}
