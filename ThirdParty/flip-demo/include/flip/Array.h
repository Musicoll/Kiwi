/*****************************************************************************

      Array.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/ArrayBase.h"
#include "flip/detail/ArrayIteratorBase.h"
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
class Array
:  public ArrayBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef T value_type;
   typedef T & reference;
   typedef const T & const_reference;
   typedef T * pointer;
   typedef const T * const_pointer;
   typedef ArrayIteratorBase <typename std::map <KeyFloat, ObjectWrapper <T>>::iterator> iterator;
   typedef ArrayIteratorBase <typename std::map <KeyFloat, ObjectWrapper <T>>::reverse_iterator> reverse_iterator;
   typedef ArrayIteratorBase <typename std::map <KeyFloat, ObjectWrapper <T>>::const_iterator> const_iterator;
   typedef ArrayIteratorBase <typename std::map <KeyFloat, ObjectWrapper <T>>::const_reverse_iterator> const_reverse_iterator;
   typedef std::ptrdiff_t difference_type;
   typedef std::size_t size_type;

                  Array ();
                  Array (const Array & rhs);
                  Array (Array && rhs);
   template <class InputIterator>
                  Array (InputIterator it, InputIterator it_end);
   virtual        ~Array () = default;

   Array &        operator = (const Array & rhs);
   Array &        operator = (Array && rhs);

   inline virtual const ClassBase &
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

   iterator       insert (iterator pos, const value_type & value);
   template <class U>
   iterator       insert (iterator pos, const U & value);
   template <class U>
   iterator       insert (iterator pos, std::unique_ptr <U> && value_uptr);
   template <class InputIterator>
   void           insert (iterator pos, InputIterator it, InputIterator it_end);

   template <class... Args>
   typename std::enable_if <
      !is_mold <Args...>::value,
      iterator
   >::type        emplace (iterator pos, Args &&... args);
   template <class U, class... Args>
   typename std::enable_if <
      !is_mold <Args...>::value,
      iterator
   >::type        emplace (iterator pos, Args &&... args);

   iterator       emplace (iterator pos, const Mold & mold);
   template <class U>
   iterator       emplace (iterator pos, const Mold & mold);

   iterator       erase (iterator it);
   void           erase (iterator it, iterator it_end);

   iterator       splice (iterator pos, Array & other, iterator it);

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

   // ArrayBase
   virtual array_iterator
                  gbegin () override;
   virtual const_array_iterator
                  gbegin () const override;
   virtual const_array_iterator
                  gcbegin () const override;
   virtual array_iterator
                  gend () override;
   virtual const_array_iterator
                  gend () const override;
   virtual const_array_iterator
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

   // ArrayBase
   inline virtual const ClassBase &
                  impl_element_class () const override;
   inline virtual KeyState
                  impl_get_key_state (const KeyFloat & key) const override;
   inline virtual const Ref &
                  impl_get_key_ref (const KeyFloat & key) const override;
   inline virtual void
                  impl_insert (const KeyFloat & key, uint32_t type_id, const Ref & elem_ref) override;
   inline virtual void
                  impl_reinsert (const KeyFloat & key) override;
   inline virtual Type &
                  impl_emplace (const KeyFloat & key, const ClassBase & class_base) override;
   inline virtual void
                  impl_erase (const KeyFloat & key) override;
   inline virtual void
                  impl_move (const KeyFloat & key, ArrayBase & other, const KeyFloat & other_key) override;
   inline virtual void
                  impl_relocate (const KeyFloat & key, ArrayBase & other, const KeyFloat & other_key) override;
   inline virtual bool
                  impl_is_moving (const KeyFloat & key) const override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef std::map <KeyFloat, ObjectWrapper <T>> Map;

   KeyFloat       make_key (iterator pos);
   iterator       internal_insert (const KeyFloat & key, const ClassBase & class_base, std::shared_ptr <T> obj_sptr);

   Map            _map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Array & rhs) const = delete;
   bool           operator != (const Array & rhs) const = delete;



}; // class Array



}  // namespace flip

// COV_NF_START

#include "flip/Array.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

