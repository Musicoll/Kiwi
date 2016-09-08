/*****************************************************************************

      Collection.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/CollectionBase.h"
#include "flip/detail/CollectionIteratorBase.h"
#include "flip/detail/ObjectWrapper.h"
#include "flip/detail/TypeTraits.h"

#include <map>
#include <memory>
#include <type_traits>



namespace flip
{



class Mold;
class Transaction;

template <class T>
class Collection
:  public CollectionBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef T value_type;
   typedef T & reference;
   typedef const T & const_reference;
   typedef T * pointer;
   typedef const T * const_pointer;
   typedef CollectionIteratorBase <typename std::map <KeyRandom, ObjectWrapper <T>>::iterator> iterator;
   typedef CollectionIteratorBase <typename std::map <KeyRandom, ObjectWrapper <T>>::reverse_iterator> reverse_iterator;
   typedef CollectionIteratorBase <typename std::map <KeyRandom, ObjectWrapper <T>>::const_iterator> const_iterator;
   typedef CollectionIteratorBase <typename std::map <KeyRandom, ObjectWrapper <T>>::const_reverse_iterator> const_reverse_iterator;
   typedef std::ptrdiff_t difference_type;
   typedef std::size_t size_type;

                  Collection ();
                  Collection (const Collection & rhs);
                  Collection (Collection && rhs);
   template <class InputIterator>
                  Collection (InputIterator it, InputIterator it_end);
   virtual        ~Collection () = default;

   Collection &   operator = (const Collection & rhs);
   Collection &   operator = (Collection && rhs);

   virtual const ClassBase &
                  get_class () const override;

   // iterators
   iterator       begin ();
   const_iterator begin () const;
   const_iterator cbegin () const;
   iterator       end ();
   const_iterator end () const;
   const_iterator cend () const;
   reverse_iterator
                  rbegin ();
   const_reverse_iterator
                  rbegin () const;
   const_reverse_iterator
                  crbegin () const;
   reverse_iterator
                  rend ();
   const_reverse_iterator
                  rend () const;
   const_reverse_iterator
                  crend () const;

   // modifiers
   void           clear ();

   iterator       insert (const value_type & value);
   template <class U>
   iterator       insert (const U & value);
   template <class U>
   iterator       insert (std::unique_ptr <U> && value_uptr);
   template <class InputIterator>
   void           insert (InputIterator it, InputIterator it_end);

   template <class... Args>
   typename std::enable_if <
      !is_mold <Args...>::value,
      iterator
   >::type        emplace (Args &&... args);
   template <class U, class... Args>
   typename std::enable_if <
      !is_mold <Args...>::value,
      iterator
   >::type        emplace (Args &&... args);

   iterator       emplace (const Mold & mold);
   template <class U>
   iterator       emplace (const Mold & mold);

   iterator       erase (iterator it);

   iterator       splice (Collection & other, iterator it);

   // lookup
   template <class UnaryPredicate>
   size_t         count_if (UnaryPredicate p);
   template <class UnaryPredicate>
   size_t         count_if (UnaryPredicate p) const;
   template <class UnaryPredicate>
   iterator       find_if (UnaryPredicate p);
   template <class UnaryPredicate>
   const_iterator find_if (UnaryPredicate p) const;

   // Type
   inline virtual void
                  revert () override;

   // CollectionBase
   virtual collection_iterator
                  gbegin () override;
   virtual const_collection_iterator
                  gbegin () const override;
   virtual const_collection_iterator
                  gcbegin () const override;
   virtual collection_iterator
                  gend () override;
   virtual const_collection_iterator
                  gend () const override;
   virtual const_collection_iterator
                  gcend () const override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // Type
   inline virtual void
                  impl_set_class (const ClassBase & class_base) override;
   inline virtual void
                  impl_bind (DocumentBase & document, Ref & ref) override;
   inline virtual void
                  impl_unbind (DocumentBase & document) override;

   inline virtual void
                  impl_add () override;
   inline virtual void
                  impl_remove () override;

   inline virtual void
                  impl_make (Transaction & tx, ImplUndoRedoMode parent_mode, TxPostProcessInfo & ppinfo) const override;
   inline virtual bool
                  impl_is_reset () const override;
   inline virtual void
                  impl_reset () override;
   inline virtual void
                  impl_validate () override;
   inline virtual void
                  impl_synchronize () override;
   inline virtual void
                  impl_entity_reset () override;

   // CollectionBase
   inline virtual const ClassBase &
                  impl_element_class () const override;
   inline virtual KeyState
                  impl_get_key_state (const KeyRandom & key) const override;
   inline virtual const Ref &
                  impl_get_key_ref (const KeyRandom & key) const override;
   inline virtual void
                  impl_insert (const KeyRandom & key, uint32_t type_id, const Ref & elem_ref) override;
   inline virtual void
                  impl_reinsert (const KeyRandom & key) override;
   inline virtual Type &
                  impl_emplace (const KeyRandom & key, const ClassBase & class_base) override;
   inline virtual void
                  impl_erase (const KeyRandom & key) override;
   inline virtual void
                  impl_move (const KeyRandom & key, CollectionBase & other, const KeyRandom & other_key) override;
   inline virtual void
                  impl_relocate (const KeyRandom & key, CollectionBase & other, const KeyRandom & other_key) override;
   inline virtual bool
                  impl_is_moving (const KeyRandom & key) const override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef std::map <KeyRandom, ObjectWrapper <T>> Map;

   iterator       internal_insert (const KeyRandom & key, const ClassBase & class_base, std::shared_ptr <T> obj_sptr);

   Map            _map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Collection & rhs) const = delete;
   bool           operator != (const Collection & rhs) const = delete;



}; // class Collection



}  // namespace flip

// COV_NF_START

#include "flip/Collection.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

