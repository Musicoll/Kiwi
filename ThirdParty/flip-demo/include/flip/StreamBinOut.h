/*****************************************************************************

      StreamBinOut.h
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



class flip_API StreamBinOut
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                  StreamBinOut (std::vector <uint8_t> & data);
   virtual        ~StreamBinOut () = default;

   std::vector <uint8_t> &
                  data () const;

   StreamBinOut & operator << (bool val);
   StreamBinOut & operator << (char val);
   StreamBinOut & operator << (uint8_t val);
   StreamBinOut & operator << (int16_t val);
   StreamBinOut & operator << (uint16_t val);
   StreamBinOut & operator << (int32_t val);
   StreamBinOut & operator << (uint32_t val);
   StreamBinOut & operator << (int64_t val);
   StreamBinOut & operator << (uint64_t val);
   StreamBinOut & operator << (float val);
   StreamBinOut & operator << (double val);

   StreamBinOut & operator << (const std::string & val);
   StreamBinOut & operator << (const std::vector <uint8_t> & val);

   template <class T>
   StreamBinOut & operator << (const std::vector <T> & val);
   template <class T>
   StreamBinOut & operator << (const std::list <T> & val);
   template <class T, class U>
   StreamBinOut & operator << (const std::map <T, U> & val);
   template <class T, class U>
   StreamBinOut & operator << (const std::pair <T, U> & val);

   template <class InputIterator>
   void           write (InputIterator it, InputIterator it_end);

   template <class T>
   StreamBinOut & operator << (const T & val);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   std::vector <uint8_t> &
                  _data;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  StreamBinOut () = delete;
                  StreamBinOut (const StreamBinOut & rhs) = delete;
   StreamBinOut & operator = (const StreamBinOut & rhs) = delete;
                  StreamBinOut (StreamBinOut && rhs) = delete;
   StreamBinOut & operator = (StreamBinOut && rhs) = delete;
   bool           operator == (const StreamBinOut & rhs) const = delete;
   bool           operator != (const StreamBinOut & rhs) const = delete;



}; // class StreamBinOut



}  // namespace flip



#include "flip/StreamBinOut.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

