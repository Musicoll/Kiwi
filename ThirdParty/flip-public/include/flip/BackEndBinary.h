/*****************************************************************************

      BackEndBinary.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/detail/BackEndBase.h"
#include "flip/BackEndIR.h"

#include <map>
#include <set>
#include <string>

#include <cstdint>



namespace flip
{



class flip_API BackEndBinary
:  public BackEndBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static bool    is_format (DataProviderBase & provider);
   static std::unique_ptr <BackEndBase>
                  create ();

                  BackEndBinary () = default;
   virtual        ~BackEndBinary () = default;

   // BackEndBase
   virtual void   write (DataConsumerBase & consumer, const BackEndIR & ir) const override;
   virtual void   read (DataProviderBase & provider, BackEndIR & ir) override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   using NameIdMap = std::map <std::string, uint32_t>;
   class NameIdMaps
   {
   public:
      NameIdMap   _types;
      NameIdMap   _members;
      NameIdMap   _enums;
      NameIdMap   _enumerators;
   };
   using IdNameMap = std::map <uint32_t, std::string>;

   static void    read_header (DataProviderBase & provider);

   void           make (NameIdMap & map, const std::set <std::string> & names) const;
   void           write (DataConsumerBase & consumer, const NameIdMap & map) const;
   void           read (DataProviderBase & provider, IdNameMap & map);
   void           make (std::set <std::string> & names, const IdNameMap & map);

   void           write (DataConsumerBase & consumer, const BackEndIR::Type & type, const NameIdMaps & names) const;
   void           write_bool (DataConsumerBase & consumer, const BackEndIR::Type & type) const;
   void           write_int (DataConsumerBase & consumer, const BackEndIR::Type & type) const;
   void           write_float (DataConsumerBase & consumer, const BackEndIR::Type & type) const;
   void           write_enum (DataConsumerBase & consumer, const BackEndIR::Type & type, const NameIdMaps & names) const;
   void           write_object_ref (DataConsumerBase & consumer, const BackEndIR::Type & type) const;
   void           write_blob (DataConsumerBase & consumer, const BackEndIR::Type & type) const;
   void           write_collection (DataConsumerBase & consumer, const BackEndIR::Type & type, const NameIdMaps & names) const;
   void           write_array (DataConsumerBase & consumer, const BackEndIR::Type & type, const NameIdMaps & names) const;
   void           write_message (DataConsumerBase & consumer, const BackEndIR::Type & type) const;
   void           write_object (DataConsumerBase & consumer, const BackEndIR::Type & type, const NameIdMaps & names) const;

   void           read (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_bool (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_int (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_float (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_enum (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_object_ref (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_blob (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_collection (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_array (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_message (DataProviderBase & provider, BackEndIR::Type & type);
   void           read_object (DataProviderBase & provider, BackEndIR::Type & type);

   IdNameMap      _map_read_type;
   IdNameMap      _map_read_member;
   IdNameMap      _map_read_enum;
   IdNameMap      _map_read_enumerator;

   static const uint32_t
                  _header_joy;
   static const uint32_t
                  _header_flip;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BackEndBinary (const BackEndBinary & rhs) = delete;
   BackEndBinary &
                  operator = (const BackEndBinary & rhs) = delete;
                  BackEndBinary (BackEndBinary && rhs) = delete;
   BackEndBinary &
                  operator = (BackEndBinary && rhs) = delete;
   bool           operator == (const BackEndBinary & rhs) const = delete;
   bool           operator != (const BackEndBinary & rhs) const = delete;



}; // class BackEndBinary



}  // namespace flip



//#include  "flip/BackEndBinary.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

