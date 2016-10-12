/*****************************************************************************

      EnumBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"
#include "flip/detail/BasicType.h"
#include "flip/detail/Direction.h"

#include <map>
#include <stdexcept>
#include <vector>

#include <cstdint>
#include <cassert>



namespace flip
{



class EnumClassBase;

class EnumBase
:  public BasicType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  EnumBase () = default;
                  EnumBase (const EnumBase & rhs) = default;
                  EnumBase (EnumBase && rhs) : BasicType (std::move (rhs)) {}
   virtual        ~EnumBase () = default;

   // Type
   virtual const ClassBase &
                  get_class () const override {flip_FATAL;}   // COV_NF_LINE
   virtual void   revert () override {flip_FATAL;} // COV_NF_LINE



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   virtual void   impl_internal_set (int64_t /* value */) {flip_FATAL;}  // COV_NF_LINE

   // Type
   virtual void   impl_set_class (const ClassBase & /* class_base */) override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_make (Transaction & /* tx */, ImplUndoRedoMode /* parent_mode */, TxPostProcessInfo & /* ppinfo */) const override {flip_FATAL;}  // COV_NF_LINE
   virtual bool   impl_is_reset () const override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_reset () override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_synchronize () override {flip_FATAL;}   // COV_NF_LINE

   virtual const EnumClassBase &
                  impl_enum_class () const {flip_FATAL;}   // COV_NF_LINE
   virtual int64_t
                  get () const {flip_FATAL;} // COV_NF_LINE
   virtual int64_t
                  get_before () const {flip_FATAL;}   // COV_NF_LINE



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   EnumBase &  operator = (const EnumBase & rhs) = delete;
   EnumBase &  operator = (EnumBase && rhs) = delete;
   bool           operator == (const EnumBase & rhs) const = delete;
   bool           operator != (const EnumBase & rhs) const = delete;



}; // class EnumBase



}  // namespace flip



//#include  "flip/EnumBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

