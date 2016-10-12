/*****************************************************************************

      CollectionIteratorBase.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_CollectionIteratorBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of CollectionIteratorBase code header.
#endif
#define  flip_CollectionIteratorBase_CURRENT_CODEHEADER

#if ! defined (flip_CollectionIteratorBase_CODEHEADER_INCLUDED)
#define  flip_CollectionIteratorBase_CODEHEADER_INCLUDED



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
CollectionIteratorBase <MapIterator>::CollectionIteratorBase (MapIterator it)
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
CollectionIteratorBase <MapIterator>::CollectionIteratorBase (const CollectionIteratorBase <U> & rhs)
:  _it (rhs.base ())
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class MapIterator>
CollectionIteratorBase <MapIterator>::CollectionIteratorBase (CollectionIteratorBase && rhs)
:  _it (std::move (rhs._it))
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class MapIterator>
CollectionIteratorBase <MapIterator> & CollectionIteratorBase <MapIterator>::operator = (CollectionIteratorBase && rhs)
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
CollectionIteratorBase <MapIterator> & CollectionIteratorBase <MapIterator>::operator ++ ()
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
CollectionIteratorBase <MapIterator> CollectionIteratorBase <MapIterator>::operator ++ (int)
{
   CollectionIteratorBase tmp (_it);
   ++_it;

   return tmp;
}



/*
==============================================================================
Name : operator --
==============================================================================
*/

template <class MapIterator>
CollectionIteratorBase <MapIterator> & CollectionIteratorBase <MapIterator>::operator -- ()
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
CollectionIteratorBase <MapIterator> CollectionIteratorBase <MapIterator>::operator -- (int)
{
   CollectionIteratorBase tmp (_it);
   --_it;

   return tmp;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class MapIterator>
typename CollectionIteratorBase <MapIterator>::reference CollectionIteratorBase <MapIterator>::operator * () const
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
typename CollectionIteratorBase <MapIterator>::pointer CollectionIteratorBase <MapIterator>::operator -> () const
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
bool  CollectionIteratorBase <MapIterator>::operator == (const CollectionIteratorBase & rhs) const
{
   return _it == rhs._it;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

template <class MapIterator>
bool  CollectionIteratorBase <MapIterator>::operator != (const CollectionIteratorBase & rhs) const
{
   return _it != rhs._it;
}



/*
==============================================================================
Name : added
==============================================================================
*/

template <class MapIterator>
const typename MapIterator::value_type::first_type &  CollectionIteratorBase <MapIterator>::key () const
{
   return _it->first;
}



/*
==============================================================================
Name : added
==============================================================================
*/

template <class MapIterator>
bool  CollectionIteratorBase <MapIterator>::added () const
{
   return _it->second._state == ElementState::ADDED;
}



/*
==============================================================================
Name : resident
==============================================================================
*/

template <class MapIterator>
bool  CollectionIteratorBase <MapIterator>::resident () const
{
   return _it->second._state == ElementState::RESIDENT;
}



/*
==============================================================================
Name : removed
==============================================================================
*/

template <class MapIterator>
bool  CollectionIteratorBase <MapIterator>::removed () const
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
MapIterator CollectionIteratorBase <MapIterator>::base () const
{
   return _it;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_CollectionIteratorBase_CODEHEADER_INCLUDED

#undef flip_CollectionIteratorBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

