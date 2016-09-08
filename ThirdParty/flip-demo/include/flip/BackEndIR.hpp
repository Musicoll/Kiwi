/*****************************************************************************

      BackEndIR.hpp
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#if defined (flip_BackEndIR_CURRENT_CODEHEADER)
   #error Recursive inclusion of BackEndIR code header.
#endif
#define  flip_BackEndIR_CURRENT_CODEHEADER

#if ! defined (flip_BackEndIR_CODEHEADER_INCLUDED)
#define  flip_BackEndIR_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : register_backend
Description :
   register 'BackEnd' class as a back-end reader.
==============================================================================
*/

template <class BackEnd>
void  BackEndIR::register_backend ()
{
   BackEndDelegate backend_delegate;
   backend_delegate._is_format_proc = &BackEnd::is_format;
   backend_delegate._create_proc = &BackEnd::create;

   _backend_list.push_back (backend_delegate);
}



/*
==============================================================================
Name : write
==============================================================================
*/

template <class BackEnd>
void  BackEndIR::write (DataConsumerBase & consumer) const
{
   BackEnd backend;
   backend.write (consumer, *this);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace flip



#endif   // flip_BackEndIR_CODEHEADER_INCLUDED

#undef flip_BackEndIR_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

