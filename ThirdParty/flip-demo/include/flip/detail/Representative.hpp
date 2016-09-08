/*****************************************************************************

      Representative.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Representative_CURRENT_CODEHEADER)
   #error Recursive inclusion of Representative code header.
#endif
#define  flip_Representative_CURRENT_CODEHEADER

#if ! defined (flip_Representative_CODEHEADER_INCLUDED)
#define  flip_Representative_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : set
==============================================================================
*/

template <class MemberType, class Class, MemberType Class::*member>
void  Representative::set ()
{
   _stub = &proc <MemberType, Class, member>;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : proc
==============================================================================
*/

template <class MemberType, class Class, MemberType Class::*member>
Type &   Representative::proc (Type & obj)
{
   auto & cast = dynamic_cast <Class &> (obj);

   return cast.*member;
}



}  // namespace flip



#endif   // flip_Representative_CODEHEADER_INCLUDED

#undef flip_Representative_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

