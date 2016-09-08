/*****************************************************************************

      ArrayIteratorBase.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_ArrayIteratorBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of ArrayIteratorBase code header.
#endif
#define  flip_ArrayIteratorBase_CURRENT_CODEHEADER

#if ! defined (flip_ArrayIteratorBase_CODEHEADER_INCLUDED)
#define  flip_ArrayIteratorBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/ElementState.h"

#include <cassert>



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class MapIterator>
ArrayIteratorBase <MapIterator>::ArrayIteratorBase (MapIterator it)
:  _it (it)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class MapIterator>
template <class U>
ArrayIteratorBase <MapIterator>::ArrayIteratorBase (const ArrayIteratorBase <U> & rhs)
:  _it (rhs.base ())
{
}



/*
==============================================================================
Name : ctor
Note :
   Support for MSVC 2013u4.
==============================================================================
*/

template <class MapIterator>
ArrayIteratorBase <MapIterator>::ArrayIteratorBase (ArrayIteratorBase && rhs)
:  _it (std::move (rhs._it))
{
}



/*
==============================================================================
Name : ctor
Note :
   Support for MSVC 2013u4.
==============================================================================
*/

template <class MapIterator>
ArrayIteratorBase <MapIterator> &  ArrayIteratorBase <MapIterator>::operator = (ArrayIteratorBase && rhs)
{
   _it = std::move (rhs._it);

   return *this;
}



/*
==============================================================================
Name : operator ++
==============================================================================
*/

template <class MapIterator>
ArrayIteratorBase <MapIterator> & ArrayIteratorBase <MapIterator>::operator ++ ()
{
   ++_it;

   return *this;
}



/*
==============================================================================
Name : operator ++ (post)
==============================================================================
*/

template <class MapIterator>
ArrayIteratorBase <MapIterator> ArrayIteratorBase <MapIterator>::operator ++ (int)
{
   ArrayIteratorBase tmp (_it);
   ++_it;

   return tmp;
}



/*
==============================================================================
Name : operator --
==============================================================================
*/

template <class MapIterator>
ArrayIteratorBase <MapIterator> & ArrayIteratorBase <MapIterator>::operator -- ()
{
   --_it;

   return *this;
}



/*
==============================================================================
Name : operator -- (post)
==============================================================================
*/

template <class MapIterator>
ArrayIteratorBase <MapIterator> ArrayIteratorBase <MapIterator>::operator -- (int)
{
   ArrayIteratorBase tmp (_it);
   --_it;

   return tmp;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class MapIterator>
typename ArrayIteratorBase <MapIterator>::reference ArrayIteratorBase <MapIterator>::operator * () const
{
   assert (_it->second._obj_sptr.get () != nullptr);

   return *_it->second._obj_sptr;
}



/*
==============================================================================
Name : operator ->
==============================================================================
*/

template <class MapIterator>
typename ArrayIteratorBase <MapIterator>::pointer ArrayIteratorBase <MapIterator>::operator -> () const
{
   assert (_it->second._obj_sptr.get () != nullptr);

   return &*_it->second._obj_sptr;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

template <class MapIterator>
bool  ArrayIteratorBase <MapIterator>::operator == (const ArrayIteratorBase & rhs) const
{
   return _it == rhs._it;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

template <class MapIterator>
bool  ArrayIteratorBase <MapIterator>::operator != (const ArrayIteratorBase & rhs) const
{
   return _it != rhs._it;
}



/*
==============================================================================
Name : added
==============================================================================
*/

template <class MapIterator>
const typename MapIterator::value_type::first_type &  ArrayIteratorBase <MapIterator>::key () const
{
   return _it->first;
}



/*
==============================================================================
Name : added
==============================================================================
*/

template <class MapIterator>
bool  ArrayIteratorBase <MapIterator>::added () const
{
   return _it->second._state == ElementState::ADDED;
}



/*
==============================================================================
Name : resident
==============================================================================
*/

template <class MapIterator>
bool  ArrayIteratorBase <MapIterator>::resident () const
{
   return _it->second._state == ElementState::RESIDENT;
}



/*
==============================================================================
Name : removed
==============================================================================
*/

template <class MapIterator>
bool  ArrayIteratorBase <MapIterator>::removed () const
{
   return _it->second._state == ElementState::REMOVED;
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : base
==============================================================================
*/

template <class MapIterator>
MapIterator ArrayIteratorBase <MapIterator>::base () const
{
   return _it;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_ArrayIteratorBase_CODEHEADER_INCLUDED

#undef flip_ArrayIteratorBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

