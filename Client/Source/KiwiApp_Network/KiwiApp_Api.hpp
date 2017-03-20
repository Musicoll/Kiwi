/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_APP_API_REQUEST_HPP_INCLUDED
#define KIWI_APP_API_REQUEST_HPP_INCLUDED

#include <juce_core/juce_core.h>

#include <cpr/cpr.h>

#include <json.hpp>
using nlohmann::json;

namespace kiwi
{
    // ================================================================================ //
    //                                    API REQUEST                                   //
    // ================================================================================ //
    
    //! @brief An API request handler class.
    class Api
    {   
    public: // methods
        
        struct Error
        {
            
        };
        
        struct Document
        {
            std::string _id = "0";
            std::string name = "";
            uint64_t    session_id = 0ul;
        };
        
        using Documents = std::vector<Api::Document>;
        
        class User;
        
        //! @brief Constructor
        Api(std::string const& host, uint16_t port = 80);
        
        //! @brief Destructor
        ~Api();
        
        //! @brief Get the API root URL
        std::string getApiRootUrl() const;
        
        //! @brief Make an API request to get a list of documents
        void getDocuments(std::function<void(Api::Documents)> const& success_callback);
        
        //! @brief Make an API request to create a new document
        //! @param callback
        void createDocument(std::function<void(Api::Document)> const& success,
                            std::string const& document_name = "");
        
    private: // members
        
        const std::string sep = "/";
        const std::string api_root = "/api";
    };
    
    //! @brief Helper function to convert an Api::Document into a json object
    void to_json(json& j, Api::Document const& doc);
    
    //! @brief Helper function to convert a json object into an Api::Document
    void from_json(json const& j, Api::Document& doc);
}


#endif // KIWI_APP_API_REQUEST_HPP_INCLUDED
