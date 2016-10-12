/*****************************************************************************

      History.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_History_CURRENT_CODEHEADER)
   #error Recursive inclusion of History code header.
#endif
#define  flip_History_CURRENT_CODEHEADER

#if ! defined (flip_History_CODEHEADER_INCLUDED)
#define  flip_History_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"
#include "flip/detail/DataModelBase.h"
#include "flip/detail/DocumentBase.h"



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class HistoryStoreImpl>
template <class... Args>
History <HistoryStoreImpl>::History (DocumentBase & document, Args &&... args)
:  _document (document)
,  _store (document.data_model ().version (), std::forward <Args> (args)...)
{
}



/*
==============================================================================
Name : store
==============================================================================
*/

template <class HistoryStoreImpl>
HistoryStoreImpl &   History <HistoryStoreImpl>::store ()
{
   return _store;
}



/*
==============================================================================
Name : version
==============================================================================
*/

template <class HistoryStoreImpl>
std::string History <HistoryStoreImpl>::version () const
{
   return _store.version ();
}



/*
==============================================================================
Name : begin
Description :
   Returns an iterator to the first element of the history.
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::iterator History <HistoryStoreImpl>::begin ()
{
   return _store.begin ();
}



/*
==============================================================================
Name : begin
Description :
   Returns an iterator to the first element of the history.
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::const_iterator History <HistoryStoreImpl>::begin () const
{
   return _store.begin ();
}



/*
==============================================================================
Name : cbegin
Description :
   Returns an iterator to the first element of the history.
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::const_iterator History <HistoryStoreImpl>::cbegin () const
{
   return _store.begin ();
}



/*
==============================================================================
Name : end
Description :
   Returns an iterator to the last element past the history.
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::iterator History <HistoryStoreImpl>::end ()
{
   return _store.end ();
}



/*
==============================================================================
Name : end
Description :
   Returns an iterator to the last element past the history.
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::const_iterator History <HistoryStoreImpl>::end () const
{
   return _store.end ();
}



/*
==============================================================================
Name : cend
Description :
   Returns an iterator to the last element past the history.
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::const_iterator History <HistoryStoreImpl>::cend () const
{
   return _store.end ();
}



/*
==============================================================================
Name : last_undo
Description :
   Returns an iterator to the last undo or 'end ()' if it does not exist.

   Dereferencing the iterator returns a transaction proxy that allows to
   replace the last undo point with a new transaction.

Note :
   If both last undo and first redo exist, the following assertion is true :
   last_undo () ++ == first_redo ()
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::iterator History <HistoryStoreImpl>::last_undo ()
{
   return _store.last_undo ();
}



/*
==============================================================================
Name : last_undo
Description :
   Returns an iterator to the last undo or 'end ()' if it does not exist.

   Dereferencing the iterator returns a transaction proxy that allows to
   replace the last undo point with a new transaction.

Note :
   If both last undo and first redo exist, the following assertion is true :
   last_undo () ++ == first_redo ()
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::const_iterator History <HistoryStoreImpl>::last_undo () const
{
   return _store.last_undo ();
}



/*
==============================================================================
Name : clast_undo
Description :
   Returns an iterator to the last undo or 'end ()' if it does not exist.

   Dereferencing the iterator returns a transaction proxy that allows to
   replace the last undo point with a new transaction.

Note :
   If both last undo and first redo exist, the following assertion is true :
   last_undo () ++ == first_redo ()
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::const_iterator History <HistoryStoreImpl>::clast_undo () const
{
   return _store.last_undo ();
}



/*
==============================================================================
Name : first_redo
Description :
   Returns an iterator to the first redo or 'end ()' if it does not exist.
Note :
   If both last undo and first redo exist, the following assertion is true :
   last_undo () ++ == first_redo ()
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::iterator History <HistoryStoreImpl>::first_redo ()
{
   return _store.first_redo ();
}



/*
==============================================================================
Name : first_redo
Description :
   Returns an iterator to the first redo or 'end ()' if it does not exist.
Note :
   If both last undo and first redo exist, the following assertion is true :
   last_undo () ++ == first_redo ()
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::const_iterator History <HistoryStoreImpl>::first_redo () const
{
   return _store.first_redo ();
}



/*
==============================================================================
Name : cfirst_redo
Description :
   Returns an iterator to the first redo or 'end ()' if it does not exist.
Note :
   If both last undo and first redo exist, the following assertion is true :
   last_undo () ++ == first_redo ()
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::const_iterator History <HistoryStoreImpl>::cfirst_redo () const
{
   return _store.first_redo ();
}



/*
==============================================================================
Name : clear
Description :
   Clear the history.
==============================================================================
*/

template <class HistoryStoreImpl>
void  History <HistoryStoreImpl>::clear ()
{
   _store.clear ();
   _store.set_first_redo (end ());
}



/*
==============================================================================
Name : erase
Description :
   Erase an element from history.
==============================================================================
*/

template <class HistoryStoreImpl>
typename History <HistoryStoreImpl>::iterator History <HistoryStoreImpl>::erase (iterator it)
{
   auto it_next = it;
   ++it_next;

   if (it == _store.first_redo ())
   {
      _store.set_first_redo (it_next);
   }

   _store.erase (it);

   return it_next;
}



/*
==============================================================================
Name : add_undo_step
Description :
   Add an undo step to the undo stack and flush the redo stack.
==============================================================================
*/

template <class HistoryStoreImpl>
void  History <HistoryStoreImpl>::add_undo_step (Transaction tx)
{
   if (!tx.has_opcode_in_undo ()) return; // abort

   auto it = first_redo ();
   auto it_end = end ();

   for (; it != it_end ;)
   {
      it = erase (it);
   }

   _store.push (std::move (tx));
   _store.set_first_redo (end ());
}



/*
==============================================================================
Name : execute_undo
==============================================================================
*/

template <class HistoryStoreImpl>
bool  History <HistoryStoreImpl>::execute_undo ()
{
   auto it_last = last_undo ();

   if (it_last == end ()) flip_FATAL;

   Transaction tx = *it_last;

   // last undo becomes first redo
   _store.set_first_redo (it_last);

   const bool undo_mode_flag = true;

   bool ok_flag = _document.execute_correct_backward (tx, undo_mode_flag);

   return ok_flag;
}



/*
==============================================================================
Name : execute_redo
==============================================================================
*/

template <class HistoryStoreImpl>
bool  History <HistoryStoreImpl>::execute_redo ()
{
   auto it_first = first_redo ();

   if (it_first == end ()) flip_FATAL;

   Transaction tx = *it_first;

   ++it_first;

   _store.set_first_redo (it_first);

   const bool undo_mode_flag = true;

   bool ok_flag = _document.execute_correct_forward (tx, undo_mode_flag);

   return ok_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_History_CODEHEADER_INCLUDED

#undef flip_History_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

