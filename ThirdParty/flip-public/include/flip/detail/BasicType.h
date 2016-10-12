/*****************************************************************************

      BasicType.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/Type.h"



namespace flip
{



class flip_API BasicType
:  public Type
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static void    declare ();

                  BasicType () = default;
                  BasicType (const BasicType & rhs) = default;
                  BasicType (BasicType && rhs);
   virtual        ~BasicType () = default;

   BasicType &    operator = (const BasicType & rhs) = default;
   BasicType &    operator = (BasicType && rhs);

   static const char * const
                  _name_bool_0;
   static const char * const
                  _name_int_0;
   static const char * const
                  _name_float_0;
   static const char * const
                  _name_enum_0;
   static const char * const
                  _name_object_ref_0;
   static const char * const
                  _name_blob_0;
   static const char * const
                  _name_collection_0;
   static const char * const
                  _name_array_0;
   static const char * const
                  _name_message_0;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // Type
   virtual void   impl_change_parent (Type * parent_ptr) override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   static bool    _declared_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const BasicType & rhs) const = delete;
   bool           operator != (const BasicType & rhs) const = delete;



}; // class BasicType



}  // namespace flip



//#include  "flip/BasicType.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

