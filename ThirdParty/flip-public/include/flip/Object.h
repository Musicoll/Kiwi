/*****************************************************************************

      Object.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Entity.h"
#include "flip/detail/Type.h"
#include "flip/detail/Default.h"

#include <memory>



namespace flip
{



class Document;
class SignalBase;

class flip_API Object
:  public Type
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   class flip_API Parent
   {
   public:
                  Parent (Object & parent);

      bool        changed () const;
      template <class T>
      T *         ptr ();
      template <class T>
      T &         before ();
      template <class T>
      T *         before_ptr ();

   private:
      Object &    _parent;

                  Parent () = delete;
                  Parent (const Parent & rhs) = delete;
                  Parent (Parent && rhs) = delete;
      Parent &    operator = (const Parent & rhs) = delete;
      Parent &    operator = (Parent && rhs) = delete;
   };

                  Object ();
                  Object (const Object & rhs);
                  Object (Object && rhs);
   virtual        ~Object () = default;

   // Type
   virtual const ClassBase &
                  get_class () const override;

   Object &       operator = (const Object & rhs);
   Object &       operator = (Object && rhs);

   template <class T>
   T &            parent ();
   Parent &       parent ();

#if (flip_ENTITY_LOCATION == flip_ENTITY_LOCATION_OBJECT)
   Entity &       entity ();
   const Entity & entity () const;
#endif

   // Type
   virtual void   revert () override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // Type
   virtual void   impl_set_class (const ClassBase & class_base) override;
   virtual void   impl_bind (DocumentBase & document, Ref & ref) override;
   virtual void   impl_unbind (DocumentBase & document) override;

   virtual void   impl_change_parent (Type * parent_ptr) override;

   void           impl_bind (SignalBase & signal, uint32_t type);
   void           impl_unbind (SignalBase & signal, uint32_t type);
   void           impl_receive_signal (uint32_t type, const std::vector <uint8_t> & data);

   virtual void   impl_add () override;
   virtual void   impl_remove () override;

   virtual void   impl_make (Transaction & tx, ImplUndoRedoMode mode, TxPostProcessInfo & ppinfo) const override;

   virtual bool   impl_is_reset () const override;
   virtual void   impl_reset () override;
   virtual void   impl_validate () override;
   virtual void   impl_synchronize () override;
   virtual void   impl_entity_reset () override;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   friend class Parent;

   const ClassBase *
                  _base_ptr = nullptr;

   Type *         _parent_before_ptr = nullptr;
   Parent         _parent;

#if (flip_ENTITY_LOCATION == flip_ENTITY_LOCATION_OBJECT)
   Entity         _entity;
#endif
   std::map <uint32_t, SignalBase *>
                  _signal_map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Object & rhs) const = delete;
   bool           operator != (const Object & rhs) const = delete;



}; // class Object



}  // namespace flip



#include  "flip/Object.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

