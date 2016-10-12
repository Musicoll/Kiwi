/*****************************************************************************

      Type.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/config.h"
#include "flip/Entity.h"
#include "flip/Transaction.h"
#include "flip/Ref.h"

#include <memory>
#include <vector>

#include <cstdint>



namespace flip
{



class ClassBase;
class DocumentBase;
class Ref;
class RefAllocator;
class Transaction;

class None {};
None const none {};

class flip_API Type
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  Type ();
                  Type (const Type & rhs);
                  Type (Type && rhs);
   virtual        ~Type () = default;

   Type &         operator = (const Type & rhs);
   Type &         operator = (Type && rhs);

   DocumentBase & document () const;
   const Ref &    ref () const;
   virtual const ClassBase &
                  get_class () const = 0;

   template <class T>
   T &            ancestor ();
   template <class T>
   const T &      ancestor () const;
   template <class T>
   T *            ancestor_ptr ();
   template <class T>
   const T *      ancestor_ptr () const;

   // life cycle
   bool           added () const;
   bool           resident () const;
   bool           removed () const;

   virtual bool   changed () const;

#if (flip_ENTITY_LOCATION == flip_ENTITY_LOCATION_TYPE)
   Entity &       entity ();
   const Entity & entity () const;
#endif

   void           disable_in_undo ();
   void           inherit_in_undo ();
   bool           is_in_undo_enabled () const;

   bool           is_bound () const;

   bool           can_change () const;

   void           make (Transaction & tx) const;
   virtual void   revert () = 0;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   enum class ImplUndoRedoMode
   {
                  Enable,
                  Disable,
                  ForceEnable,
   };

   class flip_API TxPostProcessInfo
   {
   public:
      void        push (Ref ref, Opcodes::iterator it);

      std::list <std::pair <Ref, Opcodes::iterator>>
                  _ref_it_list;
   };

   virtual void   impl_set_class (const ClassBase & class_base) = 0;
   virtual void   impl_bind (DocumentBase & document, Ref & ref);
   virtual void   impl_unbind (DocumentBase & document);

   virtual void   impl_change_parent (Type * parent_ptr) = 0;
   void           impl_set_parent (Type * parent_ptr);
   Type *         impl_parent_ptr () const;

   virtual void   impl_add ();
   virtual void   impl_remove ();
   bool           impl_is_added () const;

   void           impl_incr_modification_cnt (int mod_cnt);
   void           impl_incr_modification_cnt_no_propagation (int mod_cnt);
   int            impl_get_total_modification_cnt () const;

   virtual void   impl_make (Transaction & tx, ImplUndoRedoMode mode, TxPostProcessInfo & ppinfo) const = 0;
   ImplUndoRedoMode
                  impl_make_undo_redo_mode (ImplUndoRedoMode parent_mode) const;
   uint8_t        impl_make_tx_flags (ImplUndoRedoMode mode) const;

   virtual bool   impl_is_reset () const = 0;
   virtual void   impl_reset () = 0;
   virtual void   impl_validate ();
   virtual void   impl_synchronize ();
   virtual void   impl_entity_reset ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   friend class DocumentObserverTest;

   DocumentBase * _document_ptr = nullptr;
   Ref            _ref = Ref::null;

#if (flip_ENTITY_LOCATION == flip_ENTITY_LOCATION_TYPE)
   Entity         _entity;
#endif

   bool           _added_flag = false;
   bool           _added_before_flag = false;
   bool           _inherit_undo_redo_flag = true;

   Type *         _parent_ptr = nullptr;

   int            _total_modification_cnt = 0;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const Type & rhs) const = delete;
   bool           operator != (const Type & rhs) const = delete;



}; // class Type



}  // namespace flip



#include "flip/detail/Type.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

