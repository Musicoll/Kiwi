/*****************************************************************************

      iterator.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Object.h"
#include "flip/detail/ContainerIterator.h"
#include "flip/detail/KeyFloat.h"
#include "flip/detail/KeyRandom.h"



namespace flip
{



using array_iterator = ContainerIterator <KeyFloat, Object>;
using const_array_iterator = ContainerIterator <KeyFloat, const Object>;

using collection_iterator = ContainerIterator <KeyRandom, Object>;
using const_collection_iterator = ContainerIterator <KeyRandom, const Object>;



}  // namespace flip



//#include  "flip/iterator.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

