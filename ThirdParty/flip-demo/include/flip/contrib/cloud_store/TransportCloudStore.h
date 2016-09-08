/*****************************************************************************

      TransportCloudStore.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Document.h"
#include "flip/contrib/cloud_store/CloudStore.h"
#include "flip/detail/TransportBase.h"

#include <string>

#include <cstdint>



namespace flip
{



class DataModelBase;
class DocumentBase;

class flip_API TransportCloudStore
:  public TransportBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static void    create (CloudStore & store, const DataModelBase & data_model);

                  TransportCloudStore (TransportListener & listener, DocumentBase & document, CloudStore & store);
   virtual        ~TransportCloudStore ();

   void           limit_rate (std::chrono::seconds rate);

   void           process ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // TransportBase
   virtual void   send (const std::vector <uint8_t> & data) override;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   using Object = CloudStore::Object;

   class CompareTx
   {
   public:
      bool        operator () (const Object & lhs, const Object & rhs) const;
   };

   void           process_receive ();
   void           process_receive_init (std::set <Object> & object_set);
   void           process_receive_tx (std::set <Object> & object_set);
   void           process_receive_execute ();
   void           process_receive_execute (const std::vector <uint8_t> & data);
   void           process_receive_execute_tx (const std::vector <uint8_t> & data);

   void           process_send ();

   CloudStore &   _store;
   Document       _document;
   const uint64_t _user_id;

   std::chrono::seconds
                  _rate_limit = std::chrono::seconds (10);

   bool           _inited_flag = false;

   std::chrono::system_clock::time_point
                  _last_sent_point; // clock epoch
   uint64_t       _txs_send_nbr = 1;
   std::list <std::vector <uint8_t>>
                  _data_send;

   std::set <Object>
                  _object_set;
   std::map <Object, std::vector <uint8_t>, CompareTx>
                  _tx_map;

   uint64_t       _store_time_remote = 0ULL;
   std::chrono::system_clock::time_point
                  _store_time_local;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  TransportCloudStore () = delete;
                  TransportCloudStore (const TransportCloudStore & rhs) = delete;
   TransportCloudStore &
                  operator = (const TransportCloudStore & rhs) = delete;
                  TransportCloudStore (TransportCloudStore && rhs) = delete;
   TransportCloudStore &
                  operator = (TransportCloudStore && rhs) = delete;
   bool           operator == (const TransportCloudStore & rhs) const = delete;
   bool           operator != (const TransportCloudStore & rhs) const = delete;



}; // class TransportCloudStore



}  // namespace flip



//#include  "flip/TransportCloudStore.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

