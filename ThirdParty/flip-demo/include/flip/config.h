/*****************************************************************************

      config.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#if defined (flip_HAS_FLIP_USER_CONFIG_H)
   #include "flip_user_config.h"
#endif


/*\\\ DEFINES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

// trace virtual machine internals
#undef flip_TRACE_VM



// FATAL error throws (used in regression tests)
#define flip_FATAL_MODE_THROW          1

// FATAL error abort (used for debugging)
#define flip_FATAL_MODE_ABORT          2

// FATAL error user defined (user provides a definition of flip_FATAL)
#define flip_FATAL_MODE_USER           3



// accessing a nothing value returns 0
#define flip_NOTHING_ZERO              1

// accessing a nothing value will fatal
#define flip_NOTHING_FATAL             2



// entity must be empty on dtor and not used outside of observers
#define flip_ENTITY_USE_PEDANTIC       1

// entity must be empty on dtor
#define flip_ENTITY_USE_STRICT         2

// entity auto release on dtor
#define flip_ENTITY_USE_RELAXED        3



// entity in Type
#define flip_ENTITY_LOCATION_TYPE      1

// entity in Object
#define flip_ENTITY_LOCATION_OBJECT    2

// no entity
#define flip_ENTITY_LOCATION_NONE      3



// type registry in use () functions
#define flip_TYPE_REGISTRY_DISABLED    1

// type registry in a single registry
#define flip_TYPE_REGISTRY_ENABLED     2



/*\\\ CONFIG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

// default flip_FATAL_MODE_ABORT
#if ! defined (flip_FATAL_MODE)
   #define flip_FATAL_MODE (flip_FATAL_MODE_ABORT)
#endif



// default flip_NOTHING_ZERO
#if ! defined (flip_NOTHING)
   #define flip_NOTHING (flip_NOTHING_ZERO)
#endif



// default flip_ENTITY_USE_STRICT
#if ! defined (flip_ENTITY_USE)
   #define flip_ENTITY_USE (flip_ENTITY_USE_STRICT)
#endif

// default flip_ENTITY_LOCATION_OBJECT
#if ! defined (flip_ENTITY_LOCATION)
   #define flip_ENTITY_LOCATION (flip_ENTITY_LOCATION_OBJECT)
#endif



// when building a DLL build for windows define this to __declspec(dllexport)
// and __declspec(dllimport) for using code.  Keep this empty for static
// builds or for non-windows platforms
#if ! defined (flip_API)
   #define flip_API
#endif

// when building a DLL build for windows define this to __declspec(dllexport).
// Keep this empty for static builds or for non-windows platforms
#if ! defined (flip_API_EXPORT)
   #define flip_API_EXPORT
#endif



// when building a DLL build for windows define this to
// flip_TYPE_REGISTRY_ENABLED use a single type registry which works across
// dll boundaries.  By default this is disabled.
#if ! defined (flip_TYPE_REGISTRY_MODE)
   #define flip_TYPE_REGISTRY_MODE (flip_TYPE_REGISTRY_DISABLED)
#endif



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

