/*****************************************************************************

      PortDirect.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/PortBase.h"



namespace flip
{



class CarrierDirect;

class flip_API PortDirect
:  public PortBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PortDirect () = default;
   virtual        ~PortDirect () = default;

   // PortBase
   virtual void   receive_backend (const BackEndIR & backend) override;
   virtual void   receive_commit (const Transaction & tx) override;
   virtual void   receive_squash (const TxIdRange & range, const Transaction & tx) override;
   virtual void   receive_transaction (const Transaction & tx) override;
   virtual void   receive_ack (const TxId & tx_id) override;
   virtual void   receive_deny (const TxId & tx_id) override;
   virtual void   receive_signal (const SignalData & data) override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   void           impl_bind (CarrierDirect & carrier);
   void           impl_unbind (CarrierDirect & carrier);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   friend class CarrierDirect;

   CarrierDirect *
                  _carrier_ptr = nullptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PortDirect (const PortDirect & rhs) = delete;
   PortDirect &   operator = (const PortDirect & rhs) = delete;
                  PortDirect (PortDirect && rhs) = delete;
   PortDirect &   operator = (PortDirect && rhs) = delete;
   bool           operator == (const PortDirect & rhs) const = delete;
   bool           operator != (const PortDirect & rhs) const = delete;



}; // class PortDirect



}  // namespace flip



//#include  "flip/PortDirect.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

