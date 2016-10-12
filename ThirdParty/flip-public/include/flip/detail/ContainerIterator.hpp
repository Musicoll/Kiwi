/*****************************************************************************

      ContainerIterator.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_ContainerIterator_CURRENT_CODEHEADER)
   #error Recursive inclusion of ContainerIterator code header.
#endif
#define  flip_ContainerIterator_CURRENT_CODEHEADER

#if ! defined (flip_ContainerIterator_CODEHEADER_INCLUDED)
#define  flip_ContainerIterator_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/ElementState.h"

#include <cstring>
#include <cassert>



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
ContainerIterator <KeyType, DerefType>::ContainerIterator (Iterator it)
{
   static_assert (sizeof (Buf) >= sizeof (Iterator), "ContainerIterator storage is not sufficient");

   new (&_it) Iterator (it);

   _ctor_proc = &func_ctor <Iterator>;
   _copy_proc = &func_copy <Iterator>;
   _move_proc = &func_move <Iterator>;
   _dtor_proc = &func_dtor <Iterator>;
   _next_proc = &func_next <Iterator>;
   _prev_proc = &func_prev <Iterator>;
   _dereference_proc = &func_dereference <Iterator>;
   _equal_proc = &func_equal <Iterator>;
   _key_proc = &func_key <Iterator>;
   _added_proc = &func_added <Iterator>;
   _resident_proc = &func_resident <Iterator>;
   _removed_proc = &func_removed <Iterator>;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class KeyType, class DerefType>
ContainerIterator <KeyType, DerefType>::~ContainerIterator ()
{
   if (_dtor_proc != 0)
   {
      _dtor_proc (_it);
   }
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class KeyType, class DerefType>
ContainerIterator <KeyType, DerefType>::ContainerIterator (const ContainerIterator & rhs)
{
   ctor (rhs);

   _copy_proc (_it, rhs._it);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class KeyType, class DerefType>
ContainerIterator <KeyType, DerefType> & ContainerIterator <KeyType, DerefType>::operator = (const ContainerIterator & rhs)
{
   ctor (rhs);

   _copy_proc (_it, rhs._it);

   return *this;
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class KeyType, class DerefType>
ContainerIterator <KeyType, DerefType>::ContainerIterator (ContainerIterator && rhs)
{
   ctor (rhs);

   _move_proc (_it, rhs._it);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class KeyType, class DerefType>
ContainerIterator <KeyType, DerefType> & ContainerIterator <KeyType, DerefType>::operator = (ContainerIterator && rhs)
{
   ctor (rhs);

   _move_proc (_it, rhs._it);

   return *this;
}



/*
==============================================================================
Name : operator ++
==============================================================================
*/

template <class KeyType, class DerefType>
ContainerIterator <KeyType, DerefType> & ContainerIterator <KeyType, DerefType>::operator ++ ()
{
   assert (_next_proc != 0);

   _next_proc (_it);

   return *this;
}



/*
==============================================================================
Name : operator ++ (post)
==============================================================================
*/

template <class KeyType, class DerefType>
ContainerIterator <KeyType, DerefType> ContainerIterator <KeyType, DerefType>::operator ++ (int)
{
   assert (_next_proc != 0);

   ContainerIterator tmp;
   tmp.ctor (*this);
   _copy_proc (tmp._it, _it);

   _next_proc (_it);

   return tmp;
}



/*
==============================================================================
Name : operator --
==============================================================================
*/

template <class KeyType, class DerefType>
ContainerIterator <KeyType, DerefType> & ContainerIterator <KeyType, DerefType>::operator -- ()
{
   assert (_prev_proc != 0);

   _prev_proc (_it);

   return *this;
}



/*
==============================================================================
Name : operator -- (post)
==============================================================================
*/

