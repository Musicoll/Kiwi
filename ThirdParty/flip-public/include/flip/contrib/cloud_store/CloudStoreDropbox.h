/*****************************************************************************

      CloudStoreDropbox.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/contrib/cloud_store/CloudStore.h"
#include "flip/contrib/cloud_store/CurlEasy.h"
#include "flip/contrib/cloud_store/CurlEasyPool.h"
#include "flip/contrib/cloud_store/CurlEasyQueue.h"
#include "flip/contrib/cloud_store/CurlMulti.h"

#include <list>
#include <map>
#include <vector>



namespace flip
{



class flip_API CloudStoreDropbox
:  public CloudStore
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  CloudStoreDropbox (const std::string & access_token);
   virtual        ~CloudStoreDropbox ();

   virtual void   turn_async () override;
   virtual std::set <Object>
                  fetch () override;
   virtual void   put (const std::string & key, const std::vector <uint8_t> & data) override;
   virtual std::vector <uint8_t>
                  get (const std::string & key) override;
   virtual void   remove (const std::string & key) override;

   virtual void   process () override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum QueueName
   {
                  QueueName_PutDelete = 0,
                  QueueName_Get,
                  QueueName_DeltaPoll,

                  QueueName_NBR
   };

   enum PoolName
   {
                  PoolName_Api = 0,
                  PoolName_ApiContent,
                  PoolName_ApiNotify,

                  PoolName_NBR
   };

   void           process_fetch (std::set <Object> & object_set, CurlEasy & curl);
   static uint64_t
                  to_timestamp (const std::string & str);

   std::string    _access_token;

   bool           _sync_flag = true;
   std::string    _cursor;

   CurlEasyQueue  _queue_arr [QueueName_NBR];
   CurlEasyPool   _pool_arr [PoolName_NBR];
   CurlMulti      _curl_multi;

   std::set <Object>
                  _cache_fetch;
   std::map <std::string, std::vector <uint8_t>>
                  _cache_get;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CloudStoreDropbox () = delete;
                  CloudStoreDropbox (const CloudStoreDropbox & rhs) = delete;
   CloudStoreDropbox &
                  operator = (const CloudStoreDropbox & rhs) = delete;
                  CloudStoreDropbox (CloudStoreDropbox && rhs) = delete;
   CloudStoreDropbox &
                  operator = (CloudStoreDropbox && rhs) = delete;
   bool           operator == (const CloudStoreDropbox & rhs) const = delete;
   bool           operator != (const CloudStoreDropbox & rhs) const = delete;



}; // class CloudStoreDropbox



}  // namespace flip



//#include  "flip/CloudStoreDropbox.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

