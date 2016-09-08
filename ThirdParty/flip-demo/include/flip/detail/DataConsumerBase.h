/*****************************************************************************

      DataConsumerBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <string>

#include <cstdint>
#include <cstddef>



namespace flip
{



class flip_API DataConsumerBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DataConsumerBase () = default;
   virtual        ~DataConsumerBase () = default;

   DataConsumerBase &
                  operator << (bool val);
   DataConsumerBase &
                  operator << (char val);
   DataConsumerBase &
                  operator << (uint8_t val);
   DataConsumerBase &
                  operator << (int32_t val);
   DataConsumerBase &
                  operator << (uint32_t val);
   DataConsumerBase &
                  operator << (int64_t val);
   DataConsumerBase &
                  operator << (uint64_t val);
   DataConsumerBase &
                  operator << (double val);
   /*DataConsumerBase &
                  operator << (size_t val);*/
   DataConsumerBase &
                  operator << (const char * val_0);
   DataConsumerBase &
                  operator << (const std::string & val);

   virtual void   push (const uint8_t * data_ptr, size_t data_size) = 0;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DataConsumerBase (const DataConsumerBase & rhs) = delete;
   DataConsumerBase &
                  operator = (const DataConsumerBase & rhs) = delete;
                  DataConsumerBase (DataConsumerBase && rhs) = delete;
   DataConsumerBase &
                  operator = (DataConsumerBase && rhs) = delete;
   bool           operator == (const DataConsumerBase & rhs) const = delete;
   bool           operator != (const DataConsumerBase & rhs) const = delete;



}; // class DataConsumerBase



}  // namespace flip



//#include  "flip/DataConsumerBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

