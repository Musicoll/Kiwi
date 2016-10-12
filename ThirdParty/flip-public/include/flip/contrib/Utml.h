/*****************************************************************************

      Utml.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/detail/def.h"
#include "flip/config.h"
#include "flip/BackEndIR.h"
#include "flip/Ref.h"

#include <string>



namespace flip
{



class ClassBase;
class ConsumerTextOut;
class DataConsumerBase;
class DataModelBase;
class DataProviderBase;
class DocumentBase;

class flip_API Utml
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class Error
   {
   public:
      void        trace () const;

      size_t      position = size_t (-1);
      size_t      line = size_t (-1);
      size_t      column = size_t (-1);
      std::string message;
      std::string context;
   };

   virtual        ~Utml () = default;

   static void    write (DataConsumerBase & consumer, const BackEndIR & backend);
   static BackEndIR
                  read (DataProviderBase & provider, const DataModelBase & model);



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   class Writer
   {
   public:
                  Writer (ConsumerTextOut & out);
      virtual     ~Writer () = default;

      void        process (const BackEndIR::Type & type);

   private:
      using RefNameMap = std::map <Ref, std::string>;
      using NameSet = std::set <std::string>;

      void        map_names (const BackEndIR::Type & type);
      void        map_names_collection (const BackEndIR::Type & type);
      void        map_names_array (const BackEndIR::Type & type);
      void        map_names_object (const BackEndIR::Type & type, const std::string & parent_name);
      static std::string
                  to_member_name (const std::string & class_name);

      void        process_object (const BackEndIR::Type & type);
      void        process_members (const BackEndIR::Type & type);
      void        process_member_bool (const BackEndIR::Type & type, const std::string & name);
      void        process_member_int (const BackEndIR::Type & type, const std::string & name);
      void        process_member_float (const BackEndIR::Type & type, const std::string & name);
      void        process_member_enum (const BackEndIR::Type & type, const std::string & name);
      void        process_member_object_ref (const BackEndIR::Type & type, const std::string & name);
      void        process_member_blob (const BackEndIR::Type & type, const std::string & name);
      void        process_member_collection (const BackEndIR::Type & type, const std::string & name);
      void        process_member_array (const BackEndIR::Type & type, const std::string & name);
      void        process_member_message (const BackEndIR::Type & type, const std::string & name);
      void        process_member_object (const BackEndIR::Type & type, const std::string & name);

      ConsumerTextOut &
                  _out;

      RefNameMap  _refs;
      NameSet     _names;

                  Writer () = delete;
                  Writer (const Writer & rhs) = delete;
      Writer &    operator = (const Writer & rhs) = delete;
                  Writer (Writer && rhs) = delete;
      Writer &    operator = (Writer && rhs) = delete;
   };

   class Reader
   {
   public:
                  Reader (DataProviderBase & provider, const DataModelBase & model);
      virtual     ~Reader () = default;

      void        process (BackEndIR::Type & type);

   private:
      using AddrNames = std::map <BackEndIR::Type *, std::string>;
      using NameAddrs = std::map <std::string, BackEndIR::Type *>;
      using TypePtrList = std::list <BackEndIR::Type *>;

      void        process_object (BackEndIR::Type & type, const ClassBase & class_base);
      void        process_member (BackEndIR::Type & type);
      void        process_member_bool (BackEndIR::Type & type);
      void        process_member_int (BackEndIR::Type & type);
      void        process_member_float (BackEndIR::Type & type);
      void        process_member_enum (BackEndIR::Type & type);
      void        process_member_object_ref (BackEndIR::Type & type);
      void        process_member_blob (BackEndIR::Type & type);
      void        process_member_collection (BackEndIR::Type & type);
      void        process_member_array (BackEndIR::Type & type);
      void        process_member_object (BackEndIR::Type & type);

      void        assign_ref (BackEndIR::Type & root);
      void        assign_ref (BackEndIR::Type & type, Ref & ref);

      void        post_process_object (BackEndIR::Type & type);
      void        post_process_member (BackEndIR::Type & type);
      void        post_process_member_object_ref (BackEndIR::Type & type);
      void        post_process_member_collection (BackEndIR::Type & type);
      void        post_process_member_array (BackEndIR::Type & type);
      void        post_process_member_object (BackEndIR::Type & type);

      std::string read_token ();
      std::string read_string ();
      std::string read_base64 ();
      bool        compare (const std::string & str);
      void        expect_symbol (const std::string & symbol);
      void        skip_whitespaces ();
      static bool is_whitespace (char c);
      static bool is_token (char c);

      void        validate_idt (const std::string & idt);
      void        validate_expr (const std::string & expr);
      std::list <std::string>
                  split_expr (const std::string & expr);

      void        init (BackEndIR::Type & type, const ClassBase & class_base);
      BackEndIR::Type &
                  find (BackEndIR::Type & type, const std::string & name);
      flip_NORETURN void
                  throw_error (const std::string & message);
      std::string grab_line ();

      DataProviderBase &
                  _provider;
      const DataModelBase &
                  _model;

      size_t      _pos = 0;
      AddrNames   _addr_names;
      NameAddrs   _name_addrs;
      TypePtrList _ref_todo;

                  Reader () = delete;
                  Reader (const Reader & rhs) = delete;
      Reader &    operator = (const Reader & rhs) = delete;
                  Reader (Reader && rhs) = delete;
      Reader &    operator = (Reader && rhs) = delete;
   };



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Utml () = delete;
                  Utml (const Utml & rhs) = delete;
   Utml &         operator = (const Utml & rhs) = delete;
                  Utml (Utml && rhs) = delete;
   Utml &         operator = (Utml && rhs) = delete;
   bool           operator == (const Utml & rhs) const = delete;
   bool           operator != (const Utml & rhs) const = delete;



}; // class Utml



}  // namespace flip



//#include  "flip/Utml.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

