/*****************************************************************************

      TypeTraits.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"
#include "flip/detail/Default.h"

#include <type_traits>



namespace flip
{



class ArrayBase;
class Blob;
class CollectionBase;
class EnumBase;
class MessageBase;
class Object;
class ObjectRefBase;

template <typename T>
struct is_enum
{
   enum { value = std::is_base_of <EnumBase, T>::value };
};



template <typename T>
struct is_blob
{
   enum { value = std::is_base_of <Blob, T>::value };
};



template <typename T>
struct is_object_ref
{
   enum { value = std::is_base_of <ObjectRefBase, T>::value };
};



template <typename T>
struct is_collection
{
   enum { value = std::is_base_of <CollectionBase, T>::value };
};



template <typename T>
struct is_array
{
   enum { value = std::is_base_of <ArrayBase, T>::value };
};



template <typename T>
struct is_message
{
   enum { value = std::is_base_of <MessageBase, T>::value };
};



template <typename T>
struct is_object
{
   enum { value = std::is_base_of <Object, T>::value };
};



class StreamBinOut;
class StreamBinIn;

template <typename T>
struct has_sbio
{
   template <typename U, void (U::*) (StreamBinOut &) const, void (U::*) (StreamBinIn &)> struct sfinae {};

   template <typename U> static char test (sfinae <U, &U::write, &U::read> *);
   template <typename U> static long test (...);

   enum { value = (sizeof test <T> (0) == 1) };
};



class Mold;

template <typename... Args> struct is_mold;

template <>
struct is_mold <>
{
   enum { value = false };
};

template <class T>
struct is_mold <T>
{
   enum { value
      = std::is_same <T, Mold>::value
      || std::is_same <T, Mold &>::value
      || std::is_same <T, const Mold &>::value
   };
};

template <typename T, typename... Args>
struct is_mold <T, Args...>
{
   enum { value = false };
};



template <typename T>
struct is_flip_constructible
{
   enum {
      value
         = std::is_default_constructible <T>::value
         || std::is_constructible <T, Default &>::value
   };
};



// MSVC 12.0 (Visual 2013) Support
#if (flip_COMPILER == flip_COMPILER_MSVC) && (_MSC_VER == 1800)

/*
__is_constructible intrinsic is buggy as it will return 'true' for
a declared constructor that is deleted.

the same kind of bug also occurs for decltype, so we cannot make an
Expression SFINAE. For example the following technic :
template <typename U>
static yes_type test (U&, decltype(U(std::declval <const U &>())) * = 0);

We end up in using std::is_copy_constructible and using other type traits
to find out if the class is actually copy construtible.
*/

template <typename T>
struct is_copy_constructible
{
   enum {
      value
         = std::is_copy_constructible <T>::value
         && !std::is_abstract <T>::value
   };
};

#else

template <typename T>
struct is_copy_constructible : std::is_copy_constructible <T> {};

#endif



}  // namespace flip



//#include  "flip/TypeTraits.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

