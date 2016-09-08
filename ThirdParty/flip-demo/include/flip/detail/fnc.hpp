/*****************************************************************************

      fnc.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_fnc_CURRENT_CODEHEADER)
   #error Recursive inclusion of fnc code header.
#endif
#define  flip_fnc_CURRENT_CODEHEADER

#if ! defined (flip_fnc_CODEHEADER_INCLUDED)
#define  flip_fnc_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/Type.h"

#include <cassert>



namespace flip
{



/*
==============================================================================
Name : modify
Description :
   Set and notify modification :
   - if the new value is different from before, we have one more modification
   - if the new value is the same as before, we have one modification less
==============================================================================
*/

template <class T>
void  modify (Type & obj, T & before, T & current, T value)
{
   assert (current != value);

   if (before == current)
   {
      obj.impl_incr_modification_cnt (1);
   }
   else if (before == value)
   {
      obj.impl_incr_modification_cnt (-1);
   }

   current = value;
}



/*
==============================================================================
Name : modify_move
Description :
   Set and notify modification :
   - if the new value is different from before, we have one more modification
   - if the new value is the same as before, we have one modification less
==============================================================================
*/

template <class T>
void  modify_move (Type & obj, T & before, T & current, T && value)
{
   assert (current != value);

   if (before == current)
   {
      obj.impl_incr_modification_cnt (1);
   }
   else if (before == value)
   {
      obj.impl_incr_modification_cnt (-1);
   }

   current = std::move (value);
}



/*
==============================================================================
Name : base
Description :
   Return the base type when available.
==============================================================================
*/

template <class T>
typename std::enable_if <
   is_enum <T>::value, ClassBase &
>::type  base ()
{
   // Enumeration is used but was not previously declared
   if (EnumClass <typename T::value_type>::use ().name () == nullptr) flip_FATAL;

   return Class <EnumBase>::use ();
}



/*
==============================================================================
Name : base
Description :
   Return the base type when available.
==============================================================================
*/

template <class T>
typename std::enable_if <
   is_blob <T>::value, ClassBase &
>::type  base ()
{
   return Class <Blob>::use ();
}



/*
==============================================================================
Name : base
Description :
   Return the base type when available.
==============================================================================
*/

template <class T>
typename std::enable_if <
   is_object_ref <T>::value, ClassBase &
>::type  base ()
{
   return Class <ObjectRefBase>::use ();
}



/*
==============================================================================
Name : base
Description :
   Return the base type when available.
==============================================================================
*/

template <class T>
typename std::enable_if <
   is_collection <T>::value, ClassBase &
>::type  base ()
{
   return Class <CollectionBase>::use ();
}



/*
==============================================================================
Name : base
Description :
   Return the base type when available.
==============================================================================
*/

template <class T>
typename std::enable_if <
   is_array <T>::value, ClassBase &
>::type  base ()
{
   return Class <ArrayBase>::use ();
}



/*
==============================================================================
Name : base
Description :
   Return the base type when available.
==============================================================================
*/

template <class T>
typename std::enable_if <
   is_message <T>::value, ClassBase &
>::type  base ()
{
   return Class <MessageBase>::use ();
}



/*
==============================================================================
Name : base
Description :
   Return the base type when available.
==============================================================================
*/

template <class T>
typename std::enable_if <
   ! (is_enum <T>::value || is_blob <T>::value || is_object_ref <T>::value || is_collection <T>::value || is_array <T>::value || is_message <T>::value), ClassBase &
>::type  base ()
{
   return Class <T>::use ();
}



}  // namespace flip



#endif   // flip_fnc_CODEHEADER_INCLUDED

#undef flip_fnc_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

