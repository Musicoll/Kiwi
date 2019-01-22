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

#include <flip/BackEndIR.h>
#include <KiwiModel/KiwiModel_Converters/KiwiModel_ConverterBase.h>

namespace kiwi { namespace model {
    
    //! @brief Nothing to do from v2 to v3
    struct Converter_v2_v3 : public ConverterBase
    {
        Converter_v2_v3() : ConverterBase("v2", "v3") {}
        
        bool operator () (flip::BackEndIR& backend) const override
        {
            // nothing to do from v2 to v3
            return true;
        }
    };
}}
