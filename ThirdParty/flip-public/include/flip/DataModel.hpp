/*****************************************************************************

      Model.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Model_CURRENT_CODEHEADER)
   #error Recursive inclusion of Model code header.
#endif
#define  flip_Model_CURRENT_CODEHEADER

#if ! defined (flip_Model_CODEHEADER_INCLUDED)
#define  flip_Model_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"
#include "flip/detail/TypeRegistry.h"
#include "flip/detail/RandomDevice.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : use
==============================================================================
*/

template <class T>
const DataModelBase &   DataModel <T>::use ()
{
   auto & instance = use_mutable ();

   instance.impl_post_process ();

   return instance;
}



/*
==============================================================================
Name : declare
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <!std::is_enum <U>::value, Class <U> &>::type  DataModel <T>::declare ()
{
   auto & the_class = Class <U>::declare ();

   use_mutable ().impl_add_class (the_class);

   return the_class;
}



/*
==============================================================================
Name : declare
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <std::is_enum <U>::value, EnumClass <U> &>::type  DataModel <T>::declare ()
{
   auto & the_enum = EnumClass <U>::declare ();

   use_mutable ().impl_add_enum (the_enum);

   return the_enum;
}



/*
==============================================================================
Name : add
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <!std::is_enum <U>::value, void>::type  DataModel <T>::add ()
{
   auto & the_class = Class <U>::use ();

   use_mutable ().impl_add_class (the_class);
}



/*
==============================================================================
Name : add
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <std::is_enum <U>::value, void>::type DataModel <T>::add ()
{
   auto & the_enum = EnumClass <U>::use ();

   use_mutable ().impl_add_enum (the_enum);
}



/*
==============================================================================
Name : version
==============================================================================
*/

template <class T>
void  DataModel <T>::version (const std::string & version)
{
   // force instantiation to avoid concurrency problem
   RandomDevice::use ();

   use_mutable ().impl_set_version (version);
}



/*
==============================================================================
Name : subset
==============================================================================
*/

template <class T>
template <class U>
void  DataModel <T>::set_root ()
{
   use_mutable ().impl_set_root (Class <U>::use ());
}



/*
==============================================================================
Name : subset
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <!std::is_enum <U>::value, bool>::type  DataModel <T>::has ()
{
   return use_mutable ().impl_has_class (Class <U>::use ());
}



/*
==============================================================================
Name : subset
==============================================================================
*/

template <class T>
template <class U>
typename std::enable_if <std::is_enum <U>::value, bool>::type  DataModel <T>::has ()
{
   return use_mutable ().impl_has_enum (EnumClass <U>::use ());
}



/*
==============================================================================
Name : subset
==============================================================================
*/

template <class T>
template <class Model>
void  DataModel <T>::clone_from ()
{
   use_mutable ().impl_clone_from (Model::use ());
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : use_mutable
==============================================================================
*/

template <class T>
DataModelBase &   DataModel <T>::use_mutable ()
{
#if (flip_TYPE_REGISTRY_MODE == flip_TYPE_REGISTRY_ENABLED)
   static DataModel <T> * instance_ptr = registry_create_type_def <DataModel <T>> ();

   return *instance_ptr;
#elif (flip_TYPE_REGISTRY_MODE == flip_TYPE_REGISTRY_DISABLED)
   flip_STATIC_LOCAL(DataModel <T>, instance, ());

   return instance;
#else
   #error Unsupported type registry mode
#endif
}  // COV_NF_LINE



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_Model_CODEHEADER_INCLUDED

#undef flip_Model_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

