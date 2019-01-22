/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <string>

#include <KiwiModel/KiwiModel_Def.h>
#include <KiwiModel/KiwiModel_Converters/KiwiModel_Converter.h>

#include <KiwiModel/KiwiModel_Converters/KiwiModel_Converter_v1_v2.cpp>
#include <KiwiModel/KiwiModel_Converters/KiwiModel_Converter_v2_v3.cpp>
#include <KiwiModel/KiwiModel_Converters/KiwiModel_Converter_v3_v4.cpp>
#include <KiwiModel/KiwiModel_Converters/KiwiModel_Converter_v4_v40x.cpp>

namespace kiwi { namespace model {

    // ================================================================================ //
    //                                   CONVERTER                                      //
    // ================================================================================ //
    
    Converter::Converter()
    {
        addConverter<Converter_v1_v2>();
        addConverter<Converter_v2_v3>();
        addConverter<Converter_v3_v4>();
        addConverter<Converter_v4_v401>();
        addConverter<Converter_v401_v402>();
        addConverter<Converter_v402_v403>();
    }
    
    Converter::~Converter()
    {}
    
    Converter& Converter::use()
    {
        static Converter instance;
        return instance;
    }
    
    std::string const& Converter::getLatestVersion()
    {
        static const std::string latest_version = KIWI_MODEL_VERSION_STRING;
        return latest_version;
    }
    
    bool Converter::canConvertToLatestFrom(std::string const& version)
    {
        auto& converters = use().converters();
        
        if(version == getLatestVersion())
        {
            return true;
        }
        
        for(auto& conversion : converters)
        {
            if(conversion->v_from == version)
            {
                if(canConvertToLatestFrom(conversion->v_to))
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    template<class T>
    bool Converter::addConverter()
    {
        m_converters.emplace_back(std::make_unique<T>());
        return true;
    }
    
    auto Converter::converters() -> converters_t&
    {
        return m_converters;
    }
    
    bool Converter::process(flip::BackEndIR & backend)
    {
        auto& conversions = use().converters();
        for(auto& conversion : conversions)
        {
            conversion->process(backend);
            if(backend.version == getLatestVersion())
            {
                return true;
            }
        }
        
        return false;
    }
}}
