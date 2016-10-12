/*****************************************************************************

      KeyRandom.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <array>

#include <cstdint>



namespace flip
{



class StreamBinIn;
class StreamBinOut;

class flip_API KeyRandom
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum {         SIZE = 20   };
   typedef std::array <uint8_t, SIZE> value_type;

                  KeyRandom ();
                  KeyRandom (const KeyRandom & rhs) = default;
                  KeyRandom (KeyRandom && rhs);
                  KeyRandom (std::array <uint8_t, SIZE> array);
   virtual        ~KeyRandom () = default;

   KeyRandom &    operator = (const KeyRandom & rhs) = default;
   KeyRandom &    operator = (KeyRandom && rhs);

   bool           operator < (const KeyRandom & rhs) const;
   bool           operator == (const KeyRandom & rhs) const;
   bool           operator != (const KeyRandom & rhs) const;

   value_type     get () const;

   void           write (StreamBinOut & sbo) const;
   void           read (StreamBinIn & sbi);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   value_type     _array;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class KeyRandom



}  // namespace flip



//#include  "flip/KeyRandom.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

