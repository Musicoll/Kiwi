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

namespace kiwi { namespace network { namespace http {
    
    // ================================================================================ //
    //                                     HTTP QUERY                                   //
    // ================================================================================ //
    
    template<class ReqType, class ResType>
    Query<ReqType, ResType>::Query(std::unique_ptr<beast::http::request<ReqType>> request,
                                   std::string port)
    : m_request(std::move(request))
    , m_response()
    , m_port(port)
    , m_io_context()
    , m_stream(nullptr)
    , m_timer(m_io_context)
    , m_resolver(m_io_context)
    , m_buffer()
    , m_thread()
    , m_executed(false)
    , m_callback()
    , m_secure(false)
    {
        ssl::context ctx(ssl::context::method::sslv23_client);
        m_stream.reset(new ssl::stream<tcp::socket>(m_io_context, ctx));
    }
    
    template<class ReqType, class ResType>
    Query<ReqType, ResType>::Query(std::unique_ptr<beast::http::request<ReqType>> request,
                                   std::string port,
                                   ssl::context & ctx)
    : m_request(std::move(request))
    , m_response()
    , m_port(port)
    , m_io_context()
    , m_stream(new ssl::stream<tcp::socket>(m_io_context, ctx))
    , m_timer(m_io_context)
    , m_resolver(m_io_context)
    , m_buffer()
    , m_thread()
    , m_executed(false)
    , m_callback()
    , m_secure(true)
    {
    }
    
    template<class ReqType, class ResType>
    Query<ReqType, ResType>::~Query()
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }
    
    template<class ReqType, class ResType>
    Response<ResType> Query<ReqType, ResType>::writeQuery(Timeout timeout)
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
        
        if (!executed())
        {
            init(timeout);
            
            while(!executed())
            {
                m_io_context.poll_one();
            }
        }
        
        return m_response;
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::writeQueryAsync(std::function<void(Response<ResType> const& res)> && callback, Timeout timeout)
    {
        if (!executed() && !m_thread.joinable())
        {
            init(timeout);
            
            m_callback = std::move(callback);
            
            m_thread = std::thread([this]()
            {
                while(!executed())
                {
                    m_io_context.poll_one();
                }
            });
        }
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::cancel()
    {
        if (m_timer.cancel() != 0)
        {
            if (m_thread.joinable())
            {
                m_thread.join();
            }
        }
        
        if (!m_executed)
        {
            shutdown(boost::asio::error::basic_errors::timed_out);
        }
    }
    
    template<class ReqType, class ResType>
    bool Query<ReqType, ResType>::executed()
    {
        return m_executed.load();
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::init(Timeout timeout)
    {
        if (timeout > Timeout(0))
        {
            m_timer.expires_from_now(timeout);
            
            m_timer.async_wait([this](Error const& error)
            {
                handleTimeout(error);
            });
        }
        
        m_request->prepare_payload();
        
        const std::string host = m_request->at(beast::http::field::host).to_string();
        
        if (m_secure)
        {
            SSL_set_tlsext_host_name(m_stream->native_handle(), host.c_str());
        }
        
        m_resolver.async_resolve(host, m_port, [this](beast::error_code ec,
                                                      tcp::resolver::results_type results)
        {
            if (ec)
            {
                shutdown(ec);
            }
            else
            {
                connect(results);
            }
        });
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::handleTimeout(Error const& error)
    {
        shutdown(boost::asio::error::basic_errors::timed_out);
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::connect(tcp::resolver::results_type results)
    {
        boost::asio::async_connect(m_stream->next_layer(), results, [this](beast::error_code ec, tcp::endpoint const& endpoint){
            
            boost::ignore_unused(endpoint);
            
            if (ec)
            {
                shutdown(ec);
            }
            else
            {
                if (!m_secure)
                {
                    write();
                }
                else
                {
                    handshake();
                }
            }
        });
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::handshake()
    {
        m_stream->async_handshake(ssl::stream_base::client, [this](boost::system::error_code ec)
        {
            if (ec)
            {
                shutdown(ec);
            }
            else
            {
                write();
            }
        });
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::write()
    {
        auto callback = [this](beast::error_code ec, std::size_t bytes_transferred)
        {
            boost::ignore_unused(bytes_transferred);
            
            if (ec)
            {
                shutdown(ec);
            }
            else
            {
                read();
            }
        };
        
        if (!m_secure)
        {
            beast::http::async_write(m_stream->next_layer(), *m_request, callback);
        }
        else
        {
            beast::http::async_write(*m_stream, *m_request, callback);
        }
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::read()
    {
        auto callback = [this](beast::error_code ec, std::size_t bytes_transferred)
        {
            boost::ignore_unused(bytes_transferred);
            shutdown(ec);
        };
        
        if (!m_secure)
        {
            beast::http::async_read(m_stream->next_layer(), m_buffer, m_response, callback);
        }
        else
        {
            beast::http::async_read(*m_stream, m_buffer, m_response, callback);
        }
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::shutdown(Error const& error)
    {
        if (error)
        {
            m_response.error = error;
        }
        
        boost::system::error_code ec;
        m_stream->next_layer().shutdown(tcp::socket::shutdown_both, ec);
        
        if (m_callback)
        {
            m_callback(m_response);
        }
        
        m_executed.store(true);
    }
    
}}} // namespace kiwi::network::http
