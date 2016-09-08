/*****************************************************************************

      PullStack.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/BackEndIR.h"
#include "flip/Transaction.h"
#include "flip/detail/SignalData.h"
#include "flip/detail/TxId.h"
#include "flip/detail/TxIdRange.h"



namespace flip
{



class DocumentBase;

class flip_API PullStack
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PullStack (DocumentBase & document);
   virtual        ~PullStack () = default;

   void           push_beir (const BackEndIR & backend);
   void           push_tx_local (const Transaction & tx);
   void           push_tx_squash (const TxIdRange & range, const Transaction & tx);
   void           push_tx_ext (const Transaction & tx);
   void           push_tx_ack (const TxId & tx_id);
   void           push_tx_deny (const TxId & tx_id);
   void           push_signal (const SignalData & data);

   void           process ();

   size_t         size () const;
   bool           empty () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class Elem
   {
   public:
      enum class Type
      {
                  NONE,
                  BACKEND,
                  TX_LOCAL,
                  TX_SQUASH,
                  TX_EXT,
                  TX_ACK,
                  TX_DENY,
                  SIGNAL,
      };

                  Elem (Type type) : _type (type) {}

      Type        _type;
      BackEndIR   _backend;
      Transaction _tx;
      TxId        _tx_id;
      TxIdRange   _tx_id_range;
      SignalData  _signal_data;
   };

   DocumentBase & _document;

   std::list <Elem>
                  _list;
   bool           _processing_guard_flag = false;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PullStack () = delete;
                  PullStack (const PullStack & rhs) = delete;
   PullStack &    operator = (const PullStack & rhs) = delete;
                  PullStack (PullStack && rhs) = delete;
   PullStack &    operator = (PullStack && rhs) = delete;
   bool           operator == (const PullStack & rhs) const = delete;
   bool           operator != (const PullStack & rhs) const = delete;



}; // class PullStack



}  // namespace flip



//#include  "flip/PullStack.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

