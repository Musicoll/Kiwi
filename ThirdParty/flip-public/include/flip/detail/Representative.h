/*****************************************************************************

      Representative.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"


namespace flip
{



class Type;

class flip_API Representative
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  Representative () = default;
                  Representative (const Representative & rhs) = default;
                  Representative (Representative && rhs);
   virtual        ~Representative () = default;

   Representative &
                  operator = (const Representative & rhs) = default;
   Representative &
                  operator = (Representative && rhs);

   template <class MemberType, class Class, MemberType Class::*member>
   void           set ();

   Type &         from (Type & obj) const;
   const Type &   cfrom (const Type & obj) const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef Type & (*StubType) (Type &);

   template <class MemberType, class Class, MemberType Class::*member>
   static Type &  proc (Type & obj);

   StubType       _stub = 0;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Representative & rhs) const = delete;
   bool           operator != (const Representative & rhs) const = delete;



}; // class Representative



}  // namespace flip



#include "flip/detail/Representative.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

