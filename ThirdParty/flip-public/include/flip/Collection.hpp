/*****************************************************************************

      Collection.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Collection_CURRENT_CODEHEADER)
   #error Recursive inclusion of Collection code header.
#endif
#define  flip_Collection_CURRENT_CODEHEADER

#if ! defined (flip_Collection_CODEHEADER_INCLUDED)
#define  flip_Collection_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Class.h"

#include "flip/Mold.h"
#include "flip/detail/def.h"
#include "flip/detail/ClassRegistry.h"
#include "flip/detail/DataModelBase.h"
#include "flip/detail/DocumentBase.h"
#include "flip/detail/TypeTraits.h"

#include <algorithm>
#include <type_traits>



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Collection <T>::Collection ()
{
   static_assert (is_object <T>::value, "T must inherit from flip::Object");
   static_assert (is_flip_constructible <T>::value || std::is_abstract <T>::value, "T must support default constructor");
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Collection <T>::Collection (const Collection & rhs)
:  CollectionBase (rhs)
{
   static_assert (std::is_copy_constructible <T>::value || std::is_abstract <T>::value, "T must support copy constructor");

   auto it = rhs.begin ();
   auto it_end = rhs.end ();

   for (; it != it_end ; ++it)
   {
      if (it.removed ()) continue;

      auto & obj = *it;
      const auto & class_base = obj.get_class ();

      auto obj_sptr = std::shared_ptr <T> (
         dynamic_cast <T *> (class_base.impl_copy_ctor (obj))
      );

      obj_sptr->impl_set_class (class_base);

      internal_insert (KeyRandom (), class_base, obj_sptr);
   }
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Collection <T>::Collection (Collection && rhs)
:  CollectionBase (std::move (rhs))
{
   auto it = rhs._map.begin ();
   auto it_end = rhs._map.end ();

   for (; it != it_end ; ++it)
   {
      auto obj_sptr = it->second._obj_sptr;

      internal_insert (KeyRandom (), obj_sptr->get_class (), obj_sptr);
   }

   rhs._map.clear ();
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
template <class InputIterator>
Collection <T>::Collection (InputIterator it, InputIterator it_end)
{
   static_assert (is_object <T>::value, "T must inherit from flip::Object");
   static_assert (is_flip_constructible <T>::value || std::is_abstract <T>::value, "T must support default constructor");

   insert (it, it_end);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Collection <T> &   Collection <T>::operator = (const Collection & rhs)
{
   static_assert (std::is_copy_constructible <T>::value || std::is_abstract <T>::value, "T must support copy constructor");

   clear ();

   this->CollectionBase::operator = (rhs);

   auto it = rhs.begin ();
   auto it_end = rhs.end ();

   for (; it != it_end ; ++it)
   {
      if (it.removed ()) continue;

      auto & obj = *it;
      const auto & class_base = obj.get_class ();

      auto obj_sptr = std::shared_ptr <T> (
         dynamic_cast <T *> (class_base.impl_copy_ctor (obj))
      );

      obj_sptr->impl_set_class (class_base);

      internal_insert (KeyRandom (), class_base, obj_sptr);
   }

   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Collection <T> &   Collection <T>::operator = (Collection && rhs)
{
   clear ();

   this->CollectionBase::operator = (std::move (rhs));

   auto it = rhs._map.begin ();
   auto it_end = rhs._map.end ();

   for (; it != it_end ; ++it)
   {
      auto obj_sptr = it->second._obj_sptr;

      internal_insert (KeyRandom (), obj_sptr->get_class (), obj_sptr);
   }

   rhs._map.clear ();

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : get_class
==============================================================================
*/

template <class T>
const ClassBase & Collection <T>::get_class () const
{
   return Class <CollectionBase>::get ();
}



/*
==============================================================================
Name : begin
==============================================================================
*/

template <class T>
typename Collection <T>::iterator   Collection <T>::begin ()
{
   return iterator (_map.begin ());
}



/*
==============================================================================
Name : begin
==============================================================================
*/

template <class T>
typename Collection <T>::const_iterator   Collection <T>::begin () const
{
   return const_iterator (_map.begin ());
}



/*
==============================================================================
Name : cbegin
==============================================================================
*/

template <class T>
typename Collection <T>::const_iterator   Collection <T>::cbegin () const
{
   return const_iterator (_map.begin ());
}



