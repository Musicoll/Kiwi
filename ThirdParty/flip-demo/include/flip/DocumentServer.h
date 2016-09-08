/*****************************************************************************

      DocumentServer.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/def.h"
#include "flip/detail/DocumentBase.h"
#include "flip/detail/PortFactoryListener.h"
#include "flip/detail/PortListener.h"

#include <functional>
#include <set>



namespace flip
{



class DocumentValidatorBase;
class PortBase;
class Transaction;

class flip_API DocumentServer
:  public DocumentBase
,  public PortFactoryListener
,  public PortListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DocumentServer (const DataModelBase & data_model, DocumentValidatorBase & validator, uint64_t session_id);
   virtual        ~DocumentServer ();

   void           listen_connecting (std::function <void (PortBase &)> func);
   void           listen_connected (std::function <void (PortBase &)> func);
   void           listen_disconnected (std::function <void (PortBase &)> func);

   template <class UnaryPredicate>
   void           send_tx_if (const Transaction & tx, UnaryPredicate p);

   void           reply_signal (const SignalData & data);
   template <class UnaryPredicate>
   void           send_signal_if (const SignalData & data, UnaryPredicate p);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   const std::set <PortBase *> &
                  ports () const;

   // PortFactoryListener
   virtual void   port_factory_add (PortBase & port) override;
   virtual void   port_factory_remove (PortBase & port) override;

   // PortListener
   virtual void   port_greet (PortBase & from) override;
   virtual void   port_commit (PortBase & from, const Transaction & tx) override;
   virtual void   port_squash (PortBase & from, const TxIdRange & range, const Transaction & tx) override;
   virtual void   port_push (PortBase & from, const Transaction & tx) override;
   virtual void   port_signal (PortBase & from, const SignalData & data) override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class AutoCurrentPort
   {
   public:
                  AutoCurrentPort (DocumentServer & parent, PortBase & port);
      virtual     ~AutoCurrentPort ();

   private:
     DocumentServer &
                  _parent;

                  AutoCurrentPort () = delete;
                  AutoCurrentPort (const AutoCurrentPort & rhs) = delete;
      AutoCurrentPort &
                  operator = (const AutoCurrentPort & rhs) = delete;
                  AutoCurrentPort (AutoCurrentPort && rhs) = delete;
      AutoCurrentPort &
                  operator = (AutoCurrentPort && rhs) = delete;
   };

   friend class AutoCurrentPort;

   std::set <PortBase *>
                  _ports;

   PortBase *     _current_port_ptr = nullptr;

   std::function <void (PortBase &)>
                  _func_connecting;
   std::function <void (PortBase &)>
                  _func_connected;
   std::function <void (PortBase &)>
                  _func_disconnected;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentServer () = delete;
                  DocumentServer (const DocumentServer & rhs) = delete;
   DocumentServer &  operator = (const DocumentServer & rhs) = delete;
                  DocumentServer (DocumentServer && rhs) = delete;
   DocumentServer &  operator = (DocumentServer && rhs) = delete;
   bool           operator == (const DocumentServer & rhs) const = delete;
   bool           operator != (const DocumentServer & rhs) const = delete;



}; // class DocumentServer



}  // namespace flip



#include "flip/DocumentServer.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

