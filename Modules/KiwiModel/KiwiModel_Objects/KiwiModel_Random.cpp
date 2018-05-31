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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Random.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                     RANDOM                                       //
    // ================================================================================ //
    
    void Random::declare()
    {
        auto kiwi_class = std::make_unique<ObjectClass>("random", &Random::create);
        
        auto& flip_class = DataModel::declare<Random>()
        .name(kiwi_class->getModelName().c_str())
        .inherit<Object>();
        
        Factory::add<Random>(std::move(kiwi_class), flip_class);
    }
    
    std::unique_ptr<Object> Random::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Random>(args);
    }
    
    Random::Random(std::vector<tool::Atom> const& args)
    {
        if (args.size() > 2)
        {
            throw Error("random: too many arguments");
        }
        
        if (args.size() > 0 && !args[0].isNumber())
        {
            throw Error("random: range argument must be a number");
        }
        
        if (args.size() > 1 && !args[1].isNumber())
        {
            throw Error("random: seed argument must be a number");
        }
        
        pushInlet({PinType::IType::Control}); // bang or seed message inlet
        
        if (args.size() == 0)
        {
            pushInlet({PinType::IType::Control}); // range inlet
        }
        
        pushOutlet(PinType::IType::Control);
    }
    
    std::string Random::getIODescription(bool is_inlet, size_t index) const
    {
        if (is_inlet)
        {
            if (index == 0)
            {
                return "bang causes random number generation, seed sets the seed";
            }
            else if (index == 1)
            {
                return "Sets the maximum range (exclusive)";
            }
        }
        else if(index == 0)
        {
            return "Outputs random number";
        }
     
        return {};
    }
    
}}
