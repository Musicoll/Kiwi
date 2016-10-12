/*****************************************************************************

      StreamBinIn.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <list>
#include <map>
#include <string>
#include <vector>

#include <cstdint>



namespace flip
{



/* WARNING

This class is not exception safe.
If an instance throws an exception, manipulating it afterward
has undefined behaviors.

*/

class flip_API StreamBinIn
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  StreamBinIn (const std::vector <uint8_t> & data);
                  StreamBinIn (const uint8_t * data_ptr, size_t data_size);
   virtual        ~StreamBinIn () = default;

   StreamBinIn & operator >> (bool & val);
   StreamBinIn & operator >> (char & val);
   StreamBinIn & operator >> (uint8_t & val);
   StreamBinIn & operator >> (int16_t & val);
   StreamBinIn & operator >> (uint16_t & val);
   StreamBinIn & operator >> (int32_t & val);
   StreamBinIn & operator >> (uint32_t & val);
   StreamBinIn & operator >> (int64_t & val);
   StreamBinIn & operator >> (uint64_t & val);
   StreamBinIn & operator >> (float & val);
   StreamBinIn & operator >> (double & val);

   StreamBinIn & operator >> (std::string & val);
   StreamBinIn & operator >> (std::vector <uint8_t> & val);

   template <class T>
   StreamBinIn & operator >> (std::vector <T> & val);
   template <class T>
   StreamBinIn & operator >> (std::list <T> & val);
   template <class T, class U>
   StreamBinIn & operator >> (std::map <T, U> & val);
   template <class T, class U>
   StreamBinIn & operator >> (std::pair <T, U> & val);

   template <class InputIterator>
   void           read (InputIterator it, InputIterator it_end);

   template <class T>
   StreamBinIn & operator >> (T & val);

   const uint8_t *
                  pull (size_t data_size);

   bool           is_eos () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   const uint8_t * const
                  _data_ptr;
   const size_t   _data_size;
   size_t         _pos = 0;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  StreamBinIn () = delete;
                  StreamBinIn (const StreamBinIn & rhs) = delete;
   StreamBinIn &  operator = (const StreamBinIn & rhs) = delete;
                  StreamBinIn (StreamBinIn && rhs) = delete;
   StreamBinIn &  operator = (StreamBinIn && rhs) = delete;
   bool           operator == (const StreamBinIn & rhs) const = delete;
   bool           operator != (const StreamBinIn & rhs) const = delete;



}; // class StreamBinIn



}  // namespace flip



#include "flip/StreamBinIn.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

