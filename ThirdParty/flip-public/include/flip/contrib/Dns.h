/*****************************************************************************

      Dns.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"

#include <string>

#if (flip_PLATFORM == flip_PLATFORM_WIN32)
   #define WIN32_LEAN_AND_MEAN
   #define NOMINMAX
   #define NOGDI
   flip_DISABLE_WARNINGS_SYSTEM_HEADERS
   #include <winsock2.h>	// Must be included before windows.h
   flip_RESTORE_WARNINGS

#else    // flip_PLATFORM
   #include <netdb.h>

#endif   // flip_PLATFORM



namespace flip
{



class Dns
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   using Host = const struct hostent *;

   virtual        ~Dns () = default;

   static Host    get_host (const std::string & name);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  Dns () = default;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Dns (const Dns & rhs) = delete;
   Dns &          operator = (const Dns & rhs) = delete;
                  Dns (Dns && rhs) = delete;
   Dns &          operator = (Dns && rhs) = delete;
   bool           operator == (const Dns & rhs) const = delete;
   bool           operator != (const Dns & rhs) const = delete;



}; // class Dns



}  // namespace flip



//#include  "flip/Dns.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

