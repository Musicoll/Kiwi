/*****************************************************************************

      ObjectRef.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_ObjectRef_CURRENT_CODEHEADER)
   #error Recursive inclusion of ObjectRef code header.
#endif
#define  flip_ObjectRef_CURRENT_CODEHEADER

#if ! defined (flip_ObjectRef_CODEHEADER_INCLUDED)
#define  flip_ObjectRef_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Class.h"
#include "flip/Transaction.h"
#include "flip/detail/def.h"
#include "flip/detail/fnc.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
ObjectRef <T>::ObjectRef (const ObjectRef & rhs)
:  ObjectRefBase (rhs)
{
   this->operator = (rhs);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
ObjectRef <T>::ObjectRef (ObjectRef && rhs)
:  ObjectRefBase (std::move (rhs))
{
   this->operator = (std::move (rhs));
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
ObjectRef <T>::ObjectRef (T * ptr)
{
   if (ptr == nullptr) return;   // abort, no change

   impl_internal_set (make_value (ptr));
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
ObjectRef <T>::ObjectRef (const Ref & ref)
{
   if (ref == Ref::null) return; // abort, no change

   impl_internal_set (ref);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
ObjectRef <T> &   ObjectRef <T>::operator = (const ObjectRef & rhs)
{
   return this->operator = (rhs._value);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
ObjectRef <T> &   ObjectRef <T>::operator = (ObjectRef && rhs)
{
   this->ObjectRefBase::operator = (std::move (rhs));

   return this->operator = (rhs._value);
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
ObjectRef <T> &   ObjectRef <T>::operator = (T * ptr)
{
   // cannot change model while observing
   if (!can_change ()) flip_FATAL;

   Ref value = make_value (ptr);

   if (_value == value) return *this;  // abort, no change

   impl_internal_set (value);

   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T>
ObjectRef <T> &   ObjectRef <T>::operator = (const Ref & value)
{
   // cannot change model while observing
   if (!can_change ()) flip_FATAL;

   if (_value == value) return *this;  // abort, no change

   impl_internal_set (value);

   return *this;
}



/*
==============================================================================
Name : get_class
==============================================================================
*/

template <class T>
const ClassBase & ObjectRef <T>::get_class () const
{
   return Class <ObjectRefBase>::get ();
}



/*
==============================================================================
Name : changed
Description :
   See base class 'Type' description.
==============================================================================
*/

template <class T>
bool  ObjectRef <T>::changed () const
{
   // ignore restore change

   return (_value != _value_before) || added ();
}



/*
==============================================================================
Name : operator int64_t
==============================================================================
*/

template <class T>
ObjectRef <T>::operator T * () const
{
   flip_NOTHING_CHECK_VALUE;

   return document ().template object_ptr <T> (_value);
}



/*
==============================================================================
Name : value
==============================================================================
*/

template <class T>
T *   ObjectRef <T>::value () const
{
   flip_NOTHING_CHECK_VALUE;

   return document ().template object_ptr <T> (_value);
}



/*
==============================================================================
Name : before
==============================================================================
*/

template <class T>
T *   ObjectRef <T>::before () const
{
   flip_NOTHING_CHECK_BEFORE;

   return document ().template object_ptr <T> (_value_before);
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class T>
Ref   ObjectRef <T>::get () const
{
   flip_NOTHING_CHECK_VALUE;

   return _value;
}



/*
==============================================================================
Name : get_before
==============================================================================
*/

template <class T>
Ref   ObjectRef <T>::get_before () const
{
   flip_NOTHING_CHECK_BEFORE;

   return _value_before;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

template <class T>
bool  ObjectRef <T>::operator == (T * rhs) const
{
   flip_NOTHING_CHECK_VALUE;

   auto rhs_ref = make_value (rhs);

   if (rhs_ref == Ref::null)
   {
      return this->operator T * () == rhs;
   }
   else
   {
      return _value == make_value (rhs);
   }
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

template <class T>
bool  ObjectRef <T>::operator != (T * rhs) const
{
   flip_NOTHING_CHECK_VALUE;

   auto rhs_ref = make_value (rhs);

   if (rhs_ref == Ref::null)
   {
      return this->operator T * () != rhs;
   }
   else
   {
      return _value != make_value (rhs);
   }
}



/*
==============================================================================
Name : revert
==============================================================================
*/

template <class T>
void  ObjectRef <T>::revert ()
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
void  ObjectRef <T>::impl_make (Transaction & tx, ImplUndoRedoMode parent_mode, TxPostProcessInfo & /* ppinfo */) const
{
   if (_value == _value_before) return;   // abort

   auto mode = impl_make_undo_redo_mode (parent_mode);

   tx.push_object_ref_set (ref (), impl_make_tx_flags (mode), _value_before, _value);
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
void  ObjectRef <T>::impl_internal_set (const Ref & value)
{
   modify (*this, _value_before, _value, value);
}



/*
==============================================================================
Name : impl_set_class
==============================================================================
*/

template <class T>
void  ObjectRef <T>::impl_set_class (const ClassBase & class_base)
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
bool  ObjectRef <T>::impl_is_reset () const
{
   return _value == Ref::null;
}



/*
==============================================================================
Name : impl_reset
==============================================================================
*/

template <class T>
void  ObjectRef <T>::impl_reset ()
{
   if (_value == Ref::null) return; // abort

   impl_internal_set (Ref::null);
}



/*
==============================================================================
Name : impl_synchronize
==============================================================================
*/

template <class T>
void  ObjectRef <T>::impl_synchronize ()
{
   Type::impl_synchronize ();

   _value_before = _value;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : make_value
==============================================================================
*/

template <class T>
Ref   ObjectRef <T>::make_value (T * ptr) const
{
   if (ptr == nullptr)
   {
      return Ref::null;
   }
   else
   {
      Ref ref = ptr->ref ();

      if (ref == Ref::null) flip_FATAL;

      return ref;
   }
}



/*
==============================================================================
Name : global operator ==
==============================================================================
*/

template <class T>
bool  operator == (T * lhs, const ObjectRef <T> & rhs)
{
   return rhs.operator == (lhs);
}



/*
==============================================================================
Name : global operator !=
==============================================================================
*/

template <class T>
bool  operator != (T * lhs, const ObjectRef <T> & rhs)
{
   return rhs.operator != (lhs);
}



}  // namespace flip



#endif   // flip_ObjectRef_CODEHEADER_INCLUDED

#undef flip_ObjectRef_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

