/*****************************************************************************

      ArrayIteratorBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <iterator>

#include <cstddef>



namespace flip
{



template <class MapIterator>
class ArrayIteratorBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   typedef typename MapIterator::difference_type difference_type;
   typedef typename MapIterator::value_type::second_type::value_type value_type;
   typedef typename MapIterator::value_type::second_type::reference reference;
   typedef typename MapIterator::value_type::second_type::pointer pointer;
   typedef typename std::bidirectional_iterator_tag iterator_category;

                  ArrayIteratorBase () = default;
                  ArrayIteratorBase (MapIterator it);
   template <class U>
                  ArrayIteratorBase (const ArrayIteratorBase <U> & rhs);
   virtual        ~ArrayIteratorBase () = default;

                  ArrayIteratorBase (const ArrayIteratorBase & rhs) = default;
   ArrayIteratorBase &
                  operator = (const ArrayIteratorBase & rhs) = default;
                  ArrayIteratorBase (ArrayIteratorBase && rhs);
   ArrayIteratorBase &
                  operator = (ArrayIteratorBase && rhs);

   ArrayIteratorBase &
                  operator ++ ();
   ArrayIteratorBase
                  operator ++ (int);
   ArrayIteratorBase &
                  operator -- ();
   ArrayIteratorBase
                  operator -- (int);
   reference      operator * () const;
   pointer        operator -> () const;

   bool           operator == (const ArrayIteratorBase & rhs) const;
   bool           operator != (const ArrayIteratorBase & rhs) const;

   const typename MapIterator::value_type::first_type &
                  key () const;
   bool           added () const;
   bool           resident () const;
   bool           removed () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   MapIterator    base () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   MapIterator    _it;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class ArrayIteratorBase



}  // namespace flip



#include "flip/detail/ArrayIteratorBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

