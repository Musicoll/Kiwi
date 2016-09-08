/*****************************************************************************

      DocumentObserverVoid.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/DocumentObserverBase.h"



namespace flip
{



class DocumentObserverVoid
:  public DocumentObserverBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DocumentObserverVoid () = default;
   virtual        ~DocumentObserverVoid () = default;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // DocumentObserverBase
   virtual void   notify_document_changed (Object & /* object */) override {}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentObserverVoid (const DocumentObserverVoid & rhs) = delete;
   DocumentObserverVoid &
                  operator = (const DocumentObserverVoid & rhs) = delete;
                  DocumentObserverVoid (DocumentObserverVoid && rhs) = delete;
   DocumentObserverVoid &
                  operator = (DocumentObserverVoid && rhs) = delete;
   bool           operator == (const DocumentObserverVoid & rhs) const = delete;
   bool           operator != (const DocumentObserverVoid & rhs) const = delete;



}; // class DocumentObserverVoid



}  // namespace flip



//#include  "flip/DocumentObserverVoid.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

