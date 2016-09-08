/*****************************************************************************

      Signal.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/SignalConnection.h"
#include "flip/StreamBinIn.h"
#include "flip/StreamBinOut.h"
#include "flip/detail/def.h"
#include "flip/detail/SignalBase.h"
#include "flip/detail/SignalData.h"
#include "flip/detail/TypeTraits.h"

#include <functional>
#include <list>
#include <map>
#include <type_traits>
#include <vector>

#include <cstdint>



namespace flip
{



class Object;

template <class... Args>
class Signal
:  public SignalBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                  Signal (uint32_t type, Object & object);
   virtual        ~Signal ();

#if (flip_COMPILER == flip_COMPILER_MSVC) && (_MSC_VER == 1800)
   // MSVC 12.0 does not detect well constructibility in case of non-accessible functions
                  Signal (const Signal & rhs);
#else
                  Signal (const Signal & rhs) = delete;  // see note in .hpp
#endif

   uint32_t       type () const;
   Object &       object ();

   void           operator () (Args... args);
   SignalData     make (Args... args);

   template <class T, class F>
   SignalConnection
                  connect (T & t, F f) flip_WARN_UNUSED_RESULT;
   template <class F>
   SignalConnection
                  connect (F f) flip_WARN_UNUSED_RESULT;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // SignalBase
   virtual void   impl_signal (const std::vector <uint8_t> & data) override;
   virtual void   impl_disconnect (const ConnectionKey & key) override;

   void           impl_call (Args... args);




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef void (*CallProc) (void * ptr, void * func_ptr, Args... args);
   typedef void (*FunctionDtorProc) (void * func_ptr);

   template <class U>
   typename std::enable_if <
      has_sbio <U>::value
   >::type        pack_one (StreamBinOut & sbo, U val);

   template <class U>
   typename std::enable_if <
      !has_sbio <U>::value
   >::type        pack_one (StreamBinOut & sbo, U val);

   template <class U>
   void           pack_one (StreamBinOut & sbo, const std::vector <U> & val);
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
      void        unpack (Signal & parent, StreamBinIn & sbi, Args3... args3);
   };

   template <class U, class... Args2>
   struct Unpacker <U, Args2...>
   {
      template <class V>
      typename std::enable_if <
         has_sbio <V>::value
      >::type     unpack_one (Signal & parent, StreamBinIn & sbi, V & val);

      template <class V>
      typename std::enable_if <
         !has_sbio <V>::value
      >::type     unpack_one (Signal & parent, StreamBinIn & sbi, V & val);

      template <class V>
      void        unpack_one (Signal & parent, StreamBinIn & sbi, std::vector <V> & val);
      template <class V>
      void        unpack_one (Signal & parent, StreamBinIn & sbi, std::list <V> & val);
      template <class V, class W>
      void        unpack_one (Signal & parent, StreamBinIn & sbi, std::map <V, W> & val);
      template <class V, class W>
      void        unpack_one (Signal & parent, StreamBinIn & sbi, std::pair <V, W> & val);

      void        unpack (Signal & parent, StreamBinIn & sbi);
      template <class... Args3>
      void        unpack (Signal & parent, StreamBinIn & sbi, Args3... args3);
   };

   template <class T, class F>
   static void    fun_call (void * ptr, void * func_ptr, Args... args);
   template <class T, class F>
   static void    fun_function_dtor (void * func_ptr);

   template <class T, class F>
   static void    lambda_call (void * ptr, void * func_ptr, Args... args);
   template <class T, class F>
   static void    lambda_function_dtor (void * func_ptr);

   typedef uint8_t Buf [sizeof (std::function <void (void)>)];

   class Bundle
   {
   public:
                  ~Bundle (); // DON'T INHERIT
      CallProc    _call_proc = nullptr;
      void *      _target_ptr = nullptr;
#if (flip_COMPILER == flip_COMPILER_MSVC)
      Buf         _func_buf;
#else
      Buf         _func_buf = {0};
#endif
      FunctionDtorProc
                  _func_dtor_proc = nullptr;
   };

   typedef std::map <ConnectionKey, Bundle> BundleMap;

   const uint32_t _type;
   Object &       _object;

   ConnectionKey  _key = 0ULL;
   BundleMap      _bundle_map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Signal () = delete;
   Signal &       operator = (const Signal & rhs) = delete;
                  Signal (Signal && rhs) = delete;
   Signal &       operator = (Signal && rhs) = delete;
   bool           operator == (const Signal & rhs) const = delete;
   bool           operator != (const Signal & rhs) const = delete;



}; // class Signal



}  // namespace flip



#include "flip/Signal.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

