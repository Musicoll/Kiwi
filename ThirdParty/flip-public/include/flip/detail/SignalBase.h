/*****************************************************************************

      SignalBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <tuple>
#include <typeinfo>
#include <vector>

#include <cstdint>



namespace flip
{



class SignalConnection;

class flip_API SignalBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   using ConnectionKey = uint64_t;

                  SignalBase () = default;
                  SignalBase (const SignalBase & rhs) = default;
   virtual        ~SignalBase () = default;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   virtual void   impl_signal (const std::vector <uint8_t> & data) = 0;
   virtual void   impl_disconnect (const ConnectionKey & key) = 0;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   SignalConnection
                  make_connection (const ConnectionKey & key);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   SignalBase &   operator = (const SignalBase & rhs) = delete;
                  SignalBase (SignalBase && rhs) = delete;
   SignalBase &   operator = (SignalBase && rhs) = delete;
   bool           operator == (const SignalBase & rhs) const = delete;
   bool           operator != (const SignalBase & rhs) const = delete;



}; // class SignalBase



}  // namespace flip



//#include  "flip/SignalBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

