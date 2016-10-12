/*****************************************************************************

      DataConsumerMemory.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/DataConsumerBase.h"

#include <vector>

#include <cstdint>



namespace flip
{



class flip_API DataConsumerMemory
:  public DataConsumerBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DataConsumerMemory (std::vector <uint8_t> & data);
   virtual        ~DataConsumerMemory () = default;

   // DataConsumerBase
   virtual void   push (const uint8_t * data_ptr, size_t data_size) override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:




/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   std::vector <uint8_t> &
                  _data;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DataConsumerMemory () = delete;
                  DataConsumerMemory (const DataConsumerMemory & rhs) = delete;
   DataConsumerMemory &       operator = (const DataConsumerMemory & rhs) = delete;
                  DataConsumerMemory (DataConsumerMemory && rhs) = delete;
   DataConsumerMemory &       operator = (DataConsumerMemory && rhs) = delete;
   bool           operator == (const DataConsumerMemory & rhs) const = delete;
   bool           operator != (const DataConsumerMemory & rhs) const = delete;



}; // class DataConsumerMemory



}  // namespace flip



//#include  "flip/DataConsumerMemory.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