/*
==============================================================================
Name : end
==============================================================================
*/

template <class T>
typename Collection <T>::iterator   Collection <T>::end ()
{
   return iterator (_map.end ());
}



/*
==============================================================================
Name : end
==============================================================================
*/

template <class T>
typename Collection <T>::const_iterator   Collection <T>::end () const
{
   return const_iterator (_map.end ());
}



/*
==============================================================================
Name : cend
==============================================================================
*/

template <class T>
typename Collection <T>::const_iterator   Collection <T>::cend () const
{
   return const_iterator (_map.end ());
}



/*
==============================================================================
Name : rbegin
==============================================================================
*/

template <class T>
typename Collection <T>::reverse_iterator Collection <T>::rbegin ()
{
   return reverse_iterator (_map.rbegin ());
}



/*
==============================================================================
Name : rbegin
==============================================================================
*/

template <class T>
typename Collection <T>::const_reverse_iterator Collection <T>::rbegin () const
{
   return const_reverse_iterator (_map.rbegin ());
}



/*
==============================================================================
Name : crbegin
==============================================================================
*/

template <class T>
typename Collection <T>::const_reverse_iterator Collection <T>::crbegin () const
{
   return const_reverse_iterator (_map.rbegin ());
}



/*
==============================================================================
Name : rend
==============================================================================
*/

template <class T>
typename Collection <T>::reverse_iterator Collection <T>::rend ()
{
   return reverse_iterator (_map.rend ());
}



/*
==============================================================================
Name : rend
==============================================================================
*/

template <class T>
typename Collection <T>::const_reverse_iterator Collection <T>::rend () const
{
   return const_reverse_iterator (_map.rend ());
}



/*
==============================================================================
Name : crend
==============================================================================
*/

template <class T>
typename Collection <T>::const_reverse_iterator Collection <T>::crend () const
{
   return const_reverse_iterator (_map.rend ());
}



/*
==============================================================================
Name : clear
==============================================================================
*/

template <class T>
void  Collection <T>::clear ()
{
   auto it = begin ();
   auto it_end = end ();

   for (; it != it_end ;)
   {
      if (it.removed ())
      {
         ++it;
      }
      else
      {
         it = erase (it);
      }
   }
}



/*
==============================================================================
Name : insert
==============================================================================
*/

template <class T>
typename Collection <T>::iterator   Collection <T>::insert (const value_type & value)
{
   return insert <T> (value);
}



/*
==============================================================================
Name : insert
==============================================================================
*/

template <class T>
template <class U>
typename Collection <T>::iterator   Collection <T>::insert (const U & value)
{
   static_assert (is_object <U>::value, "U must inherit from flip::Object");
   static_assert (std::is_base_of <T, U>::value, "U must inherit from T");
   static_assert (is_flip_constructible <U>::value, "U must support default constructor");
   static_assert (std::is_copy_constructible <U>::value, "U must support copy constructor");

   // cannot change model while observing
   if (!can_change ()) flip_FATAL;

   auto obj_sptr = std::shared_ptr <U> (new U (value));

   const auto & class_base = Class <U>::get ();

   obj_sptr->impl_set_class (class_base);

   return internal_insert (KeyRandom (), class_base, obj_sptr);
}



/*
==============================================================================
Name : insert
==============================================================================
*/

template <class T>
template <class U>
typename Collection <T>::iterator   Collection <T>::insert (std::unique_ptr <U> && value_uptr)
{
   static_assert (is_object <U>::value, "U must inherit from flip::Object");
   static_assert (std::is_base_of <T, U>::value, "U must inherit from T");
   static_assert (is_flip_constructible <U>::value || std::is_abstract <U>::value, "U must support default constructor");

   // cannot change model while observing
   if (!can_change ()) flip_FATAL;

   auto value_ptr = value_uptr.release ();

   auto obj_sptr = std::shared_ptr <U> (value_ptr);

   const auto & class_base = ClassRegistry::use ().find (typeid (*value_ptr));

   // cannot insert abstract class
   if (class_base.abstract ()) flip_FATAL;
   
   obj_sptr->impl_set_class (class_base);

   return internal_insert (KeyRandom (), class_base, obj_sptr);
}



/*
==============================================================================
Name : insert
==============================================================================
*/

