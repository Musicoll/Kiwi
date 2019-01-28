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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_MinusTilde.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT -~                                       //
    // ================================================================================ //
    
    void MinusTilde::declare()
    {
        if (!DataModel::has<model::OperatorTilde>())
        {
            OperatorTilde::declare();
        }
        
        std::unique_ptr<ObjectClass> minustilde_class(new ObjectClass("-~", &MinusTilde::create));
        
        flip::Class<MinusTilde> & plustilde_model = DataModel::declare<MinusTilde>()
                                                    .name(minustilde_class->getModelName().c_str())
                                                    .inherit<OperatorTilde>();
        
        Factory::add<MinusTilde>(std::move(minustilde_class), plustilde_model);
    }
    
    std::unique_ptr<Object> MinusTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<MinusTilde>(args);
    }
    
    MinusTilde::MinusTilde(std::vector<tool::Atom> const& args):
    OperatorTilde(args)
    {
    }
    
}}
