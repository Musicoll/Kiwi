/*****************************************************************************

      ArrayBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/iterator.h"
#include "flip/detail/def.h"
#include "flip/detail/ContainerType.h"
#include "flip/detail/KeyFloat.h"

#include <map>
#include <stdexcept>

#include <cassert>



namespace flip
{



class flip_API ArrayBase
:  public ContainerType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  ArrayBase () = default;
                  ArrayBase (const ArrayBase & rhs) = default;
                  ArrayBase (ArrayBase && rhs) : ContainerType (std::move (rhs)) {}
   virtual        ~ArrayBase () = default;

   ArrayBase &    operator = (const ArrayBase & rhs) = default;
   ArrayBase &    operator = (ArrayBase && rhs) {this->ContainerType::operator = (std::move (rhs)); return *this;}

   virtual array_iterator
                  gbegin () {flip_FATAL;} // COV_NF_LINE
   virtual const_array_iterator
                  gbegin () const {flip_FATAL;} // COV_NF_LINE
   virtual const_array_iterator
                  gcbegin () const {flip_FATAL;} // COV_NF_LINE
   virtual array_iterator
                  gend () {flip_FATAL;} // COV_NF_LINE
   virtual const_array_iterator
                  gend () const {flip_FATAL;} // COV_NF_LINE
   virtual const_array_iterator
                  gcend () const {flip_FATAL;} // COV_NF_LINE

   // Type
   virtual const ClassBase &
                  get_class () const override {flip_FATAL;}   // COV_NF_LINE
   virtual void   revert () override {flip_FATAL;} // COV_NF_LINE



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   enum class KeyState
   {
                  NON_PRESENT,
                  ADDED,
                  RESIDENT,
                  REMOVED,
   };

   // Type
   virtual void   impl_set_class (const ClassBase & /* class_base */) override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_make (Transaction & /* tx */, ImplUndoRedoMode /* parent_mode */, TxPostProcessInfo & /* ppinfo */) const override {flip_FATAL;}  // COV_NF_LINE
   virtual bool   impl_is_reset () const override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_reset () override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_validate () override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_synchronize () override {flip_FATAL;}   // COV_NF_LINE

   virtual const ClassBase &
                  impl_element_class () const {flip_FATAL;}   // COV_NF_LINE
   virtual KeyState
                  impl_get_key_state (const KeyFloat & /* key */) const {flip_FATAL;} // COV_NF_LINE
   virtual const Ref &
                  impl_get_key_ref (const KeyFloat & /* key */) const {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_insert (const KeyFloat & /* key */, uint32_t /* type_id */, const Ref & /* elem_ref */) {flip_FATAL;} // COV_NF_LINE
   virtual void   impl_reinsert (const KeyFloat & /* key */) {flip_FATAL;} // COV_NF_LINE
   virtual Type & impl_emplace (const KeyFloat & /* key */, const ClassBase & /* class_base */) {flip_FATAL;} // COV_NF_LINE
   virtual void   impl_erase (const KeyFloat & /* key */) {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_move (const KeyFloat & /* key */, ArrayBase & /* other */, const KeyFloat & /* other_key */) {flip_FATAL;}  // COV_NF_LINE
   virtual void   impl_relocate (const KeyFloat & /* key */, ArrayBase & /* other */, const KeyFloat & /* other_key */) {flip_FATAL;}  // COV_NF_LINE
   virtual bool   impl_is_moving (const KeyFloat & /* key */) const {flip_FATAL;}  // COV_NF_LINE



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const ArrayBase & rhs) const = delete;
   bool           operator != (const ArrayBase & rhs) const = delete;



}; // class ArrayBase



}  // namespace flip



//#include  "flip/ArrayBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

