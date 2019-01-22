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

#include <KiwiEngine/KiwiEngine_Factory.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Route.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       ROUTE                                      //
    // ================================================================================ //
    
    void Route::declare()
    {
        Factory::add<Route>("route", &Route::create);
    }
    
    std::unique_ptr<Object> Route::create(model::Object const& model, Patcher& patcher)
    {
        return std::make_unique<Route>(model, patcher);
    }
    
    Route::Route(model::Object const& model, Patcher& patcher)
    : Object(model, patcher)
    , m_args(model.getArguments())
    {
        if(m_args.empty())
        {
            // Initialize default custom selector
            m_args.emplace_back(0);
        }
    }
    
    void Route::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        // handle custom selector special case
        if(index == 1 && !args.empty())
        {
            assert(m_args.size() == 1);
            m_args.front() = args.front();
        }
        
        if(index == 0 && !args.empty())
        {
            const auto& first_arg = args.front();
            const bool is_list = args.size() > 1;
            
            const std::vector<tool::Atom> bang_msg = {"bang"};
            
            auto send_if = [&](const size_t out, bool remove_first_elem = true) {
                
                if(!remove_first_elem)
                {
                    send(out, args);
                }
                else if (is_list)
                {
                    send(out, {args.begin()+1, args.end()});
                }
                else
                {
                    send(out, bang_msg);
                }
            };
            
            bool input_matched = false;
            
            for (int i = 0; i < m_args.size(); i++)
            {
                auto const& arg = m_args[i];
                
                if(arg.isString())
                {
                    const auto& str = arg.getString();
                    
                    if(first_arg.isString() && (first_arg.getString() == str))
                    {
                        send_if(i);
                        input_matched = true;
                        continue;
                    }
                    
                    if ((!is_list && str == "int" && first_arg.isInt())
                        || (!is_list && str == "float" && first_arg.isFloat())
                        || (is_list && str == "list"))
                    {
                        send_if(i, false);
                        input_matched = true;
                    }
                }
                else
                {
                    if((first_arg.isInt() && arg.isInt()
                       && (first_arg.getInt() == arg.getInt()))
                       || ((first_arg.isFloat() && arg.isFloat())
                           && (first_arg.getFloat() == arg.getFloat())))
                    {
                        send_if(i);
                        input_matched = true;
                    }
                }
            }
            
            if(!input_matched)
            {
                // pass all input to last outlet if doesn't match
                send(m_args.size(), args);
            }
        }
    }  
}}
