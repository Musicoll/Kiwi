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

#include <string>

#include <KiwiModel/KiwiModel_Def.h>
#include <KiwiModel/KiwiModel_Converters/KiwiModel_Converter.h>

namespace kiwi { namespace model {

    // ================================================================================ //
    //                                   CONVERTER                                      //
    // ================================================================================ //
    
    bool Converter::process(flip::BackEndIR & backend)
    {
        bool success = false;
        
        process_rollback(backend);
        
        std::string current_version(KIWI_MODEL_VERSION_STRING);
        
        if (current_version.compare(backend.version) >= 0)
        {
            if (backend.version.compare("v1") >= 0)
            {
                success = true;
            }
        }
        
        return success;
    }
    
    void Converter::process_rollback(flip::BackEndIR & backend)
    {
    }
}}
