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

#include "KiwiHttp_Session.h"
#include "KiwiHttp_Util.h"

namespace kiwi { namespace network { namespace http {
    
    // ================================================================================ //
    //                                  HTTP PAYLOAD                                    //
    // ================================================================================ //
    
    Payload::Payload(const std::initializer_list<Pair>& pairs)
    : Payload(begin(pairs), end(pairs))
    {
        ;
    }
    
    void Payload::AddPair(const Pair& pair)
    {
        if(!content.empty())
        {
            content += "&";
        }
        
        auto escaped = util::urlEncode(pair.value);
        content += pair.key + "=" + escaped;
    }
    
    // ================================================================================ //
    //                                 HTTP PARAMETERS                                  //
    // ================================================================================ //
    
    Parameters::Parameters(std::initializer_list<Parameter> const& parameters)
    {
        for(auto const& parameter : parameters)
        {
            AddParameter(parameter);
        }
    }
    
    void Parameters::AddParameter(Parameter const& parameter)
    {
        if (!content.empty())
        {
            content += "&";
        }
        
        auto escapedKey = util::urlEncode(parameter.key);
        if (parameter.value.empty())
        {
            content += escapedKey;
        }
        else
        {
            auto escapedValue = util::urlEncode(parameter.value);
            content += escapedKey + "=" + escapedValue;
        }
    }
    
    // ================================================================================ //
    //                                       BODY                                       //
    // ================================================================================ //
    
    Body::Body(std::string body)
    : content(std::move(body))
    {
        
    }
    
    // ================================================================================ //
    //                                     SESSION                                      //
    // ================================================================================ //
    
    Session::Session()
    : m_port("80")
    , m_target()
    , m_parameters()
    , m_payload()
    , m_body()
    , m_timeout(0)
    , m_req_header()
    {
        ;
    }
    
    void Session::setHost(std::string const& host)
    {
        m_req_header.set(beast::http::field::host, host);
    }
    
    void Session::setPort(std::string const& port)
    {
        m_port = port;
    }
    
    void Session::setTarget(std::string const& endpoint)
    {
        m_target = endpoint;
    }
    
    void Session::setTimeout(Timeout timeout)
    {
        m_timeout = timeout;
    }
    
    void Session::setParameters(Parameters && parameters)
    {
        m_parameters = std::move(parameters);
    }
    
    void Session::setPayload(Payload && payload)
    {
        m_payload = std::move(payload);
    }
    
    Session::Response Session::Get()
    {
        return makeResponse(beast::http::verb::get);
    }
    
    Session::AsyncResponse Session::GetAsync(Callback callback)
    {
        return makeResponse(beast::http::verb::get, std::move(callback));
    }
    
    Session::Response Session::Post()
    {
        return makeResponse(beast::http::verb::post);
    }
    
    Session::AsyncResponse Session::PostAsync(Callback callback)
    {
        return makeResponse(beast::http::verb::post, std::move(callback));
    }
    
    Session::Response Session::Put()
    {
        return makeResponse(beast::http::verb::put);
    }
    
    Session::AsyncResponse Session::PutAsync(Callback callback)
    {
        return makeResponse(beast::http::verb::put, std::move(callback));
    }
    
    Session::Response Session::Delete()
    {
        return makeResponse(beast::http::verb::delete_);
    }
    
    Session::AsyncResponse Session::DeleteAsync(Callback callback)
    {
        return makeResponse(beast::http::verb::delete_, std::move(callback));
    }
    
    std::unique_ptr<Query<beast::http::string_body>> Session::makeQuery()
    {
        const auto makeTarget = [this]() {
            
            if(!m_parameters.content.empty())
            {
                return m_target + "?" + m_parameters.content;
            }
            
            return m_target;
        };
        
        auto request = std::make_unique<Request<beast::http::string_body>>(std::move(m_req_header));
        request->target(makeTarget());
        
        if(!(m_req_header.method() == beast::http::verb::get) &&
           (!m_payload.content.empty()
            || !m_body.content.empty()))
        {
            if(!m_payload.content.empty())
            {
                request->set(beast::http::field::content_type, "application/x-www-form-urlencoded");
                request->body = std::move(m_payload.content);
            }
            else if(!m_body.content.empty())
            {
                auto& req = *request;
                const auto content_type = req[beast::http::field::content_type];
                if(content_type.empty())
                {
                    request->set(beast::http::field::content_type, "text/plain");
                }
                
                request->body = std::move(m_body.content);
            }
        }
        
        request->prepare_payload();
        return std::make_unique<Query<beast::http::string_body>>(std::move(request), m_port, m_timeout);
    }
    
    Session::Response Session::makeResponse(beast::http::verb verb)
    {
        m_req_header.method(verb);
        return std::move(makeQuery()->writeRequest<Response::body_type>());
    }
    
    Session::AsyncResponse Session::makeResponse(beast::http::verb verb, Session::Callback && callback)
    {
        m_req_header.method(verb);
        return std::async(std::launch::async, [query{makeQuery()}, cb{std::move(callback)}]() {
            return cb(std::move(query->writeRequest<Response::body_type>()));
        });
    }
    
}}} // namespace kiwi::network::http
