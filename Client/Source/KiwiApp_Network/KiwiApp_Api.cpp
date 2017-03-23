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
    
    bool Api::Document::operator==(Api::Document const& other_doc) const
    {
        return (_id == other_doc._id);
    }
    
    // ================================================================================ //
    //                                    API REQUEST                                   //
    // ================================================================================ //
    
    Api::Api(std::string const& host, uint16_t port, Protocol protocol) :
    m_protocol(protocol),
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
    
    std::string Api::getHost() const
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
    
    std::string Api::getProtocolStr() const
    {
        return (m_protocol == Protocol::HTTP) ? "http" : "https";
    }
    
    std::string Api::getApiRootUrl() const
    {
        cpr::Url url{getProtocolStr() + "://" + m_host};
        
        if(m_port != 80)
        {
            url += ':' + std::to_string(m_port);
        }
        
        url += api_root + sep;
        
        return url;
        
        /*
        const auto settings = getAppSettings().useDefault();
        cpr::Url url{"http://"};
        
        url += settings.getProperty("distant_server_host", "localhost")
        .toString().toStdString();
        
        url += ':' + settings.getProperty("distant_server_api_port", "8080")
        .toString().toStdString();
        
        url += api_root + sep;
        
        //url = "http://54.234.132.205:80/api/documents";
        //url = "http://localhost:8080/api/";
        
        return url;
        */
    }
    
    void Api::getDocuments(std::function<void(Api::Documents)> callback)
    {
        auto res_callback = [callback = std::move(callback)](cpr::Response res) {

            if(res.status_code == 200)
            {
                auto j = json::parse(res.text);
                
                if(j.is_array())
                {
                    // parse each json objects as document and store them in a vector.
                    callback(j);
                    return;
                }
            }
            
            callback({});
        };
        
        
        storeRequest(cpr::GetCallback(std::move(res_callback),
                                      getApiRootUrl() + "documents",
                                      cpr::Timeout{3000}));
    }
    
    void Api::createDocument(std::function<void(Document)> callback,
                             std::string const& document_name)
    {
        
        cpr::Payload payload{{}};
        
        if(!document_name.empty())
        {
            payload.AddPair({"name", document_name});
        }
        
        auto res_callback = [callback = std::move(callback)](cpr::Response res) {
            
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
            
        };
        
        storeRequest(cpr::PostCallback(std::move(res_callback),
                                       getApiRootUrl() + "documents",
                                       std::move(payload),
                                       cpr::Timeout{3000}));
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
}
