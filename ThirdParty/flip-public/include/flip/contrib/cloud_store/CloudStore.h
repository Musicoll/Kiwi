/*****************************************************************************

      CloudStore.h
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



class flip_API CloudStore
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class Object
   {
   public:
      bool        operator < (const Object & rhs) const {return key < rhs.key;}

      std::string key;
      uint64_t    timestamp = 0ULL;
   };

                  CloudStore () = default;
                  CloudStore (const CloudStore & rhs) = default;
   virtual        ~CloudStore () = default;

   virtual void   turn_async () = 0;
   virtual std::set <Object>
                  fetch () = 0;
   virtual void   put (const std::string & key, const std::vector <uint8_t> & data) = 0;
   virtual std::vector <uint8_t>
                  get (const std::string & key) = 0;
   virtual void   remove (const std::string & key) = 0;

   virtual void   process () = 0;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   CloudStore &   operator = (const CloudStore & rhs) = delete;
                  CloudStore (CloudStore && rhs) = delete;
   CloudStore &   operator = (CloudStore && rhs) = delete;
   bool           operator == (const CloudStore & rhs) const = delete;
   bool           operator != (const CloudStore & rhs) const = delete;



}; // class CloudStore



}  // namespace flip



//#include  "flip/CloudStore.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

