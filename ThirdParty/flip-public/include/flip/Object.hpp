/*****************************************************************************

      Object.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Object_CURRENT_CODEHEADER)
   #error Recursive inclusion of Object code header.
#endif
#define  flip_Object_CURRENT_CODEHEADER

#if ! defined (flip_Object_CODEHEADER_INCLUDED)
#define  flip_Object_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : Parent::ptr
==============================================================================
*/

template <class T>
T *   Object::Parent::ptr ()
{
   return dynamic_cast <T *> (_parent.impl_parent_ptr ());
}



/*
==============================================================================
Name : Parent::before
==============================================================================
*/

template <class T>
T &   Object::Parent::before ()
{
   if (_parent._parent_before_ptr == nullptr) flip_FATAL;

   return dynamic_cast <T &> (*_parent._parent_before_ptr);
}  // COV_NF_LINE



/*
==============================================================================
Name : Parent::before_ptr
==============================================================================
*/

template <class T>
T *   Object::Parent::before_ptr ()
{
   return dynamic_cast <T *> (_parent._parent_before_ptr);
}



/*
==============================================================================
Name : parent
==============================================================================
*/

template <class T>
T &   Object::parent ()
{
   if (impl_parent_ptr () == nullptr) flip_FATAL;

   return dynamic_cast <T &> (*impl_parent_ptr ());
}  // COV_NF_LINE



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_Object_CODEHEADER_INCLUDED

#undef flip_Object_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

