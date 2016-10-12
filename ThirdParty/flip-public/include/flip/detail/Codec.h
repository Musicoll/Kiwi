/*****************************************************************************

      Codec.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/def.h"

#include <memory>
#include <tuple>
#include <vector>

#include <cstdint>



namespace flip
{



class BackEndIR;
class DocumentBase;
class SignalData;
class StreamBinIn;
class StreamBinOut;
class Transaction;
class TxId;
class TxIdRange;

class flip_API Codec
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~Codec () = default;

   static void    encode_greet (StreamBinOut & sbo, uint64_t user_id, uint64_t session_id, std::string metadata);
   static void    encode_backend (StreamBinOut & sbo, const BackEndIR & backend);
   static void    encode_tx (StreamBinOut & sbo, const Transaction & tx);
   static void    encode_tx_ack (StreamBinOut & sbo, const TxId & tx_id);
   static void    encode_tx_deny (StreamBinOut & sbo, const TxId & tx_id);
   static void    encode_tx_local (StreamBinOut & sbo, const Transaction & tx);
   static void    encode_tx_squash (StreamBinOut & sbo, const TxIdRange & range, const Transaction & tx);
   static void    encode_signal (StreamBinOut & sbo, const SignalData & data);

   static uint32_t
                  decode_msg (StreamBinIn & sbi);
   static std::tuple <uint64_t, uint64_t, std::string>
                  decode_greet (StreamBinIn & sbi);
   static BackEndIR
                  decode_backend (StreamBinIn & sbi);
   static Transaction
                  decode_tx (StreamBinIn & sbi);
   static TxId    decode_tx_ack (StreamBinIn & sbi);
   static TxId    decode_tx_deny (StreamBinIn & sbi);
   static Transaction
                  decode_tx_local (StreamBinIn & sbi);
   static std::pair <TxIdRange, Transaction>
                  decode_tx_squash (StreamBinIn & sbi);
   static SignalData
                  decode_signal (StreamBinIn & sbi);

   static const uint32_t
                  msg_greet = flip_CC4 ('h', 'e', 'l', 'o');
   static const uint32_t
                  msg_backend = flip_CC4 ('b', 'b', 'i', 'n');
   static const uint32_t
                  msg_tx = flip_CC4 ('t', 'x', ' ', ' ');
   static const uint32_t
                  msg_tx_ack = flip_CC4 ('t', 'x', 'a', 'k');
   static const uint32_t
                  msg_tx_deny = flip_CC4 ('t', 'x', 'd', 'n');
   static const uint32_t
                  msg_tx_local = flip_CC4 ('l', 'o', 'c', 'l');
   static const uint32_t
                  msg_tx_squash = flip_CC4 ('s', 'q', 's', 'h');
   static const uint32_t
                  msg_signal = flip_CC4 ('s', 'i', 'g', ' ');



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Codec () = delete;
                  Codec (const Codec & rhs) = delete;
   Codec &        operator = (const Codec & rhs) = delete;
                  Codec (Codec && rhs) = delete;
   Codec &        operator = (Codec && rhs) = delete;
   bool           operator == (const Codec & rhs) const = delete;
   bool           operator != (const Codec & rhs) const = delete;



}; // class Codec



}  // namespace flip



//#include  "flip/Codec.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
