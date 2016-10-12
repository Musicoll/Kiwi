/*****************************************************************************

      Type.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Type_CURRENT_CODEHEADER)
   #error Recursive inclusion of Type code header.
#endif
#define  flip_Type_CURRENT_CODEHEADER

#if ! defined (flip_Type_CODEHEADER_INCLUDED)
#define  flip_Type_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ancestor
==============================================================================
*/

template <class T>
T &   Type::ancestor ()
{
   Type * obj_ptr = this;

   while (obj_ptr != nullptr)
   {
      T * cast_ptr = dynamic_cast <T *> (obj_ptr);

      if (cast_ptr != nullptr) return *cast_ptr;

      obj_ptr = obj_ptr->impl_parent_ptr ();
   }

   flip_FATAL;
}  // COV_NF_LINE



/*
==============================================================================
Name : ancestor
==============================================================================
*/

template <class T>
const T &   Type::ancestor () const
{
   const Type * obj_ptr = this;

   while (obj_ptr != nullptr)
   {
      const T * cast_ptr = dynamic_cast <const T *> (obj_ptr);

      if (cast_ptr != nullptr) return *cast_ptr;

      obj_ptr = obj_ptr->_parent_ptr;
   }

   flip_FATAL;
}  // COV_NF_LINE



/*
==============================================================================
Name : ancestor
==============================================================================
*/

template <class T>
T *   Type::ancestor_ptr ()
{
   Type * obj_ptr = this;

   while (obj_ptr != nullptr)
   {
      T * cast_ptr = dynamic_cast <T *> (obj_ptr);

      if (cast_ptr != nullptr) return cast_ptr;

      obj_ptr = obj_ptr->impl_parent_ptr ();
   }

   return nullptr;
}  // COV_NF_LINE



/*
==============================================================================
Name : ancestor
==============================================================================
*/

template <class T>
const T *   Type::ancestor_ptr () const
{
   const Type * obj_ptr = this;

   while (obj_ptr != nullptr)
   {
      const T * cast_ptr = dynamic_cast <const T *> (obj_ptr);

      if (cast_ptr != nullptr) return cast_ptr;

      obj_ptr = obj_ptr->_parent_ptr;
   }

   return nullptr;
}  // COV_NF_LINE



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_Type_CODEHEADER_INCLUDED

#undef flip_Type_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

