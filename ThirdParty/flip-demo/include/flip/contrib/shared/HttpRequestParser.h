/*****************************************************************************

      HttpRequestParser.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/def.h"

#include <map>
#include <string>
#include <vector>

#include <cstdint>



namespace flip
{



class flip_API HttpRequestParser
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  HttpRequestParser (const std::vector <uint8_t> & input);
   virtual        ~HttpRequestParser () = default;

   bool           process ();

   const std::string &
                  method () const;
   const std::string &
                  resource () const;
   const std::map <std::string, std::string> &
                  parameters () const;
   const std::map <std::string, std::string> &
                  headers () const;
   const std::vector <uint8_t> &
                  body () const;
   std::string    body_str () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           decode_resource ();
   void           decode_post ();
   void           decode_parameters (std::string query);

   static std::string
                  decode_url_encoded (std::string resource);
   static std::string
                  replace (std::string str, std::string pattern, std::string replacement);

   const std::vector <uint8_t> &
                  _input;

   std::string    _method;
   std::string    _resource;
   std::map <std::string, std::string>
                  _parameters;
   std::map <std::string, std::string>
                  _headers;
   std::vector <uint8_t>
                  _body;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  HttpRequestParser () = delete;
                  HttpRequestParser (const HttpRequestParser & rhs) = delete;
   HttpRequestParser &
                  operator = (const HttpRequestParser & rhs) = delete;
                  HttpRequestParser (HttpRequestParser && rhs) = delete;
   HttpRequestParser &
                  operator = (HttpRequestParser && rhs) = delete;
   bool           operator == (const HttpRequestParser & rhs) const = delete;
   bool           operator != (const HttpRequestParser & rhs) const = delete;



}; // class HttpRequestParser



}  // namespace flip



//#include  "flip/HttpRequestParser.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

