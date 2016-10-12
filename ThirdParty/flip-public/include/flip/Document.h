/*****************************************************************************

      Document.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/DocumentBase.h"
#include "flip/DocumentObserverVoid.h"
#include "flip/Transaction.h"

#include <memory>
#include <string>

#include <cstdint>



namespace flip
{



class DocumentObserverBase;

class flip_API Document
:  public DocumentBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  Document (const DataModelBase & data_model, DocumentObserverBase & observer, DocumentValidatorBase & validator, uint64_t user_id, uint32_t manufacturer_id, uint32_t component_id);
                  Document (const DataModelBase & data_model, DocumentObserverBase & observer, uint64_t user_id, uint32_t manufacturer_id, uint32_t component_id);
                  Document (const DataModelBase & data_model, DocumentValidatorBase & validator, uint64_t user_id, uint32_t manufacturer_id, uint32_t component_id);
                  Document (const DataModelBase & data_model, uint64_t user_id, uint32_t manufacturer_id, uint32_t component_id);
   virtual        ~Document ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // DocumentBase
   virtual void   do_notify_observer () override;




/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   DocumentObserverBase &
                  _observer;

   static DocumentObserverVoid
                  _observer_void;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Document () = delete;
                  Document (const Document & rhs) = delete;
   Document &     operator = (const Document & rhs) = delete;
                  Document (Document && rhs) = delete;
   Document &     operator = (Document && rhs) = delete;
   bool           operator == (const Document & rhs) const = delete;
   bool           operator != (const Document & rhs) const = delete;



}; // class Document



}  // namespace flip



//#include  "flip/Document.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

