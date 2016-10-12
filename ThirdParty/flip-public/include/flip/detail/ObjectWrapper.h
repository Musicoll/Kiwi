/*****************************************************************************

      ObjectWrapper.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/ElementState.h"

#include <memory>



namespace flip
{



template <class T>
class ObjectWrapper
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef T value_type;
   typedef T & reference;
   typedef T * pointer;

                  ObjectWrapper () = default;
                  ObjectWrapper (std::shared_ptr <T> obj_sptr);
   virtual        ~ObjectWrapper () = default;

                  ObjectWrapper (const ObjectWrapper & rhs) = default;
   ObjectWrapper &
                  operator = (const ObjectWrapper & rhs) = default;
                  ObjectWrapper (ObjectWrapper && rhs);
   ObjectWrapper &
                  operator = (ObjectWrapper && rhs);

   T &            get () const;
   bool           resident () const;
   bool           added () const;
   bool           removed () const;

   std::shared_ptr <T>
                  _obj_sptr;
   ElementState   _state;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const ObjectWrapper & rhs) const = delete;
   bool           operator != (const ObjectWrapper & rhs) const = delete;



}; // class ObjectWrapper



}  // namespace flip



#include "flip/detail/ObjectWrapper.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

