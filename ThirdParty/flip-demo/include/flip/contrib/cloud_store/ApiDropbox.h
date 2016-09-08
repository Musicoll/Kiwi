/*****************************************************************************

      ApiDropbox.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"

#include <list>
#include <map>
#include <string>
#include <vector>

#include <cstdint>



namespace flip
{



class CurlEasy;

class flip_API ApiDropbox
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~ApiDropbox () = default;

   static void    delta (CurlEasy & curl, const std::string & access_token, const std::string & cursor);
   static void    longpoll_delta (CurlEasy & curl, const std::string & cursor);
   static void    files (CurlEasy & curl, const std::string & access_token, const std::string & path);
   static void    files_put (CurlEasy & curl, const std::string & access_token, const std::string & path, const std::vector <uint8_t> & data);
   static void    fileops_delete (CurlEasy & curl, const std::string & access_token, const std::string & path);

   class ReplyDelta
   {
   public:
      class Entry
      {
      public:
         std::string
                  path;
         bool     deleted = false;
         std::string
                  modified;
      };
      bool        has_more = false;
      std::string cursor;
      std::list <Entry>
                  entries;
   };

   static void    parse_delta (ReplyDelta & reply, const CurlEasy & curl);
   static void    parse_longpoll_delta (bool & changes_flag, const CurlEasy & curl);

   static std::string
                  base_api;
   static std::string
                  base_api_notify;
   static std::string
                  base_api_content;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  ApiDropbox () = default;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ApiDropbox (const ApiDropbox & rhs) = delete;
   ApiDropbox &   operator = (const ApiDropbox & rhs) = delete;
                  ApiDropbox (ApiDropbox && rhs) = delete;
   ApiDropbox &   operator = (ApiDropbox && rhs) = delete;
   bool           operator == (const ApiDropbox & rhs) const = delete;
   bool           operator != (const ApiDropbox & rhs) const = delete;



}; // class ApiDropbox



}  // namespace flip



//#include  "flip/ApiDropbox.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

