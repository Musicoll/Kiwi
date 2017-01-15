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

#include <KiwiNetwork/KiwiNetwork_TcpSocket.hpp>

#include <thread>

using namespace kiwi;

TEST_CASE("listening socket", "[TcpSocket]")
{
    network::TcpSocket socket_1;
    network::TcpSocket socket_2;
    
    SECTION("Before sucess")
    {
        CHECK(!socket_1.isConnected());
        CHECK(socket_1.getLocalPort() == 0);
        CHECK(socket_1.getLocalHost() == "");
        CHECK(socket_1.getRemotePort() == 0);
        CHECK(socket_1.getRemoteHost() == "");
    }
    
    SECTION("Success")
    {
        socket_1.listen(9090);
        CHECK(socket_1.isConnected());
        CHECK(socket_1.getLocalPort() == 9090);
        CHECK(socket_1.getLocalHost() == "127.0.0.1");
        CHECK(socket_1.getRemotePort() == 0);
        CHECK(socket_1.getRemoteHost() == "");
    }

    SECTION("With port 0")
    {
        socket_2.listen(0);
        CHECK(socket_2.isConnected());
        CHECK(socket_2.getLocalPort() != 0);
        CHECK(socket_2.getLocalHost() == "127.0.0.1");
        CHECK(socket_2.getRemotePort() == 0);
        CHECK(socket_2.getRemoteHost() == "");
    }

    SECTION("Rebind to other port")
    {
        socket_1.listen(9191);
        CHECK(socket_1.isConnected());
        CHECK(socket_1.getLocalPort() == 9191);
        CHECK(socket_1.getLocalHost() == "127.0.0.1");
        CHECK(socket_1.getRemotePort() == 0);
        CHECK(socket_1.getRemoteHost() == "");
    }
}

TEST_CASE("client server connexion", "[TcpSocket]")
{
    network::TcpSocket socket_server;
    network::TcpSocket socket_client;
    
    SECTION("client fails connecting")
    {
        CHECK(!socket_client.connect("127.0.0.1", 80, 3000));
    }

    SECTION("server fails accepting")
    {
        socket_server.listen(0);
        CHECK(socket_server.accept(false) == nullptr);
        CHECK(socket_server.isConnected());
        CHECK(socket_server.getLocalPort() != 0);
    }

    SECTION("connexion successful")
    {
        socket_server.listen(0);
        
        size_t listen_port = socket_server.getLocalPort();
        
        std::thread cnx_thread([&socket_client, listen_port]()
        {
            while(!socket_client.isConnected())
            {
                socket_client.connect("127.0.0.1", listen_port, 10);
            }
        });
        
        std::unique_ptr<network::TcpSocket> server_side = socket_server.accept(true);
        
        size_t client_port = socket_client.getLocalPort();
        
        // Check socket client state after connexion
        CHECK(socket_client.isConnected());
        CHECK(socket_client.getLocalPort() != 0);
        CHECK(socket_client.getLocalHost() == "127.0.0.1");
        CHECK(socket_client.getRemotePort() == listen_port);
        CHECK(socket_client.getRemoteHost() == "127.0.0.1");
        
        // Check server side socket after connexion.
        CHECK(server_side != nullptr);
        CHECK(server_side->isConnected());
        CHECK(server_side->getLocalPort() == listen_port);
        CHECK(server_side->getLocalHost() == "127.0.0.1");
        CHECK(server_side->getRemotePort() == client_port);
        CHECK(server_side->getRemoteHost() == "127.0.0.1");
        
        socket_client.close();
        CHECK(!socket_client.isConnected());
        
        CHECK(server_side->isConnected());
        server_side->close();
        CHECK(!server_side->isConnected());
        
        cnx_thread.join();
    }

    SECTION("blocking connection")
    {
        socket_server.listen(0);
        
        size_t listen_port = socket_server.getLocalPort();
        
        std::thread cnx_thread([&socket_server]()
        {
            std::unique_ptr<network::TcpSocket> server_side;
            
            while(server_side == nullptr)
            {
                server_side = socket_server.accept(false);
            }
        });
        
        while(!socket_client.isConnected())
        {
            socket_client.connect("127.0.0.1", listen_port, 10);
        }
        
        CHECK(socket_client.isConnected());;
        
        cnx_thread.join();
    }
}

TEST_CASE("read and write", "[TcpSocket]")
{
    network::TcpSocket socket_server;
    network::TcpSocket socket_client;
    
    std::unique_ptr<network::TcpSocket> server_side;
    
    SECTION("without connexion")
    {
        CHECK(!socket_client.selectRead(100));
        CHECK(!socket_client.selectWrite(100));
        CHECK(!socket_server.selectRead(100));
        CHECK(!socket_client.selectWrite(100));
    }
    
    SECTION("connecting and exchanging data")
    {
        socket_server.listen(0);
        
        size_t listen_port = socket_server.getLocalPort();
        
        std::thread cnx_thread([&socket_client, listen_port]()
        {
            while(!socket_client.isConnected())
            {
                socket_client.connect("127.0.0.1", listen_port, 10);
            }
        });
        
        server_side = socket_server.accept(true);
        
        cnx_thread.join();
        
        CHECK(socket_client.isConnected());
        CHECK(server_side->isConnected());
        CHECK(socket_client.selectWrite(100));
        CHECK(!socket_server.selectRead(100));
        
        std::string signal = "hello server";
        
        network::ByteArray write_array;
        write_array << signal;
        CHECK(socket_client.write(write_array));
        
        CHECK(server_side->selectRead(100));
        network::ByteArray read_array(signal.size());
        CHECK(server_side->read(read_array));
        
        
        std::string received_string = "";
        read_array >> received_string;
        CHECK(received_string == signal);
    }
}