template <class T>
template <class InputIterator>
void  Collection <T>::insert (InputIterator it, InputIterator it_end)
{
   for (; it != it_end ; ++it)
   {
      insert (*it);
   }
}



/*
==============================================================================
Name : emplace
==============================================================================
*/

template <class T>
template <class... Args>
typename std::enable_if <
   !is_mold <Args...>::value,
   typename Collection <T>::iterator
>::type  Collection <T>::emplace (Args &&... args)
{
   return emplace <T, Args...> (std::forward <Args> (args)...);
}



/*
==============================================================================
Name : emplace
==============================================================================
*/

template <class T>
template <class U, class... Args>
typename std::enable_if <
   !is_mold <Args...>::value,
   typename Collection <T>::iterator
>::type  Collection <T>::emplace (Args &&... args)
{
   static_assert (is_object <U>::value, "U must inherit from flip::Object");
   static_assert (std::is_base_of <T, U>::value, "U must inherit from T");
   static_assert (is_flip_constructible <U>::value, "U must support default constructor");

   // cannot change model while observing
   if (!can_change ()) flip_FATAL;

   auto obj_sptr = std::shared_ptr <U> (
      new U (std::forward <Args> (args)...)
   );

   obj_sptr->impl_set_class (Class <U>::get ());

   return internal_insert (KeyRandom (), Class <U>::get (), obj_sptr);
}



/*
==============================================================================
Name : emplace
==============================================================================
*/

template <class T>
typename Collection <T>::iterator   Collection <T>::emplace (const Mold & mold)
{
   return emplace <T> (mold);
}



/*
==============================================================================
Name : emplace
==============================================================================
*/

template <class T>
template <class U>
typename Collection <T>::iterator   Collection <T>::emplace (const Mold & mold)
{
   static_assert (is_object <U>::value, "U must inherit from flip::Object");
   static_assert (std::is_base_of <T, U>::value, "U must inherit from T");
   static_assert (is_flip_constructible <U>::value, "U must support default constructor");

   // cannot change model while observing
   if (!can_change ()) flip_FATAL;

   auto obj_sptr = std::shared_ptr <U> (&dynamic_cast <U &> (
      *Class <U>::get ().impl_ctor ()
   ));

   auto it = internal_insert (KeyRandom (), Class <U>::get (), obj_sptr);

   mold.cast <U> (dynamic_cast <U &> (*it));

   return it;
}



/*
==============================================================================
Name : erase
==============================================================================
*/

template <class T>
typename Collection <T>::iterator   Collection <T>::erase (iterator it)
{
   // cannot change model while observing
   if (!can_change ()) flip_FATAL;

   iterator it_next = it;
   ++it_next;

   auto & obj = *it;

   assert (!obj.removed ());

   obj.impl_reset ();

   if (obj.impl_parent_ptr () != nullptr)
   {
      obj.impl_change_parent (nullptr);
   }

   if (is_bound ())
   {
      obj.impl_remove ();
   }

   if (it.added ())
   {
      // case where underlying object is being moved
      if (it.base ()->second._obj_sptr.unique ())
      {
         if (obj.is_bound ()) obj.impl_unbind (document ());
         obj.impl_entity_reset ();
      }

      _map.erase (it.base ());

      impl_incr_modification_cnt (-1);
   }
   else
   {
      assert (it.resident ());

      it.base ()->second._state = ElementState::REMOVED;

      impl_incr_modification_cnt (1);
   }

   return it_next;
}



/*
==============================================================================
Name : splice
Note :
   'other' can be '*this'
==============================================================================
*/

template <class T>
typename Collection <T>::iterator   Collection <T>::splice (Collection & other, iterator it)
{
   // cannot change model while observing
   if (!can_change ()) flip_FATAL;

   if (this == &other) return it;   // abort

   auto & obj = *it;

   assert (!obj.removed ());
   assert (obj.impl_parent_ptr () != nullptr);

   auto obj_sptr = it.base ()->second._obj_sptr;

   if (it.added ())
   {
      other._map.erase (it.base ());

      other.impl_incr_modification_cnt (-1);
   }
   else
   {
      assert (it.resident ());

      it.base ()->second._state = ElementState::REMOVED;

      other.impl_incr_modification_cnt (1);
   }

   obj.impl_change_parent (this);

   auto pair = _map.insert (std::make_pair (KeyRandom (), ObjectWrapper <T> (obj_sptr)));
   assert (pair.second);

   impl_incr_modification_cnt (1);

   return pair.first;
}



