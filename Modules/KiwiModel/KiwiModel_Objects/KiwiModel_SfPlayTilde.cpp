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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_SfPlayTilde.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                       SFPLAY~                                    //
    // ================================================================================ //
    
    void SfPlayTilde::declare()
    {
        auto kiwi_class = std::make_unique<ObjectClass>("sfplay~", &SfPlayTilde::create);
        
        auto& flip_class = DataModel::declare<SfPlayTilde>()
        .name(kiwi_class->getModelName().c_str())
        .inherit<model::Object>();
        
        Factory::add<SfPlayTilde>(std::move(kiwi_class), flip_class);
    }
    
    std::unique_ptr<Object> SfPlayTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<SfPlayTilde>(args);
    }
    
    SfPlayTilde::SfPlayTilde(std::vector<tool::Atom> const& args)
    {
        pushInlet({PinType::IType::Signal, PinType::IType::Control});
        
        pushOutlet(PinType::IType::Signal);
        
        if(!args.empty() && args[0].isInt())
        {
            const auto channels = args[0].getInt();
            if(channels > 1)
            {
                for(auto i = 1; i < channels; ++i)
                {
                    pushOutlet({PinType::IType::Signal});
                }
            }
        }
        
        pushOutlet(PinType::IType::Control); // bang when finished
    }
    
    std::string SfPlayTilde::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            if(index == 0)
            {
                return "(msg) Open file, start, stop, int";
            }
        }
        else
        {
            if(index < getNumberOfOutlets() - 1)
            {
                return "Audio Channel " + std::to_string(index + 1);
            }
            else
            {
                return "bang when playing ended";
            }
        }
        
        return {};
    }
    
}}
