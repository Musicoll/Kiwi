/*****************************************************************************

      DataModelBase.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <list>
#include <map>
#include <string>

#include <cstdint>



namespace flip
{



class ClassBase;
class EnumClassBase;

class flip_API DataModelBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   virtual        ~DataModelBase () = default;  // COV_NF_LINE

   bool           has_class (const std::string & name) const;
   const ClassBase &
                  get_class (const std::string & name) const;
   bool           has_class (uint32_t type_id) const;
   const ClassBase &
                  get_class (uint32_t type_id) const;
   const ClassBase &
                  get_root_class () const;

   bool           has_enum (const std::string & name) const;
   const EnumClassBase &
                  get_enum (const std::string & name) const;


   std::string    version () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   uint32_t       impl_get_type_id (const ClassBase & class_base) const;
   void           impl_add_class (ClassBase & class_base);
   bool           impl_has_class (ClassBase & class_base);

   void           impl_add_enum (EnumClassBase & enum_base);
   bool           impl_has_enum (EnumClassBase & enum_base);

   void           impl_set_version (const std::string & version);
   void           impl_set_root (ClassBase & class_base);
   void           impl_post_process ();

   void           impl_clone_from (const DataModelBase & other);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  DataModelBase () = default;

   bool           is_name_valid (const std::string & name);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   using ClassList = std::list <ClassBase *>;
   using NameClassMap = std::map <std::string, ClassBase *>;
   using TypeIdClassMap = std::map <uint32_t, ClassBase *>;
   using ClassTypeIdMap = std::map <const ClassBase *, uint32_t>;

   using EnumClassList = std::list <EnumClassBase *>;
   using NameEnumClassMap = std::map <std::string, EnumClassBase *>;

   // description
   ClassList      _class_list;
   EnumClassList  _enum_list;
   std::string    _version;
   ClassBase *    _root_ptr = nullptr;

   // cache
   NameClassMap   _name_class_map;
   TypeIdClassMap _typeid_class_map;
   ClassTypeIdMap _class_typeid_map;

   NameEnumClassMap
                  _name_enum_map;

   bool           _processed_flag = false;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DataModelBase (const DataModelBase & rhs) = delete;
   DataModelBase &operator = (const DataModelBase & rhs) = delete;
                  DataModelBase (DataModelBase && rhs) = delete;
   DataModelBase &operator = (DataModelBase && rhs) = delete;
   bool           operator == (const DataModelBase & rhs) const = delete;
   bool           operator != (const DataModelBase & rhs) const = delete;



}; // class DataModelBase



}  // namespace flip



//#include  "flip/DataModelBase.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