/*
==============================================================================
Name : count_if
==============================================================================
*/

template <class T>
template <class UnaryPredicate>
size_t   Collection <T>::count_if (UnaryPredicate p)
{
   size_t ret_val = 0;

   iterator it = begin ();
   iterator it_end = end ();

   for (; it != it_end ; ++it)
   {
      if (p (*it)) ++ret_val;
   }

   return ret_val;
}



/*
==============================================================================
Name : count_if
==============================================================================
*/

template <class T>
template <class UnaryPredicate>
size_t   Collection <T>::count_if (UnaryPredicate p) const
{
   size_t ret_val = 0;

   const_iterator it = begin ();
   const_iterator it_end = end ();

   for (; it != it_end ; ++it)
   {
      if (p (*it)) ++ret_val;
   }

   return ret_val;
}



/*
==============================================================================
Name : find_if
==============================================================================
*/

template <class T>
template <class UnaryPredicate>
typename Collection <T>::iterator   Collection <T>::find_if (UnaryPredicate p)
{
   iterator it = begin ();
   iterator it_end = end ();

   for (; it != it_end ; ++it)
   {
      if (p (*it)) return it;
   }

   return it_end;
}



/*
==============================================================================
Name : find_if
==============================================================================
*/

template <class T>
template <class UnaryPredicate>
typename Collection <T>::const_iterator   Collection <T>::find_if (UnaryPredicate p) const
{
   const_iterator it = begin ();
   const_iterator it_end = end ();

   for (; it != it_end ; ++it)
   {
      if (p (*it)) return it;
   }

   return it_end;
}



/*
==============================================================================
Name : revert
==============================================================================
*/

template <class T>
void  Collection <T>::revert ()
{
   auto it = _map.begin ();
   auto it_end = _map.end ();

   for (; it != it_end ;)
   {
      auto it_next = it;
      ++it_next;

      auto & obj = it->second.get ();

      if (it->second.removed ())
      {
         it->second._state = ElementState::RESIDENT;

         // impl_add is recursive, so the object can be already added
         if (!obj.impl_is_added ()) obj.impl_add ();

         obj.impl_change_parent (this);
      }

      if (obj.changed ()) obj.revert ();

      if (it->second.added ())
      {
         if (it->second._obj_sptr.unique ())
         {
            obj.impl_change_parent (nullptr);
            obj.impl_remove ();
            obj.impl_unbind (document ());
            obj.impl_entity_reset ();
         }

         _map.erase (it);
      }

      it = it_next;
   }
}



/*
==============================================================================
Name : gbegin
==============================================================================
*/

template <class T>
collection_iterator  Collection <T>::gbegin ()
{
   return collection_iterator (begin ());
}  // COV_NF_LINE



/*
==============================================================================
Name : gbegin
==============================================================================
*/

template <class T>
const_collection_iterator  Collection <T>::gbegin () const
{
   return const_collection_iterator (begin ());
}  // COV_NF_LINE



/*
==============================================================================
Name : gcbegin
==============================================================================
*/

template <class T>
const_collection_iterator  Collection <T>::gcbegin () const
{
   return const_collection_iterator (begin ());
}  // COV_NF_LINE



/*
==============================================================================
Name : gend
==============================================================================
*/

template <class T>
collection_iterator  Collection <T>::gend ()
{
   return collection_iterator (end ());
}  // COV_NF_LINE



/*
==============================================================================
Name : gend
==============================================================================
*/

template <class T>
const_collection_iterator  Collection <T>::gend () const
{
   return const_collection_iterator (end ());
}  // COV_NF_LINE



/*
==============================================================================
Name : gcend
==============================================================================
*/

template <class T>
const_collection_iterator  Collection <T>::gcend () const
{
   return const_collection_iterator (end ());
}  // COV_NF_LINE



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : impl_set_class
==============================================================================
*/

template <class T>
void  Collection <T>::impl_set_class (const ClassBase & class_base)
{
#if defined (NDEBUG)
   flip_UNUSED (class_base);
#else
   assert (&class_base == &get_class ());
#endif
}



/*
==============================================================================
Name : impl_bind
==============================================================================
*/

