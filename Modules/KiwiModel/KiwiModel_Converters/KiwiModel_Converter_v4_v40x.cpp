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

#include <flip/BackEndIR.h>
#include <KiwiModel/KiwiModel_Converters/KiwiModel_ConverterBase.h>

namespace kiwi { namespace model {
    
    //! @brief removes "min_width", "min_height" and "ratio" members from model::Object
    struct Converter_v4_v401 : public ConverterBase
    {
        Converter_v4_v401() : ConverterBase("v4", "v4.0.1") {}
        
        bool operator () (flip::BackEndIR& backend) const override
        {
            flip::BackEndIR::Type& patcher = backend.root;
            
            auto& objects = patcher.member("objects").second.array;
            
            for(auto& object : objects)
            {
                object.second.object_remove_member("min_width");
                object.second.object_remove_member("min_height");
                object.second.object_remove_member("ratio");
            }
            
            return true;
        }
    };
    
    //! @brief Nothing to do from v4.0.1 to v4.0.2
    //! @details faust~ object added
    struct Converter_v401_v402 : public ConverterBase
    {
        Converter_v401_v402() : ConverterBase("v4.0.1", "v4.0.2") {}
        
        bool operator () (flip::BackEndIR& backend) const override
        {
            // nothing to do from v4.0.1 to v4.0.2
            return true;
        }
    };
    
    //! @brief Nothing to do from v4.0.2 to v4.0.3
    //! @details add Bounds and Point classes
    //! Add screen_bounds and view_position to Patcher::View
    struct Converter_v402_v403 : public ConverterBase
    {
        Converter_v402_v403() : ConverterBase("v4.0.2", "v4.0.3") {}
        
        bool operator () (flip::BackEndIR& backend) const override
        {
            flip::BackEndIR::Type& patcher = backend.root;
            
            flip::walk(patcher, [](flip::BackEndIR::Type & type) {
                
                // skip every type that is not a "patcherview"
                if (type.get_class () != "cicm.kiwi.Patcher.View")
                    return;
                
                type.object_add_member_object("screen_bounds", "cicm.kiwi.Bounds");
                type.object_add_member_object("view_position", "cicm.kiwi.Point");
            });
            
            return true;
        }
    };
}}
