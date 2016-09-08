/*****************************************************************************

      ConsumerTextOut.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Ref.h"
#include "flip/detail/DataConsumerBase.h"
#include "flip/detail/KeyFloat.h"
#include "flip/detail/KeyRandom.h"

#include <string>

#include <cstddef>
#include <cstdint>



namespace flip
{



class DataConsumerBase;

class flip_API ConsumerTextOut
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class flip_API Indent
   {
   public:
                  Indent (ConsumerTextOut & parent, size_t nbr = 4);
                  ~Indent ();

   private:
      ConsumerTextOut &
                  _parent;
      const size_t
                  _nbr;

                  Indent () = delete;
                  Indent (const Indent & rhs) = delete;
                  Indent (Indent && rhs) = delete;
      Indent &    operator = (const Indent & rhs) = delete;
      Indent &    operator = (Indent && rhs) = delete;
   };

                  ConsumerTextOut (DataConsumerBase & consumer);
   virtual        ~ConsumerTextOut () = default;

   void           print (const char * str_0);
   template <class T, class... Args>
   void           print (const char * format_0, T value, Args... args);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   friend class Indent;
   void           indent (size_t nbr);
   void           deindent (size_t nbr);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class FormatOpt
   {
   public:
      bool        _hexadecimal_flag = false;
      bool        _trailing_zero_flag = false;
   };

   void           print_indent ();

   void           write (bool value, FormatOpt opt);
   void           write (uint8_t value, FormatOpt opt);
   void           write (int32_t value, FormatOpt opt);
   void           write (uint32_t value, FormatOpt opt);
   void           write (int64_t value, FormatOpt opt);
   void           write (uint64_t value, FormatOpt opt);
   void           write (double value, FormatOpt opt);
   void           write (void * value, FormatOpt opt);
   void           write (std::nullptr_t, FormatOpt opt);
   void           write (const char * value, FormatOpt opt);
   void           write (std::string value, FormatOpt opt);
   void           write (Ref value, FormatOpt opt);
   void           write (KeyRandom value, FormatOpt opt);
   void           write (KeyFloat value, FormatOpt opt);

   DataConsumerBase &
                  _consumer;

   size_t         _cur_indent = 0;
   bool           _indent_flag = true;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ConsumerTextOut () = delete;
                  ConsumerTextOut (const ConsumerTextOut & rhs) = delete;
   ConsumerTextOut &
                  operator = (const ConsumerTextOut & rhs) = delete;
                  ConsumerTextOut (ConsumerTextOut && rhs) = delete;
   ConsumerTextOut &
                  operator = (ConsumerTextOut && rhs) = delete;
   bool           operator == (const ConsumerTextOut & rhs) const = delete;
   bool           operator != (const ConsumerTextOut & rhs) const = delete;



}; // class ConsumerTextOut



}  // namespace flip



#include "flip/detail/ConsumerTextOut.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

