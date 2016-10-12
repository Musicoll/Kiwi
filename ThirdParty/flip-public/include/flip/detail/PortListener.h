/*****************************************************************************

      PortListener.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"



namespace flip
{



class PortBase;
class SignalData;
class Transaction;
class TxId;
class TxIdRange;

class flip_API PortListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PortListener () = default;
   virtual        ~PortListener () = default;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   virtual void   port_greet (PortBase & from) = 0;
   virtual void   port_commit (PortBase & from, const Transaction & tx) = 0;
   virtual void   port_squash (PortBase & from, const TxIdRange & range, const Transaction & tx) = 0;
   virtual void   port_push (PortBase & from, const Transaction & tx) = 0;
   virtual void   port_signal (PortBase & from, const SignalData & data) = 0;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PortListener (const PortListener & rhs) = delete;
   PortListener & operator = (const PortListener & rhs) = delete;
                  PortListener (PortListener && rhs) = delete;
   PortListener & operator = (PortListener && rhs) = delete;
   bool           operator == (const PortListener & rhs) const = delete;
   bool           operator != (const PortListener & rhs) const = delete;



}; // class PortListener



}  // namespace flip



//#include  "flip/PortListener.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

