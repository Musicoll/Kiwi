/*****************************************************************************

      BackEndBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <memory>



namespace flip
{



class BackEndIR;
class DataConsumerBase;
class DataModelBase;
class DataProviderBase;

class flip_API BackEndBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef bool (*IsFormatProc)(DataProviderBase & provider);
   typedef std::unique_ptr <BackEndBase> (*CreateProc)();

   virtual        ~BackEndBase () = default;

   virtual void   write (DataConsumerBase & consumer, const BackEndIR & ir) const = 0;
   virtual void   read (DataProviderBase & provider, BackEndIR & ir) = 0;




/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const BackEndBase & rhs) const = delete;
   bool           operator != (const BackEndBase & rhs) const = delete;



}; // class BackEndBase



}  // namespace flip



//#include  "flip/BackEndBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

