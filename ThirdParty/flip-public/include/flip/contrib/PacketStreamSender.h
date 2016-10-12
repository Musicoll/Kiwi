/*****************************************************************************

      PacketStreamSender.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <vector>

#include <cstdint>



namespace flip
{



class PacketStreamSender
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PacketStreamSender () = default;
   virtual        ~PacketStreamSender () = default;

   bool           empty () const;

   void           reset ();

   void           push (const std::vector <uint8_t> & data);
   std::vector <uint8_t> &
                  data ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   std::vector <uint8_t>
                  _data;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PacketStreamSender (const PacketStreamSender & rhs) = delete;
   PacketStreamSender &
                  operator = (const PacketStreamSender & rhs) = delete;
                  PacketStreamSender (PacketStreamSender && rhs) = delete;
   PacketStreamSender &
                  operator = (PacketStreamSender && rhs) = delete;
   bool           operator == (const PacketStreamSender & rhs) const = delete;
   bool           operator != (const PacketStreamSender & rhs) const = delete;



}; // class PacketStreamSender



}  // namespace flip



//#include  "flip/PacketStreamSender.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

