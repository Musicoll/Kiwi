/*****************************************************************************

      PortTransportSocketTcp.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/contrib/PacketStreamReceiver.h"
#include "flip/contrib/PacketStreamSender.h"
#include "flip/contrib/Socket.h"
#include "flip/detail/PortTransportBase.h"



namespace flip
{



class flip_API PortTransportSocketTcp
:  public PortTransportBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PortTransportSocketTcp (Socket::Id fd);
   virtual        ~PortTransportSocketTcp ();

   Socket::Id     socket () const;
   bool           is_connected () const;

   void           process ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // PortTransportBase
   virtual void   send (const std::vector <uint8_t> & data) override;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum {         PACKET_HEADER_SIZE = 8  };

   void           connect ();
   void           ping ();
   void           send ();
   void           recv ();
   void           process_recv ();

   void           disconnect ();

   Socket::Id     _fd = Socket::invalid_id;

   PacketStreamSender
                  _sender;
   PacketStreamReceiver
                  _receiver;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PortTransportSocketTcp () = delete;
                  PortTransportSocketTcp (const PortTransportSocketTcp & rhs) = delete;
   PortTransportSocketTcp &
                  operator = (const PortTransportSocketTcp & rhs) = delete;
                  PortTransportSocketTcp (PortTransportSocketTcp && rhs) = delete;
   PortTransportSocketTcp &
                  operator = (PortTransportSocketTcp && rhs) = delete;
   bool           operator == (const PortTransportSocketTcp & rhs) const = delete;
   bool           operator != (const PortTransportSocketTcp & rhs) const = delete;



}; // class PortTransportSocketTcp



}  // namespace flip



//#include  "flip/PortTransportSocketTcp.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