template <class KeyType, class DerefType>
ContainerIterator <KeyType, DerefType> ContainerIterator <KeyType, DerefType>::operator -- (int)
{
   assert (_prev_proc != 0);

   ContainerIterator tmp;
   tmp.ctor (*this);
   _copy_proc (tmp._it, _it);

   _prev_proc (_it);

   return tmp;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class KeyType, class DerefType>
typename ContainerIterator <KeyType, DerefType>::reference ContainerIterator <KeyType, DerefType>::operator * () const
{
   assert (_dereference_proc != 0);

   return _dereference_proc (_it);
}



/*
==============================================================================
Name : operator ->
==============================================================================
*/

template <class KeyType, class DerefType>
typename ContainerIterator <KeyType, DerefType>::pointer ContainerIterator <KeyType, DerefType>::operator -> () const
{
   assert (_dereference_proc != 0);

   return &_dereference_proc (_it);
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

template <class KeyType, class DerefType>
bool  ContainerIterator <KeyType, DerefType>::operator == (const ContainerIterator & rhs) const
{
   assert (_equal_proc != 0);

   return _equal_proc (_it, rhs._it);
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

template <class KeyType, class DerefType>
bool  ContainerIterator <KeyType, DerefType>::operator != (const ContainerIterator & rhs) const
{
   assert (_equal_proc != 0);

   return ! _equal_proc (_it, rhs._it);
}



/*
==============================================================================
Name : added
==============================================================================
*/

template <class KeyType, class DerefType>
const KeyType &  ContainerIterator <KeyType, DerefType>::key () const
{
   assert (_key_proc != 0);

   return _key_proc (_it);
}



/*
==============================================================================
Name : added
==============================================================================
*/

template <class KeyType, class DerefType>
bool  ContainerIterator <KeyType, DerefType>::added () const
{
   assert (_added_proc != 0);

   return _added_proc (_it);
}



/*
==============================================================================
Name : resident
==============================================================================
*/

template <class KeyType, class DerefType>
bool  ContainerIterator <KeyType, DerefType>::resident () const
{
   assert (_resident_proc != 0);

   return _resident_proc (_it);
}



/*
==============================================================================
Name : removed
==============================================================================
*/

template <class KeyType, class DerefType>
bool  ContainerIterator <KeyType, DerefType>::removed () const
{
   assert (_removed_proc != 0);

   return _removed_proc (_it);
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class KeyType, class DerefType>
void  ContainerIterator <KeyType, DerefType>::ctor (const ContainerIterator & rhs)
{
   if (_ctor_proc != 0) return;  // already ok

   assert (rhs._ctor_proc != 0);

   _ctor_proc = rhs._ctor_proc;
   _copy_proc = rhs._copy_proc;
   _move_proc = rhs._move_proc;
   _dtor_proc = rhs._dtor_proc;
   _next_proc = rhs._next_proc;
   _prev_proc = rhs._prev_proc;
   _dereference_proc = rhs._dereference_proc;
   _equal_proc = rhs._equal_proc;
   _key_proc = rhs._key_proc;
   _added_proc = rhs._added_proc;
   _resident_proc = rhs._resident_proc;
   _removed_proc = rhs._removed_proc;

   _ctor_proc (_it);
}



/*
==============================================================================
Name : func_dtor
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
void  ContainerIterator <KeyType, DerefType>::func_ctor (Buf & buf)
{
   new (&buf) Iterator ();
}



/*
==============================================================================
Name : func_copy
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
void  ContainerIterator <KeyType, DerefType>::func_copy (Buf & dst_buf, const Buf & src_buf)
{
   Iterator & dst_it = *reinterpret_cast <Iterator *> (&dst_buf);
   const Iterator & src_it = *reinterpret_cast <const Iterator *> (&src_buf);

   dst_it = src_it;
}



/*
==============================================================================
Name : func_move
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
void  ContainerIterator <KeyType, DerefType>::func_move (Buf & dst_buf, Buf & src_buf)
{
   Iterator & dst_it = *reinterpret_cast <Iterator *> (&dst_buf);
   const Iterator & src_it = *reinterpret_cast <const Iterator *> (&src_buf);

   dst_it = std::move (src_it);
}



/*
==============================================================================
Name : func_dtor
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
void  ContainerIterator <KeyType, DerefType>::func_dtor (Buf & buf)
{
   Iterator & it = *reinterpret_cast <Iterator *> (&buf);

   it.~Iterator ();
}



/*
==============================================================================
Name : func_next
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
void  ContainerIterator <KeyType, DerefType>::func_next (Buf & buf)
{
   Iterator & it = *reinterpret_cast <Iterator *> (&buf);
   ++it;
}



/*
==============================================================================
Name : func_prev
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
void  ContainerIterator <KeyType, DerefType>::func_prev (Buf & buf)
{
   Iterator & it = *reinterpret_cast <Iterator *> (&buf);
   --it;
}



/*
==============================================================================
Name : func_dereference
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
DerefType &  ContainerIterator <KeyType, DerefType>::func_dereference (const Buf & buf)
{
   const Iterator & it = *reinterpret_cast <const Iterator *> (&buf);
   return *it;
}



/*
==============================================================================
Name : func_equal
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
bool  ContainerIterator <KeyType, DerefType>::func_equal (const Buf & buf, const Buf & rhs_buf)
{
   const Iterator & it = *reinterpret_cast <const Iterator *> (&buf);
   const Iterator & rhs_it = *reinterpret_cast <const Iterator *> (&rhs_buf);

   return it == rhs_it;
}



/*
==============================================================================
Name : func_key
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
const KeyType &   ContainerIterator <KeyType, DerefType>::func_key (const Buf & buf)
{
   const Iterator & it = *reinterpret_cast <const Iterator *> (&buf);

   return it.key ();
}



/*
==============================================================================
Name : func_added
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
bool  ContainerIterator <KeyType, DerefType>::func_added (const Buf & buf)
{
   const Iterator & it = *reinterpret_cast <const Iterator *> (&buf);

   return it.added ();
}



/*
==============================================================================
Name : func_resident
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
bool  ContainerIterator <KeyType, DerefType>::func_resident (const Buf & buf)
{
   const Iterator & it = *reinterpret_cast <const Iterator *> (&buf);

   return it.resident ();
}



/*
==============================================================================
Name : func_removed
==============================================================================
*/

template <class KeyType, class DerefType>
template <class Iterator>
bool  ContainerIterator <KeyType, DerefType>::func_removed (const Buf & buf)
{
   const Iterator & it = *reinterpret_cast <const Iterator *> (&buf);

   return it.removed ();
}



}  // namespace flip



#endif   // flip_ContainerIterator_CODEHEADER_INCLUDED

#undef flip_ContainerIterator_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

