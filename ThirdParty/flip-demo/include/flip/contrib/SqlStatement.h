/*****************************************************************************

      SqlStatement.h
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

#include <string>
#include <vector>

#include <cstdint>



namespace flip
{



class SqlDatabase;

class SqlStatement
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class Row
   {
   public:
                  Row (SqlStatement & parent);
                  Row (Row && rhs);

      int64_t     get_int64 ();
      std::vector <uint8_t>
                  get_blob ();
      std::string get_string ();

   private:
      SqlStatement &
                  _parent;
      int         _col_nbr = 0;

                  Row () = delete;
                  Row (const Row & rhs) = delete;
      Row &       operator = (const Row & rhs) = delete;
      Row &       operator = (Row && rhs) = delete;
   };

   class iterator
   {
   public:
                  iterator (SqlStatement & parent);
                  iterator (iterator && rhs);

      bool        operator != (const iterator & rhs) const;

      iterator &  operator ++ ();

      Row         operator * ();

   private:
      friend class SqlStatement;

      SqlStatement &
                  _parent;
      bool        _end_flag = true;

                  iterator () = delete;
                  iterator (const iterator & rhs) = delete;
      iterator &  operator = (const iterator & rhs) = delete;
      iterator &  operator = (iterator && rhs) = delete;
   };

   friend class iterator;

                  SqlStatement (SqlDatabase & db, const char * format_0);
   virtual        ~SqlStatement ();

   iterator       begin ();
   iterator       end ();

   SqlStatement & arg (int64_t value);
   SqlStatement & arg (const std::vector <uint8_t> & value);
   SqlStatement & arg (const std::string & value);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   sqlite3_stmt * _stmt_ptr = nullptr;
   int            _arg_nbr = 1;
   bool           _run_flag = false;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  SqlStatement () = delete;
                  SqlStatement (const SqlStatement & rhs) = delete;
   SqlStatement & operator = (const SqlStatement & rhs) = delete;
                  SqlStatement (SqlStatement && rhs) = delete;
   SqlStatement & operator = (SqlStatement && rhs) = delete;
   bool           operator == (const SqlStatement & rhs) const = delete;
   bool           operator != (const SqlStatement & rhs) const = delete;



}; // class SqlStatement



}  // namespace flip



//#include  "flip/SqlStatement.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

