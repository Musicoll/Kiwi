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

#include "KiwiNetwork_Http.h"

#include <json.hpp>

namespace kiwi
{
    using nlohmann::json;
    
    namespace network
    {
        // ================================================================================ //
        //                                         API                                      //
        // ================================================================================ //
        
        //! @brief An API request handler class.
        class Api
        {
        public: // methods
            
            enum class Protocol : uint8_t { HTTP = 0, HTTPS = 1 };
            
            struct Document
            {
                std::string _id = "0";
                std::string name = "";
                uint64_t    session_id = 0ul;
                
                //! @brief Returns true if the Document match another Document
                bool operator==(Document const& other_doc) const;
            };
            
            using Documents = std::vector<Api::Document>;
            
            class User;
            
            using Response = beast::http::response<beast::http::string_body>;
            
            using Request = beast::http::request<beast::http::string_body>;
            
            using Error = beast::error_code;
            
            //! @brief Constructor
            Api(std::string const& host, uint16_t port = 80, Protocol protocol = Api::Protocol::HTTP);
            
            //! @brief Destructor
            ~Api();
            
            //! @brief Set the API host.
            void setHost(std::string const& host);
            
            //! @brief Get the API host.
            std::string const& getHost() const;
            
            //! @brief Set the API port.
            void setPort(uint16_t port) noexcept;
            
            //! @brief Get the API port.
            uint16_t getPort() const noexcept;
            
            //! @brief Make an async API request to get a list of documents
            void getDocuments(std::function<void(Api::Response const&, Api::Error const&, Api::Documents)> callback);
            
            //! @brief Make an async API request to create a new document
            //! @param callback
            void createDocument(std::function<void(Api::Response const&, Api::Error const&, Api::Document)> callback,
                                std::string const& document_name = "");
            
            //! @brief Rename a document asynchronously.
            //! @param callback The callback method that will be called when the request is completed.
            void renameDocument(std::function<void(Api::Response const& res, Api::Error const& error)> callback,
                                std::string document_id, std::string const& new_name);
            
        private: // methods
            
            //! @internal Store the async future request in a vector
            void storeRequest(std::future<void> && future);
            
            //! @internal Check if the response header has a JSON content-type
            static bool hasJsonHeader(Api::Response const& res);
            
        private: // variables
            
            Protocol        m_protocol;
            std::string     m_host;
            uint16_t        m_port;
            
            std::vector<std::future<void>> m_pending_requests;
        };
        
        //! @brief Helper function to convert an Api::Document into a json object
        void to_json(json& j, Api::Document const& doc);
        
        //! @brief Helper function to convert a json object into an Api::Document
        void from_json(json const& j, Api::Document& doc);
    }
}
