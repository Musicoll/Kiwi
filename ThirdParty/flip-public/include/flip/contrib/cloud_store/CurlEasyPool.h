/*****************************************************************************

      CurlEasyPool.h
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

class flip_API CurlEasyPool
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  CurlEasyPool () = default;
   virtual        ~CurlEasyPool () = default;

   std::unique_ptr <CurlEasy>
                  take ();
   static void    give_back (std::unique_ptr <CurlEasy> curl_uptr);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   void           impl_give_back (std::unique_ptr <CurlEasy> curl_uptr);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   std::list <std::unique_ptr <CurlEasy>>
                  _pool;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CurlEasyPool (const CurlEasyPool & rhs) = delete;
   CurlEasyPool &operator = (const CurlEasyPool & rhs) = delete;
                  CurlEasyPool (CurlEasyPool && rhs) = delete;
   CurlEasyPool &operator = (CurlEasyPool && rhs) = delete;
   bool           operator == (const CurlEasyPool & rhs) const = delete;
   bool           operator != (const CurlEasyPool & rhs) const = delete;



}; // class CurlEasyPool



}  // namespace flip



//#include  "flip/CurlEasyPool.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

