/*****************************************************************************

      TxId.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <cstdint>



namespace flip
{



class StreamBinOut;
class StreamBinIn;

class flip_API TxId
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  TxId () = default;
                  TxId (uint64_t user, uint64_t actor, uint64_t nbr);
                  TxId (const TxId & rhs) = default;
                  TxId (TxId && rhs);
   virtual        ~TxId () = default;

   TxId &         operator = (const TxId & rhs) = default;
   TxId &         operator = (TxId && rhs);

   bool           operator == (const TxId & rhs) const;
   bool           operator != (const TxId & rhs) const;

   void           write (StreamBinOut & sbo) const;
   void           read (StreamBinIn & sbi);

   uint64_t       user_id = 0ULL;
   uint64_t       actor_id = 0ULL;
   uint64_t       nbr_id = 0ULL;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class TxId



}  // namespace flip



//#include  "flip/TxId.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
