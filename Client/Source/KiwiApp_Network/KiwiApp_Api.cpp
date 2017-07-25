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

#include "KiwiApp_Api.h"

namespace kiwi
{
    // ================================================================================ //
    //                                         API                                      //
    // ================================================================================ //
    
    void Api::getAuthToken(std::string const& username,
                           std::string const& password,
                           Callback callback)
    {
        Post("/api/login", {
            {"username", username},
            {"password", password}
        }, std::move(callback), Timeout(3000));
    }
    
    void Api::getDocuments(std::function<void(Response, Error, Api::Documents)> callback)
    
    {
        auto cb = [callback = std::move(callback)](Response res, Error error)
        {
            if (res.result() == beast::http::status::ok)
            {
                if(hasJsonHeader(res))
                {
                    auto j = json::parse(res.body);
                    
                    if(j.is_array())
                    {
                        // parse each json objects as document and store them in a vector.
                        callback(res, error, j);
                        return;
                    }
                }
            }
            
            callback(res, error, {});
        };
        
        Get("/api/documents", cb, Timeout(3000));
    }
    
    void Api::createDocument(std::string const& document_name,
                             std::function<void(Response,
                                                Error,
                                                Api::Document)> callback)
    {
        auto cb = [callback = std::move(callback)](Response res, Error error)
        {
            if (res.result() == beast::http::status::ok)
            {
                if(hasJsonHeader(res))
                {
                    auto j = json::parse(res.body);
                    
                    if(j.is_object())
                    {
                        // parse object as a document
                        callback(std::move(res), error, j);
                    }
                }
            }
            
            callback(std::move(res), error, {});
        };
        
        Post("/api/documents", {
            {"name", document_name}
        }, std::move(cb), Timeout(3000));
        
    }
    
    void Api::renameDocument(std::string document_id, std::string const& new_name,
                             Callback callback)
    {
        assert(!new_name.empty() && "name should not be empty!");
        
        Put("/api/documents/" + document_id, {
            {"name", new_name}
        }, std::move(callback), Timeout(3000));
    }
    
    bool Api::hasJsonHeader(Response const& res)
    {
        return (res[beast::http::field::content_type] == "application/json; charset=utf-8");
    }
    
    // ================================================================================ //
    //                                      SESSION                                     //
    // ================================================================================ //
    
    Api::Api(std::string const& host, uint16_t port) :
    m_host(host),
    m_port(port)
    {
        ;
    }
    
    Api::~Api()
    {
        
    }
    
    void Api::setHost(std::string const& host)
    {
        m_host = host;
    }
    
    std::string const& Api::getHost() const
    {
        return m_host;
    }
    
    void Api::setPort(uint16_t port) noexcept
    {
        m_port = port;
    }
    
    uint16_t Api::getPort() const noexcept
    {
        return m_port;
    }
    
    void Api::storeRequest(std::future<void> && future)
    {
        for(std::future<void>& f : m_pending_requests)
        {
            if(f.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            {
                f = std::move(future);
                return;
            }
        }
        
        m_pending_requests.emplace_back(std::move(future));
    }
    
    void Api::Get(std::string const& endpoint,
                  Callback callback,
                  Timeout timeout)
    {
        auto request = std::make_unique<Request>();
        request->version = 11;
        
        request->set(beast::http::field::host, getHost());
        request->method(beast::http::verb::get);
        request->target(endpoint);
        request->prepare_payload();
        
        storeRequest(Http::writeAsync(std::move(request),
                                      std::to_string(m_port),
                                      std::move(callback),
                                      timeout));
    }
    
    void Api::Post(std::string const& endpoint,
                   Payload payload,
                   Callback callback,
                   Timeout timeout)
    {
        auto request = std::make_unique<Request>();
        request->version = 11;
        
        request->set(beast::http::field::host, getHost());
        request->set(beast::http::field::content_type, "application/x-www-form-urlencoded");
        request->method(beast::http::verb::post);
        request->target(endpoint);
        
        request->body = payload.content;
        request->prepare_payload();
        
        storeRequest(Http::writeAsync(std::move(request),
                                      std::to_string(m_port),
                                      std::move(callback),
                                      timeout));
    }
    
    void Api::Put(std::string const& endpoint,
                  Payload payload,
                  Callback callback,
                  Timeout timeout)
    {
        auto request = std::make_unique<Request>();
        request->version = 11;
        
        request->set(beast::http::field::host, getHost());
        request->set(beast::http::field::content_type, "application/x-www-form-urlencoded");
        request->method(beast::http::verb::put);
        request->target(endpoint);
        
        request->body = payload.content;
        request->prepare_payload();
        
        storeRequest(Http::writeAsync(std::move(request),
                                      std::to_string(m_port),
                                      std::move(callback),
                                      timeout));
    }
    
    /*
    // ================================================================================ //
    //                                      PAYLOAD                                     //
    // ================================================================================ //
    
    Api::Payload::Payload(const std::initializer_list<Pair>& pairs)
    : Payload(begin(pairs), end(pairs))
    {
        ;
    }
    
    void Api::Payload::AddPair(const Pair& pair)
    {
        if(!content.empty())
        {
            content += "&";
        }
        
        //auto escaped = cpr::util::urlEncode(pair.value);
        //content += pair.key + "=" + escaped;
        content += pair.key + "=" + pair.value;
    }
    */
    
    // ================================================================================ //
    //                                    API DOCUMENT                                  //
    // ================================================================================ //
    
    void to_json(json& j, Api::Document const& doc)
    {
        std::stringstream session_id_converter;
        session_id_converter << std::hex << doc.session_id;
        
        j = json{
            {"_id", doc._id},
            {"name", doc.name},
            {"session_id", session_id_converter.str()}
        };
    }
    
    void from_json(json const& j, Api::Document& doc)
    {
        doc._id = j["_id"].get<std::string>();
        doc.name = j["name"].get<std::string>();
        doc.session_id = 0ul;
        
        if(j.count("session_id"))
        {
            std::stringstream converter(j["session_id"].get<std::string>());
            converter >> std::hex >> doc.session_id;
        }
    }
    
    bool Api::Document::operator==(Api::Document const& other_doc) const
    {
        return (_id == other_doc._id);
    }
}
