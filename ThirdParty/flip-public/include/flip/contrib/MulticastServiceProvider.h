/*****************************************************************************

      MulticastServiceProvider.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/contrib/MulticastServiceSocket.h"
#include "flip/detail/fnc.h"

#include <memory>
#include <map>
#include <string>

#include <cstdint>



namespace flip
{



class DocumentBase;

class flip_API MulticastServiceProvider
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  MulticastServiceProvider (uint16_t session_port, DocumentBase & document, std::map <std::string, std::string> metadata);
                  MulticastServiceProvider (uint32_t multicast_ip, uint16_t port_min, uint16_t port_max, uint16_t session_port, DocumentBase & document, std::map <std::string, std::string> metadata);
   virtual        ~MulticastServiceProvider () = default;

   void           process ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   const uint16_t _session_port;
   DocumentBase & _document;
   const std::map <std::string, std::string>
                  _metadata;

   std::unique_ptr <MulticastServiceSocket>
                  _socket_uptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  MulticastServiceProvider () = delete;
                  MulticastServiceProvider (const MulticastServiceProvider & rhs) = delete;
   MulticastServiceProvider &
                  operator = (const MulticastServiceProvider & rhs) = delete;
                  MulticastServiceProvider (MulticastServiceProvider && rhs) = delete;
   MulticastServiceProvider &
                  operator = (MulticastServiceProvider && rhs) = delete;
   bool           operator == (const MulticastServiceProvider & rhs) const = delete;
   bool           operator != (const MulticastServiceProvider & rhs) const = delete;



}; // class MulticastServiceProvider



}  // namespace flip



//#include  "flip/MulticastServiceProvider.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

