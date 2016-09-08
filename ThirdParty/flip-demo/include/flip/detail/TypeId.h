/*****************************************************************************

      TypeId.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cstdint>



namespace flip
{



enum TypeId : uint32_t
{
                  NONE = uint32_t (-1),

                  BOOL = 0,
                  INT,
                  FLOAT,
                  ENUM,
                  OBJECT_REF,
                  BLOB,
                  COLLECTION,
                  ARRAY,
                  MESSAGE,
                  OBJECT   // Start of custom classes

}; // enum TypeId



}  // namespace flip



//#include  "flip/TypeId.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

