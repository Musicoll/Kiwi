/*****************************************************************************

      Filesystem.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <set>
#include <string>
#include <vector>

#include <cstdint>



namespace flip
{



class flip_API Filesystem
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~Filesystem () = default;

   static std::vector <uint8_t>
                  read (std::string path);
   static void    write (const std::vector <uint8_t> & data, std::string path);

   static void    rename (std::string src, std::string dst);

   static void    remove (std::string path);

   static std::set <std::string>
                  list (std::string path);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Filesystem () = delete;
                  Filesystem (const Filesystem & rhs) = delete;
   Filesystem &   operator = (const Filesystem & rhs) = delete;
                  Filesystem (Filesystem && rhs) = delete;
   Filesystem &   operator = (Filesystem && rhs) = delete;
   bool           operator == (const Filesystem & rhs) const = delete;
   bool           operator != (const Filesystem & rhs) const = delete;



}; // class Filesystem



}  // namespace flip



//#include  "flip/Filesystem.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

