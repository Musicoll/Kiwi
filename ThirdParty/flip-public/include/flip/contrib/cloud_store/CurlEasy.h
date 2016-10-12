/*****************************************************************************

      CurlEasy.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"

#include <functional>
#include <list>
#include <string>
#include <vector>

#include <cstdint>

#include <curl/curl.h>



namespace flip
{



class CurlEasyPool;
class CurlEasyQueue;

class flip_API CurlEasy
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static CurlEasy *
                  get (CURL * curl_ptr);

                  CurlEasy ();
   virtual        ~CurlEasy ();

   void           set_url (const std::string & url);
   const std::string &
                  url () const;

   void           set_method_get ();
   void           set_method_put ();
   void           set_method_post ();

   void           add_header (const std::string & header);

   void           set_body (const std::string & body);
   void           set_body (const std::vector <uint8_t> & data);

   void           bind (CurlEasyPool & pool);
   CurlEasyPool & pool ();

   void           bind (CurlEasyQueue & queue);
   CurlEasyQueue &
                  unbind_queue ();

   void           bind (std::function <void (CurlEasy &)> f);

   CURL *         use ();
   void           process ();

   long           response_code () const;
   const std::vector <uint8_t> &
                  response_body () const;
   std::string    response_body_str () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   void           impl_reset ();
   void           impl_reset_curl ();
   void           impl_prepare ();
   void           impl_post_process ();
   void           impl_call_delegate ();
   bool           impl_need_retry () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           trace_error (CURLcode err);

   static size_t  write_proc (char * ptr, size_t size, size_t nmemb, void * data);
   void           write (char * ptr, size_t size);

   static size_t  read_proc (char * ptr, size_t size, size_t nmemb, void * data);
   size_t         read (char * ptr, size_t size);

   CURL *         _curl_ptr = nullptr;
   CurlEasyPool * _pool_ptr = nullptr;
   CurlEasyQueue *_queue_ptr = nullptr;

   // request
   std::string    _url;
   std::string    _method;
   std::list <std::string>
                  _headers;
   curl_slist *   _header_list_ptr = nullptr;
   std::vector <uint8_t>
                  _body;
   std::vector <uint8_t>
                  _body_send;

   // response
   long           _response_code = 0;
   std::vector <uint8_t>
                  _response_body;
   std::function <void (CurlEasy &)>
                  _delegate;

   // errors
   char           _error_0 [CURL_ERROR_SIZE];



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CurlEasy (const CurlEasy & rhs) = delete;
   CurlEasy &     operator = (const CurlEasy & rhs) = delete;
                  CurlEasy (CurlEasy && rhs) = delete;
   CurlEasy &     operator = (CurlEasy && rhs) = delete;
   bool           operator == (const CurlEasy & rhs) const = delete;
   bool           operator != (const CurlEasy & rhs) const = delete;



}; // class CurlEasy



}  // namespace flip



//#include  "flip/CurlEasy.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

