/*****************************************************************************

      Signal.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Signal_CURRENT_CODEHEADER)
   #error Recursive inclusion of Signal code header.
#endif
#define  flip_Signal_CURRENT_CODEHEADER

#if ! defined (flip_Signal_CODEHEADER_INCLUDED)
#define  flip_Signal_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"
#include "flip/detail/DocumentBase.h"
#include "flip/detail/SignalData.h"

#include <iostream>
#include <type_traits>

#include <climits>
#include <cassert>



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class... Args>
Signal <Args...>::Signal (uint32_t type, Object & object)
: _type (type)
, _object (object)
{
   _object.impl_bind (*this, type);
}



/*
==============================================================================
Name : ctor
Note :
   Copy ctor cannot be used. Instead the client must redefined the copy
   ctor of the object containing this, and call ctor above.
==============================================================================
*/

#if (flip_COMPILER == flip_COMPILER_MSVC) && (_MSC_VER == 1800)
template <class... Args>
Signal <Args...>::Signal (const Signal & other)
: _type (other._type)
, _object (other._object)
{
   assert (false);
}
#endif


/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class... Args>
Signal <Args...>::~Signal ()
{
   assert (_bundle_map.empty ());

   _object.impl_unbind (*this, _type);
}



/*
==============================================================================
Name : type
==============================================================================
*/

template <class... Args>
uint32_t Signal <Args...>::type () const
{
   return _type;
}



/*
==============================================================================
Name : object
==============================================================================
*/

template <class... Args>
Object & Signal <Args...>::object ()
{
   return _object;
}



/*
==============================================================================
Name : operator ()
==============================================================================
*/

template <class... Args>
void  Signal <Args...>::operator () (Args... args)
{
   _object.document ().signal (make (args...));
}



/*
==============================================================================
Name : make
==============================================================================
*/

template <class... Args>
SignalData  Signal <Args...>::make (Args... args)
{
   std::vector <uint8_t> data;
   StreamBinOut sbo (data);

   pack (sbo, args...);

   return SignalData (
      _object.get_class ().type_id (_object.document ().data_model ()),
      _object.ref (), _type, sbo.data ()
   );
}



/*
==============================================================================
Name : connect
Description :
   Member function version
==============================================================================
*/

template <class... Args>
template <class T, class F>
SignalConnection  Signal <Args...>::connect (T & t, F f)
{
   static_assert (sizeof (Buf) == sizeof (std::function <void (T &, Args...)>), "size mismatch");
   flip_ASSERT_NON_NULLPTR (&t);

   ConnectionKey key = ++_key;

   if (_bundle_map.find (key) != _bundle_map.end ()) flip_FATAL;

   Bundle & bundle = _bundle_map [key];

   bundle._call_proc = &Signal::fun_call <T, F>;
   bundle._target_ptr = &t;
   bundle._func_dtor_proc = &Signal::fun_function_dtor <T, F>;

   // verify function 'F' prototype
   new (bundle._func_buf) std::function <void (T &, Args...)> (std::mem_fn (f));

   return make_connection (key);
}  // COV_NF_LINE



/*
==============================================================================
Name : connect
Description :
   Lambda version
==============================================================================
*/

template <class... Args>
template <class F>
SignalConnection  Signal <Args...>::connect (F f)
{
   static_assert (sizeof (Buf) == sizeof (std::function <void (Args...)>), "size mismatch");

   ConnectionKey key = ++_key;

   if (_bundle_map.find (key) != _bundle_map.end ()) flip_FATAL;

   Bundle & bundle = _bundle_map [key];

   bundle._call_proc = &Signal::lambda_call <void, F>;
   bundle._target_ptr = nullptr;
   bundle._func_dtor_proc = &Signal::lambda_function_dtor <void, F>;

   // verify function 'F' prototype
   new (bundle._func_buf) std::function <void (Args...)> (f);

   return make_connection (key);
}  // COV_NF_LINE



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : impl_signal
Description :
   Received from DocumentBase, receive the signal, unpack, and call listeners.
==============================================================================
*/

template <class... Args>
void  Signal <Args...>::impl_signal (const std::vector <uint8_t> & data)
{
   Unpacker <Args..., void> unpacker;

   StreamBinIn sbi (data);

   unpacker.unpack (*this, sbi);
}



/*
==============================================================================
Name : impl_disconnect
==============================================================================
*/

template <class... Args>
void  Signal <Args...>::impl_disconnect (const ConnectionKey & key)
{
   auto it = _bundle_map.find (key);

   if (it == _bundle_map.end ()) flip_FATAL;

   _bundle_map.erase (it);
}



/*
==============================================================================
Name : impl_call
Description :
   When all args have been read, call the listeners.
==============================================================================
*/

