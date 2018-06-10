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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_SfRecordTilde.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                     SFRECORD~                                    //
    // ================================================================================ //
    
    void SfRecordTilde::declare()
    {
        auto kiwi_class = std::make_unique<ObjectClass>("sfrecord~", &SfRecordTilde::create);
        
        auto& flip_class = DataModel::declare<SfRecordTilde>()
        .name(kiwi_class->getModelName().c_str())
        .inherit<model::Object>();
        
        Factory::add<SfRecordTilde>(std::move(kiwi_class), flip_class);
    }
    
    std::unique_ptr<Object> SfRecordTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<SfRecordTilde>(args);
    }
    
    SfRecordTilde::SfRecordTilde(std::vector<tool::Atom> const& args)
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
                    pushInlet({PinType::IType::Signal});
                }
            }
        }
    }
    
    std::string SfRecordTilde::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            if(index == 0)
            {
                return "open, start, stop recording, Audio Channel 1";
            }
            else
            {
                return "Audio Channel " + std::to_string(index + 1);
            }
        }
        else
        {
            if(index == 0)
            {
                return "(signal) Recording time (ms)";
            }
        }
        
        return {};
    }
    
}}
