/*****************************************************************************

      Entity.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Entity_CURRENT_CODEHEADER)
   #error Recursive inclusion of Entity code header.
#endif
#define  flip_Entity_CURRENT_CODEHEADER

#if ! defined (flip_Entity_CODEHEADER_INCLUDED)
#define  flip_Entity_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"

#include <stdexcept>
#include <type_traits>



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : emplace
==============================================================================
*/

template <class T, class... Args>
T &   Entity::emplace (Args &&... args)
{
   static_assert (std::is_destructible <T>::value, "T must support destructor");

#if (flip_ENTITY_USE == flip_ENTITY_USE_PEDANTIC)
   check_pedantic ();
#endif

   if (_map.find (std::type_index (typeid (T))) != _map.end ()) flip_FATAL;

   T * ptr = new T (std::forward <Args> (args)...);

   _map.emplace (std::make_pair (std::type_index (typeid (T)),
      Bundle (
         ptr,
         &delete_func <T>,
         &copy_ctor_func <T>
      )
   ));

   return *ptr;
}  // COV_NF_LINE



/*
==============================================================================
Name : erase
==============================================================================
*/

template <class T>
void  Entity::erase ()
{
#if (flip_ENTITY_USE == flip_ENTITY_USE_PEDANTIC)
   check_pedantic ();
#endif

   if (_map.find (std::type_index (typeid (T))) == _map.end ()) flip_FATAL;

   _map.erase (std::type_index (typeid (T)));
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class T>
bool  Entity::has () const
{
   auto it = _map.find (std::type_index (typeid (T)));

   return it != _map.end ();
}



/*
==============================================================================
Name : use
==============================================================================
*/

template <class T>
T &   Entity::use ()
{
   auto t_ptr = get <T> ();

   if (t_ptr == nullptr) flip_FATAL;

   return reinterpret_cast <T &> (*t_ptr);
}  // COV_NF_LINE



/*
==============================================================================
Name : use
==============================================================================
*/

template <class T>
const T &   Entity::use () const
{
   auto t_ptr = get <T> ();

   if (t_ptr == nullptr) flip_FATAL;

   return reinterpret_cast <const T &> (*t_ptr);
}  // COV_NF_LINE



/*
==============================================================================
Name : get
==============================================================================
*/ // COV_NF_LINE

template <class T>
T *   Entity::get ()
{
   auto it = _map.find (std::type_index (typeid (T)));

   if (it == _map.end ()) return nullptr;

   return reinterpret_cast <T *> (it->second._ptr);
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class T>
const T *   Entity::get () const
{
   auto it = _map.find (std::type_index (typeid (T)));

   if (it == _map.end ()) return nullptr;

   return reinterpret_cast <T *> (it->second._ptr);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : delete_func
==============================================================================
*/

template <class T>
void  Entity::delete_func (void * ptr)
{
   delete reinterpret_cast <T *> (ptr);
}



/*
==============================================================================
Name : copy_ctor_func
==============================================================================
*/

template <class T>
typename std::enable_if <
   std::is_copy_constructible <T>::value, void *
>::type  Entity::copy_ctor_func (void * ptr)
{
   return new T (*reinterpret_cast <T *> (ptr));
}  // COV_NF_LINE



/*
==============================================================================
Name : copy_ctor_func
==============================================================================
*/

template <class T>
typename std::enable_if <
   !std::is_copy_constructible <T>::value, void *
>::type  Entity::copy_ctor_func (void * /* ptr */)
{
   flip_FATAL; // COV_NF_LINE
}  // COV_NF_LINE



}  // namespace flip



#endif   // flip_Entity_CODEHEADER_INCLUDED

#undef flip_Entity_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

