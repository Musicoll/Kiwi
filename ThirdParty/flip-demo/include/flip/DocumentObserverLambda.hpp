/*****************************************************************************

      DocumentObserverLambda.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_DocumentObserverLambda_CURRENT_CODEHEADER)
   #error Recursive inclusion of DocumentObserverLambda code header.
#endif
#define  flip_DocumentObserverLambda_CURRENT_CODEHEADER

#if ! defined (flip_DocumentObserverLambda_CODEHEADER_INCLUDED)
#define  flip_DocumentObserverLambda_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Object.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
DocumentObserverLambda <T>::DocumentObserverLambda (std::function <void (T &)> func)
:  _func (func)
{
}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : notify_document_changed
==============================================================================
*/

template <class T>
void  DocumentObserverLambda <T>::notify_document_changed (Object & object)
{
   auto & root = dynamic_cast <T &> (object);

   _func (root);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_DocumentObserverLambda_CODEHEADER_INCLUDED

#undef flip_DocumentObserverLambda_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

