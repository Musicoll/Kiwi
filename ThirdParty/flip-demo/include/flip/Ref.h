/*****************************************************************************

      Ref.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <cstdint>



namespace flip
{



class StreamBinIn;
class StreamBinOut;

class flip_API Ref
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum User : uint64_t
   {
                  Root        = 0ULL,
                  Server      = 1ULL,
                  Spectator   = 2ULL,
                  Offline     = 3ULL,
   };

   static Ref     get_first (uint64_t user_id, uint64_t actor_id);

                  Ref ();
   virtual        ~Ref () = default;

                  Ref (const Ref & rhs) = default;
   Ref &          operator = (const Ref & rhs) = default;

                  Ref (Ref && rhs);
   Ref &          operator = (Ref && rhs);

   Ref &          operator ++ ();
   Ref &          operator += (uint64_t diff_ref);

   bool           operator < (const Ref & rhs) const;
   bool           operator == (const Ref & rhs) const;
   bool           operator != (const Ref & rhs) const;

   uint64_t       user () const;
   uint64_t       actor () const;
   uint64_t       obj () const;

   void           write (StreamBinOut & sbo) const;
   void           read (StreamBinIn & sbi);

   static const Ref
                  null;
   static const Ref
                  root;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   static Ref     make (uint64_t user_id, uint64_t actor_id, uint64_t obj_id);
   static uint64_t
                  make_actor (uint32_t manufacturer_id, uint32_t component_id);
   static uint64_t
                  get_root_actor ();
   bool           is_valid () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   static Ref     get_root ();

   uint64_t       _user_id = 0ULL;
   uint64_t       _actor_id = 0ULL;
   uint64_t       _obj_id = 0ULL;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class Ref



flip_API Ref   operator + (Ref lhs, uint64_t rhs);



}  // namespace flip



//#include  "flip/Ref.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

