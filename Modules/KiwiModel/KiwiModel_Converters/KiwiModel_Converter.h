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

#pragma once

#include <KiwiModel/KiwiModel_Converters/KiwiModel_ConverterBase.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                KIWI CONVERTER                                    //
    // ================================================================================ //
    
    //! @brief Converts a document's backend representation to meet current version representation.
    class Converter
    {
    public: // methods
        
        //! @brief Returns the current version of the converter.
        static std::string const& getLatestVersion();
        
        //! @brief Returns true if a given version can be converted from.
        static bool canConvertToLatestFrom(std::string const& version);
        
        //! @brief Tries converting current data model version.
        //! @details Returns true if the conversion was successful, false otherwise. Call this function
        //! after reading from data provider.
        static bool process(flip::BackEndIR& backend);
        
    private: // methods
        
        Converter();
        ~Converter();
        static Converter& use();
        
        using converters_t = std::vector<std::unique_ptr<ConverterBase>>;
        converters_t& converters();
        
        template<class T>
        bool addConverter();
        
    private: // variables
        
        converters_t m_converters;
    };
    
}}
