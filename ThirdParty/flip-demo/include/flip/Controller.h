/*****************************************************************************

      Controller.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace flip
{



enum class Controller
{
                  NONE,
                  SELF,
                  UNDO,
                  READ,
                  LOCAL,
                  SQUASH,
                  EXTERNAL,
                  ACKNOWLEDGED,
                  DENIED,


}; // enum class Controller



}  // namespace flip



//#include  "flip/Controller.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

