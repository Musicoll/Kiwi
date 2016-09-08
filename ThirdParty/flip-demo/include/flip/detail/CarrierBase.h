/*****************************************************************************

      CarrierBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <functional>
#include <memory>

#include <cstddef>
#include <cstdint>



namespace flip
{



class BackEndIR;
class CarrierListener;
class SignalData;
class Transaction;
class TxId;
class TxIdRange;

class flip_API CarrierBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum class Transition
   {
                  Connecting,
                  Connected,
                  Disconnected,
   };

   using Error = int;

                  CarrierBase (CarrierListener & listener);
   virtual        ~CarrierBase ();

   void           listen_state_transition (std::function <void (Transition, Error)> func);
   bool           is_idle () const;
   bool           is_connecting () const;
   bool           is_connected () const;

   void           commit (const Transaction & tx);
   void           squash (const TxIdRange & range, const Transaction & tx);
   void           push (const Transaction & tx);
   void           signal (const SignalData & data);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual void   do_greet (uint64_t user_id) = 0;
   virtual void   do_commit (const Transaction & tx) = 0;
   virtual void   do_squash (const TxIdRange & range, const Transaction & tx) = 0;
   virtual void   do_push (const Transaction & tx) = 0;
   virtual void   do_signal (const SignalData & data) = 0;

   CarrierListener &
                  listener ();

   void           receive_backend (const BackEndIR & backend);
   void           receive_tx_local (const Transaction & tx);
   void           receive_tx_squash (const std::pair <TxIdRange, Transaction> & squash);
   void           receive_tx_squash (const TxIdRange & range, const Transaction & tx);
   void           receive_tx_ext (const Transaction & tx);
   void           receive_tx_ack (const TxId & tx_id);
   void           receive_tx_deny (const TxId & tx_id);
   void           receive_signal (const SignalData & data);

   void           notify_connecting ();
   void           notify_connected ();
   void           notify_disconnected (int err);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum class State
   {
                  Idle,
                  Connecting,
                  Connected,
   };

   CarrierListener &
                  _listener;

   std::function <void (Transition, int)>
                  _func_listen;
   State          _state = State::Idle;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CarrierBase () = delete;
                  CarrierBase (const CarrierBase & rhs) = delete;
   CarrierBase &  operator = (const CarrierBase & rhs) = delete;
                  CarrierBase (CarrierBase && rhs) = delete;
   CarrierBase &  operator = (CarrierBase && rhs) = delete;
   bool           operator == (const CarrierBase & rhs) const = delete;
   bool           operator != (const CarrierBase & rhs) const = delete;



}; // class CarrierBase



}  // namespace flip



//#include  "flip/CarrierBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

