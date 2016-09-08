/*****************************************************************************

      Journal.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Transaction.h"
#include "flip/detail/Direction.h"

#include <cstdint>



namespace flip
{



class ConsumerTextOut;
class DocumentBase;

class flip_API Journal
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  Journal (DocumentBase & document);
   virtual        ~Journal ();

   void           reset ();

   void           context_direction (Direction direction);
   void           context_transaction (const Transaction & tx);
   void           context_opcode_nbr (size_t nbr);
   void           context_opcode_error (std::string error);
   void           context_validation (std::string error);

   void           trace ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           trace (ConsumerTextOut & out);
   void           trace_tx (ConsumerTextOut & out);
   void           trace_op_cmd (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_bool_set (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_int_set (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_float_set (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_enum_set (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_object_ref_set (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_blob_set (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_collection_insert (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_collection_erase (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_collection_move (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_array_insert (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_array_erase (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_array_move (ConsumerTextOut & out, const Opcode & opcode);
   void           trace_op_val_message_push (ConsumerTextOut & out, const Opcode & opcode);

   void           trace_op_val_data_bool (ConsumerTextOut & out, const Opcode::Data & data);
   void           trace_op_val_data_int (ConsumerTextOut & out, const Opcode::Data & data);
   void           trace_op_val_data_float (ConsumerTextOut & out, const Opcode::Data & data);
   void           trace_op_val_data_ref (ConsumerTextOut & out, const Opcode::Data & data);
   void           trace_op_val_data_key (ConsumerTextOut & out, const KeyRandom & key);
   void           trace_op_val_data_key (ConsumerTextOut & out, const KeyFloat & key);

   DocumentBase & _document;

   Direction      _direction = Direction::FORWARD;
   Transaction    _transaction;
   size_t         _opcode_nbr = size_t (-1);
   std::string    _opcode_error;
   std::string    _validation_error;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Journal () = delete;
                  Journal (const Journal & rhs) = delete;
   Journal & operator = (const Journal & rhs) = delete;
                  Journal (Journal && rhs) = delete;
   Journal & operator = (Journal && rhs) = delete;
   bool           operator == (const Journal & rhs) const = delete;
   bool           operator != (const Journal & rhs) const = delete;



}; // class Journal



}  // namespace flip



//#include "flip/detail/Journal.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

