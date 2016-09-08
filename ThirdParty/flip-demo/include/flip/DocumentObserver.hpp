/*****************************************************************************

      DocumentObserver.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_DocumentObserver_CURRENT_CODEHEADER)
   #error Recursive inclusion of DocumentObserver code header.
#endif
#define  flip_DocumentObserver_CURRENT_CODEHEADER

#if ! defined (flip_DocumentObserver_CODEHEADER_INCLUDED)
#define  flip_DocumentObserver_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Object.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : notify_document_changed
==============================================================================
*/

template <class T>
void  DocumentObserver <T>::notify_document_changed (Object & object)
{
   auto & root = dynamic_cast <T &> (object);

   document_changed (root);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_DocumentObserver_CODEHEADER_INCLUDED

#undef flip_DocumentObserver_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

