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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Dsp/KiwiModel_DelaySimpleTilde.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      OBJECT DELAYSIMPLE~                         //
        // ================================================================================ //
        
        DelaySimpleTilde::DelaySimpleTilde(std::string const& name, std::vector<Atom> const& args)
        {
            if (args.size() > 0)
            {
                throw std::runtime_error("wrong argument for object delay~");
            }
            
            pushInlet({PinType::IType::Control, PinType::IType::Signal});
            pushInlet({PinType::IType::Control, PinType::IType::Signal});
            pushInlet({PinType::IType::Control});
            
            pushOutlet(PinType::IType::Signal);
        }
        
        void DelaySimpleTilde::declare()
        {
            Factory::add<DelaySimpleTilde>("delaysimple~");
        }
        
        std::string DelaySimpleTilde::getIODescription(bool is_inlet, size_t index) const
        {
            if(is_inlet)
            {
                if(index == 0)
                {
                    return "(signal) Input to be delayed";
                }
                else if(index == 1)
                {
                    return "(signal/float) Delay time (ms)";
                }
                else if(index == 2)
                {
                    return "(float) Feedback (0-1)";
                }
            }
            else
            {
                return "(signal) Delayed output signal";
            }
            
            return {};
        }
    }
}
