/*****************************************************************************

      PacketStreamPinger.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <chrono>



namespace flip
{



class PacketStreamSender;

class PacketStreamPinger
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PacketStreamPinger (PacketStreamSender & packet_stream);
   virtual        ~PacketStreamPinger () = default;

   void           reset (int period);
   void           process ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   PacketStreamSender &
                  _packet_stream;

   std::chrono::duration <int>
                  _period = std::chrono::duration <int> (2);
   std::chrono::time_point <std::chrono::system_clock>
                  _due_time;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PacketStreamPinger () = delete;
                  PacketStreamPinger (const PacketStreamPinger & rhs) = delete;
   PacketStreamPinger &
                  operator = (const PacketStreamPinger & rhs) = delete;
                  PacketStreamPinger (PacketStreamPinger && rhs) = delete;
   PacketStreamPinger &
                  operator = (PacketStreamPinger && rhs) = delete;
   bool           operator == (const PacketStreamPinger & rhs) const = delete;
   bool           operator != (const PacketStreamPinger & rhs) const = delete;



}; // class PacketStreamPinger



}  // namespace flip



//#include  "flip/PacketStreamPinger.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

