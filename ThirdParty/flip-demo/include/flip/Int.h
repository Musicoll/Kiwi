/*****************************************************************************

      Int.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/BasicType.h"

#include <cstdint>



namespace flip
{



class Transaction;

class flip_API Int
:  public BasicType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef int64_t internal_type;

                  Int () = default;
                  Int (const Int & rhs);
                  Int (Int && rhs);
                  Int (int64_t value);
   virtual        ~Int () = default;

   Int &          operator = (const Int & rhs);
   Int &          operator = (Int && rhs);
   Int &          operator = (int64_t value);
   Int &          operator += (int64_t value);
   Int &          operator -= (int64_t value);
   Int &          operator *= (int64_t value);
   Int &          operator /= (int64_t value);

   // Type
   virtual const ClassBase &
                  get_class () const override;
   virtual bool   changed () const override;

                  operator int64_t () const;
   int64_t        value () const;
   int64_t        before () const;

   bool           operator == (int rhs) const;
   bool           operator != (int rhs) const;

   bool           operator == (long rhs) const;
   bool           operator != (long rhs) const;

   bool           operator == (long long rhs) const;
   bool           operator != (long long rhs) const;

   virtual void   revert () override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   void           impl_internal_set (int64_t value);

   // Type
   virtual void   impl_make (Transaction & tx, ImplUndoRedoMode parent_mode, TxPostProcessInfo & ppinfo) const override;
   virtual void   impl_set_class (const ClassBase & class_base) override;
   virtual bool   impl_is_reset () const override;
   virtual void   impl_reset () override;
   virtual void   impl_synchronize () override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   int64_t        _value = 0LL;
   int64_t        _value_before = 0LL;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Int & rhs) const = delete;
   bool           operator != (const Int & rhs) const = delete;



}; // class Int



}  // namespace flip



//#include  "flip/Int.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

