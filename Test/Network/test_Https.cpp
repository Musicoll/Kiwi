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

//#include "example/common/root_certificates.hpp"

#include "../catch.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

TEST_CASE("Network - Ssl", "[Network, Ssl]")
{
    SECTION("Simple connection")
    {
        try
        {
            auto const host = "httpbin.org";
            auto const port = "443";
            auto const target = "/";
            int version = 11;
            
            // The io_context is required for all I/O
            boost::asio::io_context ioc;
            
            // The SSL context is required, and holds certificates
            ssl::context ctx{ssl::context::sslv23_client};
            
            // Verify the remote server's certificate
            ctx.set_verify_mode(ssl::verify_none);
            
            // These objects perform our I/O
            tcp::resolver resolver{ioc};
            ssl::stream<tcp::socket> stream{ioc, ctx};
            
            // Set SNI Hostname (many hosts need this to handshake successfully)
            if(! SSL_set_tlsext_host_name(stream.native_handle(), host))
            {
                boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
                throw boost::system::system_error{ec};
            }
            
            // Look up the domain name
            auto const results = resolver.resolve(host, port);
            
            // Make the connection on the IP address we get from a lookup
            boost::asio::connect(stream.next_layer(), results.begin(), results.end());
            
            // Perform the SSL handshake
            stream.handshake(ssl::stream_base::client);
            
            // Set up an HTTP GET request message
            http::request<http::string_body> req{http::verb::get, target, version};
            req.set(http::field::host, host);
            req.set(http::field::user_agent, "test");
            
            // Send the HTTP request to the remote host
            http::write(stream, req);
            
            // This buffer is used for reading and must be persisted
            boost::beast::flat_buffer buffer;
            
            // Declare a container to hold the response
            http::response<http::dynamic_body> res;
            
            // Receive the HTTP response
            http::read(stream, buffer, res);
            
            // Gracefully close the stream
            boost::system::error_code ec;
            stream.shutdown(ec);

            CHECK(res.result() == boost::beast::http::status::ok);

            if(ec == boost::asio::error::eof)
            {
                // Rationale:
                // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
                ec.assign(0, ec.category());
            }
            if(ec)
                throw boost::system::system_error{ec};
            
            // If we get here then the connection is closed gracefully
        }
        catch(std::exception const& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
