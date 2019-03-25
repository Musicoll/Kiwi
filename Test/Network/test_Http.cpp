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

// ==================================================================================== //
//                                          NETWORK                                     //
// ==================================================================================== //

#include <iostream>

#include "../catch.hpp"

#include <boost/beast/http.hpp>

namespace beast = boost::beast;

#include <KiwiNetwork/KiwiNetwork_Http.h>

std::string const root_certificate =
"-----BEGIN CERTIFICATE-----\n"
"MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n"
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n"
"WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n"
"ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n"
"MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n"
"h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n"
"0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n"
"A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n"
"T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n"
"B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n"
"B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n"
"KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n"
"OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n"
"jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n"
"qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n"
"rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n"
"HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n"
"hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n"
"ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n"
"3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n"
"NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n"
"ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n"
"TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n"
"jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n"
"oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n"
"4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n"
"mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n"
"emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n"
"-----END CERTIFICATE-----\n"
;

TEST_CASE("Network - Http Query", "[Network, Http]")
{
    using namespace kiwi::network;
    
    SECTION("Query - Client get request to echo server")
    {
        auto request = std::make_unique<beast::http::request<beast::http::string_body>>();
        request->method(beast::http::verb::get);
        request->target("/get");
        request->version(11);
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
        request->version(11);
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
        request->version(11);
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
        request->version(11);
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
        request->version(11);
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
        request->version(11);
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
    
    SECTION("Query - Secure connection to test server")
    {
        auto request = std::make_unique<beast::http::request<beast::http::string_body>>();
        request->method(beast::http::verb::get);
        request->target("/");
        request->version(11);
        request->set(beast::http::field::host, "valid-isrgrootx1.letsencrypt.org");
        
        ssl::context ctx(ssl::context::method::sslv23_client);
        ctx.add_certificate_authority(boost::asio::buffer(root_certificate.data(), root_certificate.size()));
        ctx.set_verify_mode(ssl::verify_peer);
        
        http::Query<beast::http::string_body, beast::http::string_body> query(std::move(request), "443", ctx);
        
        http::Response<beast::http::string_body> response = query.writeQuery();
        
        REQUIRE(!response.error);
        CHECK(response.result() == beast::http::status::ok);
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
    
    SECTION("Session secure encrypted connection")
    {
        http::Session session;
        session.setHost("valid-isrgrootx1.letsencrypt.org");
        session.setTarget("/");
        session.setPort("443");
        session.setSecure(http::Session::SecureMode::VerifyPeer);
        session.addCertificate(root_certificate);

        session.GetAsync([](http::Session::Response response) {
            REQUIRE(!response.error);
            CHECK(response.result() == beast::http::status::ok);
        });
    }
}
