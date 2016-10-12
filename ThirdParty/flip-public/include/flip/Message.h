/*****************************************************************************

      Message.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/StreamBinIn.h"
#include "flip/StreamBinOut.h"
#include "flip/detail/def.h"
#include "flip/detail/MessageBase.h"
#include "flip/detail/TypeTraits.h"

#include <functional>
#include <list>
#include <memory>
#include <type_traits>



namespace flip
{



template <class... Args>
class Message
:  public MessageBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  Message () = default;
                  Message (const Message & rhs);
                  Message (Message && rhs);
   virtual        ~Message () = default;

   Message &      operator = (const Message & rhs);
   Message &      operator = (Message && rhs);

   Message &      send (Args... args);

   // Type
   virtual const ClassBase &
                  get_class () const override;
   virtual bool   changed () const override;

   template <class F>
   void           get (F f) const;

   inline virtual void
                  revert () override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // MessageBase
   inline virtual void
                  impl_internal_push (Direction direction, const Data & data) override;

   // Type
   inline virtual void
                  impl_make (Transaction & tx, ImplUndoRedoMode parent_mode, TxPostProcessInfo & ppinfo) const override;
   inline virtual void
                  impl_set_class (const ClassBase & class_base) override;
   inline virtual bool
                  impl_is_reset () const override;
   inline virtual void
                  impl_reset () override;
   inline virtual void
                  impl_synchronize () override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   template <class U>
   void           pack_one (StreamBinOut & sbo, U val);
   template <class U>
   typename std::enable_if <
      std::is_same <U, uint8_t>::value
      >::type     pack_one (StreamBinOut & sbo, const std::vector <U> & val);
   template <class U>
   typename std::enable_if <
      !std::is_same <U, uint8_t>::value
      >::type     pack_one (StreamBinOut & sbo, const std::vector <U> & val);
   template <class U>
   void           pack_one (StreamBinOut & sbo, const std::list <U> & val);
   template <class U, class V>
   void           pack_one (StreamBinOut & sbo, const std::map <U, V> & val);
   template <class U, class V>
   void           pack_one (StreamBinOut & sbo, const std::pair <U, V> & val);
   void           pack (StreamBinOut & sbo);
   template <class U>
   void           pack (StreamBinOut & sbo, U val);
   template <class U, class... Args2>
   void           pack (StreamBinOut & sbo, U val, Args2... args);

   template <class... Args2> struct Unpacker;

   template <class U>
   struct Unpacker <U>
   {
      template <class... Args3>
      void        unpack (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, Args3... args3);
   };

   template <class U, class... Args2>
   struct Unpacker <U, Args2...>
   {
      template <class V>
      void        unpack_one (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, V & val);
      template <class V>
      typename std::enable_if <
      std::is_same <V, uint8_t>::value
      >::type     unpack_one (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, std::vector <V> & val);
      template <class V>
      typename std::enable_if <
      !std::is_same <V, uint8_t>::value
      >::type     unpack_one (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, std::vector <V> & val);
      template <class V>
      void        unpack_one (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, std::list <V> & val);
      template <class V, class W>
      void        unpack_one (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, std::map <V, W> & val);
      template <class V, class W>
      void        unpack_one (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, std::pair <V, W> & val);

      void        unpack (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi);
      template <class... Args3>
      void        unpack (std::function <void (bool, Args...)> & func, int cnt, StreamBinIn & sbi, Args3... args3);
   };

   std::list <std::pair <int, Data>>
                  _elements;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Message & rhs) const = delete;
   bool           operator != (const Message & rhs) const = delete;



}; // class Message



}  // namespace flip

// COV_NF_START

#include  "flip/Message.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

