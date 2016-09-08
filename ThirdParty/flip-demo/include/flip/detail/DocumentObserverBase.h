/*****************************************************************************

      DocumentObserverBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"


namespace flip
{



class Object;

class flip_API_EXPORT DocumentObserverBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                  DocumentObserverBase () = default;
                  DocumentObserverBase (const DocumentObserverBase & rhs) = default;
   virtual        ~DocumentObserverBase () = default;

   virtual void   notify_document_changed (Object & object) = 0;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentObserverBase (DocumentObserverBase && rhs) = delete;
   DocumentObserverBase &
                  operator = (const DocumentObserverBase & rhs) = delete;
   DocumentObserverBase &
                  operator = (DocumentObserverBase && rhs) = delete;
   bool           operator == (const DocumentObserverBase & rhs) const = delete;
   bool           operator != (const DocumentObserverBase & rhs) const = delete;



}; // class DocumentObserverBase



}  // namespace flip



//#include  "flip/DocumentObserverBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

