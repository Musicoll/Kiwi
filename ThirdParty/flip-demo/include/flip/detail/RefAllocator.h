/*****************************************************************************

      RefAllocator.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Ref.h"

#include <map>

#include <cstddef>
#include <cstdint>



namespace flip
{



class DocumentBase;
class Type;

class flip_API RefAllocator
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  RefAllocator (uint64_t user_id, uint64_t actor_id);
   virtual        ~RefAllocator ();

   Ref            allocate (uint64_t size);
   bool           can_allocate (const Ref & ref, uint64_t size);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   const Ref &    get_next ();

   void           bind (Type & type, const Ref & ref);
   void           unbind (Type & type, const Ref & ref);

   Type *         get (const Ref & ref) const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef std::map <Ref, Type *> RefTypeMap;

   void           update_next_ref (const Ref & ref);

   const uint64_t _user_id;
   const uint64_t _actor_id;
   Ref            _ref;

   RefTypeMap     _ref_type_map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  RefAllocator () = delete;
                  RefAllocator (const RefAllocator & rhs) = delete;
   RefAllocator & operator = (const RefAllocator & rhs) = delete;
                  RefAllocator (RefAllocator && rhs) = delete;
   RefAllocator & operator = (RefAllocator && rhs) = delete;
   bool           operator == (const RefAllocator & rhs) const = delete;
   bool           operator != (const RefAllocator & rhs) const = delete;



}; // class RefAllocator



}  // namespace flip



//#include  "flip/RefAllocator.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

