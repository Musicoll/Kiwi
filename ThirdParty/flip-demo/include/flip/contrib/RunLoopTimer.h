/*****************************************************************************

      RunLoopTimer.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <functional>



namespace flip
{



class RunLoopTimer
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  RunLoopTimer (std::function <bool ()> func, double period = 0.1);
   virtual        ~RunLoopTimer () = default;

   void           run ();



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   std::function <bool ()>
                  _func;
   const double   _period;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  RunLoopTimer () = delete;
                  RunLoopTimer (const RunLoopTimer & rhs) = delete;
                  RunLoopTimer (RunLoopTimer && rhs) = delete;
   RunLoopTimer & operator = (const RunLoopTimer & rhs) = delete;
   RunLoopTimer & operator = (RunLoopTimer && rhs) = delete;
   bool           operator == (const RunLoopTimer & rhs) const = delete;
   bool           operator != (const RunLoopTimer & rhs) const = delete;



}; // class RunLoopTimer



}  // namespace flip



//#include  "flip/contrib/RunLoopTimer.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

