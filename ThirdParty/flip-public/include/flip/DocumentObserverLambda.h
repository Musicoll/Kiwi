/*****************************************************************************

      DocumentObserverLambda.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/DocumentObserverBase.h"

#include <functional>



namespace flip
{



template <class T>
class flip_API_EXPORT DocumentObserverLambda
:  public DocumentObserverBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DocumentObserverLambda (std::function <void (T &)> func);
   virtual        ~DocumentObserverLambda () = default;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // DocumentObserverLambdaBase
   virtual void   notify_document_changed (Object & object) override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   std::function <void (T &)>
                  _func;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentObserverLambda () = delete;
                  DocumentObserverLambda (const DocumentObserverLambda & rhs) = delete;
   DocumentObserverLambda &
                  operator = (const DocumentObserverLambda & rhs) = delete;
                  DocumentObserverLambda (DocumentObserverLambda && rhs) = delete;
   DocumentObserverLambda &
                  operator = (DocumentObserverLambda && rhs) = delete;
   bool           operator == (const DocumentObserverLambda & rhs) const = delete;
   bool           operator != (const DocumentObserverLambda & rhs) const = delete;



}; // class DocumentObserverLambda



}  // namespace flip



#include "flip/DocumentObserverLambda.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

