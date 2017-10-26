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
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Slider.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT SLIDER                                   //
    // ================================================================================ //
    
    Slider::Slider(std::string const& name, std::vector<tool::Atom> const& args):
    Object()
    {
        if (!args.empty())
        {
            throw std::runtime_error("wrong arguments for object bang");
        }
        
        setFlag(Flag::IFlag::DefinedSize);
        setFlag(Flag::IFlag::ResizeWidth);
        setFlag(Flag::IFlag::ResizeHeight);
        addSignal<double>(Signal::ValueChanged, *this);
        setMinWidth(20.);
        setMinHeight(20.);
        setWidth(20);
        setHeight(50.);
        pushInlet({PinType::IType::Control});
        pushOutlet(PinType::IType::Control);
    }
    
    Slider::Slider(flip::Default& d):
    Object(d)
    {
        addSignal<double>(Signal::ValueChanged, *this);
    }
    
    void Slider::declare()
    {
        Factory::add<Slider>("slider");
    }
    
    std::string Slider::getIODescription(bool is_inlet, size_t index) const
    {
        if (is_inlet && index == 0)
        {
            return "Sets current slider value. Value is between 0 and 1";
        }
        else if(!is_inlet && index == 0)
        {
            return "Outputs the slider's value";
        }
        else
        {
            return "";
        }
    }
}}
