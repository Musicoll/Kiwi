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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Time/KiwiModel_Delay.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                  OBJECT DELAY                                    //
        // ================================================================================ //
        
        Delay::Delay(std::string const& name, std::vector<Atom> const& args)
        {
            if (args.size() > 1 || (args.size() == 1 && !args[0].isNumber()))
            {
                throw std::runtime_error("wrong argument for object Delay");
            }
            
            pushInlet({PinType::IType::Control});
            
            if (args.empty())
            {
                pushInlet({PinType::IType::Control});
            }
            
            pushOutlet(PinType::IType::Control);
        }
        
        void Delay::declare()
        {
            Factory::add<Delay>("delay");
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
