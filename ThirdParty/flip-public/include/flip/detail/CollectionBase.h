/*****************************************************************************

      CollectionBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/iterator.h"
#include "flip/detail/def.h"
#include "flip/detail/ContainerType.h"
#include "flip/detail/KeyRandom.h"

#include <map>
#include <stdexcept>

#include <cassert>



namespace flip
{



class flip_API CollectionBase
:  public ContainerType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  CollectionBase () = default;
                  CollectionBase (const CollectionBase & rhs) = default;
                  CollectionBase (CollectionBase && rhs) : ContainerType (std::move (rhs)) {}
   virtual        ~CollectionBase () = default;

   CollectionBase &
                  operator = (const CollectionBase & rhs) = default;
   CollectionBase &
                  operator = (CollectionBase && rhs) {this->ContainerType::operator = (std::move (rhs)); return *this;}

   virtual collection_iterator
                  gbegin () {flip_FATAL;} // COV_NF_LINE
   virtual const_collection_iterator
                  gbegin () const {flip_FATAL;} // COV_NF_LINE
   virtual const_collection_iterator
                  gcbegin () const {flip_FATAL;} // COV_NF_LINE
   virtual collection_iterator
                  gend () {flip_FATAL;} // COV_NF_LINE
   virtual const_collection_iterator
                  gend () const {flip_FATAL;} // COV_NF_LINE
   virtual const_collection_iterator
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
                  impl_get_key_state (const KeyRandom & /* key */) const {flip_FATAL;} // COV_NF_LINE
   virtual const Ref &
                  impl_get_key_ref (const KeyRandom & /* key */) const {flip_FATAL;}  // COV_NF_LINE
   virtual void   impl_insert (const KeyRandom & /* key */, uint32_t /* type_id */, const Ref & /* elem_ref */) {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_reinsert (const KeyRandom & /* key */) {flip_FATAL;} // COV_NF_LINE
   virtual Type & impl_emplace (const KeyRandom & /* key */, const ClassBase & /* class_base */) {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_erase (const KeyRandom & /* key */) {flip_FATAL;}  // COV_NF_LINE
   virtual void   impl_move (const KeyRandom & /* key */, CollectionBase & /* other */, const KeyRandom & /* other_key */) {flip_FATAL;} // COV_NF_LINE
   virtual void   impl_relocate (const KeyRandom & /* key */, CollectionBase & /* other */, const KeyRandom & /* other_key */) {flip_FATAL;} // COV_NF_LINE
   virtual bool   impl_is_moving (const KeyRandom & /* key */) const {flip_FATAL;}  // COV_NF_LINE



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const CollectionBase & rhs) const = delete;
   bool           operator != (const CollectionBase & rhs) const = delete;



}; // class CollectionBase



}  // namespace flip



//#include  "flip/CollectionBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

