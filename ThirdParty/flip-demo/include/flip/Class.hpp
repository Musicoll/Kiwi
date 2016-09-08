/*****************************************************************************

      Class.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Class_CURRENT_CODEHEADER)
   #error Recursive inclusion of Class code header.
#endif
#define  flip_Class_CURRENT_CODEHEADER

#if ! defined (flip_Class_CODEHEADER_INCLUDED)
#define  flip_Class_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/BasicType.h"

#include "flip/EnumClass.h"
#include "flip/Transaction.h"
#include "flip/detail/def.h"
#include "flip/detail/ArrayBase.h"
#include "flip/detail/ClassRegistry.h"
#include "flip/detail/CollectionBase.h"
#include "flip/detail/Default.h"
#include "flip/detail/EnumBase.h"
#include "flip/detail/MessageBase.h"
#include "flip/detail/ObjectRefBase.h"
#include "flip/detail/TypeId.h"
#include "flip/detail/TypeRegistry.h"

#include <cassert>


namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : declare
==============================================================================
*/

template <class T>
Class <T> & Class <T>::declare ()
{
   BasicType::declare ();

   auto & instance = use ();

   // A declaration of 'T' already exists
   if (instance._name_0 != nullptr) flip_FATAL;

   ClassRegistry::use ().register_ (typeid (T), instance);

   return instance;
}  // COV_NF_LINE



/*
==============================================================================
Name : name
==============================================================================
*/

