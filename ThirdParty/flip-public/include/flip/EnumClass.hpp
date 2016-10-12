/*****************************************************************************

      EnumClass.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_EnumClass_CURRENT_CODEHEADER)
   #error Recursive inclusion of EnumClass code header.
#endif
#define  flip_EnumClass_CURRENT_CODEHEADER

#if ! defined (flip_EnumClass_CODEHEADER_INCLUDED)
#define  flip_EnumClass_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/BasicType.h"

#include "flip/Transaction.h"
#include "flip/detail/def.h"
#include "flip/detail/ArrayBase.h"
#include "flip/detail/CollectionBase.h"
#include "flip/detail/Default.h"
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
EnumClass <T> & EnumClass <T>::declare ()
{
   BasicType::declare ();

   auto & instance = use ();

   // A declaration of 'T' already exists
   if (instance._name_0 != nullptr) flip_FATAL;

   return instance;
}  // COV_NF_LINE



/*
==============================================================================
Name : name
==============================================================================
*/

template <class T>
EnumClass <T> & EnumClass <T>::name (const char * name_0)
{
   if (name_0 == nullptr) flip_FATAL;
   if (!is_name_valid (name_0)) flip_FATAL;

   _name_0 = name_0;

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : member
==============================================================================
*/

template <class T>
template <T val>
EnumClass <T> & EnumClass <T>::enumerator (const char * name_0)
{
   // enum is anonymous
   if (_name_0 == nullptr) flip_FATAL;
   if (!is_name_valid (name_0)) flip_FATAL;

   Enumerator enumerator;
   enumerator._name_0 = name_0;
   enumerator._value = static_cast <int64_t> (val);

   _enumerators.emplace_back (enumerator);

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : use
==============================================================================
*/

template <class T>
EnumClass <T> & EnumClass <T>::use ()
{
#if (flip_TYPE_REGISTRY_MODE == flip_TYPE_REGISTRY_ENABLED)
   static EnumClass <T> * instance_ptr = registry_create_type_def <EnumClass <T>> ();

   return *instance_ptr;
#elif (flip_TYPE_REGISTRY_MODE == flip_TYPE_REGISTRY_DISABLED)
   flip_STATIC_LOCAL(EnumClass <T>, instance, ());

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
const EnumClass <T> &   EnumClass <T>::get ()
{
   const auto & enum_class = use ();

#if ! defined (NDEBUG)
   // EnumClass <T> is not declared
   if (enum_class.name () == nullptr) flip_FATAL;
#endif

   return enum_class;
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_EnumClass_CODEHEADER_INCLUDED

#undef flip_EnumClass_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
