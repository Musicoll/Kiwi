/*****************************************************************************

      String.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Blob.h"

#include <string>



namespace flip
{



class Transaction;

class flip_API String
:  public Blob
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef std::string internal_type;

                  String () = default;
                  String (const String & rhs);
                  String (String && rhs);
                  String (const std::string & value);
   virtual        ~String () = default;

   String &       operator = (const String & rhs);
   String &       operator = (String && rhs);
   String &       operator = (const std::string & str);
   String &       operator = (std::string && str);

                  operator const std::string & () const;
   const std::string &
                  value () const;
   const std::string &
                  before () const;

   bool           operator == (const std::string & rhs) const;
   bool           operator != (const std::string & rhs) const;

   virtual void   revert () override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // Blob
   virtual void   impl_internal_set (const std::vector <uint8_t> & value) override;

   // Type
   virtual void   impl_synchronize () override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   static std::vector <uint8_t>
                  make_value (const std::string & str);
   static std::string
                  make_str (const std::vector <uint8_t> & value);

   void           update_value () const;
   void           update_before () const;

   mutable bool   _cached_value_need_update_flag = true;
   mutable std::string
                  _cached_value;
   mutable bool   _cached_before_value_need_update_flag = true;
   mutable std::string
                  _cached_before_value;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const String & rhs) const = delete;
   bool           operator != (const String & rhs) const = delete;



}; // class String



flip_API bool     operator == (const std::string & lhs, const String & rhs);
flip_API bool     operator != (const std::string & lhs, const String & rhs);


}  // namespace flip



//#include  "flip/String.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

