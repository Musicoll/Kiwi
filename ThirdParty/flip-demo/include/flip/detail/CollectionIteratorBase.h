/*****************************************************************************

      CollectionIteratorBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <iterator>



namespace flip
{



template <class MapIterator>
class CollectionIteratorBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   typedef typename MapIterator::difference_type difference_type;
   typedef typename MapIterator::value_type::second_type::value_type value_type;
   typedef typename MapIterator::value_type::second_type::reference reference;
   typedef typename MapIterator::value_type::second_type::pointer pointer;
   typedef typename std::bidirectional_iterator_tag iterator_category;

                  CollectionIteratorBase () = default;
                  CollectionIteratorBase (MapIterator it);
   template <class U>
                  CollectionIteratorBase (const CollectionIteratorBase <U> & rhs);
   virtual        ~CollectionIteratorBase () = default;

                  CollectionIteratorBase (const CollectionIteratorBase & rhs) = default;
   CollectionIteratorBase &
                  operator = (const CollectionIteratorBase & rhs) = default;
                  CollectionIteratorBase (CollectionIteratorBase && rhs);
   CollectionIteratorBase &
                  operator = (CollectionIteratorBase && rhs);

   CollectionIteratorBase &
                  operator ++ ();
   CollectionIteratorBase
                  operator ++ (int);
   CollectionIteratorBase &
                  operator -- ();
   CollectionIteratorBase
                  operator -- (int);
   reference      operator * () const;
   pointer        operator -> () const;

   bool           operator == (const CollectionIteratorBase & rhs) const;
   bool           operator != (const CollectionIteratorBase & rhs) const;

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



}; // class CollectionIteratorBase



}  // namespace flip



#include "flip/detail/CollectionIteratorBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

