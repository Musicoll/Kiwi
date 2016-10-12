/*****************************************************************************

      specs.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#if defined (flip_SPECS_CHECK_FLAG)
   #define flip_SPECS_CHECK(document,sync) \
      { \
         SpecsCheck flip_auto_specks_check__ (document.root <Object> (), sync); \
         flip_auto_specks_check__.process (); \
      }
#else
   #define flip_SPECS_CHECK(document,sync) ((void)(0))
#endif


namespace flip
{



class ArrayBase;
class CollectionBase;
class Object;
class Type;

void              specs_check_recursive (Type & obj);
void              specs_check_recursive (ArrayBase & obj);
void              specs_check_recursive (CollectionBase & obj);
void              specs_check_recursive (Object & obj);

void              specs_check_object_state (ArrayBase & obj);
void              specs_check_object_state (CollectionBase & obj);

void              specs_check_object_state_added_recursive (Type & obj);
void              specs_check_object_state_added_recursive (Object & obj);
void              specs_check_object_state_added_recursive (ArrayBase & obj);
void              specs_check_object_state_added_recursive (CollectionBase & obj);
void              specs_check_object_state_removed_recursive (Type & obj);
void              specs_check_object_state_removed_recursive (Object & obj);
void              specs_check_object_state_removed_recursive (ArrayBase & obj);
void              specs_check_object_state_removed_recursive (CollectionBase & obj);

class SpecsCheck
{
public:
                  SpecsCheck (Type & root, bool synced_flag);

   void           process ();

private:

   enum class State
   {
                  None,
                  Resident,
                  Added,
                  Removed,
   };

   void           process (Type & obj, Type * parent_ptr, State parent_it_state);
   void           process_synced (Type & obj, Type * parent_ptr, State parent_it_state);
   void           process_unsynced (Type & obj, Type * parent_ptr, State parent_it_state);

   void           process_recurse (Type & obj);
   void           process_recurse (ArrayBase & obj);
   void           process_recurse (CollectionBase & obj);
   void           process_recurse (Object & obj);

   template <typename Iterator>
   State          state (const Iterator & it);

   Object &       top_level_object (Type & obj);

   Type &         _root;
   const bool     _synced_flag;

private:
                  SpecsCheck () = delete;
                  SpecsCheck (const SpecsCheck & rhs) = delete;
                  SpecsCheck (SpecsCheck && rhs) = delete;
      SpecsCheck &
                  operator = (const SpecsCheck & rhs) = delete;
      SpecsCheck &
                  operator = (SpecsCheck && rhs) = delete;
};



}  // namespace flip



#include  "flip/detail/specs.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

