/*****************************************************************************

      KeyFloat.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <vector>

#include <cstddef>
#include <cstdint>



namespace flip
{



class StreamBinIn;
class StreamBinOut;

class flip_API KeyFloat
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef std::vector <uint8_t> value_type;

   static KeyFloat
                  generate (KeyFloat left, const KeyFloat & right);

                  KeyFloat () = default;
                  KeyFloat (const KeyFloat & rhs) = default;
                  KeyFloat (KeyFloat && rhs);
                  KeyFloat (const value_type & data);
   virtual        ~KeyFloat () = default;

   KeyFloat &     operator = (const KeyFloat & rhs) = default;
   KeyFloat &     operator = (KeyFloat && rhs);

   bool           operator < (const KeyFloat & rhs) const;
   bool           operator == (const KeyFloat & rhs) const;
   bool           operator != (const KeyFloat & rhs) const;

   value_type     get () const;

   void           write (StreamBinOut & sbo) const;
   void           read (StreamBinIn & sbi);

   static const KeyFloat
                  zero;
   static const KeyFloat
                  one;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   bool           is_valid () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   static KeyFloat
                  make_one ();
   void           update_nbr_bits ();

   size_t         _nbr_bits = 0;
   std::vector <uint8_t>
                  _data;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class KeyFloat



}  // namespace flip



//#include  "flip/KeyFloat.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

