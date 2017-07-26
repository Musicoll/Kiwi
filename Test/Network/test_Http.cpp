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
    using namespace kiwi::network;
    
    SECTION("Client get request to echo server")
    {
        // Construct request and response.
        auto request = std::make_unique<beast::http::request<beast::http::string_body>>();
        request->method(beast::http::verb::get);
        request->target("/get");
        request->version = 11;
        request->set(beast::http::field::host, "httpbin.org");
        request->set(beast::http::field::user_agent, "test");
        
        request->prepare_payload();
        
        beast::error_code error;
        
        // Send request and waits for response.
        http::Response<beast::http::string_body> response =
        http::write<beast::http::string_body, beast::http::string_body>(std::move(request), "80");
        
        REQUIRE(!response.error);
        CHECK(response.result() == beast::http::status::ok);
    }
    
    SECTION("Client asynchronous get request to echo server")
    {
        // Construct request and response.
        auto request = std::make_unique<http::Request<beast::http::string_body>>();
        request->method(beast::http::verb::get);
        request->target("/get");
        request->version = 11;
        request->set(beast::http::field::host, "httpbin.org");
        request->set(beast::http::field::user_agent, "test");
        
        request->prepare_payload();
        
        std::function<void(http::Response<beast::http::string_body>)>
        callback = [](http::Response<beast::http::string_body> response)
        {
            REQUIRE(!response.error);
            CHECK(response.result() == beast::http::status::ok);
        };
        
        auto future = http::writeAsync(std::move(request), "80", callback);
        future.get();
    }
}

TEST_CASE("Network - Http Session", "[Network, Http]")
{
    using namespace kiwi::network;
    
    SECTION("Session GetAsync")
    {
        http::Session session;
        session.setHost("httpbin.org");
        session.setTarget("/get");
        
        auto future = session.GetAsync([](http::Session::Response response) {
            REQUIRE(!response.error);
            CHECK(response.result() == beast::http::status::ok);
        });
        
        future.get();
    }
    
    SECTION("Session GetAsync with Parameters")
    {
        http::Session session;
        session.setHost("httpbin.org");
        session.setTarget("/get");
        session.setParameters({
            {"foo", "bar bar"},
            {"value", "42"}
        });
        
        session.GetAsync([](http::Session::Response response) {
            REQUIRE(!response.error);
            CHECK(response.result() == beast::http::status::ok);
        });
    }
}
