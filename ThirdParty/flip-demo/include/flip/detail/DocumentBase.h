/*****************************************************************************

      DocumentBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/BackEndIR.h"
#include "flip/DocumentValidatorVoid.h"
#include "flip/Controller.h"
#include "flip/detail/CarrierListener.h"
#include "flip/detail/Direction.h"
#include "flip/detail/Journal.h"
#include "flip/detail/PullStack.h"
#include "flip/detail/RefAllocator.h"
#include "flip/detail/TransactionStack.h"

#include <list>
#include <map>
#include <string>
#include <tuple>

#include <cstdint>



namespace flip
{



class CarrierBase;
class DataModelBase;
class DataConsumerBase;
class DataProviderBase;
class DocumentValidatorBase;
class Object;
class SignalData;
class Transaction;
class TxId;
class Type;
enum class Direction;

class flip_API DocumentBase
:  public CarrierListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  DocumentBase (const DataModelBase & data_model, DocumentValidatorBase & validator, uint64_t user_id, uint64_t actor_id);
                  DocumentBase (const DataModelBase & data_model, uint64_t user_id, uint64_t actor_id);
   virtual        ~DocumentBase ();

   const DataModelBase &
                  data_model () const;
   uint64_t       user () const;
   uint64_t       actor () const;

   void           set_label (std::string label);
   void           set_metadata (std::string key, std::string value);

   bool           execute_forward (const Transaction & tx, bool undo_mode_flag = false);
   bool           execute_backward (const Transaction & tx, bool undo_mode_flag = false);
   bool           execute_correct_forward (Transaction & tx, bool undo_mode_flag = false);
   bool           execute_correct_backward (Transaction & tx, bool undo_mode_flag = false);

   Transaction    commit ();
   void           revert ();
   Transaction    squash ();
   void           push ();
   void           pull ();

   Controller     controller () const;

   BackEndIR      write ();
   void           read (BackEndIR & backend);

   template <class T>
   T &            root ();
   template <class T>
   const T &      root () const;

   template <class T>
   T &            object (const Ref & ref);
   template <class T>
   const T &      object (const Ref & ref) const;
   template <class T>
   T *            object_ptr (const Ref & ref);
   template <class T>
   const T *      object_ptr (const Ref & ref) const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   RefAllocator & impl_ref_allocator ();

   bool           is_observing () const;

   virtual void   signal (const SignalData & data);

   // CarrierListener
   virtual const DataModelBase &
                  carrier_data_model () override;
   virtual uint64_t
                  carrier_user () override;
   virtual void   carrier_receive_beir (const BackEndIR & backend) override;
   virtual void   carrier_receive_tx_local (const Transaction & tx) override;
   virtual void   carrier_receive_tx_squash (const TxIdRange & range, const Transaction & tx) override;
   virtual void   carrier_receive_tx_ext (const Transaction & tx) override;
   virtual void   carrier_receive_tx_ack (const TxId & tx_id) override;
   virtual void   carrier_receive_tx_deny (const TxId & tx_id) override;
   virtual void   carrier_receive_signal (const SignalData & data) override;

   // called from pull stack
   void           process_beir (BackEndIR & backend);
   void           process_tx_local (Transaction & tx);
   void           process_tx_squash (const TxIdRange & range, Transaction & tx);
   void           process_tx_ext (const Transaction & tx);
   void           process_tx_ack (const TxId & tx_id);
   void           process_tx_deny (const TxId & tx_id);
   void           process_signal (const SignalData & data);
   void           process_notify_observer ();

   void           impl_notify_modification ();

   void           impl_execute_no_error (const Transaction & tx, Direction direction);

   size_t         pending_txs () const;
   const PullStack &
                  pull_stack () const;

   Journal &      journal ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual void   do_notify_observer ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef std::map <std::string, std::string> MetaMap;
   typedef std::list <Transaction> PushStack;

   class AutoStash
   {
   public:
                  AutoStash (DocumentBase & parent);
      virtual     ~AutoStash ();

   private:
      DocumentBase &
                  _parent;

      bool        _has_modification_flag = false;
      MetaMap     _meta_map;
      bool        _hint_tx_flag = false;
      std::tuple <uint64_t, uint64_t, uint64_t>
                  _hint_tx_id;
      Direction   _hint_tx_direction = Direction::FORWARD;
      Transaction _tx;


                  AutoStash () = delete;
                  AutoStash (const AutoStash & rhs) = delete;
      AutoStash & operator = (const AutoStash & rhs) = delete;
                  AutoStash (AutoStash && rhs) = delete;
      AutoStash & operator = (AutoStash && rhs) = delete;
   };

   friend class AutoStash;

   bool           execute (Transaction & tx, Direction direction, bool correct_mode_flag, bool undo_mode_flag, bool no_error_mode_flag);

   void           notify_observer ();

   const DataModelBase &
                  _data_model;
   DocumentValidatorBase &
                  _validator;
   const uint64_t _user_id;
   const uint64_t _actor_id;

   RefAllocator   _allocator;

   //

   Type * const   _root_ptr;

   //

   uint64_t       _tx_nbr_id = 1ULL;
   MetaMap        _meta_map;

   //

   PushStack      _push_stack;
   PullStack      _pull_stack;
   TransactionStack
                  _tx_stack;
   Controller     _controller = Controller::NONE;
   bool           _observing_flag = false;
   bool           _undo_mode_flag = false;

   //

   bool           _hint_tx_flag = false;
   std::tuple <uint64_t, uint64_t, uint64_t>
                  _hint_tx_id;
   Direction      _hint_tx_direction;

   //

   Journal        _journal;

   //

   static DocumentValidatorVoid
                  _validator_void;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DocumentBase () = delete;
                  DocumentBase (const DocumentBase & rhs) = delete;
   DocumentBase & operator = (const DocumentBase & rhs) = delete;
                  DocumentBase (DocumentBase && rhs) = delete;
   DocumentBase & operator = (DocumentBase && rhs) = delete;
   bool           operator == (const DocumentBase & rhs) const = delete;
   bool           operator != (const DocumentBase & rhs) const = delete;



}; // class DocumentBase



}  // namespace flip



#include "flip/detail/DocumentBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

