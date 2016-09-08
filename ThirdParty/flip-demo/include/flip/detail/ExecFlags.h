/*****************************************************************************

      ExecFlags.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace flip
{



enum ExecFlags
{
                  // Opcode
                  OP_IN_UNDO           = 1 << 0,
                  OP_NOT_EXECUTED      = 1 << 1,

                  // Transaction
                  TX_UNDO_MODE         = 1 << 0,
                  TX_CORRECT_MODE      = 1 << 1,

                  // Opcode & Transaction
                  OP_TX_NO_ERROR_MODE  = 1 << 3,



}; // enum ExecFlags



}  // namespace flip



//#include  "flip/ExecFlags.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

