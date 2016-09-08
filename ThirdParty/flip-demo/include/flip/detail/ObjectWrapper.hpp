/*****************************************************************************

      ObjectWrapper.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_ObjectWrapper_CURRENT_CODEHEADER)
   #error Recursive inclusion of ObjectWrapper code header.
#endif
#define  flip_ObjectWrapper_CURRENT_CODEHEADER

#if ! defined (flip_ObjectWrapper_CODEHEADER_INCLUDED)
#define  flip_ObjectWrapper_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
ObjectWrapper <T>::ObjectWrapper (std::shared_ptr <T> obj_sptr)
:  _obj_sptr (obj_sptr)
,  _state (ElementState::ADDED)
{
}



/*
==============================================================================
Name : ctor
Note :
   Support for MSVC 2013u4.
==============================================================================
*/

template <class T>
ObjectWrapper <T>::ObjectWrapper (ObjectWrapper && rhs)
:  _obj_sptr (std::move (rhs._obj_sptr))
,  _state (std::move (rhs._state))
{
}



/*
==============================================================================
Name : operator =
Note :
   Support for MSVC 2013u4.
==============================================================================
*/

template <class T>
ObjectWrapper <T> &  ObjectWrapper <T>::operator = (ObjectWrapper && rhs)
{
   _obj_sptr = std::move (rhs._obj_sptr);
   _state = std::move (rhs._state);

   return *this;
}



/*
==============================================================================
Name : get
==============================================================================
*/

template <class T>
T &   ObjectWrapper <T>::get () const
{
   assert (_obj_sptr.get () != nullptr);

   return *_obj_sptr;
}



/*
==============================================================================
Name : resident
==============================================================================
*/

template <class T>
bool  ObjectWrapper <T>::resident () const
{
   return _state == ElementState::RESIDENT;
}



/*
==============================================================================
Name : added
==============================================================================
*/

template <class T>
bool  ObjectWrapper <T>::added () const
{
   return _state == ElementState::ADDED;
}



/*
==============================================================================
Name : removed
==============================================================================
*/

template <class T>
bool  ObjectWrapper <T>::removed () const
{
   return _state == ElementState::REMOVED;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_ObjectWrapper_CODEHEADER_INCLUDED

#undef flip_ObjectWrapper_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

