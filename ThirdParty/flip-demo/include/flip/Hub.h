/*****************************************************************************

      Hub.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Document.h"
#include "flip/detail/CarrierListener.h"
#include "flip/detail/PortListener.h"

#include <set>

#include <cstdint>



namespace flip
{



class Uplink;
class DataModelBase;
class PortBase;
class SignalData;
class TxIdRange;

class flip_API Hub
:  public CarrierListener
,  public PortListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   template <class... Ports>
                  Hub (const DataModelBase & data_model, uint64_t user_id, Ports &... ports);
   virtual        ~Hub ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // CarrierListener
   virtual const DataModelBase &
                  carrier_data_model () override;
   virtual uint64_t
                  carrier_user () override;
   virtual void   carrier_receive_beir (const BackEndIR & backend) override;
   virtual void   carrier_receive_tx_local (const Transaction & tx) override;
   virtual void   carrier_receive_tx_squash (const TxIdRange & range, const Transaction & tx) override;
   virtual void   carrier_receive_tx_ext (const Transaction & tx) override;
   virtual void   carrier_receive_tx_ack (const TxId & tx_id) override;
   virtual void   carrier_receive_tx_deny (const TxId & tx_id) override;
   virtual void   carrier_receive_signal (const SignalData & data) override;

   // PortListener
   virtual void   port_greet (PortBase & from) override;
   virtual void   port_commit (PortBase & from, const Transaction & tx) override;
   virtual void   port_squash (PortBase & from, const TxIdRange & range, const Transaction & tx) override;
   virtual void   port_push (PortBase & from, const Transaction & tx) override;
   virtual void   port_signal (PortBase & from, const SignalData & data) override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   template <class... Ports>
   void           add (PortBase & port, Ports &... ports);
   void           add (PortBase & port);

   Document       _document;

   std::set <PortBase *>
                  _ports;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Hub () = delete;
                  Hub (const Hub & rhs) = delete;
   Hub &          operator = (const Hub & rhs) = delete;
                  Hub (Hub && rhs) = delete;
   Hub &          operator = (Hub && rhs) = delete;
   bool           operator == (const Hub & rhs) const = delete;
   bool           operator != (const Hub & rhs) const = delete;



}; // class Hub



}  // namespace flip



#include  "flip/Hub.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

