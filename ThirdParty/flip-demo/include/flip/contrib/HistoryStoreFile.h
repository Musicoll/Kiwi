/*****************************************************************************

      HistoryStoreFile.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Transaction.h"

#include <memory>
#include <string>

#include <cstdint>



namespace flip
{



class SqlDatabase;

class flip_API HistoryStoreFile
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class flip_API iterator;
   class flip_API const_iterator;

   template <class Iterator>
   class TransactionProxy
   {
   public:
                  TransactionProxy (Iterator & parent);
      virtual     ~TransactionProxy () {}

      TransactionProxy &
                  operator = (Transaction tx);

      uint64_t    user () const;
      uint64_t    actor () const;
      uint64_t    nbr () const;

      std::string label () const;
      std::string metadata (const std::string & key) const;

                  operator Transaction () const;

   private:
                  TransactionProxy () = delete;
                  TransactionProxy (const TransactionProxy & rhs) = delete;
      TransactionProxy &
                  operator = (const TransactionProxy & rhs) = delete;

      Iterator &  _parent;
   };

   class flip_API iterator
   {
   public:
                  iterator ();
                  iterator (const iterator & rhs);
      virtual     ~iterator () = default;

      iterator &  operator = (const iterator & rhs);

      iterator &  operator ++ ();
      iterator &  operator -- ();

      bool        operator == (const iterator & rhs) const;
      bool        operator != (const iterator & rhs) const;

      TransactionProxy <iterator> &
                  operator * ();
      TransactionProxy <iterator> *
                  operator -> ();

   private:
      friend class HistoryStoreFile;
      friend class TransactionProxy <iterator>;
      friend class const_iterator;

      HistoryStoreFile *
                  _parent_ptr = nullptr;
      int64_t     _id = 0LL;
      TransactionProxy <iterator>
                  _proxy;
   };

   class flip_API const_iterator
   {
   public:
                  const_iterator ();
                  const_iterator (const const_iterator & rhs);
                  const_iterator (const iterator & rhs);
      virtual     ~const_iterator () = default;

      const_iterator &
                  operator = (const const_iterator & rhs);

      const_iterator &
                  operator ++ ();
      const_iterator &
                  operator -- ();

      bool        operator == (const const_iterator & rhs) const;
      bool        operator != (const const_iterator & rhs) const;

      const TransactionProxy <const_iterator> &
                  operator * () const;
      const TransactionProxy <const_iterator> *
                  operator -> () const;

   private:
      friend class HistoryStoreFile;
      friend class TransactionProxy <const_iterator>;

      const HistoryStoreFile *
                  _parent_ptr = nullptr;
      int64_t     _id = 0LL;
      TransactionProxy <const_iterator>
                  _proxy;
   };

                  HistoryStoreFile (const std::string & version, const char * path_0);
   virtual        ~HistoryStoreFile ();

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

   int64_t        get_next (int64_t row_id) const;
   int64_t        get_prev (int64_t row_id) const;
   std::string    get_metadata (int64_t row_id, const std::string & key) const;
   Transaction    get_transaction (int64_t row_id);
   void           set_transaction (int64_t row_id, Transaction tx);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           create ();

   size_t         _max_size = SIZE_MAX;
   size_t         _cur_size = 0;
   std::unique_ptr <SqlDatabase>
                  _db_uptr;

   iterator       _it_redo;

   std::string    _document_version;
   std::string    _version;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  HistoryStoreFile () = delete;
                  HistoryStoreFile (const HistoryStoreFile & rhs) = delete;
   HistoryStoreFile &
                  operator = (const HistoryStoreFile & rhs) = delete;
                  HistoryStoreFile (HistoryStoreFile && rhs) = delete;
   HistoryStoreFile &
                  operator = (HistoryStoreFile && rhs) = delete;
   bool           operator == (const HistoryStoreFile & rhs) const = delete;
   bool           operator != (const HistoryStoreFile & rhs) const = delete;



}; // class HistoryStoreFile



}  // namespace flip



#include  "flip/contrib/HistoryStoreFile.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

