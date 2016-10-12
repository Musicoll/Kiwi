/*****************************************************************************

      Mold.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include "flip/detail/Default.h"

#include <map>
#include <vector>
#include <type_traits>

#include <cstdint>



namespace flip
{



class ArrayBase;
class Blob;
class Bool;
class ClassBase;
class CollectionBase;
class DataModelBase;
class EnumBase;
class Int;
class Float;
class MessageBase;
class Object;
class ObjectRefBase;
class StreamBinOut;
class StreamBinIn;
class Type;

class flip_API Mold
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                  Mold (const DataModelBase & model);
                  Mold (const DataModelBase & model, StreamBinOut & sbo);
                  Mold (const DataModelBase & model, StreamBinIn & sbi);
                  Mold (Mold && rhs);
   virtual        ~Mold () = default;

   Mold &         operator = (Mold && rhs);

   template <class T>
   void           make (const T & obj, bool skip_container_flag = false);
   void           cure ();

   template <class T>
   typename std::enable_if <
      std::is_constructible <T, Default &>::value, T
   >::type        cast () const;
   template <class T>
   typename std::enable_if <
      !std::is_constructible <T, Default &>::value, T
   >::type        cast () const;
   template <class T>
   void           cast (T & obj) const;
   template <class T>
   bool           has () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           make (const ClassBase & class_base, const Type & obj, StreamBinOut & sbo, bool skip_container_flag);
   void           make (const Bool & obj, StreamBinOut & sbo);
   void           make (const Int & obj, StreamBinOut & sbo);
   void           make (const Float & obj, StreamBinOut & sbo);
   void           make (const EnumBase & obj, StreamBinOut & sbo);
   void           make (const ObjectRefBase & obj, StreamBinOut & sbo);
   void           make (const Blob & obj, StreamBinOut & sbo);
   void           make (const CollectionBase & obj, StreamBinOut & sbo, bool skip_container_flag);
   void           make (const ArrayBase & obj, StreamBinOut & sbo, bool skip_container_flag);
   void           make (const MessageBase & obj, StreamBinOut & sbo);
   void           make (const ClassBase & class_base, const Object & obj, StreamBinOut & sbo, bool skip_container_flag);

   void           cast (Type & obj, StreamBinIn & sbi) const;
   void           cast (Bool & obj, StreamBinIn & sbi) const;
   void           cast (Int & obj, StreamBinIn & sbi) const;
   void           cast (Float & obj, StreamBinIn & sbi) const;
   void           cast (EnumBase & obj, StreamBinIn & sbi) const;
   void           cast (ObjectRefBase & obj, StreamBinIn & sbi) const;
   void           cast (Blob & obj, StreamBinIn & sbi) const;
   void           cast (CollectionBase & obj, StreamBinIn & sbi) const;
   void           cast (ArrayBase & obj, StreamBinIn & sbi) const;
   void           cast (MessageBase & obj, StreamBinIn & sbi) const;
   void           cast (Object & obj, StreamBinIn & sbi) const;

   const DataModelBase &
                  _model;
   StreamBinOut * const
                  _sbo_ptr = nullptr;
   StreamBinIn * const
                  _sbi_ptr = nullptr;

   std::map <uint32_t, std::vector <uint8_t>>
                  _map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Mold () = delete;
                  Mold (const Mold & rhs) = delete;
   Mold &         operator = (const Mold & rhs) = delete;
   bool           operator == (const Mold & rhs) const = delete;
   bool           operator != (const Mold & rhs) const = delete;



}; // class Mold



}  // namespace flip



#include "flip/Mold.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

