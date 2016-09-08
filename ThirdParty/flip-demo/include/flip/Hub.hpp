/*****************************************************************************

      Hub.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_Hub_CURRENT_CODEHEADER)
   #error Recursive inclusion of Hub code header.
#endif
#define  flip_Hub_CURRENT_CODEHEADER

#if ! defined (flip_Hub_CODEHEADER_INCLUDED)
#define  flip_Hub_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Ref.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class... Ports>
Hub::Hub (const DataModelBase & data_model, uint64_t user_id, Ports &... ports)
:  _document (data_model, user_id, flip_CC4 ('f', 'l', 'i', 'p'), flip_CC4 ('h', 'u', 'b', ' '))
{
   add (ports...);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : add
==============================================================================
*/

template <class... Ports>
void  Hub::add (PortBase & port, Ports &... ports)
{
   add (port);
   add (ports...);
}



}  // namespace flip



#endif   // flip_Hub_CODEHEADER_INCLUDED

#undef flip_Hub_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

