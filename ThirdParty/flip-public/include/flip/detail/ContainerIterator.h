/*****************************************************************************

      ContainerIterator.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"
#include "flip/detail/ObjectWrapper.h"

#include <iterator>



namespace flip
{



template <class KeyType, class DerefType>
class ContainerIterator
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   typedef std::ptrdiff_t difference_type;
   typedef DerefType value_type;
   typedef DerefType & reference;
   typedef DerefType * pointer;
   typedef typename std::bidirectional_iterator_tag iterator_category;

                  ContainerIterator () = default;
   template <class Iterator>
                  ContainerIterator (Iterator it);
   virtual        ~ContainerIterator ();

                  ContainerIterator (const ContainerIterator & rhs);
   ContainerIterator &
                  operator = (const ContainerIterator & rhs);

                  ContainerIterator (ContainerIterator && rhs);
   ContainerIterator &
                  operator = (ContainerIterator && rhs);

   ContainerIterator &
                  operator ++ ();
   ContainerIterator
                  operator ++ (int);
   ContainerIterator &
                  operator -- ();
   ContainerIterator
                  operator -- (int);
   reference      operator * () const;
   pointer        operator -> () const;

   bool           operator == (const ContainerIterator & rhs) const;
   bool           operator != (const ContainerIterator & rhs) const;

   const KeyType &
                  key () const;
   bool           added () const;
   bool           resident () const;
   bool           removed () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef uint8_t Buf [64];

   typedef void (*CtorProc) (Buf & buf);
   typedef void (*CopyProc) (Buf & dst_buf, const Buf & src_buf);
   typedef void (*MoveProc) (Buf & dst_buf, Buf & src_buf);
   typedef void (*DtorProc) (Buf & buf);
   typedef void (*NextProc) (Buf & buf);
   typedef void (*PrevProc) (Buf & buf);
   typedef DerefType & (*DereferenceProc) (const Buf & buf);
   typedef bool (*EqualProc) (const Buf & buf, const Buf & rhs_buf);
   typedef const KeyType & (*KeyProc) (const Buf & buf);
   typedef bool (*AddedProc) (const Buf & buf);
   typedef bool (*ResidentProc) (const Buf & buf);
   typedef bool (*RemovedProc) (const Buf & buf);

   void           ctor (const ContainerIterator & rhs);

   template <class Iterator>
   static void    func_ctor (Buf & buf);
   template <class Iterator>
   static void    func_copy (Buf & dst_buf, const Buf & src_buf);
   template <class Iterator>
   static void    func_move (Buf & dst_buf, Buf & src_buf);
   template <class Iterator>
   static void    func_dtor (Buf & buf);
   template <class Iterator>
   static void    func_next (Buf & buf);
   template <class Iterator>
   static void    func_prev (Buf & buf);
   template <class Iterator>
   static DerefType &
                  func_dereference (const Buf & buf);
   template <class Iterator>
   static bool    func_equal (const Buf & buf, const Buf & rhs_buf);
   template <class Iterator>
   static const KeyType &
                  func_key (const Buf & buf);
   template <class Iterator>
   static bool    func_added (const Buf & buf);
   template <class Iterator>
   static bool    func_resident (const Buf & buf);
   template <class Iterator>
   static bool    func_removed (const Buf & buf);

#if (flip_COMPILER == flip_COMPILER_MSVC)
   Buf            _it;
#else
   Buf            _it = {0};
#endif

   CtorProc       _ctor_proc = 0;
   CopyProc       _copy_proc = 0;
   MoveProc       _move_proc = 0;
   DtorProc       _dtor_proc = 0;
   NextProc       _next_proc = 0;
   PrevProc       _prev_proc = 0;
   DereferenceProc
                  _dereference_proc = 0;
   EqualProc      _equal_proc = 0;
   KeyProc        _key_proc = 0;
   AddedProc      _added_proc = 0;
   ResidentProc   _resident_proc = 0;
   RemovedProc    _removed_proc = 0;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class ContainerIterator



}  // namespace flip



#include "flip/detail/ContainerIterator.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

