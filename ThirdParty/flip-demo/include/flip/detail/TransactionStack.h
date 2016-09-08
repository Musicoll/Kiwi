/*****************************************************************************

      TransactionStack.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <list>
#include <memory>

#include <cstdint>



namespace flip
{



class DocumentBase;
class Transaction;
class TxId;
class TxIdRange;

class flip_API TransactionStack
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  TransactionStack (DocumentBase & document);
   virtual        ~TransactionStack () = default;

   void           push_local (const Transaction & tx, bool self_flag);
   void           squash (const Transaction & tx, const TxIdRange & range);
   void           external (const Transaction & tx);
   void           acknowledge (const TxId & tx_id);
   void           deny (const TxId & tx_id);
   void           finish ();

   void           reset ();

   size_t         size () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           rollback ();
   void           reexecute ();

   DocumentBase & _document;

   std::list <Transaction>
                  _tx_list;
   bool           _rollbacked_flag = false;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TransactionStack () = delete;
                  TransactionStack (const TransactionStack & rhs) = delete;
   TransactionStack &
                  operator = (const TransactionStack & rhs) = delete;
                  TransactionStack (TransactionStack && rhs) = delete;
   TransactionStack &
                  operator = (TransactionStack && rhs) = delete;
   bool           operator == (const TransactionStack & rhs) const = delete;
   bool           operator != (const TransactionStack & rhs) const = delete;



}; // class TransactionStack



}  // namespace flip



//#include  "flip/TransactionStack.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

