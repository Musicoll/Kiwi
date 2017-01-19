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

#include "../catch.hpp"

#include <cstring>

#include <juce_core/juce_core.h>

#include <KiwiNetwork/KiwiNetwork_Query.hpp>

using namespace kiwi;

TEST_CASE("Construction", "[Query]")
{
    SECTION("good json format")
    {
        std::string json_msg("{\"id\": 1}");
        REQUIRE_NOTHROW(network::Query query(json_msg););
    }
    
    SECTION("wrong json format")
    {
        REQUIRE_THROWS(network::Query("hello you"));
    }
}

TEST_CASE("Testing query types", "[Query]")
{
    SECTION("Void")
    {
        network::Query send_query;
        
        send_query.setProperty("prop", juce::var());
        CHECK(send_query.hasProperty("prop"));
        
        std::string const& mes = send_query.getMessage();
        
        network::Query rec_query(mes);
        CHECK(rec_query.hasProperty("prop"));
        CHECK(rec_query.getProperty("prop").isVoid());
    }
    
    SECTION("Int")
    {
        network::Query send_query;
        int send_value = 666;
        
        send_query.setProperty("prop", send_value);
        CHECK(send_query.hasProperty("prop"));
        
        std::string const& mes = send_query.getMessage();
        
        network::Query rec_query(mes);
        CHECK(rec_query.hasProperty("prop"));
        CHECK(rec_query.getProperty("prop").isInt());
        CHECK(static_cast<int>(rec_query.getProperty("prop")) == 666);
    }
    
    SECTION("Int64")
    {
        network::Query send_query;
        juce::int64 send_value = 666;
        
        send_query.setProperty("prop", send_value);
        CHECK(send_query.hasProperty("prop"));
        
        std::string const& mes = send_query.getMessage();
        
        network::Query rec_query(mes);
        CHECK(rec_query.hasProperty("prop"));
        CHECK(static_cast<juce::int64>(rec_query.getProperty("prop")) == 666);
    }
    
    SECTION("Float")
    {
        network::Query send_query;
        float send_value = 6.66;
        
        send_query.setProperty("prop", send_value);
        CHECK(send_query.hasProperty("prop"));
        
        std::string const& mes = send_query.getMessage();
        
        network::Query rec_query(mes);
        CHECK(rec_query.hasProperty("prop"));
        CHECK(rec_query.getProperty("prop").isDouble());
    }
    
    SECTION("String")
    {
        network::Query send_query;
        
        send_query.setProperty("prop", "666");
        CHECK(send_query.hasProperty("prop"));
        
        std::string const& mes = send_query.getMessage();
        
        network::Query rec_query(mes);
        CHECK(rec_query.hasProperty("prop"));
        CHECK(rec_query.getProperty("prop").toString() == "666");
    }
    
    SECTION("Querry nested")
    {
        network::Query send_query;
        
        // Initialise documents
        network::Query doc;
        doc.setProperty("owner", "jean");
        
        send_query.setProperty("doc", doc);
        
        // Post insertion initialisation
        doc.setProperty("scope", "LAN");
        
        std::string const& mes = send_query.getMessage();
        
        network::Query rec_query(mes);
        CHECK(rec_query.hasProperty("doc"));
        CHECK(rec_query.getProperty("doc").isObject());
        
        network::Query rec_doc(rec_query.getProperty("doc").getDynamicObject());
        CHECK(rec_doc.getProperty("scope").toString() == "LAN");
    }
    
    SECTION("Array")
    {
        network::Query send_query;
        
        network::Query doc_1;
        network::Query doc_2;
        
        doc_1.setProperty("owner", "jean");
        doc_2.setProperty("owner", "pierre");

        juce::Array<juce::var> array;
        array.add(doc_1);
        array.add(doc_2);
        
        send_query.setProperty("tab", array);
        
        std::string const& mes = send_query.getMessage();
        
        network::Query rec_query(mes);
        CHECK(rec_query.hasProperty("tab"));
        CHECK(rec_query.getProperty("tab").isArray());
        
        juce::Array<juce::var>& rec_array = *rec_query.getProperty("tab").getArray();
        CHECK(rec_array.size() == 2);
        CHECK(rec_array[0].isObject());
        CHECK(rec_array[1].isObject());
        CHECK(rec_array[0].getDynamicObject()->getProperty("owner").toString() == "jean");
        CHECK(rec_array[1].getDynamicObject()->getProperty("owner").toString() == "pierre");
    }
}
