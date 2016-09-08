/*****************************************************************************

      Float.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/BasicType.h"



namespace flip
{



class flip_API Float
:  public BasicType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef double internal_type;

                  Float () = default;
                  Float (const Float & rhs);
                  Float (Float && rhs);
                  Float (double value);
   virtual        ~Float () = default;

   Float &        operator = (const Float & rhs);
   Float &        operator = (Float && rhs);
   Float &        operator = (double value);
   Float &        operator += (double value);
   Float &        operator -= (double value);
   Float &        operator *= (double value);
   Float &        operator /= (double value);

   // Type
   virtual const ClassBase &
                  get_class () const override;
   virtual bool   changed () const override;

                  operator double () const;
   double         value () const;
   double         before () const;

   bool           operator == (double rhs) const;
   bool           operator != (double rhs) const;

   virtual void   revert () override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   void           impl_internal_set (double value);

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


   double         _value = 0.0;
   double         _value_before = 0.0;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Float & rhs) const = delete;
   bool           operator != (const Float & rhs) const = delete;



}; // class Float



}  // namespace flip



//#include  "flip/Float.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

