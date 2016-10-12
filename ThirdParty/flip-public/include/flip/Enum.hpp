/*****************************************************************************

      Enum.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Enum_CURRENT_CODEHEADER)
   #error Recursive inclusion of Enum code header.
#endif
#define  flip_Enum_CURRENT_CODEHEADER

#if ! defined (flip_Enum_CODEHEADER_INCLUDED)
#define  flip_Enum_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Class.h"
#include "flip/EnumClass.h"
#include "flip/detail/fnc.h"

#include <type_traits>



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Enum <T>::Enum (const Enum & rhs)
:  EnumBase (rhs)
{
   static_assert (std::is_enum <T>::value, "");

   this->operator = (rhs);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Enum <T>::Enum (Enum && rhs)
:  EnumBase (std::move (rhs))
{
   static_assert (std::is_enum <T>::value, "");

   this->operator = (std::move (rhs));
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
Enum <T>::Enum (T value)
{
   static_assert (std::is_enum <T>::value, "");

   if (static_cast <int64_t> (value) == 0LL) return;  // abort, no change

   impl_internal_set (static_cast <int64_t> (value));
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Enum <T> & Enum <T>::operator = (const Enum & rhs)
{
   return this->operator = (static_cast <T> (rhs._value));
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Enum <T> & Enum <T>::operator = (Enum && rhs)
{
   this->BasicType::operator = (std::move (rhs));

   return this->operator = (static_cast <T> (rhs._value));
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
Enum <T> & Enum <T>::operator = (T value)
{
   // cannot change model while observing
   if (!can_change ()) flip_FATAL;

   if (_value == static_cast <int64_t> (value)) return *this;  // abort, no change

   impl_internal_set (static_cast <int64_t> (value));

   return *this;
}



/*
==============================================================================
Name : get_class
==============================================================================
*/

template <class T>
const ClassBase & Enum <T>::get_class () const
{
   return Class <EnumBase>::get ();
}



/*
==============================================================================
Name : changed
Description :
   See base class 'Type' description.
==============================================================================
*/

template <class T>
bool  Enum <T>::changed () const
{
   // ignore restore change

   return (_value != _value_before) || added ();
}



/*
==============================================================================
Name : operator T
==============================================================================
*/

template <class T>
Enum <T>::operator T () const
{
   return static_cast <T> (_value);
}



/*
==============================================================================
Name : value
==============================================================================
*/

template <class T>
T  Enum <T>::value () const
{
   return static_cast <T> (_value);
}



/*
==============================================================================
Name : before
==============================================================================
*/

template <class T>
T  Enum <T>::before () const
{
   return static_cast <T> (_value_before);
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

template <class T>
bool  Enum <T>::operator == (T rhs) const
{
   return _value == static_cast <int64_t> (rhs);
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

template <class T>
bool  Enum <T>::operator != (T rhs) const
{
   return _value != static_cast <int64_t> (rhs);
}



/*
==============================================================================
Name : revert
==============================================================================
*/

template <class T>
void  Enum <T>::revert ()
{
   if (_value == _value_before) return;   // abort

   _value = _value_before;
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : impl_make
==============================================================================
*/

template <class T>
void  Enum <T>::impl_make (Transaction & tx, ImplUndoRedoMode parent_mode, TxPostProcessInfo & /* ppinfo */) const
{
   if (_value == _value_before) return;   // abort

   auto mode = impl_make_undo_redo_mode (parent_mode);

   tx.push_enum_set (
      ref (), impl_make_tx_flags (mode),
      static_cast <int64_t> (_value_before),
      static_cast <int64_t> (_value)
   );
}



/*
==============================================================================
Name : impl_internal_set
Note :
   This function is called from the VM when executing a transaction, or
   when the client change the value of 'this'.
==============================================================================
*/

template <class T>
void  Enum <T>::impl_internal_set (int64_t value)
{
   modify (*this, _value_before, _value, value);
}



/*
==============================================================================
Name : impl_set_class
==============================================================================
*/

template <class T>
void  Enum <T>::impl_set_class (const ClassBase & class_base)
{
#if defined (NDEBUG)
   flip_UNUSED (class_base);
#else
   assert (&class_base == &get_class ());
#endif
}



/*
==============================================================================
Name : impl_is_reset
==============================================================================
*/

template <class T>
bool  Enum <T>::impl_is_reset () const
{
   return _value == 0LL;
}



/*
==============================================================================
Name : impl_reset
==============================================================================
*/

template <class T>
void  Enum <T>::impl_reset ()
{
   if (_value == 0LL) return; // abort

   impl_internal_set (0LL);
}



/*
==============================================================================
Name : impl_synchronize
==============================================================================
*/

template <class T>
void  Enum <T>::impl_synchronize ()
{
   Type::impl_synchronize ();

   _value_before = _value;
}



/*
==============================================================================
Name : impl_synchronize
==============================================================================
*/

template <class T>
const EnumClassBase &  Enum <T>::impl_enum_class () const
{
   return EnumClass <T>::get ();
}



/*
==============================================================================
Name : impl_synchronize
==============================================================================
*/

template <class T>
int64_t  Enum <T>::get () const
{
   return _value;
}



/*
==============================================================================
Name : impl_synchronize
==============================================================================
*/

template <class T>
int64_t  Enum <T>::get_before () const
{
   return _value_before;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : global operator ==
==============================================================================
*/

template <class T>
bool  operator == (T lhs, const Enum <T> & rhs)
{
   return rhs.operator == (lhs);
}



/*
==============================================================================
Name : global operator !=
==============================================================================
*/

template <class T>
bool  operator != (T lhs, const Enum <T> & rhs)
{
   return rhs.operator != (lhs);
}



}  // namespace flip



#endif   // flip_Enum_CODEHEADER_INCLUDED

#undef flip_Enum_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

