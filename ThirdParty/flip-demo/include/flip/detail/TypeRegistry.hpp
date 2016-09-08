/*****************************************************************************

      TypeRegistry.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_TypeRegistry_CURRENT_CODEHEADER)
   #error Recursive inclusion of TypeRegistry code header.
#endif
#define  flip_TypeRegistry_CURRENT_CODEHEADER

#if ! defined (flip_TypeRegistry_CODEHEADER_INCLUDED)
#define  flip_TypeRegistry_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/TypeRegistry.h"

#include <typeinfo>
#include <cassert>



namespace flip
{



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : registry_register_type
Note :
   Used to redirect all type registrations into one static member defined
   outside of the template and exported from a DLL.
==============================================================================
*/

flip_API void *   registry_register_type (const std::type_info & typeinfo, void * class_def);



/*
==============================================================================
Name : registry_lookup_type
Note :
   See 'registry_register_type'
==============================================================================
*/

flip_API void *   registry_lookup_type (const std::type_info & typeinfo);



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : registry_create_type_def
Note :
   See 'registry_register_type'
==============================================================================
*/

template <typename T>
inline T * registry_create_type_def ()
{
   void * type_def_ptr = registry_lookup_type (typeid (T));

   if (type_def_ptr == nullptr)
   {
      type_def_ptr = registry_register_type (typeid (T), new T);
      assert (type_def_ptr != nullptr);
   }

   return static_cast <T *> (type_def_ptr);
}



}  // namespace flip



#endif   // flip_TypeRegistry_CODEHEADER_INCLUDED

#undef flip_TypeRegistry_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

