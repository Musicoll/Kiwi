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

#include <KiwiNetwork/KiwiNetwork_Http.h>

#include <json.hpp>

namespace kiwi
{
    using nlohmann::json;
    using namespace network;
    
    // ================================================================================ //
    //                                         API                                      //
    // ================================================================================ //
    
    //! @brief A remote API request handler.
    //! @details This class provide helper methods to work with the remote Kiwi REST API.
    //! All operations are done asynchronously.
    class Api
    {
    public: // methods
        
        class Controller;
        
        using Session = network::http::Session;
        using Response = Session::Response;
        class Error;
        
        template<class Type>
        using CallbackFn = std::function<void(Type)>;
        
        using Callback = CallbackFn<Session::Response>;
        using ErrorCallback = CallbackFn<Api::Error>;
        
        class User;
        class AuthUser;
        class Folder;
        class Document;
        
        using Documents = std::vector<Api::Document>;
        using Users = std::vector<Api::User>;
        
    public: // methods
        
        //! @brief Constructor
        Api(Api::Controller& controller);
        
        //! @brief Destructor
        ~Api();
        
        //! @brief Cancel pending execution.
        void cancelPendingRequest();
        
    public: // requests
        
        //! @brief Attempt to log-in the user.
        //! @param username_or_email user name or email address
        //! @param password password
        void login(std::string const& username_or_email,
                   std::string const& password,
                   CallbackFn<AuthUser> success_cb,
                   ErrorCallback error_cb);
        
        //! @brief Attempt to register/signup the user.
        //! @param username user name
        //! @param email email address
        //! @param password password
        void signup(std::string const& username,
                    std::string const& email,
                    std::string const& password,
                    CallbackFn<std::string> success_cb,
                    ErrorCallback error_cb);
        
        //! @brief Make an async API request to get a list of documents
        void getDocuments(std::function<void(Response, Api::Documents)> callback);
        
        //! @brief Make an async API request to create a new document
        //! @param callback
        void createDocument(std::string const& document_name,
                            std::function<void(Response, Api::Document)> callback);
        
        //! @brief Rename a document asynchronously.
        //! @param callback The callback method that will be called when the request is completed.
        void renameDocument(std::string document_id,
                            std::string const& new_name,
                            Callback callback);
        
        //! @brief Moves a document to trash.
        void trashDocument(std::string document_id, Callback callback);
        
        //! @brief Moves document out of the trash.
        void untrashDocument(std::string document_id, Callback callback);
        
        //! @brief Retrieves Kiwi's latest release as a string. ex: v0.1.0
        void getLatestRelease(CallbackFn<std::string const&> success_cb, ErrorCallback error_cb);
        
        //! @brief Requests a reset token to the server.
        void requestPasswordToken(std::string const& user_mail, CallbackFn<std::string const&> success_cb, ErrorCallback error_cb);
        
        //! @brief Sends reset request token to the server.
        void resetPassword(std::string const& token,
                           std::string const& newpass,
                           CallbackFn<std::string const&> success_cb,
                           ErrorCallback error_cb);
        
    public: // helper methods
        
        template<class Type>
        static Type getJsonValue(json const& json, std::string const& key, Type default_value = {})
        {
            return json.count(key) ? json.at(key).get<Type>() : default_value;
        }
        
    private: // methods
        
        //! @brief Kiwi API Endpoints
        struct Endpoint
        {
            static const std::string root;
            static const std::string login;
            static const std::string documents;
            static const std::string users;
            static const std::string releases;
            
            static std::string document(std::string const& document_id);
            static std::string user(std::string const& user_id);
        };
        
        //! @internal Make a new session with pre-filled data.
        std::unique_ptr<Session> makeSession(std::string const& endpoint, bool add_auth = true);
        
        //! @internal Store the async future response in a vector
        void storeSession(std::unique_ptr<Session> session);
        
        //! @internal Check if the response header has a JSON content-type
        static bool hasJsonHeader(Response const& res);
        
    private: // variables
        
        Api::Controller&                        m_controller;
        std::vector<std::unique_ptr<Session>>   m_pending_requests;
        
    private: // deleted methods
        
        Api() = delete;
        Api(Api const&) = delete;
        Api(Api &&) = delete;
        Api& operator=(Api const&) = delete;
        Api& operator=(Api &&) = delete;
    };
    
    // ================================================================================ //
    //                                     API ERROR                                    //
    // ================================================================================ //
    
    class Api::Error
    {
    public: // methods
        
        Error();
        
        Error(unsigned status_code, std::string const& message);
        
        Error(Api::Response const& response);
        
        ~Error() = default;
        
