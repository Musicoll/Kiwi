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

#include <KiwiModel/KiwiModel_Factory.h>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Bang.h>

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                  OBJECT BANG                                     //
        // ================================================================================ //
        
        Bang::Bang(std::string const& name, std::vector<Atom> const& args):
        Object()
        {
            if (!args.empty())
            {
                throw std::runtime_error("wrong arguments for object bang");
            }
            
            setFlag(Flag::DefinedSize);
            addSignal<>(Signal::TriggerBang, *this);
            setWidth(20);
            setHeight(20);
            pushInlet({PinType::IType::Control});
            pushOutlet(PinType::IType::Control);
        }
        
        Bang::Bang(flip::Default& d):
        Object(d)
        {
            addSignal<>(Signal::TriggerBang, *this);
        }
        
        void Bang::declare()
        {
            Factory::add<Bang>("bang");
        }
        
        std::string Bang::getIODescription(bool is_inlet, size_t index) const
        {
            if (is_inlet && index == 0)
            {
                return "Makes the object flash and sends bang through first outlet";
            }
            else if(!is_inlet && index == 0)
            {
                return "Sends bang";
            }
            else
            {
                return "";
            }
        }
    }
}
