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

#pragma once

#include <memory>
#include <chrono>
#include <functional>
#include <thread>
#include <atomic>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace beast = boost::beast;
namespace ssl = boost::asio::ssl;

namespace kiwi { namespace network { namespace http {
    
    using Timeout = std::chrono::milliseconds;
    using Error = beast::error_code;
    
    // ================================================================================ //
    //                                      RESPONSE                                    //
    // ================================================================================ //
    
    template<class BodyType>
    class Response : public beast::http::response<BodyType>
    {
    public:
        Error error;
    };
    
    template<class BodyType>
    using Request = beast::http::request<BodyType>;
    
    // ================================================================================ //
    //                                       QUERY                                      //
    // ================================================================================ //
    
    template<class ReqType, class ResType>
    class Query
    {
    private: // classes
        
        using Callback = std::function<void(Response<ResType> const&)>;
        
    public: // methods
        
        //! @brief Constructor.
        Query(std::unique_ptr<Request<ReqType>> request,
              std::string port);
        
        //! @brief SSL constructor.
        Query(std::unique_ptr<Request<ReqType>> request,
              std::string port,
              ssl::context & context);
        
        //! @brief Destructor.
        //! @details Wait for asynchronous operation to terminate.
        ~Query();
        
        //! @brief Call request on the network.
        //! @details If query is already executed query will not be sent again.
        //! Previous reponse will be returned.
        Response<ResType> writeQuery(Timeout timeout = Timeout(0));
        
        //! @brief Calls the request on a specific thread.
        //! @details If an asnchronous read is currently running, it will not be updated or relaunched.
        void writeQueryAsync(std::function<void(Response<ResType> const& res)> && callback, Timeout timeout = Timeout(0));
        
        //! @brief Cancels the request.
        //! @details Once cancel is called query is executed and cannot be invoked again.
        //! If query was launched asynchronously callback will be called with a timeout error.
        void cancel();
        
        //! @brief Returns true if the query was executed or cancelled.
        bool executed();
        
    private: // methods
        
        using tcp = boost::asio::ip::tcp;
        
        //! @internal
        void init(Timeout timeout);
        
        //! @internal
        void handleTimeout(beast::error_code const& error);
        
        //! @internal
        void connect(tcp::resolver::results_type results);
        
        //! @internal
        void handshake();
        
        //! @internal
        void write();
        
        //! @internal
        void read();
        
        //! @internal
        void shutdown(beast::error_code const& error);
        
    private: // members
        
        std::unique_ptr<Request<ReqType>>   m_request;
        Response<ResType>                   m_response;
        
        std::string                                 m_port;
        boost::asio::io_context                     m_io_context;
        std::unique_ptr<ssl::stream<tcp::socket>>   m_stream;
        boost::asio::steady_timer                   m_timer;
        tcp::resolver                               m_resolver;
        beast::flat_buffer                          m_buffer;
        std::thread                                 m_thread;
        std::atomic<bool>                           m_executed;
        Callback                                    m_callback;
        bool                                        m_secure;
        
    private: // deleted methods
        
        Query() = delete;
        Query(Query const& other) = delete;
        Query(Query && other) = delete;
        Query& operator=(Query const& other) = delete;
        Query& operator=(Query && other) = delete;
    };
    
}}} // namespace kiwi::network::http

#include "KiwiHttp.hpp"
