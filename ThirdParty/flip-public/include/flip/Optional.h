/*****************************************************************************

      Optional.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Collection.h"
#include "flip/detail/def.h"
#include "flip/detail/TypeTraits.h"

#include <memory>


namespace flip
{

flip_DISABLE_WARNINGS_DOMINANCE

class Transaction;

template <class T>
class Optional
:  public Collection <T>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef T value_type;
   typedef T & reference;
   typedef const T & const_reference;
   typedef T * pointer;
   typedef const T * const_pointer;

                  Optional () = default;
                  Optional (const Optional & rhs);
                  Optional (Optional && rhs);
                  Optional (const T & rhs);
   virtual        ~Optional () = default;

   Optional &     operator = (const Optional & rhs);
   Optional &     operator = (Optional && rhs);
   template <class U>
   Optional &     operator = (const U & rhs);
   template <class U>
   Optional &     operator = (std::unique_ptr <U> && rhs);

   explicit       operator bool () const;
   bool           empty () const;

   void           reset (None);

   template <class... Args>
   typename std::enable_if <!is_mold <Args...>::value, T &
   >::type        reset (Args &&... args);
   template <class U, class... Args>
   typename std::enable_if <!is_mold <Args...>::value, U &
   >::type        reset (Args &&... args);

   T &            reset (const Mold & mold);
   template <class U>
   U &            reset (const Mold & mold);

                  operator T & () const;
   T &            use () const;
   template <class U>
   U &            use () const;
   T &            value () const;
   template <class U>
   U &            value () const;
   T &            before () const;
   template <class U>
   U &            before () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // Type
   inline virtual void
                  impl_validate () override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   friend class Representative;

   bool           is_valid () const;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Optional & rhs) const = delete;
   bool           operator != (const Optional & rhs) const = delete;



}; // class Optional

flip_RESTORE_WARNINGS

}  // namespace flip



#include "flip/Optional.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

