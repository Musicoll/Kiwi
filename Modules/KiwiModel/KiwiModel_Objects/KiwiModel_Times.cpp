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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Times.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    OBJECT TIMES                                  //
    // ================================================================================ //
    
    void Times::declare()
    {
        if (!DataModel::has<model::Operator>())
        {
            Operator::declare();
        }
        
        std::unique_ptr<ObjectClass> time_class(new ObjectClass("times",
                                                                &Times::create));
        
        time_class->addAlias("*");
        
        flip::Class<Times> & time_model = DataModel::declare<Times>()
                                         .name(time_class->getModelName().c_str())
                                         .inherit<Operator>();
        
        Factory::add<Times>(std::move(time_class), time_model);
    }
    
    std::unique_ptr<Object> Times::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Times>(args);
    }
    
    Times::Times(std::vector<tool::Atom> const& args):
    Operator(args)
    {
    }
    
}}
