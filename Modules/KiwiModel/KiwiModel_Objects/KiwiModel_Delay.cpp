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

#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Delay.h>

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                  OBJECT DELAY                                    //
        // ================================================================================ //
        
        void Delay::declare()
        {
            std::unique_ptr<ObjectClass> delay_class(new ObjectClass("delay",
                                                     &Delay::create));
            
            flip::Class<Delay> & delay_model = DataModel::declare<Delay>()
                                               .name(delay_class->getModelName().c_str())
                                               .inherit<Object>();
            
            Factory::add<Delay>(std::move(delay_class), delay_model);
        }
        
        std::unique_ptr<Object> Delay::create(std::vector<tool::Atom> const& args)
        {
            return std::make_unique<Delay>(args);
        }
        
        Delay::Delay(std::vector<tool::Atom> const& args)
        {
            if (args.size() > 1)
                throw Error("delay too many arguments");
            
            if(args.size() == 1 && !args[0].isNumber())
                throw Error("delay argument must be a number");
            
            pushInlet({PinType::IType::Control});
            
            if (args.empty())
            {
                pushInlet({PinType::IType::Control});
            }
            
            pushOutlet(PinType::IType::Control);
        }
        
        std::string Delay::getIODescription(bool is_inlet, size_t index) const
        {
            if(is_inlet)
            {
                if(index == 0)
                {
                    return "bang gets delayed, message \"stop\" cancels";
                }
                else if(index == 1)
                {
                    return "Set delay time";
                }
            }
            else
            {
                return "Delayed bang";
            }
            
            return {};
        }
    }
}
