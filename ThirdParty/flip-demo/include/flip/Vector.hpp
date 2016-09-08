/*****************************************************************************

      Vector.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Vector_CURRENT_CODEHEADER)
   #error Recursive inclusion of Vector code header.
#endif
#define  flip_Vector_CURRENT_CODEHEADER

#if ! defined (flip_Vector_CODEHEADER_INCLUDED)
#define  flip_Vector_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/StreamBinIn.h"
#include "flip/StreamBinOut.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Vector <T>::Vector (const Vector & rhs)
:  Blob (rhs)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Vector <T>::Vector (Vector && rhs)
:  Blob (std::move (rhs))
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Vector <T>::Vector (const std::vector <T> & value)
{
   if (value.empty ()) return;   // abort, no change

   impl_internal_set (make_value (value));
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Vector <T> & Vector <T>::operator = (const Vector & rhs)
{
   this->Blob::operator = (rhs);

   _cached_value_need_update_flag = true;

   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Vector <T> & Vector <T>::operator = (Vector && rhs)
{
   this->Blob::operator = (std::move (rhs));

   _cached_value_need_update_flag = true;

   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Vector <T> & Vector <T>::operator = (const std::vector <T> & rhs)
{
   auto && value = make_value (rhs);

   impl_internal_set (value);

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Vector <T> & Vector <T>::operator = (std::vector <T> && rhs)
{
   auto && value = make_value (rhs);

   impl_internal_set (value);

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : operator const std::vector <T> &
==============================================================================
*/

template <class T>
Vector <T>::operator const std::vector <T> & () const
{
   return value ();
}



/*
==============================================================================
Name : value
==============================================================================
*/

template <class T>
const std::vector <T> & Vector <T>::value () const
{
   flip_NOTHING_CHECK_VALUE;

   update_value ();

   return _cached_value;
}



/*
==============================================================================
Name : before
==============================================================================
*/

