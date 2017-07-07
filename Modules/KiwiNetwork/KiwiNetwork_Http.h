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
#include <chrono>
#include <functional>
#include <future>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

#include <beast/http.hpp>
#include <beast/core.hpp>

using tcp = boost::asio::ip::tcp;

namespace kiwi
{
    namespace network
    {
        template<class ReqType, class ResType>
        class HttpQuerry
        {
        public: // methods
            
            HttpQuerry(beast::http::request<ReqType>& request,
                       beast::http::response<ResType>& response,
                       std::string port,
                       beast::error_code& error,
                       std::chrono::milliseconds time_out = std::chrono::milliseconds(0)):
            m_request(request),
            m_response(response),
            m_port(port),
            m_io_service(),
            m_socket(m_io_service),
            m_error(error),
            m_time_out(time_out),
            m_timer(m_io_service),
            m_buffer()
            {
            }
            
            ~HttpQuerry()
            {
            }
            
            void writeRequest()
            {
                if (m_time_out > std::chrono::milliseconds(0))
                {
                    m_timer.expires_from_now(m_time_out);
                    
                    m_timer.async_wait([this](boost::system::error_code const& error)
                    {
                        this->handle_time_out(error);
                    });
                }
                
                tcp::resolver resolver(m_io_service);
                
                std::string host = m_request[beast::http::field::host].to_string();
                
                resolver.async_resolve({host, m_port}, [this](beast::error_code const& error,
                                                              tcp::resolver::iterator iterator)
                {
                    this->handle_resolve(error, iterator);
                });
                
                m_io_service.reset();
                
                while(!m_error && m_io_service.run_one())
                {
                }
            };
            
        private: // methods
            
            void handle_time_out(boost::system::error_code const& error)
            {
                m_io_service.stop();
                
                m_socket.shutdown(tcp::socket::shutdown_both, m_error);
                m_error = boost::asio::error::basic_errors::timed_out;
            }
            
            void handle_read(beast::error_code const& error)
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
            
            void handle_write(beast::error_code const& error)
            {
                if (error)
                {
                    m_error = error;
                }
                else
                {
                    beast::http::async_read(m_socket, m_buffer, m_response, [this](beast::error_code const& error)
                    {
                        this->handle_read(error);
                    });
                }
            }
            
            void handle_connect(beast::error_code const& error)
            {
                if (error)
                {
                    m_error = error;
                }
                else
                {
                    m_request.prepare_payload();
                    
                    beast::http::async_write(m_socket, m_request, [this](beast::error_code const& error)
                    {
                        this->handle_write(error);
                    });
                    
                }
            }
            
            void handle_resolve(beast::error_code const& error,
                                tcp::resolver::iterator iterator)
            {
                if (error)
                {
                    m_error = error;
                }
                else
                {
                    boost::asio::async_connect(m_socket, iterator, [this](beast::error_code const& error,
                                                                          tcp::resolver::iterator i)
                    {
                        this->handle_connect(error);
                    });
                }
            }
            
        private: // members
            
            beast::http::request<ReqType>&      m_request;
            beast::http::response<ResType>&     m_response;
            std::string                         m_port;
            boost::asio::io_service             m_io_service;
            tcp::socket                         m_socket;
            beast::error_code&                  m_error;
            std::chrono::milliseconds           m_time_out;
            boost::asio::steady_timer           m_timer;
            beast::flat_buffer                  m_buffer;
            
            
        private: // deleted methods
            
            HttpQuerry() = delete;
            HttpQuerry(HttpQuerry const& other) = delete;
            HttpQuerry(HttpQuerry && other) = delete;
            HttpQuerry& operator=(HttpQuerry const& other) = delete;
            HttpQuerry& operator=(HttpQuerry && other) = delete;
        };
        
        //! @brief Sends an http request.
        //! @details Returns the response generated by the server. The function blocks until response
        //! is received or error occurs.
        template<class ReqType, class ResType>
        void httpWrite(beast::http::request<ReqType>& request,
                       beast::http::response<ResType>& response,
                       std::string port,
                       beast::error_code& error,
                       std::chrono::milliseconds time_out = std::chrono::milliseconds(0))
        {
            HttpQuerry<ReqType, ResType>(request, response, port, error).writeRequest();
        }
        
        //! @brief Sends an http request asynchronously.
        //! @details Returns the response generated by the server. The function is non blocking. callback
        //! is called on another thread once response is received or error occurs.
        template <class ReqType, class ResType>
        std::future<void> httpWriteAsync(beast::http::request<ReqType> const& request,
                                         std::string port,
                                         std::function<void(beast::http::response<ResType> const& response,
                                                            beast::error_code const& error)> callback,
                                          std::chrono::milliseconds time_out = std::chrono::milliseconds(0))
        {
            return std::async(std::launch::async, [request, port, time_out, callback]()
            {
                beast::http::request<ReqType> req(request);
                beast::http::response<ResType> response;
                beast::error_code error;
                
                HttpQuerry<ReqType, ResType>(req, response, port, error, time_out).writeRequest();
                
                return callback(response, error);
            });
        }
    }
}