template <class T>
void  Collection <T>::impl_bind (DocumentBase & document, Ref & ref)
{
   Type::impl_bind (document, ref);

   // the container is not empty iff it was inserted from a non empty
   // container

   iterator it = begin ();
   iterator it_end = end ();

   for (; it != it_end ; ++it)
   {
      auto & obj = *it;

      Ref sub_ref = document.impl_ref_allocator ().allocate (obj.get_class ().size_of ());

      obj.impl_bind (document, sub_ref);
   }
}



/*
==============================================================================
Name : impl_unbind
==============================================================================
*/

template <class T>
void  Collection <T>::impl_unbind (DocumentBase & document)
{
   iterator it = begin ();
   iterator it_end = end ();

   for (; it != it_end ; ++it)
   {
      auto & obj = *it;

      if (obj.is_bound () && it.base ()->second._obj_sptr.unique ())
      {
         assert (!obj.impl_is_added ());

         obj.impl_unbind (document);
      }
   }

   Type::impl_unbind (document);
}



/*
==============================================================================
Name : impl_add
Description :
   See 'Type::impl_add'.
==============================================================================
*/

template <class T>
void Collection <T>::impl_add ()
{
   Type::impl_add ();

   for (auto & pair : _map)
   {
      auto & obj = pair.second.get ();

      if (pair.second._state == ElementState::ADDED)
      {
         obj.impl_add ();
      }
   }
}



/*
==============================================================================
Name : impl_remove
Description :
   See 'Type::impl_remove'.
==============================================================================
*/

template <class T>
void Collection <T>::impl_remove ()
{
   // restore is only going to change modification cnt
   // in the tree, so we are not obliged to do this reversed

   Type::impl_remove ();

#if ! defined (NDEBUG)
   for (auto & pair : _map)
   {
      auto & obj = pair.second.get ();

      if (pair.second._obj_sptr.unique ())
      {
         // 'reset' have already removed objects in 'this'
         assert (!obj.impl_is_added ());
      }
   }
#endif
}



/*
==============================================================================
Name : impl_make
==============================================================================
*/

template <class T>
void  Collection <T>::impl_make (Transaction & tx, ImplUndoRedoMode parent_mode, TxPostProcessInfo & ppinfo) const
{
   auto mode = impl_make_undo_redo_mode (parent_mode);

   for (auto & pair : _map)
   {
      const auto & key = pair.first;
      auto type_id = pair.second.get ().get_class ().type_id (document ().data_model ());
      const auto & elem_ref = pair.second.get ().ref ();

      auto & obj = pair.second.get ();

      if (pair.second.added ())
      {
         if (pair.second._obj_sptr.unique ())
         {
            tx.push_collection_insert (ref (), impl_make_tx_flags (mode), key, type_id, elem_ref);
         }
         else
         {
            auto & old_parent = obj.parent ().template before <Collection <T>> ();

            auto old_it = std::find_if (
               old_parent._map.begin (), old_parent._map.end (),
               [&obj, &key](typename Map::value_type & other_pair){
                  return (&other_pair.second.get () == &obj) && (other_pair.first != key);
               }
            );
            assert (old_it != old_parent._map.end ());
            auto & old_key = old_it->first;

#if ! defined (NDEBUG)
            if (ref () == old_parent.ref ()) assert (key != old_key);
#endif

            tx.push_collection_move (ref (), impl_make_tx_flags (mode), key, old_parent.ref (), old_key);

            if (old_parent.removed ())
            {
               Type * tl_ptr = &old_parent;

               for (; tl_ptr->impl_parent_ptr () != nullptr ;)
               {
                  tl_ptr = tl_ptr->impl_parent_ptr ();
               }

               ppinfo.push (tl_ptr->ref (), tx.last ());
            }
         }
      }

      ImplUndoRedoMode mode_alt = mode;
      if ((mode != ImplUndoRedoMode::Disable) && (pair.second.added () || pair.second.removed ()))
      {
         mode_alt = ImplUndoRedoMode::ForceEnable;
      }

      if (!(pair.second.removed () && ! pair.second._obj_sptr.unique ()))
      {
         if (obj.changed ()) obj.impl_make (tx, mode_alt, ppinfo);
      }

      if (pair.second.removed ())
      {
         if (pair.second._obj_sptr.unique ())
         {
            tx.push_collection_erase (ref (), impl_make_tx_flags (mode), key, type_id, elem_ref);
         }
      }
   }
}



