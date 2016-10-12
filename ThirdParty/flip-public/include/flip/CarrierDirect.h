/*****************************************************************************

      CarrierDirect.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/CarrierBase.h"



namespace flip
{



class PortDirect;

class flip_API CarrierDirect
:  public CarrierBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  CarrierDirect (CarrierListener & listener, PortDirect & port);
   virtual        ~CarrierDirect ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



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

   friend class PortDirect;

   PortDirect &   _port;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CarrierDirect () = delete;
                  CarrierDirect (const CarrierDirect & rhs) = delete;
   CarrierDirect &
                  operator = (const CarrierDirect & rhs) = delete;
                  CarrierDirect (CarrierDirect && rhs) = delete;
   CarrierDirect &
                  operator = (CarrierDirect && rhs) = delete;
   bool           operator == (const CarrierDirect & rhs) const = delete;
   bool           operator != (const CarrierDirect & rhs) const = delete;



}; // class CarrierDirect



}  // namespace flip



//#include  "flip/CarrierDirect.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

