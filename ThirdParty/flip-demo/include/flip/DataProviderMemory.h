/*****************************************************************************

      DataProviderMemory.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/DataProviderBase.h"

#include <vector>

#include <cstdint>



namespace flip
{



class flip_API DataProviderMemory
:  public DataProviderBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DataProviderMemory (const std::vector <uint8_t> & data);
   virtual        ~DataProviderMemory () = default;

   // DataProviderBase
   virtual const uint8_t *
                  pull (size_t data_size) override;
   virtual size_t tell () const override;
   virtual void   seek_start (size_t offset_abs) override;
   virtual void   seek_relative (ptrdiff_t offset_rel) override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   const uint8_t *
                  _data_ptr;
   const size_t   _data_size;

   size_t         _pos = 0;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DataProviderMemory () = delete;
                  DataProviderMemory (const DataProviderMemory & rhs) = delete;
   DataProviderMemory &       operator = (const DataProviderMemory & rhs) = delete;
                  DataProviderMemory (DataProviderMemory && rhs) = delete;
   DataProviderMemory &       operator = (DataProviderMemory && rhs) = delete;
   bool           operator == (const DataProviderMemory & rhs) const = delete;
   bool           operator != (const DataProviderMemory & rhs) const = delete;



}; // class DataProviderMemory



}  // namespace flip



//#include  "flip/DataProviderMemory.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

