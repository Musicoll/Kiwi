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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_NoiseTilde.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                     NOISE~                                       //
    // ================================================================================ //
    
    void NoiseTilde::declare()
    {
        std::unique_ptr<ObjectClass> noisetilde_class(new ObjectClass("noise~", &NoiseTilde::create));
        
        flip::Class<NoiseTilde> & noisetilde_model = DataModel::declare<NoiseTilde>().name(noisetilde_class->getModelName().c_str()).inherit<Object>();
        
        Factory::add<NoiseTilde>(std::move(noisetilde_class), noisetilde_model);
    }
    
    std::unique_ptr<Object> NoiseTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<NoiseTilde>(args);
    }
    
    NoiseTilde::NoiseTilde(std::vector<tool::Atom> const& args)
    {
        if (args.size() > 0)
        {
            throw Error("noise~ too many arguments");
        }
        
        pushOutlet(PinType::IType::Signal);
    }
    
    std::string NoiseTilde::getIODescription(bool is_inlet, size_t index) const
    {
        return "(signal) noise value";
    }
    
}}
