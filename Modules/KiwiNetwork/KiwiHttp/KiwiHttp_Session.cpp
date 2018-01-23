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
    , m_future()
    , m_keep_processing(false)
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
    
    void Session::setAuthorization(std::string const& auth)
    {
        m_req_header.set(beast::http::field::authorization, auth);
    }
    
    void Session::setParameters(Parameters && parameters)
    {
        m_parameters = std::move(parameters);
    }
    
    void Session::setPayload(Payload && payload)
    {
        m_payload = std::move(payload);
    }
    
    bool Session::isPending()
    {
        return m_future.valid() && m_future.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
    }
    
    void Session::cancel()
    {
        m_keep_processing = false;
    }
    
    Session::Response Session::Get()
    {
        return makeResponse(beast::http::verb::get);
    }
    
    void Session::GetAsync(Callback callback)
    {
        makeResponse(beast::http::verb::get, std::move(callback));
    }
    
    Session::Response Session::Post()
    {
        return makeResponse(beast::http::verb::post);
    }
    
    void Session::PostAsync(Callback callback)
    {
        makeResponse(beast::http::verb::post, std::move(callback));
    }
    
    Session::Response Session::Put()
    {
        return makeResponse(beast::http::verb::put);
    }
    
    void Session::PutAsync(Callback callback)
    {
        makeResponse(beast::http::verb::put, std::move(callback));
    }
    
    Session::Response Session::Delete()
    {
        return makeResponse(beast::http::verb::delete_);
    }
    
    void Session::DeleteAsync(Callback callback)
    {
        makeResponse(beast::http::verb::delete_, std::move(callback));
    }
    
    std::unique_ptr<Query<beast::http::string_body, Session::Response::body_type>> Session::makeQuery()
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
        return std::make_unique<Query<beast::http::string_body, Response::body_type>>(std::move(request), m_port, m_timeout);
    }
    
    Session::Response Session::makeResponse(beast::http::verb verb)
    {
        m_req_header.method(verb);
        
        auto query = makeQuery();
        
        while(!query->process()){}
        
        return query->getResponse();
    }
    
    void Session::makeResponse(beast::http::verb verb, Session::Callback && callback)
    {
        m_req_header.method(verb);
        
        m_future = std::async(std::launch::async, [this, query = makeQuery(), cb = std::move(callback)]()
        {
            while(m_keep_processing)
            {
                if (query->process())
                {
                    cb(query->getResponse());
                }
            }
        });
    }
    
}}} // namespace kiwi::network::http
