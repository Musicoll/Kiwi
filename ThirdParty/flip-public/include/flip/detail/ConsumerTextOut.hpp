/*****************************************************************************

      ConsumerTextOut.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_ConsumerTextOut_CURRENT_CODEHEADER)
   #error Recursive inclusion of ConsumerTextOut code header.
#endif
#define  flip_ConsumerTextOut_CURRENT_CODEHEADER

#if ! defined (flip_ConsumerTextOut_CODEHEADER_INCLUDED)
#define  flip_ConsumerTextOut_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : print
==============================================================================
*/

template <class T, class... Args>
void  ConsumerTextOut::print (const char * format_0, T value, Args... args)
{
   while (*format_0 != '\0')
   {
      print_indent ();

      char c = *format_0;

      if (c == '%')
      {
         char nc = *(format_0 + 1);

         if (nc == '%')
         {
            ++format_0;
         }
         else
         {
            FormatOpt opt;

            ++format_0;

            while ((*format_0 != '\0') && (*format_0 != '@'))
            {
               char fc = *format_0;

               if (fc == 'x')
               {
                  opt._hexadecimal_flag = true;
               }
               else if (fc == '0')
               {
                  opt._trailing_zero_flag = true;
               }
               else
               {
                  // ignored
               }

               ++format_0;
            }

            write (value, opt);

            if (*format_0 == '@') ++format_0;

            print (format_0, args...);

            return;  // abort
         }
      }
      else if (c == '\n')
      {
         _indent_flag = true;
      }

      _consumer << *format_0;
      ++format_0;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_ConsumerTextOut_CODEHEADER_INCLUDED

#undef flip_ConsumerTextOut_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

