/*****************************************************************************

      ApiDropboxServerEmulation.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/contrib/Socket.h"
#include "flip/detail/def.h"

#include <chrono>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include <cstdint>



namespace flip
{



class flip_API ApiDropboxServerEmulation
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum class ErrorType
   {
                  RateLimit_429,
                  RateLimit_503, // Retry-After header
                  Transient_500,
                  Transient_503,
                  Quota_507,
   };

                  ApiDropboxServerEmulation (uint16_t port);
   virtual        ~ApiDropboxServerEmulation ();

   void           set_error (int request_nbr, ErrorType type);
   void           set_longpoll_timeout (int timeout_s);

   void           process ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   using Errors = std::map <int, ErrorType>;

   class Client
   {
   public:
                  Client (ApiDropboxServerEmulation & parent, Socket::Id fd);
      virtual     ~Client ();

      void        process ();
      bool        is_closed () const;

   private:
      void        close ();
      void        process_send ();
      void        process_recv ();
      void        process_parse ();
      bool        process_error ();
      void        process_delta (std::string cursor);
      void        process_longpoll_delta (std::string cursor);
      void        process_files (std::string path);
      void        process_files_put (std::string path, const std::vector <uint8_t> & data);
      void        process_filesop_delete (std::string path);

      void        send_close_json (std::string json);
      void        send_close_bare (std::string code, std::string reason);
      void        send_close_bare (std::string code, std::string reason, std::initializer_list <const char *> headers);

      static bool initial_cursor (std::string cursor);

      ApiDropboxServerEmulation &
                  _parent;
      Socket::Id  _fd;

      std::vector <uint8_t>
                  _data_send;
      std::vector <uint8_t>
                  _data_recv;

      std::chrono::system_clock::time_point
                  _longpoll_end_time = std::chrono::system_clock::from_time_t (0);

      static const size_t
                  _buf_size = 4096;
      static const char * const
                  _initial_cursor_0;

                  Client () = delete;
                  Client (const Client & rhs) = delete;
                  Client (Client && rhs) = delete;
      Client &    operator = (const Client & rhs) = delete;
      Client &    operator = (Client && rhs) = delete;
   };

   friend class Client;

   using Clients = std::list <std::unique_ptr <Client>>;
   using Files = std::map <std::string, std::vector <uint8_t>>;

   class Event
   {
   public:
      std::string path;
      bool        deleted;
   };

   using Events = std::map <std::chrono::steady_clock::time_point, Event>;

   std::map <std::string, std::string>
                  make_state (Events::iterator it_end);

   static std::string
                  to_cursor (std::chrono::steady_clock::time_point time);
   static std::chrono::steady_clock::time_point
                  to_time_point (std::string cursor);

   std::chrono::steady_clock::time_point
                  _steady_base = std::chrono::steady_clock::now ();
   std::chrono::system_clock::time_point
                  _system_base = std::chrono::system_clock::now ();

   std::chrono::duration <int>
                  _longpoll_timeout = std::chrono::duration <int> (10);


   Socket::Id     _fd = Socket::invalid_id;
   Socket::Addr   _addr;

   Clients        _clients;

   Files          _files;
   Events         _events;

   int            _request_nbr = 0;
   Errors         _errors;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ApiDropboxServerEmulation () = delete;
                  ApiDropboxServerEmulation (const ApiDropboxServerEmulation & rhs) = delete;
   ApiDropboxServerEmulation &
                  operator = (const ApiDropboxServerEmulation & rhs) = delete;
                  ApiDropboxServerEmulation (ApiDropboxServerEmulation && rhs) = delete;
   ApiDropboxServerEmulation &
                  operator = (ApiDropboxServerEmulation && rhs) = delete;
   bool           operator == (const ApiDropboxServerEmulation & rhs) const = delete;
   bool           operator != (const ApiDropboxServerEmulation & rhs) const = delete;



}; // class ApiDropboxServerEmulation



}  // namespace flip



//#include  "flip/ApiDropboxServerEmulation.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

