/*****************************************************************************

      Enum.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/EnumBase.h"

#include <cstdint>



namespace flip
{



template <class T>
class Enum
:  public EnumBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef T value_type;
   typedef T & reference;
   typedef const T & const_reference;
   typedef T * pointer;
   typedef const T * const_pointer;

                  Enum () = default;
                  Enum (const Enum & rhs);
                  Enum (Enum && rhs);
                  Enum (T value);
   virtual        ~Enum () = default;

   Enum &         operator = (const Enum & rhs);
   Enum &         operator = (Enum && rhs);
   Enum &         operator = (T value);

   // Type
   virtual const ClassBase &
                  get_class () const override;
   virtual bool   changed () const override;

                  operator T () const;
   T              value () const;
   T              before () const;

   bool           operator == (T rhs) const;
   bool           operator != (T rhs) const;

   virtual void   revert () override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // EnumBase
   virtual void   impl_internal_set (int64_t value) override;

   // Type
   virtual void   impl_make (Transaction & tx, ImplUndoRedoMode parent_mode, TxPostProcessInfo & ppinfo) const override;
   virtual void   impl_set_class (const ClassBase & class_base) override;
   virtual bool   impl_is_reset () const override;
   virtual void   impl_reset () override;
   virtual void   impl_synchronize () override;

   // EnumBase
   virtual const EnumClassBase &
                  impl_enum_class () const override;
   virtual int64_t
                  get () const override;
   virtual int64_t
                  get_before () const override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   int64_t        _value = 0LL;
   int64_t        _value_before = 0LL;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Enum & rhs) const = delete;
   bool           operator != (const Enum & rhs) const = delete;



}; // class Enum



template <class T>
bool  operator == (T lhs, const Enum <T> & rhs);
template <class T>
bool  operator != (T lhs, const Enum <T> & rhs);



}  // namespace flip



#include  "flip/Enum.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

