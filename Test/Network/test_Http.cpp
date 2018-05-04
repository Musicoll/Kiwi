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

#include <boost/beast/http.hpp>

namespace beast = boost::beast;

#include <KiwiNetwork/KiwiNetwork_Http.h>

TEST_CASE("Network - Http Query", "[Network, Http]")
{
    using namespace kiwi::network;
    
    SECTION("Query - Client get request to echo server")
    {
        auto request = std::make_unique<beast::http::request<beast::http::string_body>>();
        request->method(beast::http::verb::get);
        request->target("/get");
        request->version = 11;
        request->set(beast::http::field::host, "httpbin.org");
        request->set(beast::http::field::user_agent, "test");
        
        http::Query<beast::http::string_body, beast::http::string_body> query(std::move(request), "80");
        
        http::Response<beast::http::string_body> response = query.writeQuery();
        
        REQUIRE(!response.error);
        CHECK(response.result() == beast::http::status::ok);
        CHECK(query.executed());
    }
    
    SECTION("Query - Client asynchronous get request to echo server")
    {
        auto request = std::make_unique<http::Request<beast::http::string_body>>();
        request->method(beast::http::verb::get);
        request->target("/get");
        request->version = 11;
        request->set(beast::http::field::host, "httpbin.org");
        request->set(beast::http::field::user_agent, "test");
        
        http::Query<beast::http::string_body, beast::http::string_body> query(std::move(request), "80");
        
        int callback_called = 0;
        
        std::function<void(http::Response<beast::http::string_body> const&)>
        callback_1 = [&query, &callback_called](http::Response<beast::http::string_body> const & response)
        {
            CHECK(!query.executed());
            REQUIRE(!response.error);
            CHECK(response.result() == beast::http::status::ok);
            callback_called = 1;
        };
        
        std::function<void(http::Response<beast::http::string_body> const&)>
        callback_2 = [&callback_called](http::Response<beast::http::string_body> const & response)
        {
            callback_called = 2;
        };
        
        query.writeQueryAsync(std::move(callback_1));
        query.writeQueryAsync(std::move(callback_2));
        
        while(!query.executed()){};
        
        CHECK(callback_called == 1);
    }
    
    SECTION("Query - Reaching timeout sync query")
    {
        auto request = std::make_unique<http::Request<beast::http::string_body>>();
        request->method(beast::http::verb::get);;
        request->version = 11;
        request->set(beast::http::field::host, "example.com");
        
        http::Query<beast::http::string_body, beast::http::string_body> query(std::move(request), "81");
        
        http::Response<beast::http::string_body> response = query.writeQuery(http::Timeout(100));
        
        CHECK(response.error);
        CHECK(response.error == boost::asio::error::basic_errors::timed_out);
    }
    
    SECTION("Query - Reaching timeout async query")
    {
        auto request = std::make_unique<http::Request<beast::http::string_body>>();
        request->method(beast::http::verb::get);
        request->version = 11;
        request->set(beast::http::field::host, "example.com");
        
        std::function<void(http::Response<beast::http::string_body> const&)>
        callback = [](http::Response<beast::http::string_body> const & response)
        {
            CHECK(response.error);
            CHECK(response.error == boost::asio::error::basic_errors::timed_out);
        };
        
        http::Query<beast::http::string_body, beast::http::string_body> query(std::move(request), "81");
        
        query.writeQueryAsync(std::move(callback), http::Timeout(100));
    }
    
    SECTION("Query - Cancelling query sync request")
    {
        auto request = std::make_unique<beast::http::request<beast::http::string_body>>();
        request->method(beast::http::verb::get);
        request->target("/get");
        request->version = 11;
        request->set(beast::http::field::host, "httpbin.org");
        request->set(beast::http::field::user_agent, "test");
        
        http::Query<beast::http::string_body, beast::http::string_body> query(std::move(request), "80");
        
        query.cancel();
        
        CHECK(query.executed());
        
        http::Response<beast::http::string_body> response = query.writeQuery();
        
        CHECK(response.error == boost::asio::error::basic_errors::timed_out);
    }
    
    SECTION("Query - Cancelling query async request")
    {
        auto request = std::make_unique<http::Request<beast::http::string_body>>();
        request->method(beast::http::verb::get);
        request->version = 11;
        request->set(beast::http::field::host, "example.com");
        
        std::function<void(http::Response<beast::http::string_body> const&)>
        callback = [](http::Response<beast::http::string_body> const & response)
        {
            CHECK(response.error);
            CHECK(response.error == boost::asio::error::basic_errors::timed_out);
        };
        
        http::Query<beast::http::string_body, beast::http::string_body> query(std::move(request), "81");
        
        query.writeQueryAsync(std::move(callback));
        
        query.cancel();
        
        CHECK(query.executed());
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
        
        session.GetAsync([](http::Session::Response response) {
            REQUIRE(!response.error);
            CHECK(response.result() == beast::http::status::ok);
        });
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
    
    SECTION("Session Get request with timeout")
    {
        http::Session session;
        session.setHost("example.com");
        session.setTarget("/get");
        session.setPort("81");
        session.setTimeout(http::Timeout(100));
        
        http::Session::Response response = session.Get();
        
        CHECK(response.error);
        CHECK(response.error == boost::asio::error::basic_errors::timed_out);
    }
    
    SECTION("Session GetAsync with timeout")
    {
        http::Session session;
        session.setHost("example.com");
        session.setTarget("/get");
        session.setPort("81");
        session.setTimeout(http::Timeout(100));
        
        session.GetAsync([](http::Session::Response response) {
            CHECK(response.error);
            CHECK(response.error == boost::asio::error::basic_errors::timed_out);
        });
    }
}
