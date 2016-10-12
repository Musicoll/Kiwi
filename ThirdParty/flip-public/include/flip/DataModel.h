/*****************************************************************************

      DataModel.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Class.h"
#include "flip/EnumClass.h"
#include "flip/detail/DataModelBase.h"

#include <type_traits>



namespace flip
{



template <class T>   class Class;
template <class T>   class EnumClass;

template <class T>
class DataModel
:  public DataModelBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static const DataModelBase &
                  use ();

   template <class U>
   static typename std::enable_if <
      !std::is_enum <U>::value,
      Class <U> &
   >::type        declare ();

   template <class U>
   static typename std::enable_if <
      std::is_enum <U>::value,
      EnumClass <U> &
   >::type        declare ();

   template <class U>
   static typename std::enable_if <
      !std::is_enum <U>::value,
      void
   >::type        add ();

   template <class U>
   static typename std::enable_if <
      std::is_enum <U>::value,
      void
   >::type        add ();

   static void    version (const std::string & version);
   template <class U>
   static void    set_root ();

   template <class U>
   static typename std::enable_if <
      !std::is_enum <U>::value,
      bool
   >::type        has ();
   template <class U>
   static typename std::enable_if <
      std::is_enum <U>::value,
      bool
   >::type        has ();

   template <class Model>
   static void    clone_from ();

   virtual        ~DataModel () = default;   // COV_NF_LINE



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  DataModel () = default;

   static DataModelBase &
                  use_mutable ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   template <typename U>
   friend U *     registry_create_type_def ();



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  DataModel (const DataModel & rhs) = delete;
   DataModel &    operator = (const DataModel & rhs) = delete;
                  DataModel (DataModel && rhs) = delete;
   DataModel &    operator = (DataModel && rhs) = delete;
   bool           operator == (const DataModel & /*rhs*/) const {throw;} // MSVC compatibility
   bool           operator != (const DataModel & /*rhs*/) const {throw;} // MSVC compatibility



}; // class DataModel



}  // namespace flip



#include "flip/DataModel.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

