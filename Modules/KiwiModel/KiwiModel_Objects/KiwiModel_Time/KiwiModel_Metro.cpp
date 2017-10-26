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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Time/KiwiModel_Metro.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                   OBJECT METRO                                   //
        // ================================================================================ //
        
        Metro::Metro(std::string const& name, std::vector<tool::Atom> const& args)
        {
            if (args.size() < 1 || (args.size() == 1 && !args[0].isNumber()))
            {
                throw std::runtime_error("wrong argument for object Metro");
            }
            
            pushInlet({PinType::IType::Control});
            
            if (args.empty())
            {
                pushInlet({PinType::IType::Control});
            }
            
            pushOutlet(PinType::IType::Control);
        }
        
        void Metro::declare()
        {
            Factory::add<Metro>("metro");
        }
        
        std::string Metro::getIODescription(bool is_inlet, size_t index) const
        {
            if(is_inlet)
            {
                if(index == 0)
                {
                    return "Start/Stop metronome";
                }
                else if(index == 1)
                {
                    return "Set time interval";
                }
            }
            else
            {
                return "Outputs metronome ticks as bang";
            }
            
            return {};
        }    }
}
