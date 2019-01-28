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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_PlusTilde.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT PLUS~                                    //
    // ================================================================================ //
    
    void PlusTilde::declare()
    {
        if (!DataModel::has<model::OperatorTilde>())
        {
            OperatorTilde::declare();
        }
        
        std::unique_ptr<ObjectClass> plustilde_class(new ObjectClass("+~", &PlusTilde::create));
        
        flip::Class<PlusTilde> & plustilde_model = DataModel::declare<PlusTilde>()
                                                   .name(plustilde_class->getModelName().c_str())
                                                   .inherit<OperatorTilde>();
        
        Factory::add<PlusTilde>(std::move(plustilde_class), plustilde_model);
    }
    
    std::unique_ptr<Object> PlusTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<PlusTilde>(args);
    }
    
    PlusTilde::PlusTilde(std::vector<tool::Atom> const& args):
    OperatorTilde(args)
    {
    }
    
}}
