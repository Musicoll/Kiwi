/*****************************************************************************

      CloudStoreFilesystem.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/contrib/cloud_store/CloudStore.h"



namespace flip
{



class flip_API CloudStoreFilesystem
:  public CloudStore
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  CloudStoreFilesystem (const char * base_path_0);
   virtual        ~CloudStoreFilesystem () = default;

   virtual void   turn_async () override {}
   virtual std::set <Object>
                  fetch () override;
   virtual void   put (const std::string & key, const std::vector <uint8_t> & data) override;
   virtual std::vector <uint8_t>
                  get (const std::string & key) override;
   virtual void   remove (const std::string & key) override;

   virtual void   process () override {}



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   std::string    make_path (const std::string & key);

   static std::set <std::string>
                  list_directory (const std::string & path);
   static uint64_t
                  get_timestamp (const std::string & path);

   std::string    _base_path;

   std::set <Object>
                  _cache;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  CloudStoreFilesystem () = delete;
                  CloudStoreFilesystem (const CloudStoreFilesystem & rhs) = delete;
   CloudStoreFilesystem &
                  operator = (const CloudStoreFilesystem & rhs) = delete;
                  CloudStoreFilesystem (CloudStoreFilesystem && rhs) = delete;
   CloudStoreFilesystem &
                  operator = (CloudStoreFilesystem && rhs) = delete;
   bool           operator == (const CloudStoreFilesystem & rhs) const = delete;
   bool           operator != (const CloudStoreFilesystem & rhs) const = delete;



}; // class CloudStoreFilesystem



}  // namespace flip



//#include  "flip/CloudStoreFilesystem.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

