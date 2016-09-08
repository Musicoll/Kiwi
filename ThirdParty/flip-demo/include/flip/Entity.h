/*****************************************************************************

      Entity.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"

#include <typeinfo>
#include <type_traits>
#include <typeindex>
#include <map>
#include <memory>



namespace flip
{



class Type;

class flip_API Entity
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  Entity () = default;
                  Entity (const Entity & rhs) = default;
                  Entity (Entity && rhs);
   virtual        ~Entity ();

   Entity &       operator = (const Entity & rhs) = default;
   Entity &       operator = (Entity && rhs);

   template <class T, class... Args>
   T &            emplace (Args &&... args);
   template <class T>
   void           erase ();
   template <class T>
   bool           has () const;
   template <class T>
   T &            use ();
   template <class T>
   const T &      use () const;
   template <class T>
   T *            get ();
   template <class T>
   const T *      get () const;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#if (flip_ENTITY_USE == flip_ENTITY_USE_PEDANTIC)
   void           impl_bind (Type & parent);
#endif

   void           impl_clear ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   friend class TestEntity;

   typedef void (*DeleteProc) (void *);
   typedef void * (*CopyCtorProc) (void *);

   class flip_API Bundle
   {
   public:
                  Bundle (void * ptr, DeleteProc delete_proc, CopyCtorProc copy_ctor_proc);
                  Bundle (const Bundle & rhs);
                  Bundle (Bundle && rhs);
                  ~Bundle ();

      Bundle &    operator = (const Bundle & rhs);

      void *      _ptr = nullptr;
      DeleteProc  _delete_proc;
      CopyCtorProc
                  _copy_ctor_proc;
   };

   typedef std::map <std::type_index, Bundle> TypeMap;

#if (flip_ENTITY_USE == flip_ENTITY_USE_PEDANTIC)
   void           check_pedantic ();
#endif

   template <class T>
   static void    delete_func (void * ptr);
   template <class T>
   static typename std::enable_if <
      std::is_copy_constructible <T>::value, void *
   >::type        copy_ctor_func (void * ptr);
   template <class T>
   static typename std::enable_if <
      !std::is_copy_constructible <T>::value, void *
   >::type        copy_ctor_func (void * ptr);

   TypeMap        _map;

#if (flip_ENTITY_USE == flip_ENTITY_USE_PEDANTIC)
   Type *         _parent_ptr = nullptr;
#endif



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Entity & rhs) const = delete;
   bool           operator != (const Entity & rhs) const = delete;



}; // class Entity



}  // namespace flip



#include "flip/Entity.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

