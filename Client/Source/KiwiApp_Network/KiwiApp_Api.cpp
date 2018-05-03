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
    const std::string Api::Endpoint::releases {Api::Endpoint::root + "/releases"};
    
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
    m_controller(controller),
    m_requests()
    {
        ;
    }
    
    Api::~Api()
    {
        
    }
    
    // ================================================================================ //
    //                                   API REQUESTS                                   //
    // ================================================================================ //
    
    void Api::cancelRequest(uint64_t request_id)
    {
        auto request = std::find_if(m_requests.begin(),
                                    m_requests.end(),
                                    [request_id](std::unique_ptr<Session> const& session)
        {
            return session->getId() == request_id;
        });
        
        if (request != m_requests.end())
        {
            (*request)->cancel();
            m_requests.erase(request);
        }
    }
    
    bool Api::isPending(uint64_t request_id)
    {
        auto request = std::find_if(m_requests.begin(),
                                    m_requests.end(),
                                    [request_id](std::unique_ptr<Session> const& session)
        {
            return session->getId() == request_id;
        });
        
        return request != m_requests.end() && !(*request)->executed();
    }
    
    void Api::cancelAll()
    {
        for (auto request = m_requests.begin(); request != m_requests.end(); ++request)
        {
            (*request)->cancel();
        }
    }
    
    uint64_t Api::login(std::string const& username_or_email,
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
                
                if(j.is_object() && j.count("user"))
                {
                    AuthUser user(j["user"].get<AuthUser>());
                    
                    if(user.isLoggedIn())
                    {
                        success(std::move(user));
                    }
                    else
                    {
                        fail({res.result_int(), "Failed to parse result"});
                    }
                    
                    return;
                }
            }
            
            fail(res);
        };
        
        session->PostAsync(std::move(cb));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::signup(std::string const& username,
                         std::string const& email,
                         std::string const& password,
                         CallbackFn<std::string> success_cb,
                         ErrorCallback error_cb)
    {
        assert(!username.empty());
        assert(!email.empty());
        assert(!password.empty());
        
        auto session = makeSession(Endpoint::users, false);
        
        session->setPayload({
            {"username", username},
            {"email", email},
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
                
                if(j.is_object() && j.count("message"))
                {
                    success(j["message"]);
                }
                else
                {
                    fail({res.result_int(), "Failed to parse result"});
                }
            }
            else
            {
                fail(res);
            }
        };
        
        session->PostAsync(std::move(cb));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::getUsers(std::unordered_set<uint64_t> const& user_ids,
                           CallbackFn<Api::Users> success_cb,
                           ErrorCallback error_cb)
    {
        auto cb =
        [success = std::move(success_cb), fail = std::move(error_cb)]
        (Response res)
        {
            if (!res.error
                && hasJsonHeader(res)
                && res.result() == beast::http::status::ok)
            {
                success(json::parse(res.body));
            }
            else
            {
                fail(res);
            }
        };
        
        auto session = makeSession(Endpoint::users);
        
        json j_users;
        
        for(uint64_t const& user_id : user_ids)
        {
            std::ostringstream result;
            result << std::hex << std::uppercase << user_id;
            
            j_users.push_back(result.str());
        }
        
        session->setParameters({{"ids", j_users.dump()}});
        
        session->GetAsync(std::move(cb));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::getDocuments(std::function<void(Response, Api::Documents)> callback)
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
        
        session->GetAsync(std::move(cb));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::createDocument(std::string const& document_name,
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
                        return;
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
        
        session->PostAsync(std::move(cb));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::uploadDocument(std::string const& name,
                                 std::string const& data,
                                 std::string const& kiwi_version,
                                 std::function<void(Response, Api::Document)> callback)
    {
        auto cb = [callback = std::move(callback)](Response res)
        {
            if (!res.error
                && res.result() == beast::http::status::ok
                && hasJsonHeader(res))
            {
                auto j = json::parse(res.body);
                
                if(j.is_object())
                {
                    // parse object as a document
                    callback(std::move(res), j);
                    return;
                }
            }
            
            callback(std::move(res), {});
        };
        
        auto session = makeSession(Endpoint::documents + "/upload");
        
        session->setParameters({{"name", name}, {"kiwi_version", kiwi_version}});
        session->setBody(data);
        
        session->PostAsync(std::move(cb));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::duplicateDocument(std::string const& document_id, Callback callback)
    {
        auto session = makeSession(Endpoint::document(document_id) + "/clone");
        
        session->PostAsync(std::move(callback));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::renameDocument(std::string document_id, std::string const& new_name,
                                 Callback callback)
    {
        assert(!new_name.empty() && "name should not be empty!");
        
        auto session = makeSession(Endpoint::document(document_id));
        session->setPayload({
            {"name", new_name}
        });
        
        session->PutAsync(std::move(callback));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::untrashDocument(std::string document_id, Callback callback)
    {
        auto session = makeSession(Endpoint::document(document_id));
        
        session->setPayload({
            {"trashed", false}
        });
        
        session->PutAsync(std::move(callback));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::trashDocument(std::string document_id, Callback callback)
    {
        auto session = makeSession(Endpoint::document(document_id));
        
        session->setPayload({
            {"trashed", true}
        });
        
        session->PutAsync(std::move(callback));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::getOpenToken(std::string document_id,
                               CallbackFn<std::string const&> success_cb,
                               ErrorCallback error_cb)
    {
        auto session = makeSession(Endpoint::document(document_id) + "/opentoken");
        
        auto callback = [success = std::move(success_cb),
                         fail = std::move(error_cb)](Response res)
        {
            if (!res.error
                && hasJsonHeader(res)
                && res.result() == beast::http::status::ok)
            {
                const auto j = json::parse(res.body);
                success(j["token"]);
            }
            else
            {
                fail(res);
            }
        };
        
        session->GetAsync(std::move(callback));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::downloadDocument(std::string document_id, Callback callback)
    {
        auto session = makeSession(Endpoint::document(document_id) + "/download");
        
        session->setParameters({{"alt", "download"}});
        session->GetAsync(std::move(callback));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::getRelease(CallbackFn<std::string const&> success_cb, ErrorCallback error_cb)
    {
        auto session = makeSession(Endpoint::releases);
        
        auto cb = [success = std::move(success_cb),
                   fail = std::move(error_cb)](Response res)
        {
            if (!res.error
                && hasJsonHeader(res)
                && res.result() == beast::http::status::ok)
            {
                const auto j = json::parse(res.body);
                
                if(j.is_object() && j.count("release"))
                {
                    std::string latest_release = j["release"];
                    success(latest_release);
                }
            }
            else
            {
                fail(res);
            }
        };
        
        session->GetAsync(std::move(cb));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::requestPasswordToken(std::string const& user_mail,
                                       CallbackFn<std::string const&> success_cb,
                                       ErrorCallback error_cb)
    {
        auto session = makeSession(Endpoint::users + "/passtoken");
        
        session->setPayload({
            {"email", user_mail}
        });
        
        auto cb = [success = std::move(success_cb),
                   fail = std::move(error_cb)](Response res)
        {
            if (!res.error
                && hasJsonHeader(res)
                && res.result() == beast::http::status::ok)
            {
                const auto j = json::parse(res.body);
                
                if(j.is_object() && j.count("message"))
                {
                    std::string message = j["message"];
                    success(message);
                }
            }
            else
            {
                fail(res);
            }
        };
        
        session->PostAsync(std::move(cb));
        
        return storeSession(std::move(session));
    }
    
    uint64_t Api::resetPassword(std::string const& token,
                            std::string const& newpass,
                            CallbackFn<std::string const&> success_cb,
                            ErrorCallback error_cb)
    {
        auto session = makeSession(Endpoint::users + "/passreset");
        
        session->setPayload({
            {"token", token},
            {"newpass", newpass}
        });
        
        auto cb = [success = std::move(success_cb),
                   fail = std::move(error_cb)](Response res)
        {
            if (!res.error
                && hasJsonHeader(res)
                && res.result() == beast::http::status::ok)
            {
                const auto j = json::parse(res.body);
                
                if(j.is_object() && j.count("message"))
                {
                    std::string message = j["message"];
                    success(message);
                }
            }
            else
            {
                fail(res);
            }
        };
        
        session->PostAsync(std::move(cb));
        
        return storeSession(std::move(session));
    }
    
    std::string Api::convertDate(std::string const& date)
    {
        std::string result = date;
        
        result.replace(result.find_first_of("T"), 1 , " ");
        result.replace(result.find_first_of("Z"), 1 , " ");
        result.append("GMT");
        
        return result;
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
        
        if(add_auth && user.isLoggedIn())
        {
            session->setAuthorization("JWT " + user.getToken());
        }
        
        return std::move(session);
    }
    
    uint64_t Api::storeSession(std::unique_ptr<Session> session)
    {
        for(auto it = m_requests.begin(); it != m_requests.end();)
        {
            if ((*it)->executed())
            {
                it = m_requests.erase(it);
            }
            else
            {
                ++it;
            }
        }
        
        uint64_t session_id = session->getId();
        
        m_requests.emplace(std::move(session));
        
        return session_id;
    }
    
    // ================================================================================ //
    //                                     API ERROR                                    //
    // ================================================================================ //
    
    Api::Error::Error()
    : m_status_code(0)
    , m_message("Unknown Error")
    {
        ;
    }
    
    Api::Error::Error(unsigned status_code, std::string const& message)
    : m_status_code(status_code)
    , m_message(message)
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
    
    std::string const& Api::User::getName() const
    {
        return m_name;
    }
    
    std::string const& Api::User::getEmail() const
    {
        return m_email;
    }
    
    std::string const& Api::User::getIdAsString() const
    {
        return m_id;
    }

    uint64_t Api::User::getIdAsInt() const
    {
        uint64_t result = 0ull;
        std::stringstream converter(m_id);
        converter >> std::hex >> result;
        
        return result;
    }
    
    bool Api::User::isValid() const noexcept
    {
        return (!m_id.empty() && !m_email.empty());
    }
    
    int Api::User::getApiVersion() const
    {
        return m_api_version;
    }
    
    void Api::User::resetWith(User const& other)
    {
        m_api_version = other.m_api_version;
        m_id = other.m_id;
        m_name = other.m_name;
        m_email = other.m_email;
    }
    
    void to_json(json& j, Api::User const& user)
    {
        j = json{
            {"__v", user.getApiVersion()},
            {"_id", user.getIdAsString()},
            {"username", user.getName()},
            {"email", user.getEmail()}
        };
    }
    
    void from_json(json const& j, Api::User& user)
    {
        user.m_api_version = Api::getJsonValue<int>(j, "__v");
        user.m_id = Api::getJsonValue<std::string>(j, "_id");
        user.m_name = Api::getJsonValue<std::string>(j, "username");
        user.m_email = Api::getJsonValue<std::string>(j, "email");
    }
    
    // ================================================================================ //
    //                                    API AUTH USER                                 //
    // ================================================================================ //
    
    Api::AuthUser::AuthUser(User const& other)
    : Api::User(other)
    , m_token()
    {
        
    }
    
    Api::AuthUser::AuthUser(AuthUser const& other)
    : Api::User(other)
    , m_token(other.getToken())
    {
        
    }
    
    Api::AuthUser::AuthUser(AuthUser&& other)
    : Api::User(std::forward<User>(other))
    , m_token(std::move(other.m_token))
    {
        
    }
    
    void Api::AuthUser::resetWith(AuthUser const& other)
    {
        User::resetWith(static_cast<User const&>(other));
        m_token = other.m_token;
    }
    
    bool Api::AuthUser::isLoggedIn() const
    {
        return isValid() && !m_token.empty();
    }
    
    std::string const& Api::AuthUser::getToken() const
    {
        return m_token;
    }
    
    //! @brief Helper function to convert an Api::AuthUser into a json object
    void to_json(json& j, Api::AuthUser const& user)
    {
        to_json(j, static_cast<Api::User const&>(user));
        j["token"] = user.getToken();
    }
    
    void from_json(json const& j, Api::AuthUser& user)
    {
        from_json(j, static_cast<Api::User&>(user));
        user.m_token = Api::getJsonValue<std::string>(j, "token");
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
            {"session_id", session_id_converter.str()},
            {"createdBy", doc.author_name},
            {"createdAt", doc.creation_date},
            {"lastOpenedAt", doc.opened_date},
            {"lastModdifyBy", doc.opened_user}
        };
        
        if (doc.trashed)
        {
            j.at("trashed") = true;
            j.at("trash_date") = doc.trashed_date;
        }
        else
        {
            j.at("trashed") = false;
        }
    }
    
    void from_json(json const& j, Api::Document& doc)
    {
        doc._id = Api::getJsonValue<std::string>(j, "_id");
        doc.name = Api::getJsonValue<std::string>(j, "name");
        doc.author_name = Api::getJsonValue<std::string>(j.at("createdBy"), "username");
        doc.creation_date = Api::convertDate(Api::getJsonValue<std::string>(j, "createdAt"));
        doc.opened_date = Api::convertDate(Api::getJsonValue<std::string>(j, "lastOpenedAt"));
        doc.opened_user = Api::getJsonValue<std::string>(j.at("lastOpenedBy"), "username");
        doc.trashed = Api::getJsonValue<bool>(j, "trashed");
        doc.trashed_date = doc.trashed ? Api::convertDate(Api::getJsonValue<std::string>(j, "trashedDate")) : "";
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
    
    bool Api::Controller::isUserLoggedIn() const
    {
        return m_auth_user.isLoggedIn();
    }
    
    Api::AuthUser const& Api::Controller::getAuthUser() const
    {
        return m_auth_user;
    }
    
    void Api::Controller::clearToken()
    {
        m_auth_user.m_token.clear();
    }
}