/*
==============================================================================
Name : impl_is_reset
==============================================================================
*/

template <class T>
bool  Collection <T>::impl_is_reset () const
{
   for (auto && pair : _map)
   {
      if (!pair.second.removed ()) return false;
   }

   return true;
}



/*
==============================================================================
Name : impl_reset
==============================================================================
*/

template <class T>
void  Collection <T>::impl_reset ()
{
   clear ();
}



/*
==============================================================================
Name : impl_validate
==============================================================================
*/

template <class T>
void  Collection <T>::impl_validate ()
{
   for (auto & pair : _map)
   {
      auto & obj = pair.second.get ();

      if (!obj.removed ())
      {
         if (obj.Type::changed ()) obj.impl_validate ();
      }
   }
}



/*
==============================================================================
Name : impl_synchronize
==============================================================================
*/

template <class T>
void  Collection <T>::impl_synchronize ()
{
   // sync children

   auto it = _map.begin ();
   auto it_end = _map.end ();

   for (; it != it_end ;)
   {
      auto it_next = it;
      ++it_next;

      auto & obj = it->second.get ();

      if (it->second.removed ())
      {
         if (it->second._obj_sptr.unique ())
         {
            if (obj.is_bound ()) obj.impl_unbind (document ());

            obj.impl_entity_reset ();
         }

         _map.erase (it);
      }
      else
      {
         if (obj.Type::changed ())
         {
            obj.impl_synchronize ();
         }

         it->second._state = ElementState::RESIDENT;
      }

      it = it_next;
   }

   // sync 'this'

   Type::impl_synchronize ();
}



/*
==============================================================================
Name : impl_entity_reset
==============================================================================
*/

template <class T>
void  Collection <T>::impl_entity_reset ()
{
   Type::impl_entity_reset ();

   for (auto & pair : _map)
   {
      auto & obj = pair.second.get ();

      if (pair.second._obj_sptr.unique ())
      {
         obj.impl_entity_reset ();
      }
   }
}



/*
==============================================================================
Name : impl_element_class
==============================================================================
*/

template <class T>
const ClassBase & Collection <T>::impl_element_class () const
{
   return Class <T>::get ();
}



/*
==============================================================================
Name : impl_get_key_state
==============================================================================
*/

template <class T>
typename Collection <T>::KeyState  Collection <T>::impl_get_key_state (const KeyRandom & key) const
{
   auto it = _map.find (key);

   if (it == _map.end ())
   {
      return KeyState::NON_PRESENT;
   }
   else
   {
      if (it->second._state == ElementState::REMOVED)
      {
         return KeyState::REMOVED;
      }
      else if (it->second._state == ElementState::ADDED)
      {
         return KeyState::ADDED;
      }
      else
      {
         return KeyState::RESIDENT;
      }
   }
}



/*
==============================================================================
Name : impl_get_key_ref
==============================================================================
*/

template <class T>
const Ref & Collection <T>::impl_get_key_ref (const KeyRandom & key) const
{
   assert (impl_get_key_state (key) != KeyState::NON_PRESENT);

   return _map.find (key)->second.get ().ref ();
}



/*
==============================================================================
Name : impl_insert
==============================================================================
*/

template <class T>
void  Collection <T>::impl_insert (const KeyRandom & key, uint32_t type_id, const Ref & elem_ref)
{
   const ClassBase & class_base = document ().data_model ().get_class (type_id);

   auto obj_sptr = std::shared_ptr <T> (&dynamic_cast <T &> (
      *class_base.impl_create (document (), elem_ref)
   ));

   internal_insert (key, class_base, obj_sptr);
}



/*
==============================================================================
Name : impl_reinsert
==============================================================================
*/

template <class T>
void  Collection <T>::impl_reinsert (const KeyRandom & key)
{
   auto it = _map.find (key);

   assert (it->second._state == ElementState::REMOVED);

   it->second._state = ElementState::RESIDENT;

   auto && obj_sptr = it->second._obj_sptr;

   obj_sptr->impl_change_parent (this);

   if (is_bound ())
   {
      obj_sptr->impl_add ();
   }

   impl_incr_modification_cnt (1);
}



/*
==============================================================================
Name : impl_emplace
==============================================================================
*/

