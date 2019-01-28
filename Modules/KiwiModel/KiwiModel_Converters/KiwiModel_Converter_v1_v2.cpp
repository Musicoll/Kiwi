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
    
    //! @brief Removes "patcher_name" member from the patcher
    struct Converter_v1_v2 : public ConverterBase
    {
        Converter_v1_v2() : ConverterBase("v1", "v2") {}
        
        bool operator () (flip::BackEndIR& backend) const override
        {
            flip::BackEndIR::Type& patcher = backend.root;
            
            // removing patcher name.
            patcher.members.remove_if([](std::pair<std::string,
                                         flip::BackEndIR::Type> const& member) {
                return member.first == "patcher_name";
            });
            
            return true;
        }
    };
}}
