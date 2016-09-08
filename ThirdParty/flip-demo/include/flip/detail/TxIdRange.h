/*****************************************************************************

      TxIdRange.h
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

class flip_API TxIdRange
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  TxIdRange () = default;
                  TxIdRange (uint64_t user, uint64_t actor, uint64_t nbr_first, uint64_t nbr_last);
                  TxIdRange (const TxIdRange & rhs) = default;
                  TxIdRange (TxIdRange && rhs);
   virtual        ~TxIdRange () = default;

   TxIdRange &    operator = (const TxIdRange & rhs) = default;
   TxIdRange &    operator = (TxIdRange && rhs);

   void           write (StreamBinOut & sbo) const;
   void           read (StreamBinIn & sbi);

   uint64_t       user_id = 0ULL;
   uint64_t       actor_id = 0ULL;
   uint64_t       nbr_id_first = 0ULL;
   uint64_t       nbr_id_last = 0ULL;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const TxIdRange & rhs) const = delete;
   bool           operator != (const TxIdRange & rhs) const = delete;



}; // class TxIdRange



}  // namespace flip



//#include  "flip/TxIdRange.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
