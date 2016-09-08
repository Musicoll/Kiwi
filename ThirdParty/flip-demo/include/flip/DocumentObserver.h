/*****************************************************************************

      DocumentObserver.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/DocumentObserverBase.h"


namespace flip
{


template <class T>
class flip_API_EXPORT DocumentObserver
:  public DocumentObserverBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DocumentObserver () = default;
   virtual        ~DocumentObserver () = default;

   virtual void   document_changed (T & root) = 0;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // DocumentObserverBase
   virtual void   notify_document_changed (Object & object) override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentObserver (const DocumentObserver & rhs) = delete;
   DocumentObserver &
                  operator = (const DocumentObserver & rhs) = delete;
                  DocumentObserver (DocumentObserver && rhs) = delete;
   DocumentObserver &
                  operator = (DocumentObserver && rhs) = delete;
   bool           operator == (const DocumentObserver & rhs) const = delete;
   bool           operator != (const DocumentObserver & rhs) const = delete;



}; // class DocumentObserver



}  // namespace flip



#include "flip/DocumentObserver.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

