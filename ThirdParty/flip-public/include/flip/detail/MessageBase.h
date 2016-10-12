/*****************************************************************************

      MessageBase.h
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



class MessageBase
:  public BasicType
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  MessageBase () = default;
                  MessageBase (const MessageBase & rhs) = default;
                  MessageBase (MessageBase && rhs) : BasicType (std::move (rhs)) {}
   virtual        ~MessageBase () = default;

   // Type
   virtual const ClassBase &
                  get_class () const override {flip_FATAL;}   // COV_NF_LINE
   virtual void   revert () override {flip_FATAL;} // COV_NF_LINE



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   using Data = std::vector <uint8_t>;

   virtual void   impl_internal_push (Direction /*direction*/, const Data & /*data*/) {flip_FATAL;}  // COV_NF_LINE

   // Type
   virtual void   impl_set_class (const ClassBase & /* class_base */) override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_make (Transaction & /* tx */, ImplUndoRedoMode /* parent_mode */, TxPostProcessInfo & /* ppinfo */) const override {flip_FATAL;}  // COV_NF_LINE
   virtual bool   impl_is_reset () const override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_reset () override {flip_FATAL;}   // COV_NF_LINE
   virtual void   impl_synchronize () override {flip_FATAL;}   // COV_NF_LINE



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   MessageBase &  operator = (const MessageBase & rhs) = delete;
   MessageBase &  operator = (MessageBase && rhs) = delete;
   bool           operator == (const MessageBase & rhs) const = delete;
   bool           operator != (const MessageBase & rhs) const = delete;



}; // class MessageBase



}  // namespace flip



//#include  "flip/MessageBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

