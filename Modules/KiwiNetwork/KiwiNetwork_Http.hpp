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

namespace kiwi { namespace network {
    
    // ================================================================================ //
    //                                        HTTP                                      //
    // ================================================================================ //
    
    template<class ReqType, class ResType>
    beast::http::response<ResType>
    Http::write(std::unique_ptr<beast::http::request<ReqType>> request,
                std::string port,
                beast::error_code& error,
                std::chrono::milliseconds timeout)
    {
        return Query<ReqType, ResType>(std::move(request), port, timeout).writeRequest(error);
    }
    
    template <class ReqType, class ResType>
    std::future<void>
    Http::writeAsync(std::unique_ptr<beast::http::request<ReqType>> request,
                     std::string port,
                     std::function<void(beast::http::response<ResType> response,
                                        beast::error_code error)> callback,
                     std::chrono::milliseconds timeout)
    {
        auto query = std::make_unique<Query<ReqType, ResType>>(std::move(request), port, timeout);
        
        return std::async(std::launch::async, [query = std::move(query), cb = std::move(callback)]() {
            
            beast::error_code error;
            beast::http::response<ResType> response = query->writeRequest(error);
            return cb(std::move(response), error);
            
        });
    }
    
    // ================================================================================ //
    //                                     HTTP QUERY                                   //
    // ================================================================================ //
    
    template<class ReqType, class ResType>
    Http::Query<ReqType, ResType>::Query(std::unique_ptr<beast::http::request<ReqType>> request,
                                         std::string port,
                                         std::chrono::milliseconds timeout)
    : m_request(std::move(request))
    , m_response()
    , m_port(port)
    , m_io_service()
    , m_socket(m_io_service)
    , m_error()
    , m_timeout(timeout)
    , m_timer(m_io_service)
    , m_buffer()
    {
        ;
    }
    
    template<class ReqType, class ResType>
    Http::Query<ReqType, ResType>::~Query()
    {
        ;
    }
    
    template<class ReqType, class ResType>
    beast::http::response<ResType>
    Http::Query<ReqType, ResType>::writeRequest(beast::error_code& error)
    {
        if (m_timeout > std::chrono::milliseconds(0))
        {
            m_timer.expires_from_now(m_timeout);
            
            m_timer.async_wait([this](boost::system::error_code const& error) {
                this->handleTimeout(error);
            });
        }
        
        tcp::resolver resolver(m_io_service);
        
        const std::string host = m_request->at(beast::http::field::host).to_string();
        
        resolver.async_resolve({host, m_port}, [this](beast::error_code const& error,
                                                      tcp::resolver::iterator iterator) {
            handleResolve(error, iterator);
        });
        
        m_io_service.reset();
        
        // block here until error or success
        while(!m_error && m_io_service.run_one())
        {
            ;
        }
        
        error = m_error;
        
        return m_response;
    };
    
    template<class ReqType, class ResType>
    void
    Http::Query<ReqType, ResType>::handleTimeout(boost::system::error_code const& error)
    {
        m_io_service.stop();
        
        m_socket.shutdown(tcp::socket::shutdown_both, m_error);
        m_error = boost::asio::error::basic_errors::timed_out;
    }
    
    template<class ReqType, class ResType>
    void
    Http::Query<ReqType, ResType>::handleRead(beast::error_code const& error)
    {
        m_io_service.stop();
        
        if (error)
        {
            m_error = error;
        }
        else
        {
            m_socket.shutdown(tcp::socket::shutdown_both, m_error);
        }
    }
    
    template<class ReqType, class ResType>
    void
    Http::Query<ReqType, ResType>::handleWrite(beast::error_code const& error)
    {
        if (error)
        {
            m_error = error;
        }
        else
        {
            beast::http::async_read(m_socket, m_buffer, m_response, [this](beast::error_code const& error) {
                handleRead(error);
            });
        }
    }
    
    template<class ReqType, class ResType>
    void
    Http::Query<ReqType, ResType>::handleConnect(beast::error_code const& error)
    {
        if (error)
        {
            m_error = error;
        }
        else
        {
            beast::http::async_write(m_socket, *m_request, [this](beast::error_code const& error) {
                handleWrite(error);
            });
            
        }
    }
    
    template<class ReqType, class ResType>
    void
    Http::Query<ReqType, ResType>::handleResolve(beast::error_code const& error,
                                                 tcp::resolver::iterator iterator)
    {
        if (error)
        {
            m_error = error;
        }
        else
        {
            boost::asio::async_connect(m_socket, iterator, [this](beast::error_code const& error,
                                                                  tcp::resolver::iterator i) {
                this->handleConnect(error);
            });
        }
    }
    
}} // namespace kiwi::network
