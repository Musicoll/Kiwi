/*****************************************************************************

      Direction.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"


namespace flip
{



enum class Direction : int
{

                  BACKWARD = -1,
                  FORWARD = 1


}; // enum class Direction


flip_API Direction
                  opposite (Direction direction);


}  // namespace flip



//#include  "flip/Direction.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

