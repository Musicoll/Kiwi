/*****************************************************************************

      Socket.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/def.h"
#include "flip/contrib/Dns.h"

#include <vector>

#include <cstdint>

#if (flip_PLATFORM == flip_PLATFORM_WIN32)
   #define WIN32_LEAN_AND_MEAN
   #define NOMINMAX
   #define NOGDI
   flip_DISABLE_WARNINGS_SYSTEM_HEADERS
   #include <winsock2.h>	// Must be included before windows.h
   flip_RESTORE_WARNINGS

#else    // flip_PLATFORM
   #include <netinet/in.h>

#endif   // flip_PLATFORM



namespace flip
{



class flip_API Socket
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

#if (flip_PLATFORM == flip_PLATFORM_WIN32)
   using Id = ::SOCKET;

#else
   using Id = int;

#endif

   using Addr = ::sockaddr_in;
   using Data = std::vector <uint8_t>;

   virtual        ~Socket () = default;

   static Id      invalid_id;

   static Id      create_tcp ();
   static Id      create_udp ();
   static void    release (Id & fd);

   static Id      create_tcp_bind_listen (Addr & addr, uint16_t port);
   static Id      accept (Id fd, Addr & addr);

   static Id      create_tcp_connect (Dns::Host host, uint16_t port);
   static Id      create_tcp_connect_local (uint16_t port);

   static bool    poll_receive (Id fd);
   static int     receive (Id fd, void * buf_ptr, size_t buf_size);
   static int     send (Id fd, const void * buf_ptr, size_t buf_size);

   static void    send (Data & data, bool & error_flag, Id fd);
   static Data    receive (bool & close_flag, bool & error_flag, Id fd);

   static void    set_non_blocking (Id fd);
   static void    set_opt_nodelay (Id fd);
   static void    set_opt_reuseaddr (Id fd);
   static void    set_opt_multicast_add_membership (Id fd, uint32_t ip);
   static void    set_opt_multicast_drop_membership (Id fd, uint32_t ip);
   static void    set_opt_multicast_loop (Id fd, bool loop_flag);
   static void    set_opt_multicast_ttl (Id fd, uint8_t hop_nbr);

   static struct ::sockaddr_in
                  to_addr (uint32_t ip, uint16_t port);




/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   static void    set_opt_nosigpipe (Id fd);

#if (flip_PLATFORM == flip_PLATFORM_WIN32)
   static void    setup_wsa ();
#endif

   static const size_t
                  _buf_size = 4096;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Socket () = delete;
                  Socket (const Socket & rhs) = delete;
   Socket &       operator = (const Socket & rhs) = delete;
                  Socket (Socket && rhs) = delete;
   Socket &       operator = (Socket && rhs) = delete;
   bool           operator == (const Socket & rhs) const = delete;
   bool           operator != (const Socket & rhs) const = delete;



}; // class Socket



}  // namespace flip



//#include  "flip/Socket.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

