/*****************************************************************************

      LockFreeQueue.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_LockFreeQueue_CURRENT_CODEHEADER)
   #error Recursive inclusion of LockFreeQueue code header.
#endif
#define  flip_LockFreeQueue_CURRENT_CODEHEADER

#if ! defined (flip_LockFreeQueue_CODEHEADER_INCLUDED)
#define  flip_LockFreeQueue_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <typename T>
LockFreeQueue <T>::LockFreeQueue ()
{
   _first_ptr = new Node (T ());

   _divider_ptr = _first_ptr;
   _last_ptr = _first_ptr;
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <typename T>
LockFreeQueue <T>::~LockFreeQueue ()
{
   while (_first_ptr != nullptr)
   {
      auto tmp_ptr = _first_ptr;
      _first_ptr = tmp_ptr->_next_ptr;
      delete tmp_ptr;
   }
}



/*
==============================================================================
Name : push
Description :
   Push 'val' into 'this'
==============================================================================
*/

template <typename T>
void  LockFreeQueue <T>::push (const T & val)
{
   _last_ptr.load ()->_next_ptr = new Node (val);
   _last_ptr = _last_ptr.load ()->_next_ptr;

   // trim
   while (_first_ptr != _divider_ptr.load ())
   {
      auto tmp_ptr = _first_ptr;
      _first_ptr = _first_ptr->_next_ptr;
      delete tmp_ptr;
   }
}



/*
==============================================================================
Name : pop
==============================================================================
*/

template <typename T>
T *   LockFreeQueue <T>::pop ()
{
   if (_divider_ptr.load () != _last_ptr.load ())
   {
      auto ret_ptr = &_divider_ptr.load ()->_next_ptr->_val;
      _divider_ptr = _divider_ptr.load ()->_next_ptr;

      return ret_ptr;
   }

   return nullptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_LockFreeQueue_CODEHEADER_INCLUDED

#undef flip_LockFreeQueue_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

