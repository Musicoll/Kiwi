/*****************************************************************************

      Message.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Message_CURRENT_CODEHEADER)
   #error Recursive inclusion of Message code header.
#endif
#define  flip_Message_CURRENT_CODEHEADER

#if ! defined (flip_Message_CODEHEADER_INCLUDED)
#define  flip_Message_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Class.h"
#include "flip/Transaction.h"
#include "flip/detail/def.h"
#include "flip/detail/fnc.h"

#include <climits>



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class... Args>
Message <Args...>::Message (const Message & rhs)
:  MessageBase (rhs)
{
   // nothing
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class... Args>
Message <Args...>::Message (Message && rhs)
:  MessageBase (std::move (rhs))
{
   // nothing
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class... Args>
Message <Args...> &   Message <Args...>::operator = (const Message & /*rhs*/)
{
   return *this;
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class... Args>
Message <Args...> &   Message <Args...>::operator = (Message && /*rhs*/)
{
   return *this;
}



/*
==============================================================================
Name : send
==============================================================================
*/

template <class... Args>
Message <Args...> &   Message <Args...>::send (Args... args)
{
   // cannot change model while observing
   if (!can_change ()) flip_FATAL;

   std::vector <uint8_t> data;
   StreamBinOut sbo (data);

   pack (sbo, args...);

   impl_incr_modification_cnt (1);

   return *this;
}



/*
==============================================================================
Name : get_class
==============================================================================
*/

template <class... Args>
const ClassBase & Message <Args...>::get_class () const
{
   return Class <MessageBase>::get ();
}



/*
==============================================================================
Name : changed
Description :
   See base class 'Type' description.
==============================================================================
*/

template <class... Args>
bool  Message <Args...>::changed () const
{
   // ignore restore change

   return !_elements.empty ();
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class... Args>
template <class F>
void  Message <Args...>::get (F f) const
{
   std::function <void (bool, Args...)> func (f);

   for (auto && element : _elements)
   {
      Unpacker <Args..., void> unpacker;

      StreamBinIn sbi (element.second);

      unpacker.unpack (func, element.first, sbi);

      // deserialize, get direction, call f
   }
}



/*
==============================================================================
Name : revert
==============================================================================
*/

template <class... Args>
void  Message <Args...>::revert ()
{
   _elements.clear ();
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : impl_make
==============================================================================
*/

template <class... Args>
void  Message <Args...>::impl_make (Transaction & tx, ImplUndoRedoMode parent_mode, TxPostProcessInfo & /* ppinfo */) const
{
   auto mode = impl_make_undo_redo_mode (parent_mode);
   auto flags = impl_make_tx_flags (mode);

   for (auto && element : _elements)
   {
      tx.push_message_push (ref (), flags, element.second);
   }
}



/*
==============================================================================
Name : impl_internal_push
Note :
   This function is called from the VM when executing a transaction only.
==============================================================================
*/

template <class... Args>
void  Message <Args...>::impl_internal_push (Direction direction, const Data & data)
{
   auto it = _elements.begin ();
   auto it_end = _elements.end ();

   bool found_flag = false;

   // check if we have the same data in opposite direction

   for (; it != it_end ; ++it)
   {
      const auto & sub_data = it->second;

      if (data == sub_data)
      {
         found_flag = true;

         auto & cnt = it->first;

         switch (direction)
         {
         case Direction::FORWARD:
            cnt += 1;
            break;

         case Direction::BACKWARD:
            cnt -= 1;
            break;
         }

         if (cnt == 0)
         {
            impl_incr_modification_cnt (-1);

            _elements.erase (it);
            break;
         }
      }
   }

   // add

   if (!found_flag)
   {
      impl_incr_modification_cnt (1);

      switch (direction)
      {
      case Direction::FORWARD:
         _elements.push_back (std::make_pair (1, data));
         break;

      case Direction::BACKWARD:
         _elements.push_back (std::make_pair (-1, data));
         break;
      }
   }
}



/*
==============================================================================
Name : impl_set_class
==============================================================================
*/

template <class... Args>
void  Message <Args...>::impl_set_class (const ClassBase & class_base)
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

template <class... Args>
bool  Message <Args...>::impl_is_reset () const
{
   return true;
}



/*
==============================================================================
Name : impl_reset
==============================================================================
*/

template <class... Args>
void  Message <Args...>::impl_reset ()
{
   _elements.clear ();
}



/*
==============================================================================
Name : impl_synchronize
==============================================================================
*/

template <class... Args>
void  Message <Args...>::impl_synchronize ()
{
   Type::impl_synchronize ();

   _elements.clear ();
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
void  Message <Args...>::pack_one (StreamBinOut & sbo, U val)
{
   /*
   Either use types supported by StreamBinOut/In like uint32_t, etc.
   or allow your type to support write/read functions
   */

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
std::is_same <U, uint8_t>::value
>::type  Message <Args...>::pack_one (StreamBinOut & sbo, const std::vector <U> & val)
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
!std::is_same <U, uint8_t>::value
>::type  Message <Args...>::pack_one (StreamBinOut & sbo, const std::vector <U> & val)
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
void  Message <Args...>::pack_one (StreamBinOut & sbo, const std::list <U> & val)
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
void  Message <Args...>::pack_one (StreamBinOut & sbo, const std::map <U, V> & val)
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
void  Message <Args...>::pack_one (StreamBinOut & sbo, const std::pair <U, V> & val)
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
void  Message <Args...>::pack (StreamBinOut & sbo)
{
   _elements.emplace (
      _elements.end (),
      std::make_pair (1, sbo.data ())
   );
}



/*
==============================================================================
Name : pack
==============================================================================
*/

template <class... Args>
template <class U>
void  Message <Args...>::pack (StreamBinOut & sbo, U val)
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
void  Message <Args...>::pack (StreamBinOut & sbo, U val, Args2... args)
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
void  Message <Args...>::Unpacker <U>::unpack (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, Args3... args3)
{
   if (!sbi.is_eos ()) flip_THROW;

   assert (cnt != 0);

   bool forward_flag = cnt > 0;

   if (cnt < 0) cnt = - cnt;

   for (int i = 0 ; i < cnt ; ++i)
   {
      func (forward_flag, args3...);
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
void  Message <Args...>::Unpacker <U, Args2...>::unpack_one (std::function <void (bool, Args...)> & /*func*/, int /*cnt*/, StreamBinIn & sbi, V & val)
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
std::is_same <V, uint8_t>::value
>::type  Message <Args...>::Unpacker <U, Args2...>::unpack_one (std::function <void (bool, Args...)> & /*func*/, int /*cnt*/, StreamBinIn & sbi, std::vector <V> & val)
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
!std::is_same <V, uint8_t>::value
>::type  Message <Args...>::Unpacker <U, Args2...>::unpack_one (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, std::vector <V> & val)
{
   uint64_t val_size;
   sbi >> val_size;

   if (val_size > UINT_MAX) flip_THROW;

   val.resize (size_t (val_size));

   for (size_t i = 0 ; i < size_t (val_size) ; ++i)
   {
      unpack_one (func, cnt, sbi, val [i]);
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
void  Message <Args...>::Unpacker <U, Args2...>::unpack_one (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, std::list <V> & val)
{
   uint64_t val_size;
   sbi >> val_size;

   for (uint64_t i = 0 ; i < val_size ; ++i)
   {
      V sub_val;
      unpack_one (func, cnt, sbi, sub_val);
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
void  Message <Args...>::Unpacker <U, Args2...>::unpack_one (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, std::map <V, W> & val)
{
   uint64_t val_size;
   sbi >> val_size;

   for (uint64_t i = 0 ; i < val_size ; ++i)
   {
      std::pair <V, W> sub_val;
      unpack_one (func, cnt, sbi, sub_val);
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
void  Message <Args...>::Unpacker <U, Args2...>::unpack_one (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, std::pair <V, W> & val)
{
   unpack_one (func, cnt, sbi, val.first);
   unpack_one (func, cnt, sbi, val.second);
}



/*
==============================================================================
Name : Unpacker::unpack
==============================================================================
*/

template <class... Args>
template <class U, class... Args2>
void  Message <Args...>::Unpacker <U, Args2...>::unpack (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi)
{
   U val;
   unpack_one (func, cnt, sbi, val);

   Unpacker <Args2...> unpacker;
   unpacker.unpack (func, cnt, sbi, U (val));
}



/*
==============================================================================
Name : Unpacker::unpack
==============================================================================
*/

template <class... Args>
template <class U, class... Args2>
template <class... Args3>
void  Message <Args...>::Unpacker <U, Args2...>::unpack (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, Args3... args3)
{
   U val;
   unpack_one (func, cnt, sbi, val);

   Unpacker <Args2...> unpacker;
   unpacker.unpack (func, cnt, sbi, args3..., U (val));
}



}  // namespace flip



#endif   // flip_Message_CODEHEADER_INCLUDED

#undef flip_Message_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