template <class T>
Class <T> & Class <T>::name (const char * name_0)
{
   if (name_0 == nullptr) flip_FATAL;
   if (!is_name_valid (name_0)) flip_FATAL;

   _name_0 = name_0;

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : inherit
==============================================================================
*/

template <class T>
template <class U>
Class <T> & Class <T>::inherit ()
{
   // class is anonymous
   if (_name_0 == nullptr) flip_FATAL;

   // inheritance must be declared before members
   if (!_members.empty ()) flip_FATAL;

   auto & super = Class <U>::use ();

   // 'U' is not declared
   if (super.name () == nullptr) flip_FATAL;

   // multiple inheritance is not supported
   if (_super_class_ptr != nullptr) flip_FATAL;
   _super_class_ptr = &super;

   _members.insert (
      _members.end (),
      super.members ().begin (), super.members ().end ()
   );

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : member
==============================================================================
*/

template <class T>
template <class U, U T::*ptr_to_member>
Class <T> & Class <T>::member (const char * name_0)
{
   // class is anonymous
   if (_name_0 == nullptr) flip_FATAL;
   if (!is_name_valid (name_0)) flip_FATAL;

   auto & type = base <U> ();

   // 'U' is not declared
   if (type.name () == nullptr) flip_FATAL;

   Member member;
   member._base_ptr = &type;
   member._name_0 = name_0;
   member._representative.set <U, T, ptr_to_member> ();

   _members.emplace_back (member);

   return *this;
}



/*
==============================================================================
Name : use
==============================================================================
*/

template <class T>
Class <T> & Class <T>::use ()
{
#if (flip_TYPE_REGISTRY_MODE == flip_TYPE_REGISTRY_ENABLED)
   static Class <T> * instance_ptr = registry_create_type_def <Class <T>> ();

   return *instance_ptr;
#elif (flip_TYPE_REGISTRY_MODE == flip_TYPE_REGISTRY_DISABLED)
   flip_STATIC_LOCAL(Class <T>, instance, ());

   return instance;
#else
   #error Unsupported type registry mode
#endif
}  // COV_NF_LINE



/*
==============================================================================
Name : get
==============================================================================
*/

template <class T>
const Class <T> & Class <T>::get ()
{
   const auto & class_ = use ();

#if ! defined (NDEBUG)
   // Class <T> is not declared
   if (class_.name () == nullptr) flip_FATAL;
#endif

   return class_;
}  // COV_NF_LINE



/*
==============================================================================
Name : use
==============================================================================
*/

template <class T>
bool  Class <T>::abstract () const
{
   return std::is_abstract <T>::value;
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : impl_create
Note :
   Used for root creation, and creation from transactions.
==============================================================================
*/

template <class T>
Type *   Class <T>::impl_create (DocumentBase & document, Ref ref) const
{
   // T must support default or (Default &) constructor
   T * obj_ptr = ctor <T> ();

   // T must inherit from Object
   obj_ptr->impl_set_class (*this);
   obj_ptr->impl_bind (document, ref);

#if ! defined (NDEBUG)
   Transaction tx;
   obj_ptr->make (tx);

   // default constructor contains flip object modification
   // use (Default &) constructor in 'T' declaration
   if (!tx.empty ())
   {
      delete obj_ptr;
      obj_ptr = nullptr;

      flip_FATAL;
   }
#endif

   return obj_ptr;
}  // COV_NF_LINE



/*
==============================================================================
Name : impl_release
==============================================================================
*/

template <class T>
void  Class <T>::impl_release (Type * obj_ptr, DocumentBase & document) const
{
   obj_ptr->impl_unbind (document);

   obj_ptr->impl_entity_reset ();

   delete obj_ptr;
   obj_ptr = 0;
}



/*
==============================================================================
Name : impl_ctor
Note :
   Used indirectly from molds.
==============================================================================
*/

template <class T>
Type *   Class <T>::impl_ctor () const
{
   // T must support default or (Default &) constructor
   T * ptr = ctor <T> ();

   ptr->impl_set_class (*this);

   return ptr;
}  // COV_NF_LINE



/*
==============================================================================
Name : impl_copy_ctor
==============================================================================
*/

template <class T>
Type *   Class <T>::impl_copy_ctor (Type & obj) const
{
   return copy_ctor <T> (obj);
}  // COV_NF_LINE



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : base
Description :
   Return the base type when available.
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <
   is_enum <U>::value, ClassBase &
>::type  Class <T>::base ()
{
   // Enumeration is used but was not previously declared
   if (EnumClass <typename U::value_type>::get ().name () == nullptr) flip_FATAL;

   return Class <EnumBase>::use ();
}  // COV_NF_LINE



/*
==============================================================================
Name : base
Description :
   Return the base type when available.
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <
   is_blob <U>::value, ClassBase &
>::type  Class <T>::base ()
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
template <class U>
typename std::enable_if <
   is_object_ref <U>::value, ClassBase &
>::type  Class <T>::base ()
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
template <class U>
typename std::enable_if <
   is_collection <U>::value, ClassBase &
>::type  Class <T>::base ()
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
template <class U>
typename std::enable_if <
   is_array <U>::value, ClassBase &
>::type  Class <T>::base ()
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
template <class U>
typename std::enable_if <
   is_message <U>::value, ClassBase &
>::type  Class <T>::base ()
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
template <class U>
typename std::enable_if <
   ! (is_enum <U>::value || is_blob <U>::value || is_object_ref <U>::value || is_collection <U>::value || is_array <U>::value || is_message <U>::value), ClassBase &
>::type  Class <T>::base ()
{
   return Class <U>::use ();
}



/*
==============================================================================
Name : copy_ctor
Note :
   See definition of 'is_copy_constructible' in flip/detail/TypeTraits.h
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <
   is_copy_constructible <U>::value, Type *
>::type  Class <T>::copy_ctor (Type & obj) const
{
   U * cast_ptr = &dynamic_cast <U &> (obj);

   // T must support copy constructor
   U * ptr = new U (*cast_ptr);

   ptr->impl_set_class (*this);

   return ptr;
}  // COV_NF_LINE



/*
==============================================================================
Name : copy_ctor
Note :
   See definition of 'is_copy_constructible' in flip/detail/TypeTraits.h
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <
   ! is_copy_constructible <U>::value, Type *
>::type  Class <T>::copy_ctor (Type & /* obj */) const
{
   flip_FATAL; // COV_NF_LINE
}  // COV_NF_LINE



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <
   std::is_constructible <U, Default &>::value, U *
>::type  Class <T>::ctor () const
{
   Default default_;

   return new U (default_);
}  // COV_NF_LINE



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <
   (! std::is_constructible <U, Default &>::value) && (! std::is_abstract <U>::value), U *
>::type  Class <T>::ctor () const
{
   return new U;
}  // COV_NF_LINE



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <
   std::is_abstract <U>::value, U *
>::type  Class <T>::ctor () const
{
   flip_FATAL; // COV_NF_LINE
}  // COV_NF_LINE



}  // namespace flip



#endif   // flip_Class_CODEHEADER_INCLUDED

#undef flip_Class_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
