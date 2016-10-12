/*****************************************************************************

      BackEndIR.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Ref.h"
#include "flip/Transaction.h"
#include "flip/detail/BackEndBase.h"
#include "flip/detail/KeyRandom.h"
#include "flip/detail/TypeId.h"

#include <list>
#include <map>
#include <set>
#include <string>



namespace flip
{



class DataConsumerBase;
class DataModelBase;
class DataProviderBase;
class DocumentBase;
class Type;

class flip_API BackEndIR
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class Type
   {
   public:
      class Value
      {
      public:
         int64_t  int_num = 0LL;
         double   float_num = 0.0;
         std::pair <std::string, std::string>
                  enum_val;
         Ref      ref = Ref::null;
         std::vector <uint8_t>
                  blob;
         bool     blob_string_flag = false;
         bool     bool_num = false;
      };

      Ref         ref;
      std::string type_name;
      TypeId      type_id = TypeId::NONE;
      std::list <std::pair <std::string, Type>>
                  members;
      Value       value;
      std::map <KeyRandom, Type>
                  collection;
      std::map <KeyFloat, Type>
                  array;

      // !!! DONT READ/WRITE IN BACK-ENDS !!!
      uint32_t    _model_typeid = TypeId::NONE; // only relevant in conjunction with a DataModel

      bool        equal (const Type & rhs) const;
   };

   typedef std::set <std::string> Names;

                  BackEndIR () = default;
                  BackEndIR (const BackEndIR & rhs) = default;
                  BackEndIR (BackEndIR && rhs);
   virtual        ~BackEndIR () = default;

   BackEndIR &    operator = (const BackEndIR & rhs) = default;
   BackEndIR &    operator = (BackEndIR && rhs);

   void           clear ();

   template <class BackEnd>
   void           register_backend ();

   void           write (DocumentBase & document);
   void           read (DocumentBase & document);

   template <class BackEnd>
   void           write (DataConsumerBase & consumer) const;
   bool           read (DataProviderBase & provider);

   const DataModelBase &
                  data_model () const;

   bool           empty () const;
   bool           equal (const BackEndIR & rhs) const;

   std::string    version;
   Type           root;

   Names          member_names;
   Names          typenames;
   Names          enum_names;
   Names          enumerator_names;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class BackEndDelegate
   {
   public:
      BackEndBase::IsFormatProc
                  _is_format_proc = 0;
      BackEndBase::CreateProc
                  _create_proc = 0;
   };

   typedef std::list <BackEndDelegate> BackEndDelegateList;


   void           write (Type & dst, flip::Type & src);
   void           write_bool (Type & dst, flip::Type & src);
   void           write_int (Type & dst, flip::Type & src);
   void           write_float (Type & dst, flip::Type & src);
   void           write_enum (Type & dst, flip::Type & src);
   void           write_object_ref (Type & dst, flip::Type & src);
   void           write_blob (Type & dst, flip::Type & src);
   void           write_collection (Type & dst, flip::Type & src);
   void           write_array (Type & dst, flip::Type & src);
   void           write_message (Type & dst, flip::Type & src);
   void           write_object (Type & dst, flip::Type & src);

   void           consolidate_typeid ();
   void           consolidate_typeid (Type & type);

   uint32_t       get_type_id (flip::Type & type);

   const DataModelBase *
                  _data_model_ptr = nullptr;

   BackEndDelegateList
                  _backend_list;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const BackEndIR & rhs) const = delete;
   bool           operator != (const BackEndIR & rhs) const = delete;



}; // class BackEndIR



flip_API Transaction
                  substract (const BackEndIR & lhs, DocumentBase & document);



}  // namespace flip



#include "flip/BackEndIR.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

