/*****************************************************************************

      StreamBinIn.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_StreamBinIn_CURRENT_CODEHEADER)
   #error Recursive inclusion of StreamBinIn code header.
#endif
#define  flip_StreamBinIn_CURRENT_CODEHEADER

#if ! defined (flip_StreamBinIn_CODEHEADER_INCLUDED)
#define  flip_StreamBinIn_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator >>
==============================================================================
*/

template <class T>
StreamBinIn &  StreamBinIn::operator >> (std::vector <T> & val)
{
   uint64_t val_size;
   this->operator >> (val_size);

   for (uint64_t i = 0 ; i < val_size ; ++i)
   {
      T sub_val;
      this->operator >> (sub_val);
      val.emplace_back (sub_val);
   }

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : operator >>
==============================================================================
*/

template <class T>
StreamBinIn &  StreamBinIn::operator >> (std::list <T> & val)
{
   uint64_t val_size;
   this->operator >> (val_size);

   for (uint64_t i = 0 ; i < val_size ; ++i)
   {
      T sub_val;
      this->operator >> (sub_val);
      val.emplace_back (sub_val);
   }

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : operator >>
==============================================================================
*/

template <class T, class U>
StreamBinIn &  StreamBinIn::operator >> (std::map <T, U> & val)
{
   uint64_t val_size;
   this->operator >> (val_size);

   for (uint64_t i = 0 ; i < val_size ; ++i)
   {
      std::pair <T, U> sub_val;
      this->operator >> (sub_val);
      val.emplace_hint (val.end (), sub_val);
   }

   return *this;
}  // COV_NF_LINE



/*
==============================================================================
Name : operator >>
==============================================================================
*/

template <class T, class U>
StreamBinIn &  StreamBinIn::operator >> (std::pair <T, U> & val)
{
   this->operator >> (val.first);
   this->operator >> (val.second);

   return *this;
}



/*
==============================================================================
Name : read
==============================================================================
*/

template <class InputIterator>
void  StreamBinIn::read (InputIterator it, InputIterator it_end)
{
   for (; it != it_end ; ++it)
   {
      this->operator >> (*it);
   }
}



/*
==============================================================================
Name : operator >>
Description :
   T must support 'void T::read (StreamBinIn &)' and throw on any error.
==============================================================================
*/

template <class T>
StreamBinIn &  StreamBinIn::operator >> (T & val)
{
   val.read (*this);

   return *this;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_StreamBinIn_CODEHEADER_INCLUDED

#undef flip_StreamBinIn_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

