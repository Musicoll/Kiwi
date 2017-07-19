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
    using network::Http;
    
    // ================================================================================ //
    //                                         API                                      //
    // ================================================================================ //
    
    //! @brief An API request handler class.
    class Api
    {
    public: // methods
        
        using BodyType = beast::http::string_body;
        using Response = Http::Response<BodyType>;
        using Request = beast::http::request<BodyType>;
        using Error = Http::Error;
        using Callback = std::function<void(Response, Error)>;
        using Timeout = Http::Timeout;
        
        class Document;
        class User;
        
        using Documents = std::vector<Api::Document>;
        
        using Payload = Http::Payload;
        
        //! @brief Constructor
        Api(std::string const& host = "127.0.0.1", uint16_t port = 80);
        
        //! @brief Destructor
        ~Api();
        
        //! @brief Set the host.
        void setHost(std::string const& host);
        
        //! @brief Get the host.
        std::string const& getHost() const;
        
        //! @brief Set the port.
        void setPort(uint16_t port) noexcept;
        
        //! @brief Get the port.
        uint16_t getPort() const noexcept;
        
        //! @brief Get the authentication token
        //! @param username_or_email user name or email adress
        //! @param password password
        void getAuthToken(std::string const& username_or_email,
                          std::string const& password,
                          Callback callback);
        
        //! @brief Make an async API request to get a list of documents
        void getDocuments(std::function<void(Response, Error, Api::Documents)> callback);
        
        //! @brief Make an async API request to create a new document
        //! @param callback
        void createDocument(std::string const& document_name,
                            std::function<void(Response, Error, Api::Document)> callback);
        
        //! @brief Rename a document asynchronously.
        //! @param callback The callback method that will be called when the request is completed.
        void renameDocument(std::string document_id, std::string const& new_name,
                            Callback callback);
        
    private: // methods
        
        //! @brief Make a Post request.
        void Post(std::string const& endpoint,
                  Payload payload,
                  Callback callback,
                  Timeout timeout = Timeout(0));
        
        //! @brief Make a Get request.
        void Get(std::string const& endpoint,
                 Callback callback,
                 Timeout timeout = Timeout(0));
        
        //! @brief Make a Put request.
        void Put(std::string const& endpoint,
                 Payload payload,
                 Callback callback,
                 Timeout timeout = Timeout(0));
        
        //! @internal Store the async future request in a vector
        void storeRequest(std::future<void> && future);
        
        //! @internal Check if the response header has a JSON content-type
        static bool hasJsonHeader(Response const& res);
        
    private: // variables
        
        std::string     m_host;
        uint16_t        m_port;
        std::string     m_auth_token;
        
        std::vector<std::future<void>> m_pending_requests;
        
    private: // deleted methods
        
        Api() = delete;
        Api(Api const&) = delete;
        Api(Api &&) = delete;
        Api& operator=(Api const&) = delete;
        Api& operator=(Api &&) = delete;
    };
    
    // ================================================================================ //
    //                                    API DOCUMENT                                  //
    // ================================================================================ //
    
    class Api::Document
    {
    public:
        
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
}
