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

namespace kiwi { namespace network { namespace http {
    
    // ================================================================================ //
    //                                    HTTP PAYLOAD                                  //
    // ================================================================================ //
    
    template <class It>
    Payload::Payload(const It begin, const It end)
    {
        for (It pair = begin; pair != end; ++pair)
        {
            AddPair(*pair);
        }
    }
    
    struct Payload::Pair
    {
        template <typename KeyType, typename ValueType,
        typename std::enable_if<!std::is_integral<ValueType>::value, bool>::type = true>
        Pair(KeyType&& p_key, ValueType&& p_value)
        : key{std::forward<KeyType>(p_key)}
        , value{std::forward<ValueType>(p_value)}
        {
            ;
        }
        
        template <typename KeyType>
        Pair(KeyType&& p_key, const std::int32_t& p_value)
        : key{std::forward<KeyType>(p_key)}
        , value{std::to_string(p_value)}
        {
            ;
        }
        
        std::string key;
        std::string value;
    };
    
    // ================================================================================ //
    //                                  HTTP PARAMETERS                                 //
    // ================================================================================ //
    
    template <typename KeyType, typename ValueType>
    Parameters::Parameter::Parameter(KeyType&& key, ValueType&& value)
    : key{std::forward<KeyType>(key)}
    , value{std::forward<ValueType>(value)}
    {
        ;
    }
    
}}} // namespace kiwi::network::http
