/*****************************************************************************

      Vector.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Blob.h"

#include <vector>



namespace flip
{



template <class T>
class Vector
:  public Blob
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef T value_type;
   typedef T & reference;
   typedef const T & const_reference;
   typedef T * pointer;
   typedef const T * const_pointer;
   typedef std::vector <T> internal_type;
   typedef typename std::vector <T>::const_iterator const_iterator;

                  Vector () = default;
                  Vector (const Vector & rhs);
                  Vector (Vector && rhs);
                  Vector (const std::vector <T> & value);
   virtual        ~Vector () = default;

   Vector &       operator = (const Vector & rhs);
   Vector &       operator = (Vector && rhs);
   Vector &       operator = (const std::vector <T> & rhs);
   Vector &       operator = (std::vector <T> && rhs);

                  operator const std::vector <T> & () const;
   const std::vector <T> &
                  value () const;
   const std::vector <T> &
                  before () const;

   bool           operator == (const std::vector <T> & rhs) const;
   bool           operator != (const std::vector <T> & rhs) const;

   // iterators
   const_iterator begin () const;
   const_iterator cbegin () const;

   const_iterator end () const;
   const_iterator cend () const;

   // modifiers
   void           clear ();
   void           replace (const_iterator it, const_iterator it_end, const std::vector <T> & value);
   template <class InputIterator>
   void           replace (const_iterator it, const_iterator it_end, InputIterator it2, InputIterator it_end2);

   // misc
   virtual void   revert () override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // Blob
   virtual void   impl_internal_set (const std::vector <uint8_t> & value) override;
   void           impl_internal_set (std::vector <uint8_t> && value);

   // Type
   virtual void   impl_synchronize () override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   static std::vector <uint8_t>
                  make_value (const std::vector <T> & elems);
   static std::vector <T>
                  make_vector (const std::vector <uint8_t> & value);

   void           update_value () const;
   void           update_before () const;

   mutable bool   _cached_value_need_update_flag = true;
   mutable std::vector <T>
                  _cached_value;
   mutable bool   _cached_before_value_need_update_flag = true;
   mutable std::vector <T>
                  _cached_before_value;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Vector & rhs) const = delete;
   bool           operator != (const Vector & rhs) const = delete;



}; // class Vector



template <class T>
bool  operator == (const std::vector <T> & lhs, const Vector <T> & rhs);
template <class T>
bool  operator != (const std::vector <T> & lhs, const Vector <T> & rhs);



}  // namespace flip



#include  "flip/Vector.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

