/*****************************************************************************

      DataProviderBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <string>

#include <cstddef>
#include <cstdint>



namespace flip
{



class flip_API DataProviderBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DataProviderBase () = default;
   virtual        ~DataProviderBase () = default;

   DataProviderBase &
                  operator >> (bool & val);
   DataProviderBase &
                  operator >> (char & val);
   DataProviderBase &
                  operator >> (uint8_t & val);
   DataProviderBase &
                  operator >> (int32_t & val);
   DataProviderBase &
                  operator >> (uint32_t & val);
   DataProviderBase &
                  operator >> (int64_t & val);
   DataProviderBase &
                  operator >> (uint64_t & val);
   DataProviderBase &
                  operator >> (double & val);
   /*DataProviderBase &
                  operator >> (size_t & val);*/

   std::string    read_string (size_t size);

   virtual const uint8_t *
                  pull (size_t data_size) = 0;
   virtual size_t tell () const = 0;
   virtual void   seek_start (size_t offset_abs = 0) = 0;
   virtual void   seek_relative (ptrdiff_t offset_rel) = 0;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DataProviderBase (const DataProviderBase & rhs) = delete;
   DataProviderBase &
                  operator = (const DataProviderBase & rhs) = delete;
                  DataProviderBase (DataProviderBase && rhs) = delete;
   DataProviderBase &
                  operator = (DataProviderBase && rhs) = delete;
   bool           operator == (const DataProviderBase & rhs) const = delete;
   bool           operator != (const DataProviderBase & rhs) const = delete;



}; // class DataProviderBase



}  // namespace flip



//#include  "flip/DataProviderBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

