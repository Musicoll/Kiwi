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

#ifndef KIWI_NETWORK_TCPSOCKET_HPP_INCLUDED
#define KIWI_NETWORK_TCPSOCKET_HPP_INCLUDED

#include <string>

#include <juce_core/juce_core.h>

#include "KiwiNetwork_Error.hpp"
#include "KiwiNetwork_ByteArray.hpp"

namespace kiwi
{
    namespace network
    {
        
        // ==================================================================================== //
        //                                          TCPSOCKET                                   //
        // ==================================================================================== //
        
        //! @brief A low level Tcp Socket class implementing typical messages.
        //! @details A wrapper over a TCP socket that redefines its interface enabling
        //! exception handling. Please note that it can only handle IPv4 sockets.
        //! @see http://broux.developpez.com/articles/c/sockets/
        
        class TcpSocket final
        {
        public: // methods
            
            //! @brief The default constructor.
            TcpSocket();
            
            //! @brief The destructor
            //! @details Disconnect the socket if connexion was established.
            ~TcpSocket() = default;
            
            //! @brief Starts listening on specified port.
            //! @details On server side, once in listening mode the socket can accept connexion with accept().
            //! @param port The port to listen to. If 0 the system will attribute an open port.
            //! @throw TcpSocketError If fails to listen.
            void listen(size_t port);
            
            //! @brief Accepts new connexions.
            //! @details Waits for new connexions.
            //! @param is_blocking If set to yes accept shall block until next connexion.
            std::unique_ptr<TcpSocket> accept(bool is_blocking);
            
            //! @brief Returns the remote port on which socket is connected.
            //! @return The port, 0 if not connected or if listening to port.
            size_t getRemotePort() const;
            
            //! @brief Returns the local port on which socket is connected.
            //! @return The port, 0 if not connected.
            size_t getLocalPort() const;
            
            //! @brief Returns the remote host's adress on which socket is connected.
            //! @details It returns an IPv4 adress.
            std::string const getRemoteHost() const;
            
            //! @brief Returns the local host's adress.
            //! @details It returns an IPv4 adress.
            std::string const getLocalHost() const;
            
            //! @brief Tries to establish connexion with listening socket.
            //! @details Will disconnect and reconnect if connexion was already establsihed. Due to juce
            //! implementation blocking connect can't be implemented yet.
            //! @param time_out time before connexion times out.
            bool connect(const std::string & ip_adress, size_t port, size_t time_out);
            
            //! @brief Returns true if the socket is connected false otherwise.
            bool isConnected() const;
            
            //! @brief Closes the connexion.
            void close();
            
            //! @brief Waits until data can be read by socket.
            //! @param time_out If 0 selectRead is blocking.
            //! @return true if data is ready.
            bool selectRead(size_t time_out) const;
            
            //! @brief Waits until data can be written by socket.
            //! @param time_out If 0 selectWrite is blocking.
            //! @return true if socket can write.
            bool selectWrite(size_t time_out) const;
            
            //! @brief Reads the socket's data into an array of bytes.
            //! @details Blocks until bytes_to_read bytes have been read.
            //! @return true if sucessfull.
            bool read(ByteArray & data);
            
            //! @brief Writes an array of bytes into a socket.
            //! @details Blocks until write operation is complete.
            //! @return true if sucessfull.
            bool write(ByteArray & data);
            
        private: // methods
            
            bool setBlockingState(bool shall_block);
            
            TcpSocket(std::unique_ptr<juce::StreamingSocket> juce_socket);
            
        private: // members
            
            std::unique_ptr<juce::StreamingSocket> m_socket;
            
        private: // deleted methods
            
            TcpSocket(TcpSocket const& other) = delete;
            TcpSocket(TcpSocket &&other) = delete;
            TcpSocket &operator=(TcpSocket const& other) = delete;
            TcpSocket &operator=(TcpSocket &&other) = delete;
        };
        
        // ==================================================================================== //
        //                                    TCPSOCKET ERROR                                   //
        // ==================================================================================== //
        
        //! @brief An exception that TcpSocket might throw.
        
        class TcpSocketError final : public network::Error
        {
        public:
            explicit TcpSocketError(const std::string& message) :
            Error(std::string("TcpSocket:") + message) {}
            
            explicit TcpSocketError(const char* message) :
            Error(std::string("TcpSocket:") + std::string(message)) {}
            
            //! @brief The destructor.
            ~TcpSocketError() noexcept override = default;
        };
        
    }
}


#endif // KIWI_NETWORK_TCPSOCKET_HPP_INCLUDED
