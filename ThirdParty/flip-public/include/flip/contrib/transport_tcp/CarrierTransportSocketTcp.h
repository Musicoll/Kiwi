/*****************************************************************************

      CarrierTransportSocketTcp.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/contrib/PacketStreamPinger.h"
#include "flip/contrib/PacketStreamReceiver.h"
#include "flip/contrib/PacketStreamSender.h"
#include "flip/contrib/Socket.h"
#include "flip/detail/CarrierTransportBase.h"

#include <string>

#include <cstdint>



namespace flip
{



class flip_API CarrierTransportSocketTcp
:  public CarrierTransportBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum Error
   {
                  OK = 0,        // connection closed

                  DNS,           // "please check your internet connection"
                  CNX_REFUSED,   // "please try again later"
                  CNX_LOST,      // socket error
                  CNX_IO,        // received data is corrupted
   };

                  CarrierTransportSocketTcp (CarrierListener & listener, uint64_t session_id, const std::string & host, uint16_t port);
                  CarrierTransportSocketTcp (CarrierListener & listener, uint64_t session_id, const std::string & metadata, const std::string & host, uint16_t port);
   virtual        ~CarrierTransportSocketTcp ();

   Socket::Id     socket () const;

   void           process ();

   void           rebind (const std::string & host, uint16_t port);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // CarrierTransportBase
   virtual void   send (const std::vector <uint8_t> & data) override;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum {         PACKET_HEADER_SIZE = 8  };

   bool           is_socket_connected () const;
   void           connect ();
   void           ping ();
   void           send ();
   void           recv ();
   void           process_recv ();

   void           disconnect ();
   void           disconnect (int err);

   std::string    _host;
   uint16_t       _port;

   Socket::Id     _fd = Socket::invalid_id;
   bool           _connecting_flag = false;

   PacketStreamSender
                  _sender;
   PacketStreamReceiver
                  _receiver;
   PacketStreamPinger
                  _pinger;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CarrierTransportSocketTcp () = delete;
                  CarrierTransportSocketTcp (const CarrierTransportSocketTcp & rhs) = delete;
   CarrierTransportSocketTcp &
                  operator = (const CarrierTransportSocketTcp & rhs) = delete;
                  CarrierTransportSocketTcp (CarrierTransportSocketTcp && rhs) = delete;
   CarrierTransportSocketTcp &
                  operator = (CarrierTransportSocketTcp && rhs) = delete;
   bool           operator == (const CarrierTransportSocketTcp & rhs) const = delete;
   bool           operator != (const CarrierTransportSocketTcp & rhs) const = delete;



}; // class CarrierTransportSocketTcp



}  // namespace flip



//#include  "flip/CarrierTransportSocketTcp.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

