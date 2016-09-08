/*****************************************************************************

      specs.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_specs_CURRENT_CODEHEADER)
   #error Recursive inclusion of specs code header.
#endif
#define  flip_specs_CURRENT_CODEHEADER

#if ! defined (flip_specs_CODEHEADER_INCLUDED)
#define  flip_specs_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : state
==============================================================================
*/

template <typename Iterator>
SpecsCheck::State  SpecsCheck::state (const Iterator & it)
{
   State state = State::None;

   if (it.resident ())
   {
      state = State::Resident;
   }
   else if (it.added ())
   {
      state = State::Added;
   }
   else if (it.removed ())
   {
      state = State::Removed;
   }

   assert (state != State::None);

   return state;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_specs_CODEHEADER_INCLUDED

#undef flip_specs_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

