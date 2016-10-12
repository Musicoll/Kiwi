/*****************************************************************************

      BackEndMl.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/BackEndBase.h"
#include "flip/BackEndIR.h"

#include <string>



namespace flip
{



class ConsumerTextOut;
class DataConsumerBase;
class DataProviderBase;

class flip_API BackEndMl
:  public BackEndBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static bool    is_format (DataProviderBase & provider);
   static std::unique_ptr <BackEndBase>
                  create ();

                  BackEndMl () = default;
   virtual        ~BackEndMl () = default;

   // BackEndBase
   virtual void   write (DataConsumerBase & consumer, const BackEndIR & ir) const override;
   virtual void   read (DataProviderBase & provider, BackEndIR & ir) override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   using TypePtrList = std::list <const BackEndIR::Type *>;

   void           write (ConsumerTextOut & out, TypePtrList & todo) const;
   void           write (ConsumerTextOut & out, const BackEndIR::Type & type, TypePtrList & todo) const;
   void           write_members (ConsumerTextOut & out, const BackEndIR::Type & type, TypePtrList & todo) const;
   void           write_member_bool (ConsumerTextOut & out, const BackEndIR::Type & type, const std::string & name) const;
   void           write_member_int (ConsumerTextOut & out, const BackEndIR::Type & type, const std::string & name) const;
   void           write_member_float (ConsumerTextOut & out, const BackEndIR::Type & type, const std::string & name) const;
   void           write_member_enum (ConsumerTextOut & out, const BackEndIR::Type & type, const std::string & name) const;
   void           write_member_object_ref (ConsumerTextOut & out, const BackEndIR::Type & type, const std::string & name) const;
   void           write_member_blob (ConsumerTextOut & out, const BackEndIR::Type & type, const std::string & name) const;
   void           write_member_collection (ConsumerTextOut & out, const BackEndIR::Type & type, const std::string & name, TypePtrList & todo) const;
   void           write_member_array (ConsumerTextOut & out, const BackEndIR::Type & type, const std::string & name, TypePtrList & todo) const;
   void           write_member_message (ConsumerTextOut & out, const BackEndIR::Type & type, const std::string & name) const;
   void           write_member_object (ConsumerTextOut & out, const BackEndIR::Type & type, const std::string & name, TypePtrList & todo) const;

   static void    read_header (DataProviderBase & provider);
   bool           read_member (DataProviderBase & provider, BackEndIR::Type & type, Ref & ref);
   void           read_member_bool (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_member_int (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_member_float (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_member_enum (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_member_object_ref (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_member_blob (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_member_collection (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_member_array (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_member_message (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_member_object (DataProviderBase & provider, BackEndIR::Type & type, Ref & ref);

   static std::string
                  read_token (DataProviderBase & provider);
   Ref            read_ref (DataProviderBase & provider);
   void           expect_symbol (DataProviderBase & provider, const std::string & token);
   static void    skip_white_spaces (DataProviderBase & provider);
   static bool    is_white_space (char c);

   std::map <Ref, BackEndIR::Type *>
                  _read_type_ptr_map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BackEndMl (const BackEndMl & rhs) = delete;
   BackEndMl &operator = (const BackEndMl & rhs) = delete;
                  BackEndMl (BackEndMl && rhs) = delete;
   BackEndMl &operator = (BackEndMl && rhs) = delete;
   bool           operator == (const BackEndMl & rhs) const = delete;
   bool           operator != (const BackEndMl & rhs) const = delete;



}; // class BackEndMl



}  // namespace flip



//#include  "flip/BackEndMl.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

