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

// ==================================================================================== //
//                                          NETWORK                                     //
// ==================================================================================== //

#include <iostream>

#include "../catch.hpp"

#include <beast/http.hpp>

#include <KiwiNetwork/KiwiNetwork_Http.h>

TEST_CASE("Network - Http", "[Network, Http]")
{
    SECTION("Client get request to echo server")
    {
        // Construct request and response.
        beast::http::request<beast::http::string_body> request;
        request.method(beast::http::verb::get);
        request.target("/get");
        request.version = 11;
        request.set(beast::http::field::host, "httpbin.org");
        request.set(beast::http::field::user_agent, "test");
        
        beast::http::response<beast::http::dynamic_body> response;
        
        // Send request and waits for response.
        kiwi::network::httpWrite(request, response);
        
        CHECK(response.result() == beast::http::status::ok);
    }
}
