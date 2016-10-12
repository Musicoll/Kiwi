/*****************************************************************************

      DataProviderFile.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/DataProviderBase.h"

#include <vector>

#include <cstdint>
#include <cstdio>



namespace flip
{



class flip_API DataProviderFile
:  public DataProviderBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DataProviderFile (const char * path_0);
   virtual        ~DataProviderFile ();

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

   FILE *         _file_ptr = nullptr;
   std::vector <uint8_t>
                  _data;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DataProviderFile () = delete;
                  DataProviderFile (const DataProviderFile & rhs) = delete;
   DataProviderFile &
                  operator = (const DataProviderFile & rhs) = delete;
                  DataProviderFile (DataProviderFile && rhs) = delete;
   DataProviderFile &
                  operator = (DataProviderFile && rhs) = delete;
   bool           operator == (const DataProviderFile & rhs) const = delete;
   bool           operator != (const DataProviderFile & rhs) const = delete;



}; // class DataProviderFile



}  // namespace flip



//#include  "flip/contrib/DataProviderFile.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

