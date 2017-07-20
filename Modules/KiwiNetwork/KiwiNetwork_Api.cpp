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

#include "KiwiNetwork_Api.h"
#include "KiwiNetwork_Http.h"

namespace kiwi
{
    namespace network
    {
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
        
        // ================================================================================ //
        //                                    API REQUEST                                   //
        // ================================================================================ //
        
        Api::Api(std::string const& host, uint16_t port) :
        m_host(host),
        m_port(port)
        {
            ;
        }
        
        //! @brief Destructor
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
        
        void Api::getAuthToken(std::string const& username, std::string const& password,
                               std::function<void(Api::Response res, Api::Error error)> callback)
        {
            auto request = std::make_unique<Api::Request>();
            request->set(beast::http::field::host, m_host);
            request->set(beast::http::field::content_type, "application/x-www-form-urlencoded");
            request->method(beast::http::verb::post);
            request->target("/api/login");
            request->version = 11;
            
            request->body = "username=" + username + "&password=" + password;
            
            request->prepare_payload();
            
            storeRequest(Http::writeAsync(std::move(request),
                                          std::to_string(m_port),
                                          std::move(callback),
                                          std::chrono::milliseconds(3000)));
        }
        
        void Api::getDocuments(std::function<void(Api::Response, Api::Error, Api::Documents)> callback)
        {
            std::function<void(Api::Response, Api::Error)> res_callback =
            [callback = std::move(callback)](Api::Response res, Api::Error error)
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
            
            auto request = std::make_unique<Api::Request>();
            request->set(beast::http::field::host, m_host);
            request->method(beast::http::verb::get);
            request->target("/api/documents");
            request->version = 11;
            
            request->prepare_payload();
            
            storeRequest(Http::writeAsync(std::move(request),
                                          std::to_string(m_port),
                                          res_callback,
                                          std::chrono::milliseconds(3000)));
        }
        
        void Api::createDocument(std::function<void(Api::Response, Api::Error, Document)> callback,
                                 std::string const& document_name)
        {
            auto request = std::make_unique<Api::Request>();
            request->set(beast::http::field::host, m_host);
            request->set(beast::http::field::content_type, "application/x-www-form-urlencoded");
            request->method(beast::http::verb::post);
            request->target("/api/documents");
            request->version = 11;
            
            request->body = "name=" + document_name;
            
            request->prepare_payload();
            
            std::function<void(Api::Response, Api::Error)> res_callback =
            [callback = std::move(callback)](Api::Response res, Api::Error error)
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
            
            storeRequest(Http::writeAsync(std::move(request),
                                          std::to_string(m_port),
                                          res_callback,
                                          std::chrono::milliseconds(3000)));
        }
        
        void Api::renameDocument(std::function<void(Api::Response res, Api::Error error)> callback,
                                 std::string document_id, std::string const& new_name)
        {
            assert(!new_name.empty() && "name should not be empty!");
            
            auto request = std::make_unique<Api::Request>();
            request->set(beast::http::field::host, m_host);
            request->set(beast::http::field::content_type, "application/x-www-form-urlencoded");
            request->method(beast::http::verb::put);
            request->version = 11;
            request->target("/api/documents/" + document_id);
            
            request->body = "name=" + new_name;
            
            request->prepare_payload();
            
            storeRequest(Http::writeAsync(std::move(request),
                                          std::to_string(m_port),
                                          callback,
                                          std::chrono::milliseconds(3000)));
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
        
        bool Api::hasJsonHeader(Api::Response const& res)
        {
            return (res[beast::http::field::content_type] == "application/json; charset=utf-8");
        }
        
        // ================================================================================ //
        //                                    API REQUEST                                   //
        // ================================================================================ //
        
        /*
         Api::Request::Request()
         {
         ;
         }
         
         Api::Request::~Request()
         {
         ;
         }
         */
    }
}
