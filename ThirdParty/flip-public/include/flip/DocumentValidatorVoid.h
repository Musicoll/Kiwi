/*****************************************************************************

      DocumentValidatorVoid.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/DocumentValidatorBase.h"



namespace flip
{



class DocumentValidatorVoid
:  public DocumentValidatorBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DocumentValidatorVoid () = default;
   virtual        ~DocumentValidatorVoid () = default;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // DocumentValidatorBase
   virtual void   notify_validate (uint64_t /* user_id */, Object & /* object */) override {}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentValidatorVoid (const DocumentValidatorVoid & rhs) = delete;
   DocumentValidatorVoid &
                  operator = (const DocumentValidatorVoid & rhs) = delete;
                  DocumentValidatorVoid (DocumentValidatorVoid && rhs) = delete;
   DocumentValidatorVoid &
                  operator = (DocumentValidatorVoid && rhs) = delete;
   bool           operator == (const DocumentValidatorVoid & rhs) const = delete;
   bool           operator != (const DocumentValidatorVoid & rhs) const = delete;



}; // class DocumentValidatorVoid



}  // namespace flip



//#include  "flip/DocumentValidatorVoid.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

