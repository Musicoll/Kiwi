/*****************************************************************************

      PortSocketTcp.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/contrib/PacketStreamReceiver.h"
#include "flip/contrib/PacketStreamSender.h"
#include "flip/contrib/Socket.h"
#include "flip/detail/PortBase.h"

#include <list>
#include <set>

#include <cstdint>



namespace flip
{



class flip_API PortSocketTcp
:  public PortBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PortSocketTcp (uint16_t port);
   virtual        ~PortSocketTcp ();

   std::set <Socket::Id>
                  sockets () const;

   void           process ();

   // PortBase
   virtual void   receive_backend (const BackEndIR & backend) override;
   virtual void   receive_commit (const Transaction & tx) override;
   virtual void   receive_squash (const TxIdRange & range, const Transaction & tx) override;
   virtual void   receive_transaction (const Transaction & tx) override;
   virtual void   receive_ack (const TxId & tx_id) override;
   virtual void   receive_deny (const TxId & tx_id) override;
   virtual void   receive_signal (const SignalData & data) override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class Client
   {
   public:
                  Client (PortSocketTcp & parent, Socket::Id fd);
      virtual     ~Client ();

      Socket::Id  socket () const;
      bool        is_connected () const;

      void        send (const std::vector <uint8_t> & data);
      void        process ();
      void        disconnect ();

   private:
      PortSocketTcp &
                  _parent;
      Socket::Id  _fd = Socket::invalid_id;

      PacketStreamSender
                  _sender;
      PacketStreamReceiver
                  _receiver;

                  Client () = delete;
                  Client (const Client & rhs) = delete;
      Client &    operator = (const Client & rhs) = delete;
                  Client (Client && rhs) = delete;
      Client &    operator = (Client && rhs) = delete;
   };

   friend class Client;

   void           accept ();

   void           send (const std::vector <uint8_t> & data);
   void           receive (const uint8_t * data_ptr, size_t data_size);

   Socket::Id     _fd = Socket::invalid_id;
   Socket::Addr   _addr;

   std::list <Client>
                  _ports;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PortSocketTcp () = delete;
                  PortSocketTcp (const PortSocketTcp & rhs) = delete;
   PortSocketTcp &operator = (const PortSocketTcp & rhs) = delete;
                  PortSocketTcp (PortSocketTcp && rhs) = delete;
   PortSocketTcp &operator = (PortSocketTcp && rhs) = delete;
   bool           operator == (const PortSocketTcp & rhs) const = delete;
   bool           operator != (const PortSocketTcp & rhs) const = delete;



}; // class PortSocketTcp



}  // namespace flip



//#include  "flip/PortSocketTcp.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

