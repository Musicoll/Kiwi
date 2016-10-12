/*****************************************************************************

      Variant.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Variant_CURRENT_CODEHEADER)
   #error Recursive inclusion of Variant code header.
#endif
#define  flip_Variant_CURRENT_CODEHEADER

#if ! defined (flip_Variant_CODEHEADER_INCLUDED)
#define  flip_Variant_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/DocumentValidator.h"
#include "flip/Transaction.h"
#include "flip/detail/def.h"
#include "flip/detail/fnc.h"
#include "flip/detail/TypeTraits.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Variant <T>::Variant (const Variant & rhs)
:  Collection <T> (rhs)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Variant <T>::Variant (Variant && rhs)
:  Collection <T> (std::move (rhs))
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Variant <T>::Variant (const T & rhs)
{
   this->operator = (rhs);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Variant <T> &   Variant <T>::operator = (const Variant & rhs)
{
   Collection <T> & dst = *this;

   dst = rhs;

   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Variant <T> &   Variant <T>::operator = (Variant && rhs)
{
   Collection <T> & dst = *this;

   dst = std::move (rhs);

   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
template <class U>
Variant <T> &   Variant <T>::operator = (const U & rhs)
{
   // cannot change model while observing
   if (!Collection <T>::can_change ()) flip_FATAL;

   Collection <T>::clear ();
   Collection <T>::insert (rhs);

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
template <class U>
Variant <T> &   Variant <T>::operator = (std::unique_ptr <U> && rhs)
{
   // cannot change model while observing
   if (!Collection <T>::can_change ()) flip_FATAL;

   Collection <T>::clear ();
   Collection <T>::insert (std::forward <std::unique_ptr <U>> (rhs));

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : reset
==============================================================================
*/

template <class T>
template <class... Args>
typename std::enable_if <!is_mold <Args...>::value, T &
>::type  Variant <T>::reset (Args &&... args)
{
   return reset <T, Args...> (std::forward <Args> (args)...);
}



/*
==============================================================================
Name : reset
==============================================================================
*/

template <class T>
template <class U, class... Args>
typename std::enable_if <!is_mold <Args...>::value, U &
>::type  Variant <T>::reset (Args &&... args)
{
   static_assert (is_object <U>::value, "U must inherit from flip::Object");
   static_assert (std::is_base_of <T, U>::value, "U must inherit from T");
   static_assert (is_flip_constructible <U>::value, "U must support default constructor");

   // cannot change model while observing
   if (!Collection <T>::can_change ()) flip_FATAL;

   Collection <T>::clear ();
   auto it = Collection <T>::template emplace <U, Args...> (std::forward <Args> (args)...);

   T & val = *it;

   return dynamic_cast <U &> (val);
}



/*
==============================================================================
Name : reset
==============================================================================
*/

template <class T>
T &   Variant <T>::reset (const Mold & mold)
{
   return reset <T> (mold);
}



/*
==============================================================================
Name : reset
==============================================================================
*/

template <class T>
template <class U>
U &   Variant <T>::reset (const Mold & mold)
{
   static_assert (is_object <U>::value, "U must inherit from flip::Object");
   static_assert (std::is_base_of <T, U>::value, "U must inherit from T");
   static_assert (is_flip_constructible <U>::value, "U must support default constructor");

   // cannot change model while observing
   if (!Collection <T>::can_change ()) flip_FATAL;

   Collection <T>::clear ();
   auto it = Collection <T>::template emplace <U> (mold);

   U & val = dynamic_cast <U &> (*it);

   return val;
}



/*
==============================================================================
Name : operator T &
==============================================================================
*/

template <class T>
Variant <T>::operator T & () const
{
   if (!is_valid ()) flip_THROW;

   auto it = Collection <T>::begin ();
   const auto it_end = Collection <T>::end ();

   for (; it != it_end ; ++it)
   {
      if (it.added () || it.resident ()) return *it;
   } // COV_NF_LINE

   flip_THROW; // COV_NF_LINE
}



/*
==============================================================================
Name : use
==============================================================================
*/

template <class T>
T &   Variant <T>::use () const
{
   return this->operator T & ();
}



/*
==============================================================================
Name : use
==============================================================================
*/

template <class T>
template <class U>
U &   Variant <T>::use () const
{
   return dynamic_cast <U &> (this->operator T & ());
}



/*
==============================================================================
Name : value
==============================================================================
*/

template <class T>
T &   Variant <T>::value () const
{
   return this->operator T & ();
}



/*
==============================================================================
Name : value
==============================================================================
*/

template <class T>
template <class U>
U &   Variant <T>::value () const
{
   return dynamic_cast <const U &> (this->operator T & ());
}



/*
==============================================================================
Name : before
==============================================================================
*/

template <class T>
T &   Variant <T>::before () const
{
   if (!is_valid ()) flip_THROW;

   auto it = Collection <T>::begin ();
   const auto it_end = Collection <T>::end ();

   for (; it != it_end ; ++it)
   {
      if (it.removed () || it.resident ()) return *it;
   } // COV_NF_LINE

   flip_THROW; // COV_NF_LINE
}



/*
==============================================================================
Name : before
==============================================================================
*/

template <class T>
template <class U>
U &   Variant <T>::before () const
{
   return dynamic_cast <U &> (before ());
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : impl_validate
Description :
   Validate exactly one element after synchronisation
==============================================================================
*/

template <class T>
void  Variant <T>::impl_validate ()
{
   if (!is_valid ()) flip_VALIDATION_FAILED ("Variant constraint violation");
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : is_valid
Description :
   Validate either one added and removed or one resident.
==============================================================================
*/

template <class T>
bool  Variant <T>::is_valid () const
{
   int cnt_added = 0;
   int cnt_resident = 0;
   int cnt_removed = 0;

   auto it = Collection <T>::begin ();
   const auto it_end = Collection <T>::end ();

   for (; it != it_end ; ++it)
   {
      cnt_added += it.added () ? 1 : 0;
      cnt_resident += it.resident () ? 1 : 0;
      cnt_removed += it.removed () ? 1 : 0;
   }

   bool ok_flag = false;
   ok_flag |= (cnt_added == 1) && (cnt_removed == 0) && (cnt_resident == 0);
   ok_flag |= (cnt_added == 1) && (cnt_removed == 1) && (cnt_resident == 0);
   ok_flag |= (cnt_added == 0) && (cnt_removed == 0) && (cnt_resident == 1);

   return ok_flag;
}



}  // namespace flip



#endif   // flip_Variant_CODEHEADER_INCLUDED

#undef flip_Variant_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

