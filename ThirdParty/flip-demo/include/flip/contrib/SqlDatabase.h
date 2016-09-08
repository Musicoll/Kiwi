/*****************************************************************************

      SqlDatabase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#if defined (__clang__)
   #pragma clang diagnostic push

   #if __has_warning ("-Wreserved-id-macro")
      #pragma clang diagnostic ignored "-Wreserved-id-macro"
   #endif
#endif

#include "flip/contrib/sqlite3.h"

#if defined (__clang__)
   #pragma clang diagnostic pop
#endif

#include <cstdint>



namespace flip
{



class SqlDatabase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  SqlDatabase (const char * path_0);
   virtual        ~SqlDatabase ();

   sqlite3 &      use ();

   int64_t        last_insert_rowid ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   sqlite3 *      _db_ptr = nullptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SqlDatabase () = delete;
                  SqlDatabase (const SqlDatabase & rhs) = delete;
   SqlDatabase &  operator = (const SqlDatabase & rhs) = delete;
                  SqlDatabase (SqlDatabase && rhs) = delete;
   SqlDatabase &  operator = (SqlDatabase && rhs) = delete;
   bool           operator == (const SqlDatabase & rhs) const = delete;
   bool           operator != (const SqlDatabase & rhs) const = delete;



}; // class SqlDatabase



}  // namespace flip



//#include  "flip/SqlDatabase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

