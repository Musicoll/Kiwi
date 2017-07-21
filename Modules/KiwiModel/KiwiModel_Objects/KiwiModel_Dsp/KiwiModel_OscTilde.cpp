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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Dsp/KiwiModel_OscTilde.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                  OBJECT OSC~                                     //
        // ================================================================================ //
        
        OscTilde::OscTilde(std::string const& name, std::vector<Atom> const& args)
        {
            pushInlet({PinType::IType::Control, PinType::IType::Signal});
            pushInlet({PinType::IType::Control, PinType::IType::Signal});
            
            pushOutlet(PinType::IType::Signal);
        }
        
        void OscTilde::declare()
        {
            Factory::add<OscTilde>("osc~");
        }
        
        std::string OscTilde::getIODescription(bool is_inlet, size_t index) const
        {
            if(is_inlet)
            {
                if(index == 0)
                {
                    return "(signal/float) Set frequency";
                }
                else if(index == 1)
                {
                    return "(signal/float) Set phase (0-1)";
                }
            }
            else
            {
                return "(signal) Output";
            }
            
            return {};
        }
    }
}
