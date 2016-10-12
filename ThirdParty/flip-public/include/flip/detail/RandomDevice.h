/*****************************************************************************

      RandomDevice.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <array>
#include <random>
#include <stdexcept>

#include <cstdint>






namespace flip
{



class flip_API RandomDevice
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static RandomDevice &
                  use ();

   virtual        ~RandomDevice ();

   template <std::size_t SIZE>
   void           get (std::array <uint8_t, SIZE> & array);
   uint8_t        get ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  RandomDevice ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   std::mt19937   _engine;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  RandomDevice (const RandomDevice & rhs) = delete;
   RandomDevice & operator = (const RandomDevice & rhs) = delete;
                  RandomDevice (RandomDevice && rhs) = delete;
   RandomDevice & operator = (RandomDevice && rhs) = delete;
   bool           operator == (const RandomDevice & rhs) const = delete;
   bool           operator != (const RandomDevice & rhs) const = delete;



}; // class RandomDevice



}  // namespace flip



#include "flip/detail/RandomDevice.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

