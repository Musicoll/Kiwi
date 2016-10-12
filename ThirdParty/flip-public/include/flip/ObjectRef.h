/*****************************************************************************

      ObjectRef.h
      Copyright (c) 2014 Raphael DINGE

*Tab=3***********************************************************************/



#pragma once



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "flip/Ref.h"
#include "flip/detail/ObjectRefBase.h"



namespace flip
{



class Transaction;

template <class T>
class ObjectRef
:  public ObjectRefBase
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef Ref internal_type;
   typedef T value_type;
   typedef T & reference;
   typedef const T & const_reference;
   typedef T * pointer;
   typedef const T * const_pointer;

                  ObjectRef () = default;
                  ObjectRef (const ObjectRef & rhs);
                  ObjectRef (ObjectRef && rhs);
                  ObjectRef (T * ptr);
                  ObjectRef (const Ref & value);
   virtual        ~ObjectRef () = default;

   ObjectRef &    operator = (const ObjectRef & rhs);
   ObjectRef &    operator = (ObjectRef && rhs);
   ObjectRef &    operator = (T * ptr);
   ObjectRef &    operator = (const Ref & value);

   // Type
   virtual const ClassBase &
                  get_class () const override;
   virtual bool   changed () const override;

                  operator T * () const;
   T *            value () const;
   T *            before () const;

   // ObjectRefBase
   virtual Ref    get () const override;
   virtual Ref    get_before () const override;

   bool           operator == (T * rhs) const;
   bool           operator != (T * rhs) const;

   inline virtual void
                  revert () override;



/*\\\ INTERNAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // ObjectRefBase
   inline virtual void
                  impl_internal_set (const Ref & value) override;

   // Type
   inline virtual void
                  impl_make (Transaction & tx, ImplUndoRedoMode parent_mode, TxPostProcessInfo & /* ppinfo */) const override;
   inline virtual void
                  impl_set_class (const ClassBase & class_base) override;
   inline virtual bool
                  impl_is_reset () const override;
   inline virtual void
                  impl_reset () override;
   inline virtual void
                  impl_synchronize () override;




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   Ref            make_value (T * ptr) const;

   Ref            _value = Ref::null;
   Ref            _value_before = Ref::null;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   bool           operator == (const ObjectRef & rhs) const = delete;
   bool           operator != (const ObjectRef & rhs) const = delete;



}; // class ObjectRef



template <class T>
bool              operator == (T * lhs, const ObjectRef <T> & rhs);
template <class T>
bool              operator != (T * lhs, const ObjectRef <T> & rhs);



}  // namespace flip

// COV_NF_START

#include "flip/ObjectRef.hpp"



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

