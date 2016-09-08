/*****************************************************************************

      ObjectRefBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/def.h"
#include "flip/detail/BasicType.h"

#include <map>
#include <stdexcept>

#include <cassert>



namespace flip
{



class flip_API ObjectRefBase
:  public BasicType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  ObjectRefBase () = default;
                  ObjectRefBase (const ObjectRefBase & rhs) = default;
                  ObjectRefBase (ObjectRefBase && rhs) : BasicType (std::move (rhs)) {}
   virtual        ~ObjectRefBase () = default;

   ObjectRefBase &
                  operator = (const ObjectRefBase & rhs) = default;
   ObjectRefBase &
                  operator = (ObjectRefBase && rhs) {this->BasicType::operator = (std::move (rhs)); return *this;}

   // Type
   virtual const ClassBase &
                  get_class () const override {flip_FATAL;}   // COV_NF_LINE
   virtual void   revert () override {flip_FATAL;} // COV_NF_LINE



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   virtual void   impl_internal_set (const Ref & /* value */) {flip_FATAL;}  // COV_NF_LINE

   // Type
   virtual void   impl_set_class (const ClassBase & /* class_base */) override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_make (Transaction & /* tx */, ImplUndoRedoMode /* parent_mode */, TxPostProcessInfo & /* ppinfo */) const override {flip_FATAL;}  // COV_NF_LINE
   virtual bool   impl_is_reset () const override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_reset () override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_synchronize () override {flip_FATAL;}   // COV_NF_LINE

   virtual Ref    get () const {flip_FATAL;} // COV_NF_LINE
   virtual Ref    get_before () const {flip_FATAL;}   // COV_NF_LINE



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const ObjectRefBase & rhs) const = delete;
   bool           operator != (const ObjectRefBase & rhs) const = delete;



}; // class ObjectRefBase



}  // namespace flip



//#include  "flip/ObjectRefBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

