/*****************************************************************************

      DataConsumerFile.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/DataConsumerBase.h"

#include <cstdint>
#include <cstdio>



namespace flip
{



class flip_API DataConsumerFile
:  public DataConsumerBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DataConsumerFile (const char * path_0);
   virtual        ~DataConsumerFile ();

   // DataConsumerBase
   virtual void   push (const uint8_t * data_ptr, size_t data_size) override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:




/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   FILE *         _file_ptr = nullptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DataConsumerFile () = delete;
                  DataConsumerFile (const DataConsumerFile & rhs) = delete;
   DataConsumerFile &
                  operator = (const DataConsumerFile & rhs) = delete;
                  DataConsumerFile (DataConsumerFile && rhs) = delete;
   DataConsumerFile &
                  operator = (DataConsumerFile && rhs) = delete;
   bool           operator == (const DataConsumerFile & rhs) const = delete;
   bool           operator != (const DataConsumerFile & rhs) const = delete;



}; // class DataConsumerFile



}  // namespace flip



//#include  "flip/contrib/DataConsumerFile.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

