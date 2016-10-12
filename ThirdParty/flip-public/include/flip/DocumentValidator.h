/*****************************************************************************

      DocumentValidator.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/DocumentValidatorBase.h"

#include <stdexcept>



#if defined (__GNUC__) || defined (__clang__)
   #define flip_VALIDATION_FAILED(x) \
      throw std::runtime_error (std::string ("" x " in ") + __PRETTY_FUNCTION__)

#elif defined (_MSC_VER)
   #define flip_VALIDATION_FAILED(x) \
      throw std::runtime_error (std::string ("" x " in ") + __FUNCDNAME__)

#else
   #error Unsupported Compiler.

#endif



namespace flip
{



template <class T>
class DocumentValidator
:  public DocumentValidatorBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DocumentValidator () = default;
   virtual        ~DocumentValidator () = default;

   virtual void   validate (T & root) = 0;

   uint64_t       user () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // DocumentValidatorBase
   virtual void   notify_validate (uint64_t user_id, Object & object) override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   uint64_t       _user_id = 0ULL;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentValidator (const DocumentValidator & rhs) = delete;
   DocumentValidator &
                  operator = (const DocumentValidator & rhs) = delete;
                  DocumentValidator (DocumentValidator && rhs) = delete;
   DocumentValidator &
                  operator = (DocumentValidator && rhs) = delete;
   bool           operator == (const DocumentValidator & rhs) const = delete;
   bool           operator != (const DocumentValidator & rhs) const = delete;



}; // class DocumentValidator



}  // namespace flip



#include "flip/DocumentValidator.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

