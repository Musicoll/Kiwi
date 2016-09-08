/*****************************************************************************

      HistoryStoreMemory.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Transaction.h"

#include <list>
#include <string>

#include <cstdint>



namespace flip
{



class flip_API HistoryStoreMemory
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   using iterator = std::list <Transaction>::iterator;
   using const_iterator = std::list <Transaction>::const_iterator;

                  HistoryStoreMemory (const std::string & version);
   virtual        ~HistoryStoreMemory () = default;

   std::string    version () const;

   void           set_max_size (size_t size);

   iterator       begin ();
   const_iterator begin () const;

   iterator       end ();
   const_iterator end () const;

   iterator       last_undo ();
   const_iterator last_undo () const;

   iterator       first_redo ();
   const_iterator first_redo () const;

   void           set_first_redo (iterator it);

   void           clear ();
   void           erase (iterator it);
   void           push (Transaction tx);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   using Transactions = std::list <Transaction>;

   size_t         _max_size = SIZE_MAX;
   Transactions   _transactions;
   Transactions::iterator
                  _it_redo;
   std::string    _version;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  HistoryStoreMemory () = delete;
                  HistoryStoreMemory (const HistoryStoreMemory & rhs) = delete;
   HistoryStoreMemory &
                  operator = (const HistoryStoreMemory & rhs) = delete;
                  HistoryStoreMemory (HistoryStoreMemory && rhs) = delete;
   HistoryStoreMemory &
                  operator = (HistoryStoreMemory && rhs) = delete;
   bool           operator == (const HistoryStoreMemory & rhs) const = delete;
   bool           operator != (const HistoryStoreMemory & rhs) const = delete;



}; // class HistoryStoreMemory



}  // namespace flip



//#include  "flip/HistoryStoreMemory.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

