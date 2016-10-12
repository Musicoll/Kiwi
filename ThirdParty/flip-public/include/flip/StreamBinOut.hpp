/*****************************************************************************

      StreamBinOut.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_StreamBinOut_CURRENT_CODEHEADER)
   #error Recursive inclusion of StreamBinOut code header.
#endif
#define  flip_StreamBinOut_CURRENT_CODEHEADER

#if ! defined (flip_StreamBinOut_CODEHEADER_INCLUDED)
#define  flip_StreamBinOut_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : operator <<
==============================================================================
*/

template <class T>
StreamBinOut & StreamBinOut::operator << (const std::vector <T> & val)
{
   this->operator << (uint64_t (val.size ()));

   write (val.begin (), val.end ());

   return *this;
}



/*
==============================================================================
Name : operator <<
==============================================================================
*/

template <class T>
StreamBinOut & StreamBinOut::operator << (const std::list <T> & val)
{
   this->operator << (uint64_t (val.size ()));

   write (val.begin (), val.end ());

   return *this;
}



/*
==============================================================================
Name : operator <<
==============================================================================
*/

template <class T, class U>
StreamBinOut & StreamBinOut::operator << (const std::map <T, U> & val)
{
   this->operator << (uint64_t (val.size ()));

   write (val.begin (), val.end ());

   return *this;
}



/*
==============================================================================
Name : operator <<
==============================================================================
*/

template <class T, class U>
StreamBinOut & StreamBinOut::operator << (const std::pair <T, U> & val)
{
   this->operator << (val.first);
   return this->operator << (val.second);
}



/*
==============================================================================
Name : write
==============================================================================
*/

template <class InputIterator>
void  StreamBinOut::write (InputIterator it, InputIterator it_end)
{
   for (; it != it_end ; ++it)
   {
      this->operator << (*it);
   }
}



/*
==============================================================================
Name : write
Description :
   General case. T must support 'void T::write (StreamBinOut &)'
==============================================================================
*/

template <class T>
StreamBinOut & StreamBinOut::operator << (const T & val)
{
   val.write (*this);

   return *this;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_StreamBinOut_CODEHEADER_INCLUDED

#undef flip_StreamBinOut_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

