/*****************************************************************************

      DocumentBase.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_DocumentBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of DocumentBase code header.
#endif
#define  flip_DocumentBase_CURRENT_CODEHEADER

#if ! defined (flip_DocumentBase_CODEHEADER_INCLUDED)
#define  flip_DocumentBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Object.h"
#include "flip/detail/def.h"

#include <cassert>



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : root
==============================================================================
*/

template <class T>
T &   DocumentBase::root ()
{
   return dynamic_cast <T &> (*_root_ptr);
}



/*
==============================================================================
Name : root
==============================================================================
*/

template <class T>
const T &   DocumentBase::root () const
{
   return dynamic_cast <const T &> (*_root_ptr);
}



/*
==============================================================================
Name : object
==============================================================================
*/

template <class T>
T &   DocumentBase::object (const Ref & ref)
{
   T * ptr = object_ptr <T> (ref);

   if (ptr == nullptr) flip_FATAL;

   return *ptr;
}  // COV_NF_LINE



/*
==============================================================================
Name : object
==============================================================================
*/

template <class T>
const T &   DocumentBase::object (const Ref & ref) const
{
   const T * ptr = object_ptr <T> (ref);

   if (ptr == nullptr) flip_FATAL;

   return *ptr;
}  // COV_NF_LINE



/*
==============================================================================
Name : object_ptr
==============================================================================
*/

template <class T>
T *   DocumentBase::object_ptr (const Ref & ref)
{
   return dynamic_cast <T *> (_allocator.get (ref));
}  // COV_NF_LINE



/*
==============================================================================
Name : object_ptr
==============================================================================
*/

template <class T>
const T *   DocumentBase::object_ptr (const Ref & ref) const
{
   return dynamic_cast <const T *> (_allocator.get (ref));
}  // COV_NF_LINE



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_DocumentBase_CODEHEADER_INCLUDED

#undef flip_DocumentBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

