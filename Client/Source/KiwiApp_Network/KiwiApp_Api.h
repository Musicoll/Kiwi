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
        
    public: // requests
        
        //! @brief Attempt to log the user in.
        //! @param username_or_email user name or email address
        //! @param password password
        void login(std::string const& username_or_email,
                   std::string const& password,
                   CallbackFn<AuthUser> success_cb,
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
        
    private: // methods
        
        //! @brief Kiwi API Endpoints
        struct Endpoint
        {
            static const std::string root;
            static const std::string login;
            static const std::string documents;
            static const std::string users;
            
            static std::string document(std::string const& document_id);
            static std::string user(std::string const& user_id);
        };
        
        //! @internal Make a new session with pre-filled data.
        std::unique_ptr<Session> makeSession(std::string const& endpoint, bool add_auth = true);
        
        //! @internal Store the async future response in a vector
        void storeFuture(std::future<void> && future);
        
        //! @internal Check if the response header has a JSON content-type
        static bool hasJsonHeader(Response const& res);
        
    private: // variables
        
        Api::Controller&                m_controller;
        std::vector<std::future<void>>  m_pending_requests;
        
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
        
        User() = default;
        virtual ~User() = default;
        
        uint64_t getIdAsInt() const;
        
        int             api_version = 0;
        std::string     _id {};
        std::string     name {};
        std::string     email {};
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
        
        AuthUser(User user, std::string token);
        
        ~AuthUser() = default;
        
        bool isLoggedIn() const;
        
        bool isValid() const noexcept;
        
        std::string token;
    };
    
    // ================================================================================ //
    //                                    API DOCUMENT                                  //
    // ================================================================================ //
    
    class Api::Document
    {
    public:
        
        Document() = default;
        
        std::string _id = "0";
        std::string name = "";
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
