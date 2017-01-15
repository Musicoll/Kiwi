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

#include "KiwiNetwork_TcpSocket.hpp"

#if JUCE_WINDOWS
    #include <winsock2.h>
    #include <ws2tcpip.h>

    typedef int       kiwi_socklen_t;
    typedef SOCKET    SocketHandle;
#else
    #include <netinet/ip.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>

    typedef socklen_t kiwi_socklen_t;
    typedef int       SocketHandle;
#endif

//! check http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
//! for details on implementation.

namespace kiwi
{
    namespace network
    {
        TcpSocket::TcpSocket():
        m_socket(new juce::StreamingSocket())
        {
        }
        
        TcpSocket::TcpSocket(std::unique_ptr<juce::StreamingSocket>(juce_socket)):
        m_socket(std::move(juce_socket))
        {
        }
        
        void TcpSocket::listen(size_t port)
        {
            if(!m_socket->createListener(port, "127.0.0.1"))
            {
                throw TcpSocketError("failed listening to port " + std::to_string(port));
            }
        }
        
        bool TcpSocket::setBlockingState(bool shall_block)
        {
            SocketHandle handle = m_socket->getRawSocketHandle();
            
            #if JUCE_WINDOWS
            u_long nonBlocking = shouldBlock ? 0 : (u_long) 1;
            return ioctlsocket (handle, FIONBIO, &nonBlocking) == 0;
            #else
            int socketFlags = fcntl (handle, F_GETFL, 0);
            
            if (socketFlags == -1)
                return false;
            
            if (shall_block)
                socketFlags &= ~O_NONBLOCK;
            else
                socketFlags |= O_NONBLOCK;
            
            return fcntl (handle, F_SETFL, socketFlags) == 0;
            #endif
        }
        
        std::unique_ptr<TcpSocket> TcpSocket::accept(bool shall_block)
        {
            std::unique_ptr<TcpSocket> created_socket(nullptr);
            
            if (!shall_block)
            {
                setBlockingState(false);
            }
            
            juce::StreamingSocket* juce_socket = m_socket->waitForNextConnection();
            
            if (juce_socket)
            {
                created_socket.reset(new TcpSocket(std::unique_ptr<juce::StreamingSocket>(juce_socket)));
            }
            
            if (!shall_block)
            {
                setBlockingState(true);
            }
            
            return created_socket;
        }
        
        size_t TcpSocket::getLocalPort() const
        {
            size_t port = 0;
            
            SocketHandle handle = m_socket->getRawSocketHandle();
            
            if (handle > 0)
            {
                struct sockaddr_in addr;
                kiwi_socklen_t len = sizeof (addr);
                
                if (getsockname (handle, (struct sockaddr*) &addr, &len) == 0)
                    port = ntohs (addr.sin_port);
            }
            
            return port;
        }
        
        size_t TcpSocket::getRemotePort() const
        {
            size_t port = 0;
            
            SocketHandle handle = m_socket->getRawSocketHandle();
            
            if (handle > 0)
            {
                struct sockaddr_in addr;
                kiwi_socklen_t len = sizeof (addr);
                
                if (getpeername (handle, (struct sockaddr*) &addr, &len) == 0)
                    port = ntohs (addr.sin_port);
            }
            
            return port;
        }
        
        std::string const TcpSocket::getRemoteHost() const
        {
            std::string inet_adress = "";
            
            SocketHandle handle = m_socket->getRawSocketHandle();
            
            if (handle > 0)
            {
                struct sockaddr_in addr;
                kiwi_socklen_t len = sizeof (addr);
                
                if (getpeername (handle, (struct sockaddr*) &addr, &len) == 0)
                {
                    char str[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(addr.sin_addr), str, INET_ADDRSTRLEN);
                    inet_adress.append(str);
                }
            }
            
            return inet_adress;
        }
        
        std::string const TcpSocket::getLocalHost() const
        {
            std::string inet_adress = "";
            
            SocketHandle handle = m_socket->getRawSocketHandle();
            
            if (handle > 0)
            {
                struct sockaddr_in addr;
                kiwi_socklen_t len = sizeof (addr);
                
                if (getsockname(handle, (struct sockaddr*) &addr, &len) == 0)
                {
                    char str[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(addr.sin_addr), str, INET_ADDRSTRLEN);
                    inet_adress.append(str);
                }
            }
            
            return inet_adress;
        }
        
        bool TcpSocket::connect(const std::string & ip_adress, size_t port, size_t time_out)
        {
            return m_socket->connect(ip_adress, port, time_out);
        }
        
        bool TcpSocket::isConnected() const
        {
            return m_socket->isConnected();
        }
        
        void TcpSocket::close()
        {
            m_socket->close();
        }
        
        bool TcpSocket::selectRead(size_t time_out) const
        {
            return m_socket->waitUntilReady(true, time_out) == 1;
        }
        
        bool TcpSocket::selectWrite(size_t time_out) const
        {
            return m_socket->waitUntilReady(false, time_out) == 1;
        }
        
        bool TcpSocket::read(ByteArray & byte_array)
        {
            return m_socket->read(byte_array.getData(), byte_array.getSize(), true) >= 0;
        }
        
        bool TcpSocket::write(ByteArray & byte_array)
        {
            return m_socket->write(byte_array.getData(), byte_array.getSize()) >= 0;
        }
    }
}
