/*****************************************************************************

      EnumClassBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <cstdint>
#include <string>
#include <vector>



namespace flip
{



class DataModelBase;

class flip_API EnumClassBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class Enumerator
   {
   public:
      const char *
                  _name_0 = nullptr;
      int64_t     _value = 0LL;
   };

   typedef std::vector <Enumerator> Enumerators;

                  EnumClassBase () = default;
   virtual        ~EnumClassBase () = default;   // COV_NF_LINE

   const Enumerators &
                  enumerators () const;
   const char *   name () const;
   size_t         size () const;

   const char *   get_enumerator_name (int64_t value) const;
   int64_t        get_enumerator_value (const std::string & name) const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   void           impl_check_enumerators () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   bool           is_name_valid (const std::string & name);

   const char *   _name_0 = nullptr;
   Enumerators    _enumerators;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  EnumClassBase (const EnumClassBase & rhs) = delete;
   EnumClassBase &operator = (const EnumClassBase & rhs) = delete;
                  EnumClassBase (EnumClassBase && rhs) = delete;
   EnumClassBase &operator = (EnumClassBase && rhs) = delete;
   bool           operator == (const EnumClassBase & rhs) const = delete;
   bool           operator != (const EnumClassBase & rhs) const = delete;



}; // class EnumClassBase



}  // namespace flip



//#include  "flip/EnumClassBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