template <class T>
const std::vector <T> & Vector <T>::before () const
{
   flip_NOTHING_CHECK_BEFORE;

   update_before ();

   return _cached_before_value;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

template <class T>
bool  Vector <T>::operator == (const std::vector <T> & rhs) const
{
   return this->Blob::operator == (make_value (rhs));
}  // COV_NF_LINE



/*
==============================================================================
Name : operator !=
==============================================================================
*/

template <class T>
bool  Vector <T>::operator != (const std::vector <T> & rhs) const
{
   return this->Blob::operator != (make_value (rhs));
}  // COV_NF_LINE



/*
==============================================================================
Name : begin
==============================================================================
*/

template <class T>
typename Vector <T>::const_iterator Vector <T>::begin () const
{
   update_value ();

   return _cached_value.cbegin ();
}



/*
==============================================================================
Name : cbegin
==============================================================================
*/

template <class T>
typename Vector <T>::const_iterator Vector <T>::cbegin () const
{
   return begin ();
}



/*
==============================================================================
Name : end
==============================================================================
*/

template <class T>
typename Vector <T>::const_iterator Vector <T>::end () const
{
   update_value ();

   return _cached_value.cend ();
}



/*
==============================================================================
Name : cend
==============================================================================
*/

template <class T>
typename Vector <T>::const_iterator Vector <T>::cend () const
{
   return end ();
}



/*
==============================================================================
Name : clear
==============================================================================
*/

template <class T>
void  Vector <T>::clear ()
{
   if (Blob::empty ()) return;   // abort

   Blob::impl_internal_set (std::vector <uint8_t> {});

   _cached_value.clear ();
   _cached_value_need_update_flag = false;
}



/*
==============================================================================
Name : replace
==============================================================================
*/

template <class T>
void  Vector <T>::replace (const_iterator it, const_iterator it_end, const std::vector <T> & value)
{
   replace (it, it_end, value.begin (), value.end ());
}



/*
==============================================================================
Name : replace
==============================================================================
*/

template <class T>
template <class InputIterator>
void  Vector <T>::replace (const_iterator it, const_iterator it_end, InputIterator it2, InputIterator it_end2)
{
   update_value ();

   auto pos = std::distance (_cached_value.cbegin (), it);

#if defined (__GLIBCXX__)
   // libstdc++ does not support C++11 insert/erase with const_iterator

   auto itt = _cached_value.begin () + pos;
   auto pos_end = std::distance (_cached_value.cbegin (), it_end);
   auto itt_end = _cached_value.begin () + pos_end;

   _cached_value.erase (itt, itt_end);
   _cached_value.insert (_cached_value.begin () + pos, it2, it_end2);

#else
   _cached_value.erase (it, it_end);
   _cached_value.insert (begin () + pos, it2, it_end2);

#endif

   impl_internal_set (make_value (_cached_value));
}



/*
==============================================================================
Name : revert
==============================================================================
*/

template <class T>
void  Vector <T>::revert ()
{
   Blob::revert ();

   _cached_value_need_update_flag = true;
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : impl_internal_set
Note :
   This function is called from the VM when executing a transaction, or
   when the client change the value of 'this'.
==============================================================================
*/

template <class T>
void  Vector <T>::impl_internal_set (const std::vector <uint8_t> & value)
{
   if (Blob::value () == value) return;   // no change, abort

   Blob::impl_internal_set (value);

   _cached_value_need_update_flag = true;
}



/*
==============================================================================
Name : impl_internal_set
==============================================================================
*/

template <class T>
void  Vector <T>::impl_internal_set (std::vector <uint8_t> && value)
{
   if (Blob::value () == value) return;   // no change, abort

   Blob::impl_internal_set (std::move (value));

   _cached_value_need_update_flag = true;
}



/*
==============================================================================
Name : impl_synchronize
==============================================================================
*/

template <class T>
void  Vector <T>::impl_synchronize ()
{
   Blob::impl_synchronize ();

   _cached_before_value_need_update_flag = true;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : make_value
==============================================================================
*/

template <class T>
std::vector <uint8_t>   Vector <T>::make_value (const std::vector <T> & elems)
{
   std::vector <uint8_t> data;

   StreamBinOut sbo (data);

   for (auto && elem : elems)
   {
      sbo << elem;
   }

   return data;
}



/*
==============================================================================
Name : make_vector
==============================================================================
*/

template <class T>
std::vector <T>   Vector <T>::make_vector (const std::vector <uint8_t> & value)
{
   std::vector <T> elems;

   StreamBinIn sbi (value);

   while (!sbi.is_eos ())
   {
      T elem;
      sbi >> elem;
      elems.insert (elems.end (), elem);
   }

   return elems;
}



/*
==============================================================================
Name : update_value
==============================================================================
*/

template <class T>
void   Vector <T>::update_value () const
{
   if (!_cached_value_need_update_flag) return; // abort

   _cached_value_need_update_flag = false;
   _cached_value = make_vector (this->Blob::value ());
}



/*
==============================================================================
Name : update_before
==============================================================================
*/

template <class T>
void   Vector <T>::update_before () const
{
   if (!_cached_before_value_need_update_flag) return; // abort

   _cached_before_value_need_update_flag = false;
   _cached_before_value = make_vector (this->Blob::before ());
}



/*
==============================================================================
Name : global operator ==
==============================================================================
*/

template <class T>
bool  operator == (const std::vector <T> & lhs, const Vector <T> & rhs)
{
   return rhs.operator == (lhs);
}



/*
==============================================================================
Name : global operator !=
==============================================================================
*/

template <class T>
bool  operator != (const std::vector <T> & lhs, const Vector <T> & rhs)
{
   return rhs.operator != (lhs);
}



}  // namespace flip



#endif   // flip_Vector_CODEHEADER_INCLUDED

#undef flip_Vector_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