        unsigned getStatusCode() const;
        
        std::string const& getMessage() const;
        
    private: // members
        
        unsigned m_status_code;
        std::string m_message {};
    };
    
    // ================================================================================ //
    //                                      API USER                                    //
    // ================================================================================ //
    
    class Api::User
    {
    public: // methods
        
        //! @brief Constructor.
        User() = default;
        
        virtual ~User() = default;
        
        //! @brief Constructor.
        //User& operator = (User && user);
        
        //! @brief Get the user name.
        std::string const& getName() const;
        
        //! @brief Get the user email.
        std::string const& getEmail() const;
        
        //! @brief Get the user id as a string.
        std::string const& getIdAsString() const;
        
        //! @brief Get the user id as an integer.
        uint64_t getIdAsInt() const;
        
        //! @brief Get the user api version.
        int getApiVersion() const;
        
        //! @brief Returns true if the user has a valid id and email.
        bool isValid() const noexcept;
        
        void resetWith(User const& other);
        
    private: // variables
        
        friend void from_json(json const&, Api::User&);
        
        int             m_api_version = 0;
        std::string     m_id {};
        std::string     m_name {};
        std::string     m_email {};
        
        //uint64_t        m_int_id = 0ull;
        //bool            m_int_id_need_update_flag = true;
    };
    
    //! @brief Helper function to convert an Api::User into a json object
    void to_json(json& j, Api::User const& user);
    
    //! @brief Helper function to convert a json object into an Api::User
    void from_json(json const& j, Api::User& user);
    
    // ================================================================================ //
    //                                    API AUTH USER                                 //
    // ================================================================================ //
    
    class Api::AuthUser : public Api::User
    {
    public: // methods
        
        AuthUser() = default;
        
        AuthUser(User const& other);
        
        AuthUser(AuthUser const& other);
        
        AuthUser(AuthUser && other);
        
        ~AuthUser() = default;
        
        void resetWith(AuthUser const& other);
        
        //! @brief Returns true if the user has a token.
        //! @see getToken
        bool isLoggedIn() const;
        
        //! @brief Returns the user token.
        //! @see isLoggedIn()
        std::string const& getToken() const;
        
    private: // variables
        
        friend void from_json(json const&, Api::AuthUser&);
        
        friend Api::Controller;
        
        std::string m_token {};
    };
    
    //! @brief Helper function to convert an Api::AuthUser into a json object
    void to_json(json& j, Api::AuthUser const& user);
    
    //! @brief Helper function to convert a json object into an Api::AuthUser
    void from_json(json const& j, Api::AuthUser& user);
    
    // ================================================================================ //
    //                                    API DOCUMENT                                  //
    // ================================================================================ //
    
    class Api::Document
    {
    public:
        
        Document() = default;
        
        std::string _id = "0";
        std::string name = "";
        std::string author_name = "";
        std::string creation_date = "";
        bool        trashed = false;
        std::string trashed_date = "";
        uint64_t    session_id = 0ul;
        
        //! @brief Returns true if the Document match another Document
        bool operator==(Document const& other_doc) const;
    };
    
    //! @brief Helper function to convert an Api::Document into a json object
    void to_json(json& j, Api::Document const& doc);
    
    //! @brief Helper function to convert a json object into an Api::Document
    void from_json(json const& j, Api::Document& doc);
    
    // ================================================================================ //
    //                                   API CONTROLLER                                 //
    // ================================================================================ //
    
    class Api::Controller
    {
    public: // methods
        
        //! @brief Constructor
        Controller();
        
        //! @brief Constructor
        //! @param remote_host The remote host to connect the client to.
        //! @param remote_port The remote port to connect the client to.
        Controller(std::string const& remote_host, uint16_t remote_port);
        
        virtual ~Controller() = default;
        
        //! @brief Set the host.
        void setHost(std::string const& host);
        
        //! @brief Get the host.
        std::string const& getHost() const;
        
        //! @brief Set the port.
        void setPort(uint16_t port) noexcept;
        
        //! @brief Get the port.
        uint16_t getPort() const noexcept;
        
        //! @brief Returns true if the current client api user is logged in.
        //! @see logout
        bool isUserLoggedIn() const;
        
        //! @brief Returns the authenticated user
        Api::AuthUser const& getAuthUser() const;
        
    protected: // methods
        
        //! @brief Clears the token.
        //! @details This will clear the stored token.
        //! The client will need to login again after this function call to perform authenticated request.
        //! @see login, isUserLoggedIn
        virtual void clearToken();
        
    protected: // variables
        
        std::string     m_host;
        uint16_t        m_port;
        Api::AuthUser   m_auth_user;
    };
}
