/*****************************************************************************

      Transaction.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Ref.h"
#include "flip/detail/KeyFloat.h"
#include "flip/detail/KeyRandom.h"
#include "flip/detail/Opcode.h"
#include "flip/detail/TxId.h"

#include <list>
#include <map>
#include <string>
#include <vector>

#include <cstdint>



namespace flip
{



enum class Direction;
class DocumentBase;
class DocumentValidatorBase;

class flip_API Transaction
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  Transaction () = default;
                  Transaction (const Transaction & rhs) = default;
                  Transaction (Transaction && rhs);
                  Transaction (uint64_t user_id, uint64_t actor_id, uint64_t nbr_id);
                  Transaction (const std::vector <uint8_t> & data);
   virtual        ~Transaction () = default;

   const TxId &   id () const;
   uint64_t       user () const;
   uint64_t       actor () const;
   uint64_t       nbr () const;

   Transaction &  operator = (const Transaction & rhs) = default;
   Transaction &  operator = (Transaction && rhs);

   bool           equal (const Transaction & rhs) const;

   std::vector <uint8_t>
                  serialize () const;

   void           clear ();
   bool           empty () const;

   Transaction &  push_bool_set (const Ref & ref, uint8_t flags, bool value_old, bool value_new);
   Transaction &  push_int64_set (const Ref & ref, uint8_t flags, int64_t value_old, int64_t value_new);
   Transaction &  push_float64_set (const Ref & ref, uint8_t flags, double value_old, double value_new);
   Transaction &  push_enum_set (const Ref & ref, uint8_t flags, int64_t value_old, int64_t value_new);
   Transaction &  push_object_ref_set (const Ref & ref, uint8_t flags, const Ref & value_old, const Ref & value_new);
   Transaction &  push_blob_set (const Ref & ref, uint8_t flags, const std::vector <uint8_t> & value_old, const std::vector <uint8_t> & value_new);
   Transaction &  push_collection_insert (const Ref & ref, uint8_t flags, const KeyRandom & key, uint32_t type_id, const Ref & elem_ref);
   Transaction &  push_collection_erase (const Ref & ref, uint8_t flags, const KeyRandom & key, uint32_t type_id, const Ref & elem_ref);
   Transaction &  push_collection_move (const Ref & ref, uint8_t flags, const KeyRandom & key, const Ref & other_ref, const KeyRandom & other_key);
   Transaction &  push_array_insert (const Ref & ref, uint8_t flags, const KeyFloat & key, uint32_t type_id, const Ref & elem_ref);
   Transaction &  push_array_erase (const Ref & ref, uint8_t flags, const KeyFloat & key, uint32_t type_id, const Ref & elem_ref);
   Transaction &  push_array_move (const Ref & ref, uint8_t flags, const KeyFloat & key, const Ref & other_ref, const KeyFloat & other_key);
   Transaction &  push_message_push (const Ref & ref, uint8_t flags, const std::vector <uint8_t> & data);
   Transaction &  push (Transaction && tx);

   void           invert_direction ();

   bool           execute (DocumentBase & document, DocumentValidatorBase & validator, Direction direction, uint8_t flags);

   const std::string &
                  label () const;
   const std::string &
                  metadata (std::string key) const;
   bool           has_metadata (std::string key) const;

   static const std::string
                  key_label;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   typedef std::map <std::string, std::string> MetaMap;

   MetaMap &      impl_use_metadata_map ();
   const MetaMap &impl_use_metadata_map () const;

   bool           is_executable () const;
   bool           has_opcode_in_undo () const;

   Opcodes::iterator
                  last ();
   Opcodes &      opcodes ();

   void           invert_inplace ();

   void           set_hint (uint64_t user_id, uint64_t actor_id, uint64_t nbr_id, Direction direction);
   bool           has_hint () const;
   std::tuple <uint64_t, uint64_t, uint64_t, Direction>
                  get_hint () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   friend class Journal;

   bool           execute_forward (DocumentBase & document, DocumentValidatorBase & validator, uint8_t flags);
   bool           execute_backward (DocumentBase & document, DocumentValidatorBase & validator, uint8_t flags);
   bool           validate (DocumentBase & document, DocumentValidatorBase & validator);
   void           consolidate ();

   TxId           _id;

   MetaMap        _meta_map;
   Opcodes        _opcodes;
   bool           _invert_flag = false;

   bool           _executable_flag = true;

   bool           _hint_flag = false;
   TxId           _hint_id;
   Direction      _hint_direction;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Transaction & rhs) const = delete;
   bool           operator != (const Transaction & rhs) const = delete;



}; // class Transaction



}  // namespace flip



//#include  "flip/Transaction.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

