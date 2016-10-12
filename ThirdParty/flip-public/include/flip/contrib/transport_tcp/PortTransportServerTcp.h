/*****************************************************************************

      PortTransportServerTcp.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/contrib/Socket.h"
#include "flip/contrib/transport_tcp/PortTransportSocketTcp.h"

#include <list>

#include <cstdint>



namespace flip
{



class PortFactoryListener;

class flip_API PortTransportServerTcp
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PortTransportServerTcp (PortFactoryListener & listener, uint16_t port);
   virtual        ~PortTransportServerTcp ();

   void           process ();
   void           reset ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           accept ();

   PortFactoryListener &
                  _listener;

   Socket::Id     _fd = Socket::invalid_id;
   Socket::Addr   _addr;

   std::list <PortTransportSocketTcp>
                  _ports;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PortTransportServerTcp () = delete;
                  PortTransportServerTcp (const PortTransportServerTcp & rhs) = delete;
   PortTransportServerTcp &
                  operator = (const PortTransportServerTcp & rhs) = delete;
                  PortTransportServerTcp (PortTransportServerTcp && rhs) = delete;
   PortTransportServerTcp &
                  operator = (PortTransportServerTcp && rhs) = delete;
   bool           operator == (const PortTransportServerTcp & rhs) const = delete;
   bool           operator != (const PortTransportServerTcp & rhs) const = delete;



}; // class PortTransportServerTcp



}  // namespace flip



//#include  "flip/PortTransportServerTcp.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