template <class... Args>
void  Signal <Args...>::impl_call (Args... args)
{
   for (auto & pair : _bundle_map)
   {
      Bundle & bundle = pair.second;

      bundle._call_proc (bundle._target_ptr, bundle._func_buf, args...);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : pack_one
==============================================================================
*/

template <class... Args>
template <class U>
typename std::enable_if <
   has_sbio <U>::value
>::type  Signal <Args...>::pack_one (StreamBinOut & sbo, U val)
{
   sbo << val;
}



/*
==============================================================================
Name : pack_one
==============================================================================
*/

template <class... Args>
template <class U>
typename std::enable_if <
   !has_sbio <U>::value
>::type  Signal <Args...>::pack_one (StreamBinOut & sbo, U val)
{
   static_assert (std::is_pod <U>::value, "Type to complex");

   sbo.data ().resize (sbo.data ().size () + sizeof (U), 0);

   new (&sbo.data () [sbo.data ().size () - sizeof (U)]) U (val);
}



/*
==============================================================================
Name : pack_one
==============================================================================
*/

template <class... Args>
template <class U>
void  Signal <Args...>::pack_one (StreamBinOut & sbo, const std::vector <U> & val)
{
   sbo << uint64_t (val.size ());

   for (const auto & sub_val : val)
   {
      pack_one (sbo, sub_val);
   }
}



/*
==============================================================================
Name : pack_one
==============================================================================
*/

template <class... Args>
template <class U>
void  Signal <Args...>::pack_one (StreamBinOut & sbo, const std::list <U> & val)
{
   sbo << uint64_t (val.size ());

   for (const auto & sub_val : val)
   {
      pack_one (sbo, sub_val);
   }
}



/*
==============================================================================
Name : pack_one
==============================================================================
*/

template <class... Args>
template <class U, class V>
void  Signal <Args...>::pack_one (StreamBinOut & sbo, const std::map <U, V> & val)
{
   sbo << uint64_t (val.size ());

   for (const auto & sub_val : val)
   {
      pack_one (sbo, sub_val);
   }
}



/*
==============================================================================
Name : pack_one
==============================================================================
*/

template <class... Args>
template <class U, class V>
void  Signal <Args...>::pack_one (StreamBinOut & sbo, const std::pair <U, V> & val)
{
   pack_one (sbo, val.first);
   pack_one (sbo, val.second);
}



/*
==============================================================================
Name : pack
==============================================================================
*/

template <class... Args>
void  Signal <Args...>::pack (StreamBinOut & /* sbo */)
{
   // nothing, end of recursion
}



/*
==============================================================================
Name : pack
==============================================================================
*/

template <class... Args>
template <class U>
void  Signal <Args...>::pack (StreamBinOut & sbo, U val)
{
   pack_one (sbo, val);
   pack (sbo);
}



/*
==============================================================================
Name : pack
==============================================================================
*/

template <class... Args>
template <class U, class... Args2>
void  Signal <Args...>::pack (StreamBinOut & sbo, U val, Args2... args)
{
   pack_one (sbo, val);
   pack (sbo, args...);
}



/*
==============================================================================
Name : Unpacker::unpack
==============================================================================
*/

template <class... Args>
template <class U>
template <class... Args3>
void  Signal <Args...>::Unpacker <U>::unpack (Signal & parent, StreamBinIn & sbi, Args3... args3)
{
   if (!sbi.is_eos ()) flip_THROW;

   parent.impl_call (args3...);
}



/*
==============================================================================
Name : Unpacker::unpack_one
==============================================================================
*/

template <class... Args>
template <class U, class... Args2>
template <class V>
typename std::enable_if <
   has_sbio <V>::value
>::type  Signal <Args...>::Unpacker <U, Args2...>::unpack_one (Signal & /* parent */, StreamBinIn & sbi, V & val)
{
   sbi >> val;
}



/*
==============================================================================
Name : Unpacker::unpack_one
==============================================================================
*/

template <class... Args>
template <class U, class... Args2>
template <class V>
typename std::enable_if <
   !has_sbio <V>::value
>::type  Signal <Args...>::Unpacker <U, Args2...>::unpack_one (Signal & /* parent */, StreamBinIn & sbi, V & val)
{
   val = *reinterpret_cast <const V *> (sbi.pull (sizeof (V)));
}



/*
==============================================================================
Name : Unpacker::unpack_one
==============================================================================
*/

template <class... Args>
template <class U, class... Args2>
template <class V>
void  Signal <Args...>::Unpacker <U, Args2...>::unpack_one (Signal & parent, StreamBinIn & sbi, std::vector <V> & val)
{
   uint64_t val_size;
   sbi >> val_size;

   if (val_size > UINT_MAX) flip_THROW;

   val.resize (size_t (val_size));

   for (size_t i = 0 ; i < size_t (val_size) ; ++i)
   {
      unpack_one (parent, sbi, val [i]);
   }
}



/*
==============================================================================
Name : Unpacker::unpack_one
==============================================================================
*/

template <class... Args>
template <class U, class... Args2>
template <class V>
void  Signal <Args...>::Unpacker <U, Args2...>::unpack_one (Signal & parent, StreamBinIn & sbi, std::list <V> & val)
{
   uint64_t val_size;
   sbi >> val_size;

   for (uint64_t i = 0 ; i < val_size ; ++i)
   {
      V sub_val;
      unpack_one (parent, sbi, sub_val);
      val.emplace_back (sub_val);
   }
}



/*
==============================================================================
Name : Unpacker::unpack_one
==============================================================================
*/

template <class... Args>
template <class U, class... Args2>
template <class V, class W>
void  Signal <Args...>::Unpacker <U, Args2...>::unpack_one (Signal & parent, StreamBinIn & sbi, std::map <V, W> & val)
{
   uint64_t val_size;
   sbi >> val_size;

   for (uint64_t i = 0 ; i < val_size ; ++i)
   {
      std::pair <V, W> sub_val;
      unpack_one (parent, sbi, sub_val);
      val.emplace_hint (val.end (), sub_val);
   }
}



/*
==============================================================================
Name : Unpacker::unpack_one
==============================================================================
*/

template <class... Args>
template <class U, class... Args2>
template <class V, class W>
void  Signal <Args...>::Unpacker <U, Args2...>::unpack_one (Signal & parent, StreamBinIn & sbi, std::pair <V, W> & val)
{
   unpack_one (parent, sbi, val.first);
   unpack_one (parent, sbi, val.second);
}



/*
==============================================================================
Name : Unpacker::unpack
==============================================================================
*/

template <class... Args>
template <class U, class... Args2>
void  Signal <Args...>::Unpacker <U, Args2...>::unpack (Signal & parent, StreamBinIn & sbi)
{
   U val;
   unpack_one (parent, sbi, val);

   Unpacker <Args2...> unpacker;
   unpacker.unpack (parent, sbi, U (val));
}



/*
==============================================================================
Name : Unpacker::unpack
==============================================================================
*/

template <class... Args>
template <class U, class... Args2>
template <class... Args3>
void  Signal <Args...>::Unpacker <U, Args2...>::unpack (Signal & parent, StreamBinIn & sbi, Args3... args3)
{
   U val;
   unpack_one (parent, sbi, val);

   Unpacker <Args2...> unpacker;
   unpacker.unpack (parent, sbi, args3..., U (val));
}



/*
==============================================================================
Name : fun_call
==============================================================================
*/

template <class... Args>
template <class T, class F>
void  Signal <Args...>::fun_call (void * ptr, void * func_ptr, Args... args)
{
   T * cast_ptr = static_cast <T *> (ptr);

   std::function <void (T &, Args...)> & f
      = *reinterpret_cast <std::function <void (T &, Args...)> * > (func_ptr);

   f (*cast_ptr, args...);
}



/*
==============================================================================
Name : fun_function_dtor
==============================================================================
*/

template <class... Args>
template <class T, class F>
void  Signal <Args...>::fun_function_dtor (void * func_ptr)
{
   std::function <void (T &, Args...)> * cast_func_ptr
      = reinterpret_cast <std::function <void (T &, Args...)> *> (func_ptr);

   typedef std::function <void (T &, Args...)> FuncType;

   cast_func_ptr->~FuncType ();
}



/*
==============================================================================
Name : lambda_call
==============================================================================
*/

template <class... Args>
template <class T, class F>
void  Signal <Args...>::lambda_call (void * /* ptr */, void * func_ptr, Args... args)
{
   std::function <void (Args...)> & f
      = *reinterpret_cast <std::function <void (Args...)> * > (func_ptr);

   f (args...);
}



/*
==============================================================================
Name : lambda_function_dtor
==============================================================================
*/

template <class... Args>
template <class T, class F>
void  Signal <Args...>::lambda_function_dtor (void * func_ptr)
{
   std::function <void (Args...)> * cast_func_ptr
      = reinterpret_cast <std::function <void (Args...)> *> (func_ptr);

   typedef std::function <void (Args...)> FuncType;

   cast_func_ptr->~FuncType ();
}



/*
==============================================================================
Name : Bundle::dtor
==============================================================================
*/

template <class... Args>
Signal <Args...>::Bundle::~Bundle ()
{
   _func_dtor_proc (_func_buf);
}



}  // namespace flip



#endif   // flip_Signal_CODEHEADER_INCLUDED

#undef flip_Signal_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

