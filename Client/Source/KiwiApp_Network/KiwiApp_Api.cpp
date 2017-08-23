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
    const std::string Api::Endpoint::root = "/api";
    const std::string Api::Endpoint::login {Api::Endpoint::root + "/login"};
    const std::string Api::Endpoint::documents {Api::Endpoint::root + "/documents"};
    const std::string Api::Endpoint::users {Api::Endpoint::root + "/users"};
    
    std::string Api::Endpoint::document(std::string const& document_id)
    {
        return Api::Endpoint::documents + '/' + document_id;
    }
    
    std::string Api::Endpoint::user(std::string const& user_id)
    {
        return Api::Endpoint::users + '/' + user_id;
    }
    
    // ================================================================================ //
    //                                       API                                        //
    // ================================================================================ //
    
    Api::Api(Api::Controller& controller) :
    m_controller(controller)
    {
        ;
    }
    
    Api::~Api()
    {
        
    }
    
    // ================================================================================ //
    //                                   API REQUESTS                                   //
    // ================================================================================ //
    
    void Api::login(std::string const& username_or_email,
                    std::string const& password,
                    CallbackFn<AuthUser> success_cb,
                    ErrorCallback error_cb)
    {
        assert(!username_or_email.empty());
        assert(!password.empty());
        
        auto session = makeSession(Endpoint::login, false);
        
        session->setPayload({
            {"username", username_or_email},
            {"password", password}
        });
        
        auto cb = [success = std::move(success_cb),
                   fail = std::move(error_cb)](Response res)
        {
            if (!res.error
                && hasJsonHeader(res)
                && res.result() == beast::http::status::ok)
            {
                const auto j = json::parse(res.body);
                
                if(j.is_object() && j.count("user") && j.count("token"))
                {
                    success({j["user"], j["token"]});
                    return;
                }
            }
            
            fail(res);
        };
        
        storeFuture(session->PostAsync(std::move(cb)));
    }
    
    void Api::getDocuments(std::function<void(Response, Api::Documents)> callback)
    {
        auto cb = [callback = std::move(callback)](Response res)
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
        
        auto session = makeSession(Endpoint::documents);
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
        
        auto session = makeSession(Endpoint::documents);
        
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
        
        auto session = makeSession(Endpoint::document(document_id));
        session->setPayload({
            {"name", new_name}
        });
        
        storeFuture(session->PutAsync(std::move(callback)));
    }
    
    bool Api::hasJsonHeader(Response const& res)
    {
        return (res[beast::http::field::content_type] == "application/json; charset=utf-8");
    }
    
    std::unique_ptr<Api::Session> Api::makeSession(std::string const& endpoint, bool add_auth)
    {
        auto session = std::make_unique<Session>();
        session->setHost(m_controller.getHost());
        session->setPort(std::to_string(m_controller.getPort()));
        session->setTarget(endpoint);
        session->setTimeout(http::Timeout(3000));
        
        const AuthUser& user = m_controller.getAuthUser();
        
        if(add_auth && !user.token.empty())
        {
            session->setAuthorization("JWT " + user.token);
        }
        
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
    //                                     API ERROR                                    //
    // ================================================================================ //
    
    Api::Error::Error()
    : m_status_code(0)
    , m_message("Unknown Error")
    {
        
    }
        
    Api::Error::Error(Api::Response const& response)
    : m_status_code(response.result_int())
    , m_message(response.error ? response.error.message() : "Unknown Error")
    {
        if(!response.error && hasJsonHeader(response))
        {
            const auto j = json::parse(response.body);
            
            if(j.count("message"))
            {
                m_message = j["message"];
            }
        }
    }
    
    unsigned Api::Error::getStatusCode() const
    {
        return m_status_code;
    }
    
    std::string const& Api::Error::getMessage() const
    {
        return m_message;
    }
    
    // ================================================================================ //
    //                                      API USER                                    //
    // ================================================================================ //
    
    void to_json(json& j, Api::User const& user)
    {
        j = json{
            {"__v", user.api_version},
            {"_id", user._id},
            {"username", user.name},
            {"email", user.email}
        };
    }
    
    void from_json(json const& j, Api::User& user)
    {
        user.api_version = j.count("__v") ? j["__v"].get<int>() : 0;
        user._id = j.count("_id") ? j["_id"].get<std::string>() : "";
        user.name = j.count("username") ? j["username"].get<std::string>() : "";
        user.email = j.count("email") ? j["email"].get<std::string>() : "";
    }
    
    // ================================================================================ //
    //                                    API AUTH USER                                 //
    // ================================================================================ //
    
    Api::AuthUser::AuthUser(User const& user, std::string const& _token)
    : Api::User(user)
    , token(_token)
    {
        ;
    }
    
    bool Api::AuthUser::isValid() const noexcept
    {
        return (!_id.empty() && !token.empty());
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
        doc._id = j.count("_id") ? j["_id"].get<std::string>() : "";
        doc.name = j.count("name") ? j["name"].get<std::string>() : "";
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
    //                                   API CONTROLLER                                 //
    // ================================================================================ //
    
    Api::Controller::Controller()
    : Api::Controller("127.0.0.1", 80)
    {
        ;
    }
    
    Api::Controller::Controller(std::string const& host, uint16_t port)
    : m_host(host)
    , m_port(port)
    , m_auth_user()
    {
        ;
    }
    
    void Api::Controller::setHost(std::string const& host)
    {
        m_host = host;
    }
    
    std::string const& Api::Controller::getHost() const
    {
        return m_host;
    }
    
    void Api::Controller::setPort(uint16_t port) noexcept
    {
        m_port = port;
    }
    
    uint16_t Api::Controller::getPort() const noexcept
    {
        return m_port;
    }
    
    bool Api::Controller::isUserLoggedIn()
    {
        return m_auth_user.isValid();
    }
    
    Api::AuthUser const& Api::Controller::getAuthUser() const
    {
        return m_auth_user;
    }
    
    void Api::Controller::logout()
    {
        m_auth_user.token.clear();
    }
}
