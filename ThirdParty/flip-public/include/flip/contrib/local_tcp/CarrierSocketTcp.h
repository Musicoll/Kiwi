/*****************************************************************************

      CarrierSocketTcp.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/contrib/PacketStreamReceiver.h"
#include "flip/contrib/PacketStreamSender.h"
#include "flip/contrib/Socket.h"
#include "flip/detail/CarrierBase.h"

#include <vector>

#include <cstdint>



namespace flip
{



class flip_API CarrierSocketTcp
:  public CarrierBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum Error
   {
                  OK = 0,        // connection closed

                  CNX_REFUSED,   // "please try again later"
                  CNX_LOST,      // socket error
                  CNX_IO,        // received data is corrupted
   };

                  CarrierSocketTcp (CarrierListener & listener, uint16_t port);
   virtual        ~CarrierSocketTcp ();

   Socket::Id     socket () const;

   void           process ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   bool           impl_need_process () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // CarrierBase
   virtual void   do_greet (uint64_t user_id) override;
   virtual void   do_commit (const Transaction & tx) override;
   virtual void   do_squash (const TxIdRange & range, const Transaction & tx) override;
   virtual void   do_push (const Transaction & tx) override;
   virtual void   do_signal (const SignalData & data) override;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           send (const std::vector <uint8_t> & data);

   bool           is_socket_connected () const;
   void           connect ();
   void           send ();
   void           recv ();
   void           process_recv ();
   void           process_packet (const uint8_t * data_ptr, size_t data_size);

   void           disconnect ();
   void           disconnect (int err);

   const uint16_t _port;

   Socket::Id     _fd = Socket::invalid_id;

   PacketStreamSender
                  _sender;
   PacketStreamReceiver
                  _receiver;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CarrierSocketTcp () = delete;
                  CarrierSocketTcp (const CarrierSocketTcp & rhs) = delete;
   CarrierSocketTcp &
                  operator = (const CarrierSocketTcp & rhs) = delete;
                  CarrierSocketTcp (CarrierSocketTcp && rhs) = delete;
   CarrierSocketTcp &
                  operator = (CarrierSocketTcp && rhs) = delete;
   bool           operator == (const CarrierSocketTcp & rhs) const = delete;
   bool           operator != (const CarrierSocketTcp & rhs) const = delete;



}; // class CarrierSocketTcp



}  // namespace flip



//#include  "flip/CarrierSocketTcp.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

