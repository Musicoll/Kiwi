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
    //                                       API                                        //
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
    
    // ================================================================================ //
    //                                   API REQUESTS                                   //
    // ================================================================================ //
    
    void Api::getAuthToken(std::string const& username,
                           std::string const& password,
                           Callback callback)
    {
        auto session = makeSession("/api/login");
        session->setPayload({
            {"username", username},
            {"password", password}
        });
        
        storeFuture(session->GetAsync(std::move(callback)));
    }
    
    void Api::getDocuments(std::function<void(Response, Api::Documents)> callback)
    {
        auto cb = [callback{std::move(callback)}](Response res)
        {
            if (!res.error && res.result() == beast::http::status::ok)
            {
                if(hasJsonHeader(res))
                {
                    auto j = json::parse(res.body);
                    
                    if(j.is_array())
                    {
                        // parse each json objects as document and store them in a vector.
                        callback(std::move(res), j);
                        return;
                    }
                }
            }
            
            callback(std::move(res), {});
        };
        
        auto session = makeSession("/api/documents");
        storeFuture(session->GetAsync(std::move(cb)));
    }
    
    void Api::createDocument(std::string const& document_name,
                             std::function<void(Response, Api::Document)> callback)
    {
        auto cb = [callback = std::move(callback)](Response res)
        {
            if (!res.error && res.result() == beast::http::status::ok)
            {
                if(hasJsonHeader(res))
                {
                    auto j = json::parse(res.body);
                    
                    if(j.is_object())
                    {
                        // parse object as a document
                        callback(std::move(res), j);
                    }
                }
            }
            
            callback(std::move(res), {});
        };
        
        auto session = makeSession("/api/documents");
        if(!document_name.empty())
        {
            session->setPayload({
                {"name", document_name}
            });
        }
        
        storeFuture(session->PostAsync(std::move(cb)));
    }
    
    void Api::renameDocument(std::string document_id, std::string const& new_name,
                             Callback callback)
    {
        assert(!new_name.empty() && "name should not be empty!");
        
        auto session = makeSession("/api/documents" + document_id);
        session->setPayload({
            {"name", new_name}
        });
        
        storeFuture(session->PutAsync(std::move(callback)));
    }
    
    bool Api::hasJsonHeader(Response const& res)
    {
        return (res[beast::http::field::content_type] == "application/json; charset=utf-8");
    }
    
    std::unique_ptr<Api::Session> Api::makeSession(std::string const& endpoint)
    {
        auto session = std::make_unique<Session>();
        session->setHost(m_host);
        session->setPort(std::to_string(m_port));
        session->setTarget(endpoint);
        session->setTimeout(http::Timeout(3000));
        return std::move(session);
    }
    
    void Api::storeFuture(std::future<void> && future)
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
