/*****************************************************************************

      CurlMulti.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"

#include <string>
#include <vector>

#include <cstdint>

#include <curl/curl.h>



namespace flip
{



class CurlEasy;

class flip_API CurlMulti
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  CurlMulti ();
   virtual        ~CurlMulti ();

   void           push (std::unique_ptr <CurlEasy> curl_uptr);

   void           process ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   CURLM *        _curl_multi_ptr = nullptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CurlMulti (const CurlMulti & rhs) = delete;
   CurlMulti &     operator = (const CurlMulti & rhs) = delete;
                  CurlMulti (CurlMulti && rhs) = delete;
   CurlMulti &     operator = (CurlMulti && rhs) = delete;
   bool           operator == (const CurlMulti & rhs) const = delete;
   bool           operator != (const CurlMulti & rhs) const = delete;



}; // class CurlMulti



}  // namespace flip



//#include  "flip/CurlMulti.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

