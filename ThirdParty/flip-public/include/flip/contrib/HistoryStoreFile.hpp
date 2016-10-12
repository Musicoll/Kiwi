/*****************************************************************************

      HistoryStoreFile.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_HistoryStoreFile_CURRENT_CODEHEADER)
   #error Recursive inclusion of HistoryStoreFile code header.
#endif
#define  flip_HistoryStoreFile_CURRENT_CODEHEADER

#if ! defined (flip_HistoryStoreFile_CODEHEADER_INCLUDED)
#define  flip_HistoryStoreFile_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : TransactionProxy::ctor
==============================================================================
*/

template <class Iterator>
HistoryStoreFile::TransactionProxy <Iterator>::TransactionProxy (Iterator & parent)
:  _parent (parent)
{
}



/*
==============================================================================
Name : TransactionProxy::operator =
==============================================================================
*/

template <class Iterator>
HistoryStoreFile::TransactionProxy <Iterator> &   HistoryStoreFile::TransactionProxy <Iterator>::operator = (Transaction tx)
{
   _parent._parent_ptr->set_transaction (_parent._id, tx);

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : TransactionProxy::user
==============================================================================
*/

template <class Iterator>
uint64_t HistoryStoreFile::TransactionProxy <Iterator>::user () const
{
   return _parent._parent_ptr->get_transaction (_parent._id).user ();
}  // COV_NF_LINE



/*
==============================================================================
Name : TransactionProxy::actor
==============================================================================
*/

template <class Iterator>
uint64_t HistoryStoreFile::TransactionProxy <Iterator>::actor () const
{
   return _parent._parent_ptr->get_transaction (_parent._id).actor ();
}  // COV_NF_LINE



/*
==============================================================================
Name : TransactionProxy::nbr
==============================================================================
*/

template <class Iterator>
uint64_t HistoryStoreFile::TransactionProxy <Iterator>::nbr () const
{
   return _parent._parent_ptr->get_transaction (_parent._id).nbr ();
}  // COV_NF_LINE



/*
==============================================================================
Name : TransactionProxy::label
==============================================================================
*/

template <class Iterator>
std::string  HistoryStoreFile::TransactionProxy <Iterator>::label () const
{
   return metadata (Transaction::key_label);
}



/*
==============================================================================
Name : TransactionProxy::metadata
==============================================================================
*/

template <class Iterator>
std::string  HistoryStoreFile::TransactionProxy <Iterator>::metadata (const std::string & key) const
{
   return _parent._parent_ptr->get_metadata (_parent._id, key);
}




/*
==============================================================================
Name : TransactionProxy::operator Transaction
==============================================================================
*/

template <class Iterator>
HistoryStoreFile::TransactionProxy <Iterator>::operator Transaction () const
{
   return _parent._parent_ptr->get_transaction (_parent._id);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_HistoryStoreFile_CODEHEADER_INCLUDED

#undef flip_HistoryStoreFile_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

