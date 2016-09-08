/*****************************************************************************

      MulticastServiceSocket.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/contrib/Socket.h"

#include <vector>

#include <cstdint>



namespace flip
{



class flip_API MulticastServiceSocket
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  MulticastServiceSocket (uint32_t multicast_ip, uint16_t port);
   virtual        ~MulticastServiceSocket ();

   bool           send (uint32_t ip, uint16_t port, const std::vector <uint8_t> & data);
   bool           has_receive ();
   bool           receive (uint32_t & ip, uint16_t & port, std::vector <uint8_t> & data);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:




/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   Socket::Id     _sid = Socket::invalid_id;
   uint32_t       _multicast_ip = 0;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  MulticastServiceSocket () = delete;
                  MulticastServiceSocket (const MulticastServiceSocket & rhs) = delete;
   MulticastServiceSocket &
                  operator = (const MulticastServiceSocket & rhs) = delete;
                  MulticastServiceSocket (MulticastServiceSocket && rhs) = delete;
   MulticastServiceSocket &
                  operator = (MulticastServiceSocket && rhs) = delete;
   bool           operator == (const MulticastServiceSocket & rhs) const = delete;
   bool           operator != (const MulticastServiceSocket & rhs) const = delete;



}; // class MulticastServiceSocket



}  // namespace flip



//#include  "flip/MulticastServiceSocket.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

