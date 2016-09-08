/*****************************************************************************

      Optional.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Optional_CURRENT_CODEHEADER)
   #error Recursive inclusion of Optional code header.
#endif
#define  flip_Optional_CURRENT_CODEHEADER

#if ! defined (flip_Optional_CODEHEADER_INCLUDED)
#define  flip_Optional_CODEHEADER_INCLUDED



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
Optional <T>::Optional (const Optional & rhs)
:  Collection <T> (rhs)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Optional <T>::Optional (Optional && rhs)
:  Collection <T> (std::move (rhs))
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Optional <T>::Optional (const T & rhs)
{
   this->operator = (rhs);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Optional <T> &   Optional <T>::operator = (const Optional & rhs)
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
Optional <T> &   Optional <T>::operator = (Optional && rhs)
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
Optional <T> &   Optional <T>::operator = (const U & rhs)
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
Optional <T> &   Optional <T>::operator = (std::unique_ptr <U> && rhs)
{
   // cannot change model while observing
   if (!Collection <T>::can_change ()) flip_FATAL;

   Collection <T>::clear ();
   Collection <T>::insert (std::forward <std::unique_ptr <U>> (rhs));

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : operator bool
==============================================================================
*/

template <class T>
Optional <T>::operator bool () const
{
   return ! empty ();
}



/*
==============================================================================
Name : empty
==============================================================================
*/

template <class T>
bool  Optional <T>::empty () const
{
   auto it = Collection <T>::begin ();
   const auto it_end = Collection <T>::end ();

   for (; it != it_end ; ++it)
   {
      if (it.added () || it.resident ()) return false;
   }

   return true;
}



/*
==============================================================================
Name : reset
==============================================================================
*/

template <class T>
template <class... Args>
typename std::enable_if <!is_mold <Args...>::value, T &
>::type  Optional <T>::reset (Args &&... args)
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
>::type  Optional <T>::reset (Args &&... args)
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
void  Optional <T>::reset (None)
{
   Collection <T>::clear ();
}



/*
==============================================================================
Name : reset
==============================================================================
*/

template <class T>
T &   Optional <T>::reset (const Mold & mold)
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
U &   Optional <T>::reset (const Mold & mold)
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
Optional <T>::operator T & () const
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
T &   Optional <T>::use () const
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
U &   Optional <T>::use () const
{
   return dynamic_cast <U &> (this->operator T & ());
}



/*
==============================================================================
Name : value
==============================================================================
*/

template <class T>
T &   Optional <T>::value () const
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
U &   Optional <T>::value () const
{
   return dynamic_cast <U &> (this->operator T & ());
}



/*
==============================================================================
Name : before
==============================================================================
*/

template <class T>
T &   Optional <T>::before () const
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
U &   Optional <T>::before () const
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
void  Optional <T>::impl_validate ()
{
   if (!is_valid ()) flip_VALIDATION_FAILED ("Optional constraint violation");
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
bool  Optional <T>::is_valid () const
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
   ok_flag |= (cnt_added == 0) && (cnt_removed == 0) && (cnt_resident == 0);
   ok_flag |= (cnt_added == 1) && (cnt_removed == 0) && (cnt_resident == 0);
   ok_flag |= (cnt_added == 0) && (cnt_removed == 1) && (cnt_resident == 0);
   ok_flag |= (cnt_added == 1) && (cnt_removed == 1) && (cnt_resident == 0);
   ok_flag |= (cnt_added == 0) && (cnt_removed == 0) && (cnt_resident == 1);

   return ok_flag;
}



}  // namespace flip



#endif   // flip_Optional_CODEHEADER_INCLUDED

#undef flip_Optional_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

