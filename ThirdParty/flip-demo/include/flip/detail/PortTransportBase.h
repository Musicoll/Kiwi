/*****************************************************************************

      PortTransportBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/PortBase.h"

#include <vector>

#include <cstddef>
#include <cstdint>



namespace flip
{



class flip_API PortTransportBase
:  public PortBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PortTransportBase () = default;
   virtual        ~PortTransportBase () = default;

   // PortBase
   virtual void   receive_backend (const BackEndIR & backend) override;
   virtual void   receive_commit (const Transaction & tx) override;
   virtual void   receive_squash (const TxIdRange & range, const Transaction & tx) override;
   virtual void   receive_transaction (const Transaction & tx) override;
   virtual void   receive_ack (const TxId & tx_id) override;
   virtual void   receive_deny (const TxId & tx_id) override;
   virtual void   receive_signal (const SignalData & data) override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual void   send (const std::vector <uint8_t> & data) = 0;
   void           receive (const uint8_t * data_ptr, size_t data_size);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PortTransportBase (const PortTransportBase & rhs) = delete;
   PortTransportBase &
                  operator = (const PortTransportBase & rhs) = delete;
                  PortTransportBase (PortTransportBase && rhs) = delete;
   PortTransportBase &
                  operator = (PortTransportBase && rhs) = delete;
   bool           operator == (const PortTransportBase & rhs) const = delete;
   bool           operator != (const PortTransportBase & rhs) const = delete;



}; // class PortTransportBase



}  // namespace flip



//#include  "flip/PortTransportBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

