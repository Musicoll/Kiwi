/*****************************************************************************

      Bool.h
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

class flip_API Bool
:  public BasicType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef bool internal_type;

                  Bool () = default;
                  Bool (const Bool & rhs);
                  Bool (Bool && rhs);
                  Bool (bool value);
   virtual        ~Bool () = default;

   Bool &         operator = (const Bool & rhs);
   Bool &         operator = (Bool && rhs);
   Bool &         operator = (bool value);
   Bool &         flip ();

   // Type
   virtual const ClassBase &
                  get_class () const override;
   virtual bool   changed () const override;

                  operator bool () const;
   bool           value () const;
   bool           before () const;

   bool           operator == (bool rhs) const;
   bool           operator != (bool rhs) const;

   virtual void   revert () override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   void           impl_internal_set (bool value);

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

   bool        _value = false;
   bool        _value_before = false;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Bool & rhs) const = delete;
   bool           operator != (const Bool & rhs) const = delete;



}; // class Bool



}  // namespace flip



//#include  "flip/Bool.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

