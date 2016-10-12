/*****************************************************************************

      ClassBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Ref.h"
#include "flip/detail/Representative.h"

#include <list>
#include <map>
#include <string>
#include <vector>



namespace flip
{



class DataModelBase;
class DocumentBase;
class Type;

class flip_API ClassBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class Member
   {
   public:
      const ClassBase *
                  _base_ptr = nullptr;
      const char *
                  _name_0 = nullptr;
      Representative
                  _representative;
   };

   typedef std::vector <Member> Members;

                  ClassBase () = default;
   virtual        ~ClassBase () = default;   // COV_NF_LINE

   const Members &
                  members () const;
   const char *   name () const;
   uint32_t       type_id (const DataModelBase & model) const;
   uint64_t       size_of () const;
   bool           inherit_from (const ClassBase & super_class) const;
   virtual bool   abstract () const = 0;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   void           impl_post_process_sizeof ();
   void           impl_check_members () const;

   virtual Type * impl_create (DocumentBase & document, Ref ref) const = 0;
   virtual void   impl_release (Type * obj_ptr, DocumentBase & document) const = 0;
   virtual Type * impl_ctor () const = 0;
   virtual Type * impl_copy_ctor (Type & obj) const = 0;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   bool           is_name_valid (const std::string & name);

   const char *   _name_0 = nullptr;
   const ClassBase *
                  _super_class_ptr = nullptr;
   uint64_t       _sizeof = 1ULL;
   Members        _members;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ClassBase (const ClassBase & rhs) = delete;
   ClassBase &    operator = (const ClassBase & rhs) = delete;
                  ClassBase (ClassBase && rhs) = delete;
   ClassBase &    operator = (ClassBase && rhs) = delete;
   bool           operator == (const ClassBase & rhs) const = delete;
   bool           operator != (const ClassBase & rhs) const = delete;



}; // class ClassBase



}  // namespace flip



//#include  "flip/ClassBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

