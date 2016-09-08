/*****************************************************************************

      DocumentValidatorBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cstdint>



namespace flip
{



class Object;

class DocumentValidatorBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DocumentValidatorBase () = default;
                  DocumentValidatorBase (const DocumentValidatorBase & rhs) = default;
   virtual        ~DocumentValidatorBase () = default;

   virtual void   notify_validate (uint64_t user_id, Object & object) = 0;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentValidatorBase (DocumentValidatorBase && rhs) = delete;
   DocumentValidatorBase &
                  operator = (const DocumentValidatorBase & rhs) = delete;
   DocumentValidatorBase &
                  operator = (DocumentValidatorBase && rhs) = delete;
   bool           operator == (const DocumentValidatorBase & rhs) const = delete;
   bool           operator != (const DocumentValidatorBase & rhs) const = delete;



}; // class DocumentValidatorBase



}  // namespace flip



//#include  "flip/DocumentValidatorBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

