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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_TimesTilde.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT TIMES~                                   //
    // ================================================================================ //
    
    void TimesTilde::declare()
    {
        if (!DataModel::has<model::OperatorTilde>())
        {
            OperatorTilde::declare();
        }
        
        std::unique_ptr<ObjectClass> timestilde_class(new ObjectClass("*~", &TimesTilde::create));
        
        flip::Class<TimesTilde> & plustilde_model = DataModel::declare<TimesTilde>()
                                                    .name(timestilde_class->getModelName().c_str())
                                                    .inherit<OperatorTilde>();
        
        Factory::add<TimesTilde>(std::move(timestilde_class), plustilde_model);
    }
    
    std::unique_ptr<Object> TimesTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<TimesTilde>(args);
    }
    
    TimesTilde::TimesTilde(std::vector<tool::Atom> const& args):
    OperatorTilde(args)
    {
    }
    
}}
