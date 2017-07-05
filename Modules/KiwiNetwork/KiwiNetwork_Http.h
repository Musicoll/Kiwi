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

#include <memory>

#include <boost/asio.hpp>

#include <beast/http.hpp>
#include <beast/core.hpp>

using tcp = boost::asio::ip::tcp;

namespace kiwi
{
    namespace network
    {
        //! @brief Sends an http request.
        //! @details Returns the response generated by the server. The function blocks until response
        //! is handed over.
        //! @todo Handle errorcode message. Maybe add a timeout parameter.
        template<class ReqType, class ResType>
        void httpWrite(beast::http::request<ReqType>& request,
                       beast::http::response<ResType>& response)
        {
            beast::error_code ec;
            
            // Set up an asio socket.
            boost::asio::io_service ios;
            tcp::resolver resolver(ios);
            tcp::socket socket(ios);
            
            // Look up the domain name.
            std::string host = request[beast::http::field::host].to_string();
            auto const lookup = resolver.resolve({host, "http"}, ec);
            
            // Establish connection to server.
            boost::asio::connect(socket, lookup, ec);

            // Prepare the request.
            request.prepare_payload();
            
            // Sends the request.
            beast::http::write(socket, request, ec);
            
            // Reads the response.
            beast::flat_buffer buffer;
            beast::http::read(socket, buffer, response, ec);
            
            // Close connection to server.
            socket.shutdown(tcp::socket::shutdown_both, ec);
        }
    }
}
