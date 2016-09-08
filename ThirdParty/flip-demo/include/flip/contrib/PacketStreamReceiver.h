/*****************************************************************************

      PacketStreamReceiver.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <functional>
#include <vector>

#include <cstdint>



namespace flip
{



class PacketStreamReceiver
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   using FuncPeek = std::function <void (const uint8_t * data_ptr, size_t data_size, size_t packet_size)>;
   using FuncReceive = std::function <bool (const uint8_t * data_ptr, size_t data_size)>;

                  PacketStreamReceiver () = default;
   virtual        ~PacketStreamReceiver () = default;

   void           bind_peek (FuncPeek func);
   void           bind_receive (FuncReceive func);

   bool           empty () const;

   void           reset ();

   void           push (const std::vector <uint8_t> & data);
   bool           process ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum {         PACKET_HEADER_SIZE = 8  };

   std::vector <uint8_t>
                  _data;
   FuncPeek       _func_peek;
   FuncReceive    _func_receive;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PacketStreamReceiver (const PacketStreamReceiver & rhs) = delete;
   PacketStreamReceiver &
                  operator = (const PacketStreamReceiver & rhs) = delete;
                  PacketStreamReceiver (PacketStreamReceiver && rhs) = delete;
   PacketStreamReceiver &
                  operator = (PacketStreamReceiver && rhs) = delete;
   bool           operator == (const PacketStreamReceiver & rhs) const = delete;
   bool           operator != (const PacketStreamReceiver & rhs) const = delete;



}; // class PacketStreamReceiver



}  // namespace flip



//#include  "flip/PacketStreamReceiver.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

