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

namespace kiwi { namespace network { namespace http {
    
    // ================================================================================ //
    //                                        HTTP                                      //
    // ================================================================================ //
    
    template<class ReqType, class ResType>
    Response<ResType>
    write(std::unique_ptr<Request<ReqType>> request,
          std::string port,
          beast::error_code& error,
          Timeout timeout)
    {
        return Query<ReqType, ResType>(std::move(request), port, timeout).writeRequest(error);
    }
    
    template <class ReqType, class ResType>
    std::future<void>
    writeAsync(std::unique_ptr<Request<ReqType>> request,
               std::string port,
               std::function<void(Response<ResType> response,
                                  Error error)> callback,
               Timeout timeout)
    {
        auto query = std::make_unique<Query<ReqType, ResType>>(std::move(request), port, timeout);
        
        return std::async(std::launch::async, [query = std::move(query), cb = std::move(callback)]() {
            
            beast::error_code error;
            Response<ResType> response(query->writeRequest(error));
            return cb(std::move(response), error);
            
        });
    }
    
    // ================================================================================ //
    //                                    HTTP PAYLOAD                                  //
    // ================================================================================ //
    
    template <class It>
    Payload::Payload(const It begin, const It end)
    {
        for (It pair = begin; pair != end; ++pair)
        {
            AddPair(*pair);
        }
    }
    
    struct Payload::Pair
    {
        template <typename KeyType, typename ValueType,
        typename std::enable_if<!std::is_integral<ValueType>::value, bool>::type = true>
        Pair(KeyType&& p_key, ValueType&& p_value)
        : key{std::forward<KeyType>(p_key)}
        , value{std::forward<ValueType>(p_value)}
        {
            ;
        }
        
        template <typename KeyType>
        Pair(KeyType&& p_key, const std::int32_t& p_value)
        : key{std::forward<KeyType>(p_key)}
        , value{std::to_string(p_value)}
        {
            ;
        }
        
        std::string key;
        std::string value;
    };
    
    // ================================================================================ //
    //                                  HTTP PARAMETERS                                 //
    // ================================================================================ //
    
    template <typename KeyType, typename ValueType>
    Parameters::Parameter::Parameter(KeyType&& key, ValueType&& value)
    : key{std::forward<KeyType>(key)}
    , value{std::forward<ValueType>(value)}
    {
        ;
    }
    
    // ================================================================================ //
    //                                     HTTP QUERY                                   //
    // ================================================================================ //
    
    template<class ReqType, class ResType>
    Query<ReqType, ResType>::Query(std::unique_ptr<beast::http::request<ReqType>> request,
                                   std::string port,
                                   Timeout timeout)
    : m_request(std::move(request))
    , m_response()
    , m_error()
    , m_timeout(timeout)
    , m_port(port)
    , m_io_service()
    , m_socket(m_io_service)
    , m_timer(m_io_service)
    , m_buffer()
    {
        ;
    }
    
    template<class ReqType, class ResType>
    Query<ReqType, ResType>::~Query()
    {
        ;
    }
    
    template<class ReqType, class ResType>
    Response<ResType>
    Query<ReqType, ResType>::writeRequest(Error& error)
    {
        if (m_timeout > Timeout(0))
        {
            m_timer.expires_from_now(m_timeout);
            
            m_timer.async_wait([this](boost::system::error_code const& error) {
                handleTimeout(error);
            });
        }
        
        tcp::resolver resolver(m_io_service);
        
        const std::string host = m_request->at(beast::http::field::host).to_string();
        
        resolver.async_resolve({host, m_port}, [this](beast::error_code const& error,
                                                      tcp::resolver::iterator iterator) {
            connect(error, iterator);
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
    Query<ReqType, ResType>::handleTimeout(beast::error_code const& error)
    {
        m_io_service.stop();
        
        m_socket.shutdown(tcp::socket::shutdown_both, m_error);
        m_error = boost::asio::error::basic_errors::timed_out;
    }
    
    template<class ReqType, class ResType>
    void
    Query<ReqType, ResType>::connect(beast::error_code const& error,
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
                this->write(error);
            });
        }
    }
    
    template<class ReqType, class ResType>
    void
    Query<ReqType, ResType>::write(beast::error_code const& error)
    {
        if (error)
        {
            m_error = error;
        }
        else
        {
            beast::http::async_write(m_socket, *m_request, [this](beast::error_code const& error) {
                read(error);
            });
            
        }
    }
    
    template<class ReqType, class ResType>
    void
    Query<ReqType, ResType>::read(beast::error_code const& error)
    {
        if (error)
        {
            m_error = error;
        }
        else
        {
            beast::http::async_read(m_socket, m_buffer, m_response, [this](beast::error_code const& error) {
                shutdown(error);
            });
        }
    }
    
    template<class ReqType, class ResType>
    void
    Query<ReqType, ResType>::shutdown(beast::error_code const& error)
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
    
}}} // namespace kiwi::network::http
