/*****************************************************************************

      SignalData.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Ref.h"

#include <vector>

#include <cstdint>



namespace flip
{



class StreamBinOut;
class StreamBinIn;

class flip_API SignalData
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  SignalData () = default;
                  SignalData (uint32_t type_id_, const Ref & ref_, uint32_t type_, const std::vector <uint8_t> & data_);
                  SignalData (const SignalData & rhs) = default;
                  SignalData (SignalData && rhs);
   virtual        ~SignalData () = default;

   SignalData &   operator = (const SignalData & rhs) = default;
   SignalData &   operator = (SignalData && rhs);

   void           write (StreamBinOut & sbo) const;
   void           read (StreamBinIn & sbi);

   uint32_t       type_id = 0ULL;
   Ref            ref;
   uint32_t       type = 0ULL;
   std::vector <uint8_t>
                  data;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const SignalData & rhs) const = delete;
   bool           operator != (const SignalData & rhs) const = delete;



}; // class SignalData



}  // namespace flip



//#include  "flip/SignalData.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
