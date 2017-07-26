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
          Timeout timeout)
    {
        return Query<ReqType>(std::move(request), port, timeout).template writeRequest<ResType>();
    }
    
    template <class ReqType, class ResType>
    std::future<void>
    writeAsync(std::unique_ptr<Request<ReqType>> request,
               std::string port,
               std::function<void(Response<ResType>)> callback,
               Timeout timeout)
    {
        auto query = std::make_unique<Query<ReqType>>(std::move(request), port, timeout);
        
        return std::async(std::launch::async, [query = std::move(query), cb = std::move(callback)]() {
            
            return cb(query->template writeRequest<ResType>());
            
        });
    }
    
    // ================================================================================ //
    //                                     HTTP QUERY                                   //
    // ================================================================================ //
    
    template<class ReqType>
    Query<ReqType>::Query(std::unique_ptr<beast::http::request<ReqType>> request,
                          std::string port,
                          Timeout timeout)
    : m_request(std::move(request))
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
    
    template<class ReqType>
    Query<ReqType>::~Query()
    {
        ;
    }
    
    template<class ReqType>
    template<class ResType>
    Response<ResType>
    Query<ReqType>::writeRequest()
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
        
        Response<ResType> response;
        
        resolver.async_resolve({host, m_port}, [this, &response](Error const& error,
                                                                 tcp::resolver::iterator iterator) {
            connect(response, error, iterator);
        });
        
        m_io_service.reset();
        
        // block here until error or success
        while(!m_error && m_io_service.run_one())
        {
            ;
        }
        
        response.error = m_error;
        
        return std::move(response);
    };
    
    template<class ReqType>
    void
    Query<ReqType>::handleTimeout(Error const& error)
    {
        m_io_service.stop();
        
        m_socket.shutdown(tcp::socket::shutdown_both, m_error);
        m_error = boost::asio::error::basic_errors::timed_out;
    }
    
    template<class ReqType>
    template<class ResType>
    void
    Query<ReqType>::connect(Response<ResType>& response,
                            Error const& error,
                            tcp::resolver::iterator iterator)
    {
        if (error)
        {
            m_error = error;
        }
        else
        {
            boost::asio::async_connect(m_socket, iterator, [this, &response](Error const& error,
                                                                             tcp::resolver::iterator i) {
                this->write(response, error);
            });
        }
    }
    
    template<class ReqType>
    template<class ResType>
    void
    Query<ReqType>::write(Response<ResType>& response,
                          beast::error_code const& error)
    {
        if (error)
        {
            m_error = error;
        }
        else
        {
            beast::http::async_write(m_socket, *m_request, [this, &response](Error const& error) {
                read(response, error);
            });
        }
    }
    
    template<class ReqType>
    template<class ResType>
    void
    Query<ReqType>::read(Response<ResType>& response,
                         Error const& error)
    {
        if (error)
        {
            m_error = error;
        }
        else
        {
            beast::http::async_read(m_socket, m_buffer, response, [this](Error const& error) {
                shutdown(error);
            });
        }
    }
    
    template<class ReqType>
    void
    Query<ReqType>::shutdown(Error const& error)
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
