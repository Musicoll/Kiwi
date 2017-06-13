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

#include <KiwiServer/KiwiServer_Server.h>
#include <KiwiModel/KiwiModel_DataModel.h>

#include <flip/Document.h>
#include <flip/contrib/transport_tcp/CarrierTransportSocketTcp.h>

using namespace kiwi;

// ==================================================================================== //
//                                          SERVER                                      //
// ==================================================================================== //

TEST_CASE("Server - Server", "[Server, Server]")
{
    SECTION("Simple Connection - Deconnection")
    {
        kiwi::server::Server server(9191, "./server_backend_test");
    
        std::thread server_thread([&server](){server.run();});
        
        // Initializing document
        flip::Document document (kiwi::model::DataModel::use (), 123456789ULL, 'appl', 'gui ');
        flip::CarrierTransportSocketTcp carrier (document, 987654ULL, "localhost", 9191);
        
        // Client/Document connecting to server.
        while(!carrier.is_connected())
        {
            carrier.process();
        }
        
        CHECK(carrier.is_connected());
        
        // Client/Document disconnects from server.
        carrier.rebind("", 0);
        
        CHECK(!carrier.is_connected());
        
        server.stop();
        server_thread.join();
        
        juce::File backend ("./server_backend_test");
        
        if (backend.exists())
        {
            backend.deleteRecursively();
        }
    }
    
    SECTION("Simple Connection - Server Killed")
    {
        std::unique_ptr<kiwi::server::Server> server(new kiwi::server::Server(9191, "./server_backend_test"));
        
        std::thread server_thread([&server](){server->run();});
        
        // Initializing document.
        flip::Document document (kiwi::model::DataModel::use (), 123456789ULL, 'appl', 'gui ');
        flip::CarrierTransportSocketTcp carrier (document, 987654ULL, "localhost", 9191);
        
        // Client/Document connecting to server.
        while(!carrier.is_connected())
        {
            carrier.process();
        }
        
        CHECK(carrier.is_connected());
        
        // Killing server before client is disconnected.
        server->stop();
        server_thread.join();
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
    
    SECTION("Multiple connections")
    {
        kiwi::server::Server server(9191, "./server_backend_test");
        
        std::thread server_thread([&server](){server.run();});
        
        // Initializing client 1
        flip::Document document_1 (kiwi::model::DataModel::use (), 1UL, 'appl', 'gui ');
        flip::CarrierTransportSocketTcp carrier_1 (document_1, 1234UL, "localhost", 9191);
        
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
        flip::Document document_2 (kiwi::model::DataModel::use (), 2UL, 'appl', 'gui ');
        flip::CarrierTransportSocketTcp carrier_2 (document_2, 1234ULL, "localhost", 9191);
        
        kiwi::model::Patcher& patcher_2 = document_2.root<kiwi::model::Patcher>();
        
        std::vector<uint64_t> connected_users_2;
        
        auto m_cnx_2 = patcher_2.signal_receive_connected_users.connect([&connected_users_2](std::vector<uint64_t> users)
        {
            connected_users_2 = users;
        });
        
        // Client 1 connects to server.
        while(!carrier_1.is_connected()){carrier_1.process();}
        
        CHECK(carrier_1.is_connected());
        
        // Client 2 connects to server. Client 2 receives all connected users, client 1 is notified.
        while(connected_users_2.empty()){carrier_2.process(); document_2.pull();}
        
        auto it = std::find_if(connected_users_2.begin(), connected_users_2.end(), [](uint64_t id){return id == 1;});
        
        CHECK(it != connected_users_2.end());
        
        while(other_connect_1 == 0) {carrier_1.process(); document_1.pull();}
        
        CHECK(other_connect_1 == 2);
        
        // Client 2 disconnects from server. Client 1 is notified.
        carrier_2.rebind("", 0);
        
        while(carrier_2.is_connected()){carrier_2.process();}
        while(other_disonnect_1 == 0){carrier_1.process(); document_1.pull();}
        
        CHECK(other_disonnect_1 == 2);
        
        // Client 1 disconnects from server. Client 1 is notified.
        carrier_1.rebind("", 0);
        
        while(carrier_1.is_connected()){carrier_1.process();}
        
        server.stop();
        
        server_thread.join();
        
        juce::File backend ("./server_backend_test");
        
        if (backend.exists())
        {
            backend.deleteRecursively();
        }
    }
}
