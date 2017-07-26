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

#include "KiwiHttp.h"
#include "KiwiHttp_Util.h"

namespace kiwi { namespace network { namespace http {
    
    // ================================================================================ //
    //                                  HTTP PAYLOAD                                    //
    // ================================================================================ //
    
    Payload::Payload(const std::initializer_list<Pair>& pairs)
    : Payload(begin(pairs), end(pairs))
    {
        ;
    }
    
    void Payload::AddPair(const Pair& pair)
    {
        if(!content.empty())
        {
            content += "&";
        }
        
        auto escaped = util::urlEncode(pair.value);
        content += pair.key + "=" + escaped;
    }
    
    // ================================================================================ //
    //                                 HTTP PARAMETERS                                  //
    // ================================================================================ //
    
    Parameters::Parameters(std::initializer_list<Parameter> const& parameters)
    {
        for(auto const& parameter : parameters)
        {
            AddParameter(parameter);
        }
    }
    
    void Parameters::AddParameter(Parameter const& parameter)
    {
        if (!content.empty())
        {
            content += "&";
        }
        
        auto escapedKey = util::urlEncode(parameter.key);
        if (parameter.value.empty())
        {
            content += escapedKey;
        }
        else
        {
            auto escapedValue = util::urlEncode(parameter.value);
            content += escapedKey + "=" + escapedValue;
        }
    }
    
}}} // namespace kiwi::network::http
