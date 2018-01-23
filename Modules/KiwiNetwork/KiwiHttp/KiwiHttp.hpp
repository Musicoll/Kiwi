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
        Query<ReqType, ResType> query(std::move(request), port, timeout);
        
        while(!query.process()){}
        
        return query.getResponse();
    }
    
    template <class ReqType, class ResType>
    std::future<void>
    writeAsync(std::unique_ptr<Request<ReqType>> request,
               std::string port,
               std::function<void(Response<ResType>)> callback,
               Timeout timeout)
    {
        auto query = std::make_unique<Query<ReqType, ResType>>(std::move(request), port, timeout);
        
        return std::async(std::launch::async, [query = std::move(query), cb = std::move(callback)]()
        {
            while(!query->process()){}
            
            return cb(query->getResponse());
        });
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
    , m_port(port)
    , m_io_service()
    , m_socket(m_io_service)
    , m_timer(m_io_service)
    , m_resolver(m_io_service)
    , m_buffer()
    {
        if (timeout > Timeout(0))
        {
            m_timer.expires_from_now(timeout);
            
            m_timer.async_wait([this](Error const& error)
            {
                handleTimeout(error);
            });
        }
        
        const std::string host = m_request->at(beast::http::field::host).to_string();
        
        m_resolver.async_resolve({host, m_port}, [this](Error const& error,
                                                        tcp::resolver::iterator iterator)
        {
            connect(m_response, error, iterator);
        });
        
        m_io_service.reset();
    }
    
    template<class ReqType, class ResType>
    Query<ReqType, ResType>::~Query()
    {
        ;
    }
    
    template<class ReqType, class ResType>
    bool Query<ReqType, ResType>::process()
    {
        return !m_io_service.run_one() || !m_response.error;
    }
    
    template<class ReqType, class ResType>
    Response<ResType> const& Query<ReqType, ResType>::getResponse() const
    {
        return m_response;
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::handleTimeout(Error const& error)
    {
        m_io_service.stop();
        
        m_socket.shutdown(tcp::socket::shutdown_both, m_error);
        m_response.error = boost::asio::error::basic_errors::timed_out;
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::connect(Response<ResType>& response,
                                          Error const& error,
                                          tcp::resolver::iterator iterator)
    {
        if (error)
        {
            m_response.error = error;
        }
        else
        {
            boost::asio::async_connect(m_socket, iterator, [this, &response](Error const& error,
                                                                             tcp::resolver::iterator i) {
                this->write(response, error);
            });
        }
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::write(Response<ResType>& response,
                                        beast::error_code const& error)
    {
        if (error)
        {
            m_response.error = error;
        }
        else
        {
            beast::http::async_write(m_socket, *m_request, [this, &response](Error const& error) {
                read(response, error);
            });
        }
    }
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::read(Response<ResType>& response,
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
    
    template<class ReqType, class ResType>
    void Query<ReqType, ResType>::shutdown(Error const& error)
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
