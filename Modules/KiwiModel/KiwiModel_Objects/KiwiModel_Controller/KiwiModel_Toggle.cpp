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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Toggle.h>

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                  OBJECT TOGGLE                                   //
        // ================================================================================ //
        
        Toggle::Toggle(std::string const& name, std::vector<Atom> const& args):
        Object()
        {
            if (!args.empty())
            {
                throw std::runtime_error("wrong arguments for object bang");
            }
            
            setFlag(Flag::DefinedSize);
            setFlag(Flag::ResizeWidth);
            setFlag(Flag::ResizeHeight);
            addSignal<Toggle::Request, bool>(Signal::Switch, *this);
            setRatio(1.);
            setMinWidth(20.);
            setWidth(20);
            pushInlet({PinType::IType::Control});
            pushOutlet(PinType::IType::Control);
        }
        
        Toggle::Toggle(flip::Default& d):
        Object(d)
        {
            addSignal<Toggle::Request, bool>(Signal::Switch, *this);
        }
        
        void Toggle::declare()
        {
            Factory::add<Toggle>("toggle");
        }
        
        std::string Toggle::getIODescription(bool is_inlet, size_t index) const
        {
            if (is_inlet && index == 0)
            {
                return "Switches the toggle on or of";
            }
            else if(!is_inlet && index == 0)
            {
                return "Sends 0 or 1 when toggle is switched on or off";
            }
            else
            {
                return "";
            }
        }
    }
}
