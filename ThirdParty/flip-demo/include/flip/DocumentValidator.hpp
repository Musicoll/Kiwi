/*****************************************************************************

      DocumentValidator.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_DocumentValidator_CURRENT_CODEHEADER)
   #error Recursive inclusion of DocumentValidator code header.
#endif
#define  flip_DocumentValidator_CURRENT_CODEHEADER

#if ! defined (flip_DocumentValidator_CODEHEADER_INCLUDED)
#define  flip_DocumentValidator_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Object.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : notify_validate
==============================================================================
*/

template <class T>
void  DocumentValidator <T>::notify_validate (uint64_t user_id, Object & object)
{
   _user_id = user_id;

   auto & root = dynamic_cast <T &> (object);

   validate (root);
}



/*
==============================================================================
Name : user
Description :
   Returns the user of the current validated transaction.
==============================================================================
*/

template <class T>
uint64_t DocumentValidator <T>::user () const
{
   return _user_id;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_DocumentValidator_CODEHEADER_INCLUDED

#undef flip_DocumentValidator_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

