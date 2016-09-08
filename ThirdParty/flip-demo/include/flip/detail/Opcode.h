/*****************************************************************************

      Opcode.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Ref.h"
#include "flip/detail/KeyRandom.h"
#include "flip/detail/KeyFloat.h"
#include "flip/detail/VMErr.h"

#include <list>
#include <vector>

#include <cstdint>



namespace flip
{



class DocumentBase;
class StreamBinIn;
class StreamBinOut;
class Transaction;
enum class Direction;

class flip_API Opcode;

using Opcodes = std::list <Opcode>;

class flip_API Opcode
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum class Variant {INSERT, ERASE};
   enum class Integral {NATURAL, ENUMERATION};

                  Opcode () = default;
                  Opcode (const Opcode & rhs) = default;
                  Opcode (Opcode && rhs);
                  Opcode (const Ref & ref, uint8_t flags, bool value_old, bool value_new);
                  Opcode (const Ref & ref, uint8_t flags, int64_t value_old, int64_t value_new, Integral integral);
                  Opcode (const Ref & ref, uint8_t flags, double value_old, double value_new);
                  Opcode (const Ref & ref, uint8_t flags, const Ref & value_old, const Ref & value_new);
                  Opcode (const Ref & ref, uint8_t flags, const std::vector <uint8_t> & value_old, const std::vector <uint8_t> & value_new);
                  Opcode (const Ref & ref, uint8_t flags, const KeyRandom & key, uint32_t type_id, const Ref & elem_ref, Variant variant);
                  Opcode (const Ref & ref, uint8_t flags, const KeyRandom & key, const Ref & other_ref, const KeyRandom & other_key);
                  Opcode (const Ref & ref, uint8_t flags, const KeyFloat & key, uint32_t type_id, const Ref & elem_ref, Variant variant);
                  Opcode (const Ref & ref, uint8_t flags, const KeyFloat & key, const Ref & other_ref, const KeyFloat & other_key);
                  Opcode (const Ref & ref, uint8_t flags, const std::vector <uint8_t> & data);
   virtual        ~Opcode ();

   Opcode &       operator = (const Opcode & rhs) = default;
   Opcode &       operator = (Opcode && rhs);

   bool           equal (const Opcode & rhs) const;

   void           write (StreamBinOut & sbo) const;
   void           read (StreamBinIn & sbi);

   int            execute (DocumentBase & document, Direction direction, uint8_t flags, Transaction & tx, Opcodes::iterator it);
   bool           is_executed () const;

   bool           is_in_undo () const;

   void           invert_inplace ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   friend class Journal;
   friend class Type;

   enum Command : uint8_t
   {
                  BOOL_SET = 0,
                  INT_SET,
                  FLOAT_SET,
                  ENUM_SET,
                  OBJECT_REF_SET,
                  BLOB_SET,
                  COLLECTION_INSERT,
                  COLLECTION_ERASE,
                  COLLECTION_MOVE,
                  ARRAY_INSERT,
                  ARRAY_ERASE,
                  ARRAY_MOVE,
                  MESSAGE_PUSH,
   };

   class Data
   {
   public:
                  Data () {}
                  Data (bool val) : _bool (val) {}
                  Data (int64_t val) : _int (val) {}
                  Data (double val) : _float (val) {}
                  Data (const Ref & ref) : _ref (ref) {}
                  Data (const std::vector <uint8_t> & val) : _blob (val) {}

      int64_t     _int;
      double      _float;
      Ref         _ref;
      std::vector <uint8_t>
                  _blob;
      bool        _bool;
   };

   struct Key
   {
      KeyRandom   _random;
      KeyFloat    _float;
   };

   int            execute_bool_set (DocumentBase & document, Direction direction, uint8_t flags);
   int            execute_int_set (DocumentBase & document, Direction direction, uint8_t flags);
   int            execute_float_set (DocumentBase & document, Direction direction, uint8_t flags);
   int            execute_enum_set (DocumentBase & document, Direction direction, uint8_t flags);
   int            execute_object_ref_set (DocumentBase & document, Direction direction, uint8_t flags);
   int            execute_blob_set (DocumentBase & document, Direction direction, uint8_t flags);
   int            execute_collection_insert_erase (DocumentBase & document, Direction direction, uint8_t flags, Direction tx_direction, Transaction & tx, Opcodes::iterator it);
   int            execute_collection_insert (DocumentBase & document, uint8_t flags);
   int            execute_collection_erase (DocumentBase & document, uint8_t flags, Direction tx_direction, Transaction & tx, Opcodes::iterator it);
   int            execute_collection_move (DocumentBase & document, Direction direction, uint8_t flags);
   int            execute_array_insert_erase (DocumentBase & document, Direction direction, uint8_t flags, Direction tx_direction, Transaction & tx, Opcodes::iterator it);
   int            execute_array_insert (DocumentBase & document, uint8_t flags);
   int            execute_array_erase (DocumentBase & document, uint8_t flags, Direction tx_direction, Transaction & tx, Opcodes::iterator it);
   int            execute_array_move (DocumentBase & document, Direction direction, uint8_t flags);
   int            execute_message_push (DocumentBase & document, Direction direction, uint8_t flags);

   int            error (bool correct_flag, VMErr err);
   void           check_flags ();

   Ref            _ref;
   uint8_t        _flags;
   uint8_t        _command;

   // value type
   Data           _old;
   Data           _new;

   // container type
   Key            _key;
   uint32_t       _type_id;
   Ref            _elem_ref;

   // container move type
   Ref            _other_ref;
   Key            _other_key;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Opcode & rhs) const = delete;
   bool           operator != (const Opcode & rhs) const = delete;



}; // class Opcode



}  // namespace flip



//#include  "flip/Opcode.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

