/*****************************************************************************

      BackEndMlCodec.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <string>
#include <vector>

#include <cstdint>



namespace flip
{



class flip_API BackEndMlCodec
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~BackEndMlCodec () = default;

   static std::string
                  encode_escaped_ascii (const std::vector <uint8_t> & data);
   static std::string
                  encode_escaped_ascii_bare (const std::vector <uint8_t> & data);
   static std::string
                  encode_base64 (const std::vector <uint8_t> & data);
   static std::string
                  encode_base64_bare (const std::vector <uint8_t> & data);
   static char    encode_base64 (uint8_t data);
   static std::string
                  encode_hexadecimal (const std::vector <uint8_t> & data);
   static std::string
                  encode_hexadecimal_bare (const std::vector <uint8_t> & data);
   static std::string
                  encode_hexadecimal (uint8_t data);

   static std::vector <uint8_t>
                  decode_escaped_ascii (const std::string & str);
   static std::vector <uint8_t>
                  decode_base64 (const std::string & str);
   static uint8_t decode_base64 (char c);
   static std::vector <uint8_t>
                  decode_hexadecimal (const std::string & str);
   static uint8_t decode_hexadecimal (char c0, char c1);
   static uint8_t decode_hexadecimal (char c);

   static const std::string
                  escaped_ascii;
   static const std::string
                  base64;
   static const std::string
                  hexadecimal;

   static size_t  base64_threshold; // can be changed



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BackEndMlCodec () = delete;
                  BackEndMlCodec (const BackEndMlCodec & rhs) = delete;
   BackEndMlCodec &
                  operator = (const BackEndMlCodec & rhs) = delete;
                  BackEndMlCodec (BackEndMlCodec && rhs) = delete;
   BackEndMlCodec &
                  operator = (BackEndMlCodec && rhs) = delete;
   bool           operator == (const BackEndMlCodec & rhs) const = delete;
   bool           operator != (const BackEndMlCodec & rhs) const = delete;



}; // class BackEndMlCodec



}  // namespace flip



//#include  "flip/BackEndMlCodec.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

