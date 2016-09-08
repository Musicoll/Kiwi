/*****************************************************************************

      CurlEasyQueue.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"

#include <list>
#include <memory>



namespace flip
{



class CurlEasy;

class flip_API CurlEasyQueue
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  CurlEasyQueue () = default;
   virtual        ~CurlEasyQueue () = default;

   void           push (std::unique_ptr <CurlEasy> curl_uptr);
   bool           available () const;
   std::unique_ptr <CurlEasy>
                  pop ();
   void           unlock ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   std::list <std::unique_ptr <CurlEasy>>
                  _queue;
   bool           _locked_flag = false;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CurlEasyQueue (const CurlEasyQueue & rhs) = delete;
   CurlEasyQueue &operator = (const CurlEasyQueue & rhs) = delete;
                  CurlEasyQueue (CurlEasyQueue && rhs) = delete;
   CurlEasyQueue &operator = (CurlEasyQueue && rhs) = delete;
   bool           operator == (const CurlEasyQueue & rhs) const = delete;
   bool           operator != (const CurlEasyQueue & rhs) const = delete;



}; // class CurlEasyQueue



}  // namespace flip



//#include  "flip/CurlEasyQueue.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

