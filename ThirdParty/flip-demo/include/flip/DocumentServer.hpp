/*****************************************************************************

      DocumentServer.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_DocumentServer_CURRENT_CODEHEADER)
   #error Recursive inclusion of DocumentServer code header.
#endif
#define  flip_DocumentServer_CURRENT_CODEHEADER

#if ! defined (flip_DocumentServer_CODEHEADER_INCLUDED)
#define  flip_DocumentServer_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/PortBase.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : send_tx_if
==============================================================================
*/

template <class UnaryPredicate>
void  DocumentServer::send_tx_if (const Transaction & tx, UnaryPredicate p)
{
   if (tx.empty ()) return;   // abort

   for (auto port_ptr : _ports)
   {
      auto & port = *port_ptr;

      if (port.impl_is_active ())
      {
         if (p (port))
         {
            port.receive_transaction (tx);
         }
      }
   }
}



/*
==============================================================================
Name : send_signal_if
==============================================================================
*/

template <class UnaryPredicate>
void  DocumentServer::send_signal_if (const SignalData & data, UnaryPredicate p)
{
   for (auto port_ptr : _ports)
   {
      auto & port = *port_ptr;

      if (port.impl_is_active ())
      {
         if (p (port))
         {
            port.receive_signal (data);
         }
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_DocumentServer_CODEHEADER_INCLUDED

#undef flip_DocumentServer_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

