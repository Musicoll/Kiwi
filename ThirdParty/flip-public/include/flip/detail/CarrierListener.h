/*****************************************************************************

      CarrierListener.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <cstdint>



namespace flip
{



class BackEndIR;
class CarrierBase;
class DataModelBase;
class SignalData;
class Transaction;
class TxId;
class TxIdRange;

class flip_API CarrierListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  CarrierListener () = default;
   virtual        ~CarrierListener () = default;

   bool           has_carrier () const;
   CarrierBase &  carrier ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   void           impl_bind (CarrierBase & carrier);
   void           impl_unbind (CarrierBase & carrier);

   virtual const DataModelBase &
                  carrier_data_model () = 0;
   virtual uint64_t
                  carrier_user () = 0;
   virtual void   carrier_receive_beir (const BackEndIR & backend) = 0;
   virtual void   carrier_receive_tx_local (const Transaction & tx) = 0;
   virtual void   carrier_receive_tx_squash (const TxIdRange & range, const Transaction & tx) = 0;
   virtual void   carrier_receive_tx_ext (const Transaction & tx) = 0;
   virtual void   carrier_receive_tx_ack (const TxId & tx_id) = 0;
   virtual void   carrier_receive_tx_deny (const TxId & tx_id) = 0;
   virtual void   carrier_receive_signal (const SignalData & data) = 0;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   CarrierBase *  _carrier_ptr = nullptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CarrierListener (const CarrierListener & rhs) = delete;
   CarrierListener &
                  operator = (const CarrierListener & rhs) = delete;
                  CarrierListener (CarrierListener && rhs) = delete;
   CarrierListener &
                  operator = (CarrierListener && rhs) = delete;
   bool           operator == (const CarrierListener & rhs) const = delete;
   bool           operator != (const CarrierListener & rhs) const = delete;



}; // class CarrierListener



}  // namespace flip



//#include  "flip/CarrierListener.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

