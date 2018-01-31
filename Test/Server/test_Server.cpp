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

#include "../catch.hpp"

#include <thread>

#include <json.hpp>

#include <KiwiServer/KiwiServer_Server.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Def.h>

#include <flip/Document.h>
#include <flip/contrib/transport_tcp/CarrierTransportSocketTcp.h>

using namespace kiwi;

// ==================================================================================== //
//                                          SERVER                                      //
// ==================================================================================== //

std::string token = "token";
std::string kiwi_version = "v0.1.0";

std::string getMetaData()
{
    nlohmann::json j;
    j["model_version"] = KIWI_MODEL_VERSION_STRING;
    j["open_token"] = token;
    j["kiwi_version"] = kiwi_version;
    return j.dump();
}

TEST_CASE("Server - Server", "[Server, Server]")
{
    SECTION("Simple Connection - Deconnection")
    {
        kiwi::server::Server server(9191, "./server_backend_test", token, kiwi_version);
        
        // Initializing document
        flip::Document document (kiwi::model::DataModel::use (), 123456789, 'appl', 'gui ');
        flip::CarrierTransportSocketTcp carrier (document, 987654, getMetaData(), "localhost", 9191);
        
        // Client/Document connecting to server.
        while(!carrier.is_connected() || server.getSessions().empty())
        {
            carrier.process();
            server.process();
        }
        
        CHECK(carrier.is_connected());
        CHECK(server.getSessions().count(987654));
        CHECK(server.getConnectedUsers(987654).count(123456789));
        
        // Client/Document disconnects from server.
        carrier.rebind("", 0);
        
        // Client/Document connecting to server.
        while(carrier.is_connected() || !server.getSessions().empty())
        {
            carrier.process();
            server.process();
        }
        
        CHECK(!carrier.is_connected());
        CHECK(server.getSessions().empty());
        
        juce::File backend ("./server_backend_test");
        
        if (backend.exists())
        {
            backend.deleteRecursively();
        }
    }
    
    SECTION("Simple Connection - Server Killed")
    {
        std::unique_ptr<kiwi::server::Server> server(new kiwi::server::Server(9191,
                                                                              "./server_backend_test",
                                                                              token,
                                                                              kiwi_version));
        
        // Initializing document.
        flip::Document document (kiwi::model::DataModel::use (), 123456789, 'appl', 'gui ');
        flip::CarrierTransportSocketTcp carrier (document, 987654, getMetaData(), "localhost", 9191);
        
        // Client/Document connecting to server.
        while(!carrier.is_connected() || server->getSessions().empty())
        {
            carrier.process();
            server->process();
        }
        
        CHECK(carrier.is_connected());
        CHECK(server->getSessions().count(987654));
        
        // Killing server before client is disconnected.
        server.reset(nullptr);
        
        // Client automatically disconnected from server.
        while(carrier.is_connected())
        {
            carrier.process();
        }
        
        CHECK(!carrier.is_connected());
        
        juce::File backend ("./server_backend_test");
        
        if (backend.exists())
        {
            backend.deleteRecursively();
        }
    }
    
    SECTION("One user connecting to multiple document")
    {
        kiwi::server::Server server(9191, "./server_backend_test", token, kiwi_version);
        
        // Initializing documents.
        flip::Document document_1 (kiwi::model::DataModel::use (), 123456789, 'appl', 'gui ');
        flip::CarrierTransportSocketTcp carrier_1 (document_1, 987654, getMetaData(), "localhost", 9191);
        
        flip::Document document_2 (kiwi::model::DataModel::use (), 123456789, 'appl', 'gui ');
        flip::CarrierTransportSocketTcp carrier_2 (document_2, 987655, getMetaData(), "localhost", 9191);
        
        // Client/Document connecting to server.
        while(!carrier_1.is_connected() || !carrier_2.is_connected() || server.getSessions().size() != 2)
        {
            carrier_1.process();
            carrier_2.process();
            server.process();
        }
        
        CHECK(carrier_1.is_connected());
        CHECK(carrier_2.is_connected());
        CHECK(server.getSessions().count(987654));
        CHECK(server.getSessions().count(987655));
        
        carrier_1.rebind("", 0);
        carrier_2.rebind("", 0);
        
        while(carrier_1.is_connected() || carrier_2.is_connected() || !server.getSessions().empty())
        {
            carrier_1.process();
            carrier_2.process();
            server.process();
        }
        
        CHECK(!carrier_1.is_connected());
        CHECK(!carrier_2.is_connected());
        
        juce::File backend ("./server_backend_test");
        
        if (backend.exists())
        {
            backend.deleteRecursively();
        }
    }
    
    SECTION("Multiple connections")
    {
        kiwi::server::Server server(9191, "./server_backend_test", token, kiwi_version);
        
        // Initializing client 1
        flip::Document document_1 (kiwi::model::DataModel::use (), 1, 'appl', 'gui ');
        flip::CarrierTransportSocketTcp carrier_1 (document_1, 1234, getMetaData(), "localhost", 9191);
        
        kiwi::model::Patcher& patcher_1 = document_1.root<kiwi::model::Patcher>();
        
        uint64_t other_connect_1 = 0;
        
        auto m_cnx_connect_1 = patcher_1.signal_user_connect.connect([&other_connect_1](uint64_t user_id)
        {
            other_connect_1 = user_id;
        });
        
        uint64_t other_disonnect_1 = 0;
        
        auto m_cnx_disconnect_1 = patcher_1.signal_user_disconnect.connect([&other_disonnect_1](uint64_t user_id)
        {
            other_disonnect_1 = user_id;
        });
        
        // Initializing client 2
        flip::Document document_2 (kiwi::model::DataModel::use (), 2, 'appl', 'gui ');
        flip::CarrierTransportSocketTcp carrier_2 (document_2, 1234, getMetaData(), "localhost", 9191);
        
        kiwi::model::Patcher& patcher_2 = document_2.root<kiwi::model::Patcher>();
        
        std::vector<uint64_t> connected_users_2;
        
        auto m_cnx_2 = patcher_2.signal_receive_connected_users.connect([&connected_users_2](std::vector<uint64_t> users)
        {
            connected_users_2 = users;
        });
        
        // Client 1 connects to server.
        while(!carrier_1.is_connected() || server.getSessions().empty()){carrier_1.process(); server.process();}
        
        CHECK(carrier_1.is_connected());
        CHECK(server.getConnectedUsers(1234).count(1));
        
        // Client 2 connects to server. Client 2 receives all connected users, client 1 is notified.
        while(connected_users_2.empty() || !server.getConnectedUsers(1234).count(2))
        {
            carrier_2.process();
            document_2.pull();
            server.process();
        }
        
        auto it = std::find_if(connected_users_2.begin(), connected_users_2.end(), [](uint64_t id){return id == 1;});
        
        CHECK(it != connected_users_2.end());
        
        while(other_connect_1 == 0)
        {
            carrier_1.process();
            document_1.pull();
            server.process();
        }
        
        CHECK(other_connect_1 == 2);
        
        // Client 2 disconnects from server. Client 1 is notified.
        carrier_2.rebind("", 0);
        
        while(carrier_2.is_connected() || server.getConnectedUsers(1234).count(2)){carrier_2.process(); server.process();}
        while(other_disonnect_1 == 0){carrier_1.process(); document_1.pull(); server.process();}
        
        CHECK(other_disonnect_1 == 2);
        
        // Client 1 disconnects from server. Client 1 is notified.
        carrier_1.rebind("", 0);
        
        while(carrier_1.is_connected() || !server.getSessions().empty()){carrier_1.process(); server.process();}
        
        juce::File backend ("./server_backend_test");
        
        if (backend.exists())
        {
            backend.deleteRecursively();
        }
        
        SECTION("Hexadecimal convert")
        {
            std::string hex = server::hexadecimal_convert(139928974906665000);
            
            CHECK(hex == "01f120a94b18a828");
        }
    }
}
