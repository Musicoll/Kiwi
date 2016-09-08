/*****************************************************************************

      CarrierTransportBase.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_CarrierTransportBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of CarrierTransportBase code header.
#endif
#define  flip_CarrierTransportBase_CURRENT_CODEHEADER

#if ! defined (flip_CarrierTransportBase_CODEHEADER_INCLUDED)
#define  flip_CarrierTransportBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Class.h"
#include "flip/detail/CarrierListener.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : enable_signal_passthrough
==============================================================================
*/

template <class Type>
void  CarrierTransportBase::enable_signal_passthrough (uint32_t type)
{
   uint32_t type_id = Class <Type>::get ().type_id (listener ().carrier_data_model ());

   _signal_passthrough_set.insert (std::make_pair (type_id, type));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_CarrierTransportBase_CODEHEADER_INCLUDED

#undef flip_CarrierTransportBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

