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

#include <KiwiTool/KiwiTool_Atom.h>

namespace kiwi { namespace model {

    // ================================================================================ //
    //                                   CONVERTER                                      //
    // ================================================================================ //
    
    void Converter::convert_v1_v2(flip::BackEndIR & backend)
    {
        flip::BackEndIR::Type& patcher = backend.root;
        
        // removing patcher name.
        patcher.members.remove_if([](std::pair<std::string, flip::BackEndIR::Type> const& member)
        {
            return member.first == "patcher_name";
        });
        
        backend.complete_conversion("v2");
    }
    
    void Converter::convert_v3_v4(flip::BackEndIR & backend)
    {
        flip::BackEndIR::Type& patcher = backend.root;
        
        // random object conversion:
        // - remove seed inlet (was third inlet in < v4)
        // - remove range inlet (second) if range argument is defined
        
        walk(patcher, [](flip::BackEndIR::Type& type) {
            
            if (type.get_class () != "cicm.kiwi.object.random")
                return; // abort
            
            auto const& text_value = type.member("text").second.value.blob;
            const std::string text { text_value.begin(), text_value.end() };
            const auto parsed_text = tool::AtomHelper::parse(text);
            
            auto& inlets = type.member("inlets").second.array;
            
            if(inlets.size() == 3)
            {
                inlets.erase(inlets.begin());
                
                const bool has_range_arg = (parsed_text.size() > 1
                                            && parsed_text[1].isNumber());
                if(has_range_arg)
                {
                    inlets.erase(inlets.begin());
                }
            }
        });
        
        backend.complete_conversion("v4");
    }
    
    void Converter::remove_invalid_links(flip::BackEndIR& backend)
    {
        flip::BackEndIR::Type& patcher  = backend.root;
        
        auto& objects = patcher.member("objects").second.array;
        
        // A link is considered invalid if sender or receiver object does not exist,
        // or if it's bound to a pin that does not exist.
        auto is_invalid_link = [&objects](flip::BackEndIR::Type& type) {
            
            auto const& sender_ref = type.member("sender_obj").second.value.ref;
            auto const& receiver_ref = type.member("receiver_obj").second.value.ref;
            auto const& sender_outlet = type.member("outlet_index").second.value.int_num;
            auto const& receiver_inlet = type.member("inlet_index").second.value.int_num;
            
            bool sender_found = false;
            bool receiver_found = false;
            
            // check sender validity
            for(auto& object : objects)
            {
                auto& type = object.second;
                
                if(!sender_found && type.ref == sender_ref)
                {
                    sender_found = true;
                    
                    const auto outlet_count = type.member("outlets").second.array.size();
                    if(sender_outlet >= outlet_count)
                    {
                        // bad outlet
                        return true;
                    }
                }
                
                if(!receiver_found && type.ref == receiver_ref)
                {
                    receiver_found = true;
                    
                    const auto inlet_count = type.member("inlets").second.array.size();
                    if(receiver_inlet >= inlet_count)
                    {
                        // bad inlet
                        return true;
                    }
                }
                
                if(sender_found && receiver_found)
                {
                    break;
                }
            }
            
            return (!sender_found || !receiver_found);
        };
        
        auto& links = patcher.member("links").second.array;
        
        auto iter = links.begin();
        const auto end_iter = links.cend();
        for(; iter != end_iter;)
        {
            if (is_invalid_link(iter->second))
            {
                links.erase(iter++);
            }
            else
            {
                ++iter;
            }
        }
    }
    
    bool Converter::process(flip::BackEndIR & backend)
    {
        std::string current_version(KIWI_MODEL_VERSION_STRING);
        
        if (current_version.compare(backend.version) >= 0)
        {
            if (backend.version == "v1")
            {
                convert_v1_v2(backend);
            }
            
            // no change from v2 to v3 !
            
            if (backend.version == "v2")
            {
                backend.complete_conversion("v3");
            }
            
            if (backend.version == "v3")
            {
                convert_v3_v4(backend);
                remove_invalid_links(backend);
            }
        }
        
        return (backend.version == current_version);
    }
}}
