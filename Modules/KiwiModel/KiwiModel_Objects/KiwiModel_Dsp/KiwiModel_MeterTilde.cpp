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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Dsp/KiwiModel_MeterTilde.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT METER~                                   //
    // ================================================================================ //
    
    MeterTilde::MeterTilde(flip::Default& d):
    model::Object(d)
    {
        addSignal<float>(Signal::PeakChanged, *this);
    }
    
    MeterTilde::MeterTilde(std::string const& name, std::vector<tool::Atom> const& args):
    model::Object()
    {
        if (!args.empty())
        {
            throw std::runtime_error("wrong arguments for object meter");
        }
        
        setFlag(Flag::IFlag::DefinedSize);
        setFlag(Flag::IFlag::ResizeWidth);
        setFlag(Flag::IFlag::ResizeHeight);
        setMinWidth(20);
        setMinHeight(20);
        setWidth(100);
        setHeight(20);
        pushInlet({PinType::IType::Signal});
        pushOutlet(PinType::IType::Control);
        addSignal<float>(Signal::PeakChanged, *this);
    }
    
    std::string MeterTilde::getIODescription(bool is_inlet , size_t index) const
    {
        std::string description;
        
        if (is_inlet && index == 0)
        {
            description = "input monitored signal";
        }
        else if (!is_inlet && index == 0)
        {
            description = "sends peak value each metering intervall";
        }
        
        return description;
    }
    
    void MeterTilde::declare()
    {
        Factory::add<MeterTilde>("meter~");
    }
}
}
