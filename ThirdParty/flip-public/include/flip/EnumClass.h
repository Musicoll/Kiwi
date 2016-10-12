/*****************************************************************************

      EnumClass.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/EnumClassBase.h"



namespace flip
{



template <class T> class Collection;
class Default;

template <class T>
class EnumClass
:  public EnumClassBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   using EnumClassBase::name;

                  EnumClass () = default;
   virtual        ~EnumClass () = default; // COV_NF_LINE

   static EnumClass & declare ();

   EnumClass &    name (const char * name_0);
   template <T val>
   EnumClass &    enumerator (const char * name_0);

   static EnumClass &
                  use ();
   static const EnumClass &
                  get ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EnumClass (const EnumClass & rhs) = delete;
   EnumClass &    operator = (const EnumClass & rhs) = delete;
                  EnumClass (EnumClass && rhs) = delete;
   EnumClass &    operator = (EnumClass && rhs) = delete;
   bool           operator == (const EnumClass & rhs) const = delete;
   bool           operator != (const EnumClass & rhs) const = delete;



}; // class EnumClass



}  // namespace flip



#include "flip/EnumClass.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

