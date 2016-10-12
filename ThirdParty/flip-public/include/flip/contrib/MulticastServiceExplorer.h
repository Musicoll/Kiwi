/*****************************************************************************

      MulticastServiceExplorer.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/contrib/MulticastServiceSocket.h"
#include "flip/detail/fnc.h"

#include <chrono>
#include <list>
#include <map>
#include <memory>
#include <string>

#include <cstdint>



namespace flip
{



class flip_API MulticastServiceExplorer
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class Session
   {
   public:
      uint32_t    ip;
      uint16_t    port;
      std::string version;
      std::map <std::string, std::string>
                  metadata;
   };

   using const_iterator = std::list <Session>::const_iterator;

                  MulticastServiceExplorer ();
                  MulticastServiceExplorer (uint32_t multicast_ip, uint16_t port_min, uint16_t port_max);
   virtual        ~MulticastServiceExplorer () = default;

   void           set_update_period (double period_s);

   void           process ();

   const_iterator begin () const;
   const_iterator end () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   MulticastServiceSocket &
                  impl_socket ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum class State
   {
                  NeedSend,
                  Receiving
   };

   void           process_send ();
   void           process_receive ();

   const uint32_t _multicast_ip;
   const uint16_t _port_min;
   const uint16_t _port_max;

   std::unique_ptr <MulticastServiceSocket>
                  _socket_uptr;

   std::list <Session>
                  _sessions_cache;
   std::list <Session>
                  _sessions_cur;

   State          _state = State::NeedSend;

   std::chrono::steady_clock::time_point
                  _send_time;
   double         _receive_timeout = 2.0;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  MulticastServiceExplorer (const MulticastServiceExplorer & rhs) = delete;
   MulticastServiceExplorer &
                  operator = (const MulticastServiceExplorer & rhs) = delete;
                  MulticastServiceExplorer (MulticastServiceExplorer && rhs) = delete;
   MulticastServiceExplorer &
                  operator = (MulticastServiceExplorer && rhs) = delete;
   bool           operator == (const MulticastServiceExplorer & rhs) const = delete;
   bool           operator != (const MulticastServiceExplorer & rhs) const = delete;



}; // class MulticastServiceExplorer



}  // namespace flip



//#include  "flip/MulticastServiceExplorer.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

