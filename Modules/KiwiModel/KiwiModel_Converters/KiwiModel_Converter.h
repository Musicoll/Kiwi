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

#include <flip/BackEndIR.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                   CONVERTER                                      //
    // ================================================================================ //
    
    //! @brief Converts a document's backend representation to meet current version representation.
    class Converter
    {
    public: // methods
        
        //! @brief Tries converting current data model version.
        //! @details Returns true if the conversion was successful, false otherwise. Call this function
        //! after reading from data provider.
        static bool process(flip::BackEndIR & backend);
        
    private: // methods
        
        //! @brief Converts a v1 data model to a v2 data model.
        static void convert_v1_v2(flip::BackEndIR& backend);
        
        //! @brief Converts a v3 data model to a v4 data model.
        static void convert_v3_v4(flip::BackEndIR& backend);
        
        //! @brief Removes invalid links
        static void remove_invalid_links(flip::BackEndIR& backend);
    };
    
}}
