/*****************************************************************************

      Blob.h
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

class flip_API Blob
:  public BasicType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef std::vector <uint8_t> internal_type;

                  Blob () = default;
                  Blob (const Blob & rhs);
                  Blob (Blob && rhs);
                  Blob (const std::vector <uint8_t> & value);
   virtual        ~Blob () = default;

   Blob &         operator = (const Blob & rhs);
   Blob &         operator = (Blob && rhs);
   Blob &         operator = (const std::vector <uint8_t> & value);
   Blob &         operator = (std::vector <uint8_t> && value);

   // Type
   virtual const ClassBase &
                  get_class () const override;
   virtual bool   changed () const override;

                  operator const std::vector <uint8_t> & () const;
   const std::vector <uint8_t> &
                  value () const;
   const std::vector <uint8_t> &
                  before () const;

   bool           operator == (const std::vector <uint8_t> & rhs) const;
   bool           operator != (const std::vector <uint8_t> & rhs) const;

   bool           empty () const;
   size_t         size () const;

   virtual void   revert () override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   virtual void   impl_internal_set (const std::vector <uint8_t> & value);
   void           impl_internal_set (std::vector <uint8_t> && value);

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

   std::vector <uint8_t>
                  _value;
   std::vector <uint8_t>
                  _value_before;
   mutable bool   _changed_flag_need_update_flag = true;
   mutable bool   _changed_flag = false;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Blob & rhs) const = delete;
   bool           operator != (const Blob & rhs) const = delete;



}; // class Blob



flip_API bool     operator == (const std::vector <uint8_t> & lhs, const Blob & rhs);
flip_API bool     operator != (const std::vector <uint8_t> & lhs, const Blob & rhs);



}  // namespace flip



//#include  "flip/Blob.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

