/*****************************************************************************

      MulticastService.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <cstdint>



namespace flip
{



class flip_API MulticastService
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~MulticastService () = default;  // COV_NF_LINE

   static MulticastService &
                  use ();

   void           set_ip (uint32_t ip);
   uint32_t       get_ip () const;

   void           set_port_range (uint16_t port_min, uint16_t port_max);
   uint16_t       get_port_min () const;
   uint16_t       get_port_max () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

                  MulticastService () = default;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   uint32_t       _ip = 0xefeefe89; // 239.238.254.137
   uint16_t       _port_min = 59150;
   uint16_t       _port_max = 59169;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  MulticastService (const MulticastService & rhs) = delete;
   MulticastService &
                  operator = (const MulticastService & rhs) = delete;
                  MulticastService (MulticastService && rhs) = delete;
   MulticastService &
                  operator = (MulticastService && rhs) = delete;
   bool           operator == (const MulticastService & rhs) const = delete;
   bool           operator != (const MulticastService & rhs) const = delete;



}; // class MulticastService



}  // namespace flip



//#include  "flip/MulticastService.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

