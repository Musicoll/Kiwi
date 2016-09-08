/*****************************************************************************

      ServerSimple.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/contrib/transport_tcp/PortTransportServerTcp.h"
#include "flip/contrib/RunLoopTimer.h"
#include "flip/DocumentServer.h"

#include <functional>
#include <map>
#include <memory>



namespace flip
{



class BackEndIR;
class DocumentValidatorBase;

class flip_API ServerSimple
:  public PortFactoryListener
,  public PortListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   using ValidatorFactoryFunc = std::function <std::unique_ptr <DocumentValidatorBase> (uint64_t /* session_id */)>;
   using InitFunc = std::function <void (uint64_t /* session_id */, DocumentBase & /* document */)>;
   using ReadFunc = std::function <BackEndIR (uint64_t /* session_id */)>;
   using WriteFunc = std::function <void (uint64_t /* session_id */, const BackEndIR & /* backend */)>;
   using AuthenticateFunc = std::function <bool (uint64_t /* user_id */, uint64_t /* session_id */, std::string /* metadata */)>;

                  ServerSimple (const DataModelBase & data_model, uint16_t port = 9090);
   virtual        ~ServerSimple ();

   void           bind_validator_factory (ValidatorFactoryFunc func);
   void           bind_init (InitFunc func);
   void           bind_read (ReadFunc func);
   void           bind_write (WriteFunc func);
   void           bind_authenticate (AuthenticateFunc func);

   void           run ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   void           process ();
   size_t         nbr_sessions () const;

   // PortFactoryListener
   virtual void   port_factory_add (PortBase & port) override;
   virtual void   port_factory_remove (PortBase & port) override;

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

   class Session
   {
   public:
                  Session () = default;
                  Session (Session && rhs);
                  Session (std::unique_ptr <DocumentServer> && document_uptr, std::unique_ptr <DocumentValidatorBase> && validator_uptr);
      virtual     ~Session () = default;

      std::unique_ptr <DocumentValidatorBase>
                  _validator_uptr;
      std::unique_ptr <DocumentServer>
                  _document_uptr;

   private:
                  Session (const Session & rhs) = delete;
      Session &   operator = (const Session & rhs) = delete;
      Session &   operator = (Session && rhs) = delete;
   };

   using SessionMap = std::map <uint64_t, Session>;

   SessionMap::iterator
                  create_session (uint64_t session_id);
   void           load_session (DocumentServer & server, uint64_t session_id);
   void           write_session (DocumentServer & server, uint64_t session_id);

   const DataModelBase &
                  _data_model;
   PortTransportServerTcp
                  _port;
   RunLoopTimer   _run_loop;

   SessionMap     _session_map;

   ValidatorFactoryFunc
                  _validator_factory;
   InitFunc       _init;
   ReadFunc       _read;
   WriteFunc      _write;
   AuthenticateFunc
                  _authenticate;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ServerSimple () = delete;
                  ServerSimple (const ServerSimple & rhs) = delete;
                  ServerSimple (ServerSimple && rhs) = delete;
   ServerSimple & operator = (const ServerSimple & rhs) = delete;
   ServerSimple & operator = (ServerSimple && rhs) = delete;
   bool           operator == (const ServerSimple & rhs) const = delete;
   bool           operator != (const ServerSimple & rhs) const = delete;



}; // class ServerSimple



}  // namespace flip



//#include  "flip/ServerSimple.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