template <class T>
Type &   Collection <T>::impl_emplace (const KeyRandom & key, const ClassBase & class_base)
{
   auto obj_sptr = std::shared_ptr <T> (&dynamic_cast <T &> (
      *class_base.impl_ctor ()
   ));

   auto it = internal_insert (key, class_base, obj_sptr);

   return *it;
}



/*
==============================================================================
Name : impl_erase
==============================================================================
*/

template <class T>
void  Collection <T>::impl_erase (const KeyRandom & key)
{
   auto it = _map.find (key);
   assert (it != _map.end ());

   auto && obj_sptr = it->second._obj_sptr;

   assert (!obj_sptr->removed ());

   if (obj_sptr->impl_parent_ptr () != nullptr)
   {
      obj_sptr->impl_change_parent (nullptr);
   }

   if (is_bound ())
   {
      obj_sptr->impl_remove ();
   }

   if (it->second.added ())
   {
      // case where underlying object is being moved
      if (obj_sptr.unique ())
      {
         if (obj_sptr->is_bound ()) obj_sptr->impl_unbind (document ());

         obj_sptr->impl_entity_reset ();
      }

      _map.erase (it);

      impl_incr_modification_cnt (-1);
   }
   else
   {
      assert (it->second.resident ());

      it->second._state = ElementState::REMOVED;

      impl_incr_modification_cnt (1);
   }
}



/*
==============================================================================
Name : impl_move
==============================================================================
*/

template <class T>
void  Collection <T>::impl_move (const KeyRandom & key, CollectionBase & other, const KeyRandom & other_key)
{
   auto & src = dynamic_cast <Collection <T> &> (other);

   auto src_it = src._map.find (other_key);
   assert (src_it != src._map.end ());

   auto obj_sptr = src_it->second._obj_sptr;

   if (src_it->second.added ())
   {
      src._map.erase (src_it);

      src.impl_incr_modification_cnt (-1);
   }
   else
   {
      assert (src_it->second.resident ());

      src_it->second._state = ElementState::REMOVED;

      src.impl_incr_modification_cnt (1);
   }

   if (this != &other)
   {
      obj_sptr->impl_change_parent (this);
   }

   auto pair = _map.insert (std::make_pair (key, ObjectWrapper <T> (obj_sptr)));
#if defined (NDEBUG)
   flip_UNUSED (pair);
#else
   assert (pair.second);
#endif

   impl_incr_modification_cnt (1);
}



/*
==============================================================================
Name : impl_relocate
==============================================================================
*/

template <class T>
void  Collection <T>::impl_relocate (const KeyRandom & key, CollectionBase & other, const KeyRandom & other_key)
{
   auto & src = dynamic_cast <Collection <T> &> (other);

   auto src_it = src._map.find (other_key);
   assert (src_it != src._map.end ());

   auto obj_sptr = src_it->second._obj_sptr;

   assert (src_it->second.added ());

   src._map.erase (src_it);

   src.impl_incr_modification_cnt (-1);

   if (this != &other)
   {
      obj_sptr->impl_change_parent (this);
   }

   auto dst_it = _map.find (key);

   assert (dst_it->second._state == ElementState::REMOVED);

   dst_it->second._state = ElementState::RESIDENT;

   impl_incr_modification_cnt (1);
}



/*
==============================================================================
Name : impl_is_moving
==============================================================================
*/

template <class T>
bool   Collection <T>::impl_is_moving (const KeyRandom & key) const
{
   auto it = _map.find (key);
   assert (it != _map.end ());

   return !it->second._obj_sptr.unique ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : internal_insert
==============================================================================
*/

template <class T>
typename Collection <T>::iterator   Collection <T>::internal_insert (const KeyRandom & key, const ClassBase & class_base, std::shared_ptr <T> obj_sptr)
{
   if (is_bound () && !obj_sptr->is_bound ())
   {
      Ref ref = document ().impl_ref_allocator ().allocate (class_base.size_of ());

      obj_sptr->impl_bind (document (), ref);
   }

   obj_sptr->impl_change_parent (this);

   if (is_bound ())
   {
      obj_sptr->impl_add ();
   }

   auto pair = _map.insert (std::make_pair (key, ObjectWrapper <T> (obj_sptr)));
   assert (pair.second);

   impl_incr_modification_cnt (1);

   return pair.first;
}  // COV_NF_LINE



}  // namespace flip



#endif   // flip_Collection_CODEHEADER_INCLUDED

#undef flip_Collection_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

