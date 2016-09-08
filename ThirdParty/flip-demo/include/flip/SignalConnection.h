/*****************************************************************************

      SignalConnection.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/SignalBase.h"



namespace flip
{



class flip_API SignalConnection
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  SignalConnection () = default;
                  SignalConnection (SignalConnection && rhs);
   virtual        ~SignalConnection ();

   SignalConnection &
                  operator = (SignalConnection && rhs);

   bool           connected () const;
   void           disconnect ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   friend class SignalBase;
                  SignalConnection (SignalBase & parent, const SignalBase::ConnectionKey & key);

   SignalBase *   _parent_ptr = nullptr;
   SignalBase::ConnectionKey
                  _key = 0ULL;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SignalConnection (const SignalConnection & rhs) = delete;
   SignalConnection &
                  operator = (const SignalConnection & rhs) = delete;
   bool           operator == (const SignalConnection & rhs) const = delete;
   bool           operator != (const SignalConnection & rhs) const = delete;



}; // class SignalConnection



}  // namespace flip



//#include  "flip/SignalConnection.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

