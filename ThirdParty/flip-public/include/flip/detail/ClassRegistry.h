/*****************************************************************************

      ClassRegistry.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <typeindex>
#include <typeinfo>
#include <unordered_map>



namespace flip
{



class ClassBase;

class flip_API ClassRegistry
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~ClassRegistry () = default;  // COV_NF_LINE

   static ClassRegistry &
                  use ();

   void           register_ (const std::type_info & typeinfo, ClassBase & class_base);
   ClassBase &    find (const std::type_info & typeinfo);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  ClassRegistry () = default;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   using Map = std::unordered_map <std::type_index, ClassBase *>;

   Map            _map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ClassRegistry (const ClassRegistry & rhs) = delete;
   ClassRegistry &operator = (const ClassRegistry & rhs) = delete;
                  ClassRegistry (ClassRegistry && rhs) = delete;
   ClassRegistry &operator = (ClassRegistry && rhs) = delete;
   bool           operator == (const ClassRegistry & rhs) const;
   bool           operator != (const ClassRegistry & rhs) const;



}; // class ClassRegistry



}  // namespace flip



//#include  "flip/ClassRegistry.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

