/*****************************************************************************

      LockFreeQueue.h
      Copyright (c) 2014 Raphael DINGE

      Implementation from Herb Sutter
      http://www.drdobbs.com/parallel/writing-lock-free-code-a-corrected-queue/210604448

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <atomic>



namespace flip
{



template <typename T>
class LockFreeQueue  // !!! single writer, single reader !!!
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  LockFreeQueue ();
                  ~LockFreeQueue ();

   void           push (const T & val);
   T *            pop ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class Node
   {
   public:
                  Node (T val) : _val (val) {}
      T           _val;
      Node *      _next_ptr = nullptr;
   };

   Node *         _first_ptr;
   std::atomic <Node *>
                  _divider_ptr;
   std::atomic <Node *>
                  _last_ptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  LockFreeQueue (const LockFreeQueue & other) = delete;
   LockFreeQueue &operator = (const LockFreeQueue & other) = delete;
                  LockFreeQueue (LockFreeQueue && other) = delete;
   LockFreeQueue &operator = (LockFreeQueue && other) = delete;
   bool           operator == (const LockFreeQueue &other) const = delete;
   bool           operator != (const LockFreeQueue &other) const = delete;



}; // class LockFreeQueue



}  // namespace flip



#include "flip/detail/LockFreeQueue.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

