/*****************************************************************************

      ServerHttp.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/contrib/Socket.h"

#include <list>
#include <map>
#include <memory>
#include <vector>

#include <cstdint>

#include "flip/detail/def.h"

#if (flip_PLATFORM == flip_PLATFORM_WIN32)
   #define WIN32_LEAN_AND_MEAN
   #define NOMINMAX
   #define NOGDI
   flip_DISABLE_WARNINGS_SYSTEM_HEADERS
   #include <winsock2.h>	// Must be included before windows.h
   flip_RESTORE_WARNINGS

#else    // flip_PLATFORM
   #include <netinet/in.h>

#endif   // flip_PLATFORM



namespace flip
{



class ArrayBase;
class Blob;
class Bool;
class CollectionBase;
class DocumentBase;
class EnumBase;
class Int;
class Float;
class MessageBase;
class Object;
class ObjectRefBase;
class String;
class Type;

class flip_API ServerHttp
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  ServerHttp (DocumentBase & document, uint16_t port = 80);
   virtual        ~ServerHttp ();

   void           process ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class flip_API Client
   {
   public:
                  Client (ServerHttp & parent, Socket::Id fd);
      virtual     ~Client ();

      void        process ();
      void        send (const std::string & str);
      void        close ();

   private:
      void        send ();
      void        recv ();
      void        process_recv ();
      bool        has_recv ();

      ServerHttp &
                  _parent;

      Socket::Id  _fd = Socket::invalid_id;

      std::vector <uint8_t>
                  _data_send;
      std::vector <uint8_t>
                  _data_recv;

      static const size_t
                  _buf_size = 4096;

                  Client () = delete;
                  Client (const Client & rhs) = delete;
                  Client (Client && rhs) = delete;
      Client &    operator = (const Client & rhs) = delete;
      Client &    operator = (Client && rhs) = delete;
   };

   friend class Client;

   typedef std::list <std::unique_ptr <Client>> Clients;

   void           create_listen (uint16_t port);
   void           accept ();
   bool           has_accept ();

   // from Client
   void           request (Client & client, const std::string & method, const std::string & resource, const std::string & body);
   void           request_state (Client & client);
   void           request_state (std::string & page, Object & type, const std::string & name, const std::string & path, bool button_flag = false);
   void           request_state_member (std::string & page, Type & type, const std::string & name, const std::string & path);
   void           request_state_member (std::string & page, Bool & type, const std::string & name, const std::string & path);
   void           request_state_member (std::string & page, Int & type, const std::string & name, const std::string & path);
   void           request_state_member (std::string & page, Float & type, const std::string & name, const std::string & path);
   void           request_state_member (std::string & page, EnumBase & type, const std::string & name, const std::string & path);
   void           request_state_member (std::string & page, ObjectRefBase & type, const std::string & name, const std::string & path);
   void           request_state_member (std::string & page, Blob & type, const std::string & name, const std::string & path);
   void           request_state_member (std::string & page, String & type, const std::string & name, const std::string & path);
   void           request_state_member (std::string & page, CollectionBase & type, const std::string & name, const std::string & path);
   void           request_state_member (std::string & page, ArrayBase & type, const std::string & name, const std::string & path);
   void           request_state_member (std::string & page, MessageBase & type, const std::string & name, const std::string & path);
   void           request_state_member (std::string & page, Object & type, const std::string & name, const std::string & path);

   void           request_change (const std::map <std::string, std::string> & request);
   void           request_change (const std::map <std::string, std::string> & request, Object & type, const std::string & path);
   void           request_change_member (const std::map <std::string, std::string> & request, Type & type, const std::string & path);
   void           request_change_member (const std::map <std::string, std::string> & request, Bool & type, const std::string & path);
   void           request_change_member (const std::map <std::string, std::string> & request, Int & type, const std::string & path);
   void           request_change_member (const std::map <std::string, std::string> & request, Float & type, const std::string & path);
   void           request_change_member (const std::map <std::string, std::string> & request, EnumBase & type, const std::string & path);
   void           request_change_member (const std::map <std::string, std::string> & request, ObjectRefBase & type, const std::string & path);
   void           request_change_member (const std::map <std::string, std::string> & request, Blob & type, const std::string & path);
   void           request_change_member (const std::map <std::string, std::string> & request, String & type, const std::string & path);
   void           request_change_member (const std::map <std::string, std::string> & request, CollectionBase & type, const std::string & path);
   void           request_change_member (const std::map <std::string, std::string> & request, ArrayBase & type, const std::string & path);
   void           request_change_member (const std::map <std::string, std::string> & request, MessageBase & type, const std::string & path);
   void           request_change_member (const std::map <std::string, std::string> & request, Object & type, const std::string & path);

   std::string    decode_url_encoded (const std::string & str);
   std::map <std::string, std::string>
                  decode_request (const std::string & resource);

   std::string    escape (const std::string & str);

   DocumentBase & _document;

   Socket::Id     _fd = Socket::invalid_id;

   ::sockaddr_in  _addr;

   Clients        _clients;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ServerHttp () = delete;
                  ServerHttp (const ServerHttp & rhs) = delete;
   ServerHttp &   operator = (const ServerHttp & rhs) = delete;
                  ServerHttp (ServerHttp && rhs) = delete;
   ServerHttp &   operator = (ServerHttp && rhs) = delete;
   bool           operator == (const ServerHttp & rhs) const = delete;
   bool           operator != (const ServerHttp & rhs) const = delete;



}; // class ServerHttp



}  // namespace flip



//#include  "flip/ServerHttp.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

