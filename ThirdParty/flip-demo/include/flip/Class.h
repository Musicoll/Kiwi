/*****************************************************************************

      Class.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/ClassBase.h"
#include "flip/detail/TypeTraits.h"

#include <type_traits>



namespace flip
{



template <class T> class Collection;
class Default;

template <class T>
class Class
:  public ClassBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   using ClassBase::name;

                  Class () = default;
   virtual        ~Class () = default; // COV_NF_LINE

   static Class & declare ();

   Class &        name (const char * name_0);
   template <class U>
   Class &        inherit ();
   template <class U, U T::*ptr_to_member>
   Class &        member (const char * name_0);

   static Class & use ();
   static const Class &
                  get ();

   // ClassBase
   virtual bool   abstract () const override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // ClassBase
   virtual Type * impl_create (DocumentBase & document, Ref ref) const override;
   virtual void   impl_release (Type * obj_ptr, DocumentBase & document) const override;
   virtual Type * impl_ctor () const override;
   virtual Type * impl_copy_ctor (Type & obj) const override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   template <class U>
   typename std::enable_if <
      is_enum <U>::value, ClassBase &
   >::type        base ();

   template <class U>
   typename std::enable_if <
      is_blob <U>::value, ClassBase &
   >::type        base ();

   template <class U>
   typename std::enable_if <
      is_object_ref <U>::value, ClassBase &
   >::type        base ();

   template <class U>
   typename std::enable_if <
      is_collection <U>::value, ClassBase &
   >::type        base ();

   template <class U>
   typename std::enable_if <
      is_array <U>::value, ClassBase &
   >::type        base ();

   template <class U>
   typename std::enable_if <
      is_message <U>::value, ClassBase &
   >::type        base ();

   template <class U>
   typename std::enable_if <
      ! (is_enum <U>::value || is_blob <U>::value || is_object_ref <U>::value || is_collection <U>::value || is_array <U>::value || is_message <U>::value), ClassBase &
   >::type        base ();

   template <class U>
   typename std::enable_if <
      is_copy_constructible <U>::value, Type *
   >::type        copy_ctor (Type & obj) const;

   template <class U>
   typename std::enable_if <
      ! is_copy_constructible <U>::value, Type *
   >::type        copy_ctor (Type & obj) const;

   template <class U>
   typename std::enable_if <
      std::is_constructible <U, Default &>::value, U *
   >::type        ctor () const;

   template <class U>
   typename std::enable_if <
      (! std::is_constructible <U, Default &>::value) && (! std::is_abstract <U>::value), U *
   >::type        ctor () const;

   template <class U>
   typename std::enable_if <
      std::is_abstract <U>::value, U *
   >::type        ctor () const;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Class (const Class & rhs) = delete;
   Class &        operator = (const Class & rhs) = delete;
                  Class (Class && rhs) = delete;
   Class &        operator = (Class && rhs) = delete;
   bool           operator == (const Class & rhs) const = delete;
   bool           operator != (const Class & rhs) const = delete;



}; // class Class



}  // namespace flip



#include "flip/Class.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

