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

#ifndef KIWI_NETWORK_DNS_HPP_INCLUDED
#define KIWI_NETWORK_DNS_HPP_INCLUDED

#include <cstring>

#include <juce_core/juce_core.h>

#if JUCE_WINDOWS
    #include <winsock2.h>
    #include <ws2tcpip.h>

    #pragma warning (push)
    #pragma warning (disable : 4127 4389 4018)
#else
    #include <netdb.h>
    #include <arpa/inet.h>
#endif

#include "KiwiNetwork_Error.hpp"

namespace kiwi
{
    namespace network
    {
        // ==================================================================================== //
        //                                       DNS ERROR                                      //
        // ==================================================================================== //
        
        //! @brief A error that dnslookup might throw
        
        class DnsError final : public network::Error
        {
        public:
            explicit DnsError(const std::string& message) :
            Error(std::string("DnsError:") + message) {}
            
            explicit DnsError(const char* message) :
            Error(std::string("TcpSocket:") + std::string(message)) {}
            
            //! @brief The destructor.
            ~DnsError() noexcept override = default;
        };
        
        // ==================================================================================== //
        //                                       Dns                                            //
        // ==================================================================================== //
        
        //! @brief A dns lookup method that can resolve IPv4 and IPv6 adresses.
        //! @see http://broux.developpez.com/articles/c/sockets/
        
        class Dns final
        {
        public: // methods
            
            static std::string getHost(std::string const& host_url)
            {
                std::string inet_adress = "";
                
                struct addrinfo hints;
                struct addrinfo *res = nullptr;
                
                #if JUCE_WINDOWS // Starting socket services on windows
                WSADATA wsaData;
                // 2.2 is the latest but already available on Windows 95
                const WORD wVersionRequested = MAKEWORD (2, 2);
                WSAStartup (wVersionRequested, &wsaData);
                #endif
                
                // Initialize hints before querry
                std::memset(&hints, 0, sizeof hints);
                hints.ai_family = AF_UNSPEC;     // AF_INET or AF_INET6 to force version
                hints.ai_socktype = SOCK_STREAM; // Sockets
                
                if (getaddrinfo(host_url.c_str(), NULL, &hints, &res) == 0)
                {
                    if (res->ai_family == AF_INET) // IPv4
                    {
                        char ipstr[INET_ADDRSTRLEN];
                        struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
                        inet_ntop(res->ai_family, &(ipv4->sin_addr), ipstr, INET_ADDRSTRLEN);
                        inet_adress.append(ipstr);
                    }
                    else // IPv6
                    {
                        char ipstr6[INET6_ADDRSTRLEN];
                        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)res->ai_addr;
                        inet_ntop(res->ai_family, &(ipv6->sin6_addr), ipstr6, INET6_ADDRSTRLEN);
                        inet_adress.append(ipstr6);
                    }
                }
                
                #if JUCE_WINDOWS // Stoping services socket
                WSACleanup();
                #endif
                
                return inet_adress;
            }
            
        private: // methods
            
            Dns() = delete;
            ~Dns() = delete;
        };
    }
}

#endif // KIWI_NETWORK_DNS_HPP_INCLUDED
