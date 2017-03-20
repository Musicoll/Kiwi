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

#include "KiwiApp_Api.hpp"
#include "../KiwiApp.hpp"

namespace kiwi
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
    
    // ================================================================================ //
    //                                    API REQUEST                                   //
    // ================================================================================ //
    
    Api::Api(std::string const& host, uint16_t port)
    {
        ;
    }
    
    //! @brief Destructor
    Api::~Api()
    {
        
    }
    
    std::string Api::getApiRootUrl() const
    {
        const auto settings = getAppSettings().useDefault();
        cpr::Url url{"http://"};
        
        url += settings.getProperty("distant_server_host", "localhost")
        .toString().toStdString();
        
        url += ':' + settings.getProperty("distant_server_port", "8080")
        .toString().toStdString();
        
        url += api_root + sep;
        
        //url = "http://localhost:8080/api/";
        
        return url;
    }
    
    void Api::getDocuments(std::function<void(Api::Documents)> const& callback)
    {
        cpr::GetCallback([&callback](cpr::Response res) {
            
            if(res.status_code == 200)
            {
                auto j = json::parse(res.text);
                
                if(j.is_array())
                {
                    // parse each json objects as document and store them in a vector.
                    callback({j.begin(), j.end()});
                    return;
                }
            }
            
            callback({});
            
        }, getApiRootUrl() + "documents", cpr::Timeout{1000}).wait();
    }
    
    void Api::createDocument(std::function<void(Document)> const& callback,
                             std::string const& document_name)
    {
        cpr::PostCallback([&callback](cpr::Response res) {
            
            if(res.status_code == 200)
            {
                auto j = json::parse(res.text);
                
                if(j.is_object())
                {
                    // parse object as a document
                    callback(j);
                    return;
                }
            }
            
            callback({});
            
        }, getApiRootUrl() + "documents",
                          cpr::Payload{{"name", document_name}},
                          cpr::Timeout{1000}).wait();
    }
}
