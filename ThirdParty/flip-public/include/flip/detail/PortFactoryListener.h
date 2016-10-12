/*****************************************************************************

      PortFactoryListener.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"



namespace flip
{



class PortBase;

class flip_API PortFactoryListener
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PortFactoryListener () = default;
   virtual        ~PortFactoryListener () = default;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   virtual void   port_factory_add (PortBase & port) = 0;
   virtual void   port_factory_remove (PortBase & port) = 0;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PortFactoryListener (const PortFactoryListener & rhs) = delete;
   PortFactoryListener &
                  operator = (const PortFactoryListener & rhs) = delete;
                  PortFactoryListener (PortFactoryListener && rhs) = delete;
   PortFactoryListener &
                  operator = (PortFactoryListener && rhs) = delete;
   bool           operator == (const PortFactoryListener & rhs) const = delete;
   bool           operator != (const PortFactoryListener & rhs) const = delete;



}; // class PortFactoryListener



}  // namespace flip



//#include  "flip/PortFactoryListener.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

