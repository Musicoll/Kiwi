/*****************************************************************************

      CarrierTransportBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/def.h"
#include "flip/detail/CarrierBase.h"

#include <functional>
#include <set>
#include <string>
#include <vector>

#include <cstddef>
#include <cstdint>



namespace flip
{



class flip_API CarrierTransportBase
:  public CarrierBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  CarrierTransportBase (CarrierListener & listener, uint64_t session_id, std::string metadata);
   virtual        ~CarrierTransportBase () = default;

   template <class Type>
   void           enable_signal_passthrough (uint32_t type);

   void           listen_transfer_backend (std::function <void (size_t, size_t)> func);
   void           listen_transfer_transaction (std::function <void (size_t, size_t)> func);
   void           listen_transfer_signal (std::function <void (size_t, size_t)> func);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   // CarrierBase
   virtual void   do_greet (uint64_t user_id) override;
   virtual void   do_commit (const Transaction & /* tx */) override {}
   virtual void   do_squash (const TxIdRange & /* range */, const Transaction & /* tx */) override {}
   virtual void   do_push (const Transaction & tx) override;
   virtual void   do_signal (const SignalData & data) override;

#if (flip_COMPILER == flip_COMPILER_GNU)
public:  // gcc has a bug with accessibility from lambdas
#endif
   // derived class
   virtual void   send (const std::vector <uint8_t> & data) = 0;
   void           recv (const uint8_t * data_ptr, size_t data_size);
   void           peek (const uint8_t * data_ptr, size_t data_size, size_t packet_size);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   const uint64_t _session_id;
   const std::string
                  _metadata;

   std::set <std::pair <uint32_t, uint32_t>>
                  _signal_passthrough_set;

   std::function <void (size_t, size_t)>
                  _func_backend;
   std::function <void (size_t, size_t)>
                  _func_transaction;
   std::function <void (size_t, size_t)>
                  _func_signal;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CarrierTransportBase () = delete;
                  CarrierTransportBase (const CarrierTransportBase & rhs) = delete;
   CarrierTransportBase &  operator = (const CarrierTransportBase & rhs) = delete;
                  CarrierTransportBase (CarrierTransportBase && rhs) = delete;
   CarrierTransportBase &  operator = (CarrierTransportBase && rhs) = delete;
   bool           operator == (const CarrierTransportBase & rhs) const = delete;
   bool           operator != (const CarrierTransportBase & rhs) const = delete;



}; // class CarrierTransportBase



}  // namespace flip



#include  "flip/detail/CarrierTransportBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

