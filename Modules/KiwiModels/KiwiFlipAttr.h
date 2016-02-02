/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef __DEF_KIWI_MODELS_FLIPATTR__
#define __DEF_KIWI_MODELS_FLIPATTR__

#include "KiwiFlipAtom.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
    //! The attribute is an abstract class that holds a set of values of differents kinds and differents sizes.
    /** The attribute manages a set of values that should be displayed in an inspector. The setter and getter must be override.
     */
    class AttributeBase : public flip::Object
    {
    public:
        class Manager;
        typedef shared_ptr<Manager>         sManager;
        typedef weak_ptr<Manager>           wManager;
        typedef shared_ptr<const Manager>   scManager;
        typedef weak_ptr<const Manager>     swManager;
        
        class Listener;
        typedef shared_ptr<Listener>        sListener;
        typedef weak_ptr<Listener>          wListener;
        typedef shared_ptr<const Listener>  scListener;
        typedef weak_ptr<const Listener>    wcListener;
        
        /** Flags describing the behavior of the attribute.
         @see setInvisible, setDisabled, setSaveable, setNotifyChanges
         */
        enum Behavior
        {
            Invisible			= 1<<0,     ///< Indicates that the attribute is invisible.
            Disabled			= 1<<1,     ///< Indicates that the attribute can't be changed.
            Unsaved             = 1<<2,     ///< Indicates that the attribute is not saved.
            Silent              = 1<<3,     ///< Indicates that the attribute should not notify its changes.
            All                 = Invisible | Disabled | Unsaved | Silent
        };
        
        // FL!P static declare method
        static void declare()
        {
            Model::declare<AttributeBase>()
            .name("cicm.kiwi.AttributeBase")
            .member<FlipTag,        &AttributeBase::m_name>      ("name")
            .member<flip::String,   &AttributeBase::m_label>     ("label")
            .member<flip::String,   &AttributeBase::m_category>  ("category")
            .member<flip::Int,      &AttributeBase::m_order>     ("order")
            .member<flip::Bool,     &AttributeBase::m_frozen>    ("frozen")
            .member<flip::Bool,     &AttributeBase::m_invisible> ("invisible")
            .member<flip::Bool,     &AttributeBase::m_disabled>  ("disabled")
            .member<flip::Bool,     &AttributeBase::m_unsaved>   ("unsaved")
            .member<flip::Bool,     &AttributeBase::m_silent>    ("silent");
        }
        
    protected:
        FlipTag         m_name;             ///< The name of the attribute.
        flip::String    m_label;			///< The label of the attribute.
        flip::String    m_category;			///< The category of the attribute.
        flip::Int       m_order;			///< The order of the attribute.
        flip::Bool      m_frozen;           ///< The frozen state of the attribute.
        flip::Bool      m_invisible;        ///< Indicates that the attribute is invisible.
        flip::Bool      m_disabled;         ///< Indicates that the attribute can't be changed.
        flip::Bool      m_unsaved;          ///< Indicates that the attribute is not saved.
        flip::Bool      m_silent;           ///< Indicates that the attribute should not notify its changes.
        
        ListenerSet<Listener>
                        m_listeners;        ///< The listeners.
        
        //! Sets the attribute value with an atom.
        /** The function sets the attribute value with an atom.
         @param atom The atom.
         */
        virtual inline void setValue(Atom const& atom) { assert(true); };
        
        //! Set the attribute default value with an atom.
        /** The function sets the attribute default value with an atom.
         @param atom The atom.
         */
        virtual void setDefaultValue(Atom const& atom) { assert(true); }
        
        //! Freezes or unfreezes the current value.
        /** Freezes or unfreezes the current value.
         @param frozen If true the attribute will be frozen, if false it will be unfrozen.
         */
        virtual inline void freeze(const bool frozen) {};
        
        //! Resets the value to its default state.
        /** Resets the value to its default state.
         */
        virtual inline void resetDefault()  {};
        
        //! Resets the attribute values to frozen values.
        /** Resets the attribute values to its frozen values.
         */
        virtual inline void resetFrozen() {};
        
        //! Sets the whole behavior flags field of the attribute.
        /** The function sets the whole behavior flags field of the attribute.
         @param behavior	A combination of the flags which define the attribute's behaviors.
         */
        inline void setBehavior(const ulong behavior) noexcept
        {
            m_invisible = (behavior & Behavior::Invisible);
            m_disabled  = (behavior & Behavior::Disabled);
            m_unsaved   = (behavior & Behavior::Unsaved);
            m_silent    = (behavior & Behavior::Silent);
        }
        
        //! Sets if the attribute is visible or not.
        /** The function sets if the attribute is visible or not.
         @param state If true, the attribute will be invisible, otherwise it will be visible.
         */
        inline void setInvisible(const bool state) noexcept     {m_invisible = state;}
        
        //! Sets if the attribute is disabled or not.
        /** The function sets if the attribute is disabled or not.
         @param state If true, the attribute will be disabled, otherwise it will be enabled.
         */
        inline void setDisabled(const bool state) noexcept      {m_disabled = state;}
        
        //! Sets if the attribute is saved or not.
        /** The function sets if the attribute is saved or not.
         @param state If true, the attribute will be saved, otherwise it won't be saved.
         */
        inline void setUnsaved(const bool state) noexcept       {m_unsaved = state;}
        
        //! Sets if the attribute is notifier or not.
        /** The function sets if the attribute is notifier or not.
         @param state If true, the attribute will notify changes, otherwise it won't notify changes.
         */
        inline void setSilent(const bool state) noexcept        {m_silent = state;};
        
        //! Adds a listener.
        /** The functions adds a listener to the attribute.
         @param listener The listener.
         */
        inline void addListener(sListener listener) noexcept    {m_listeners.add(listener);}
        
        //! Removes a listener.
        /** The functions removes a listener from the attribute.
         @param listener The listener.
         */
        inline void removeListener(sListener listener) noexcept {m_listeners.remove(listener);}
        
        //! Gets the listeners.
        /** The functions gets the liteners from the attribute and removes the deprecated listeners.
         @return The listeners.
         */
        inline vector<sListener> getListeners() noexcept { return m_listeners.getListeners(); }
        
    public:
        
        //inline AttributeBase(flip::Default&) {}
        
        inline AttributeBase() noexcept :
        m_name(Tags::_empty),
        m_label(""),
        m_category(""),
        m_order(0ul),
        m_frozen(false),
        m_invisible(false),
        m_disabled(false),
        m_unsaved(false),
        m_silent(false)
        {
            ;
        }
        
        //! Constructor.
        /** Allocate and initialize the member values.
         @param name			The name of the attribute (usually only letters and undescore characters).
         @param label			A short description of the attribute in a human readable style.
         @param category		A named category that the attribute fits into.
         @param order			The attribute order.
         @param behavior		A combination of the flags which define the attribute's behavior.
         */
        inline AttributeBase(const sTag name, string const& label, string const& category, const ulong behavior, const ulong order) noexcept :
        m_name(name), m_label(label), m_category(category), m_order(order), m_frozen(false)
        {
            setBehavior(behavior);
        }
        
        //! Constructor.
        /** Allocate and initialize the member values.
         @param name			The name of the attribute (usually only letters and undescore characters).
         @param label			A short description of the attribute in a human readable style.
         @param category		A named category that the attribute fits into.
         @param order			The attribute order.
         @param behavior		A combination of the flags which define the attribute's behavior.
         */
        inline AttributeBase(sTag&& name, string&& label, string&& category, const ulong behavior, const ulong order) noexcept :
        m_name(forward<sTag>(name)), m_label(forward<string>(label)), m_category(forward<string>(category)), m_order(order), m_frozen(false)
        {
            setBehavior(behavior);
        }
        
        //! Destructor.
        /** Clear the attribute.
         */
        virtual inline ~AttributeBase() noexcept {m_listeners.clear();};
        
        //! Retrieve the type index of the attribute.
        /** The function retrieves the type index of the attribute.
         @return The type index of the attribute.
         */
        virtual type_index getTypeIndex() const noexcept { return typeid(this); };
        
        //! Retrieve the attribute value an atom.
        /** The function retrieves the attribute value as  an atom.
         @return The atom.
         */
        virtual inline Atom getValue() const noexcept { return Atom(); };
        
        //! Retrieve if the attribute is from a specific template.
        /** The function retrieves if the attribute is from a specific template.
         @return true if the attribute is from a specific template.
         */
        template<class T> inline bool isType() const noexcept {return (type_index)typeid(T) == getTypeIndex();}
        
        //! Retrieve the name of the attribute.
        /** The function retrieves the name of the attribute.
         @return The name of the attribute.
         */
        inline sTag getName() const noexcept {return m_name;}
        
        //! Set the attribute name.
        /** The function sets the attribute name.
         @param label The attribute name.
         */
        inline AttributeBase& setName(const sTag name) noexcept
        {
            m_name = name;
            return *this;
        }
        
        //! Retrieve the attribute label.
        /** The function retrieves the attribute label.
         @return The attribute label.
         */
        inline string getLabel() const noexcept {return m_label;}
        
        //! Set the attribute label.
        /** The function sets the attribute label.
         @param label The attribute label.
         */
        inline AttributeBase& setLabel(const string& label) noexcept
        {
            m_label = label;
            return *this;
        }
        
        //! Set the attribute label.
        /** The function sets the attribute label.
         @param label The attribute label.
         */
        inline AttributeBase& setDefault(const Atom& atom) noexcept
        {
            setDefaultValue(atom);
            resetDefault();
            return *this;
        }
        
        //! Retrieve the attribute category.
        /** The function retrieves the attribute category.
         @return The attribute category.
         */
        inline string getCategory() const noexcept {return m_category;}
        
        //! Retrieve the attribute order.
        /** The function retrieves the attribute order.
         @return The attribute order.
         */
        inline ulong getOrder() const noexcept {return m_order;}
        
        //! Retrieve if the attribute is invisible.
        /** The function retrieves if the attribute is invisible.
         @return True if the attribute is invisible otherwise false.
         */
        inline bool isInvisible() const noexcept {return m_invisible;}
        
        //! Retrieve if the attribute is disable.
        /** The function retrieves if the attribute is disable.
         @return True if the attribute is disabled otherwise false.
         */
        inline bool isDisabled() const noexcept {return m_disabled;}
        
        //! Retrieve if the attribute is saved.
        /** The function retrieves if the attribute is saved.
         @return True if the attribute is saveable otherwise false.
         */
        inline bool isUnsaved() const noexcept {return m_unsaved;}
        
        //! Retrieve if the attribute should notify changes.
        /** The function retrieves if the attribute should notify changes.
         @return True if the attribute should notify changes otherwise false.
         */
        inline bool isSilent() const noexcept {return m_silent;}
        
        //! Retrieve if the attribute is frozen.
        /** The function retrieves if the attribute is frozen.
         @return True if the attribute is frozen, false otherwise.
         */
        inline bool isFrozen() const noexcept {return m_frozen;}
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE TYPED                                 //
    // ================================================================================ //
    
    template <class T> class Attribute : public AttributeBase
    {
    protected:
        T m_default;
        T m_value;
        T m_frozen_value;
        
    public:
        
        //! Constructor.
        inline Attribute() = default;
        
        //! Constructor.
        inline Attribute(const sTag name,
                         string const& label,
                         string const& category,
                         T const& value,
                         const ulong behavior = 0ul,
                         const ulong order = 0ul)  noexcept
        
        : AttributeBase(name, label, category, behavior, order),
        m_default(value),
        m_value(value)
        {
            static_assert(is_base_of<flip::Type, T>::value, "The class must inherit from flip::Type");
        }
        
        //! Destructor.
        /** You should never have to use the function.
         */
        inline ~Attribute() noexcept {}
        
        // static flip declare method.
        static void declare(const string& classname)
        {
            //static string class_name = "cicm.kiwi.Attribute.";
            //class_name += typeid(T).name();

            Model::declare<Attribute<T>>()
            .name(classname.c_str())
            .template inherit<AttributeBase>()
            .template member<T, &Attribute::m_default>("default_value")
            .template member<T, &Attribute::m_value>("value")
            .template member<T, &Attribute::m_frozen_value>("frozen_value");
        }
        
        //! Retrieve the type index of the attribute.
        /** The function retrieves the type index of the attribute.
         @return The type index of the attribute.
         */
        inline type_index getTypeIndex() const noexcept override {return typeid(T);}
    
        //! Retrieves the values.
        /** The current values.
         @return The current values.
         */
        inline T get() const {return m_value;}
        
        //! Retrieves the default value.
        /** Retrieve the default value.
         @return The the default value.
         */
        inline T getDefault() const {return m_default;}
        
        //! Retrieve the frozen value.
        /** Retrieve the frozen value.
         @return The the frozen value.
         */
        inline T getFrozen() const {return m_frozen_value;}
        
        //! Retrieve the attribute value as a vector of atoms.
        /** The function retrieves the attribute value as a vector of atoms.
         @return The vector of atoms.
         */
        Atom getValue() const noexcept override {return Atom(m_value);}
        
    private:
        
        //! Sets the values.
        /** The function sets the current value.
         @param elements The vector of elements.
         @see get
         */
        inline void set(T const& value) { m_value = value; }
        
        //! Set the attribute value with an atom.
        /** The function sets the attribute value with an atom.
         @param atom The atom.
         */
        virtual void setValue(Atom const& atom) override { if(!m_frozen) m_value = atom; }
        
        //! Set the attribute default value with an atom.
        /** The function sets the attribute default value with an atom.
         @param atom The atom.
         */
        virtual void setDefaultValue(Atom const& atom) override { m_default = atom; }
        
        //! Freezes or unfreezes the current value.
        /** Freezes or unfreezes the current value.
         @param frozen If true the attribute will be frozen, if false it will be unfrozen.
         */
        inline void freeze(const bool frozen) override { m_frozen = frozen; m_frozen_value = m_value; }
        
        //! Resets the value to its default state.
        /** Resets the value to its default state.
         */
        inline void resetDefault() override  {set(m_default);}
        
        //! Resets the attribute values to frozen values.
        /** Resets the attribute values to its frozen values.
         */
        inline void resetFrozen() override  {set(m_frozen_value);}
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE LISTENER                              //
    // ================================================================================ //
    
    //! The attribute manager listener is a virtual class that can be binded to an attribute manager to be notified of various changes.
    /** The attribute manager listener is a very light class that allows to be notified of the attributes modification.
     */
    class AttributeBase::Listener
    {
    public:
        virtual ~Listener() {}
        
        //! Receive the notification that an attribute has changed.
        /** The function must be implement to receive notifications when an attribute is added or removed, or when its value, appearance or behavior changes.
         @param manager     The attribute manager.
         @param attr		The attribute that has been modified.
         */
        virtual void attrChanged(AttributeBase::sManager manager, AttributeBase* attr) = 0;
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE MANAGER                               //
    // ================================================================================ //
    
    //! The attribute manager manages a set of attributes.
    /** it allows the setting and the getting of their values 
     and to retrieve them by name or by category.
     @see AttributeBaseTyped
     */
    class AttributeBase::Manager : public flip::Object
    {
    private:
        vector<AttributeBase*>              m_attrs;
        flip::Collection<AttributeBase>     m_attributes;
        mutable mutex                       m_attrs_mutex;
        
        //! Retrieves an attribute.
        /** The function retrieves an attribute.
         @param name the name of the attribute.
         @return The attribute.
         */
        inline AttributeBase* getAttributeBase(const sTag name) const noexcept
        {
            lock_guard<mutex> guard(m_attrs_mutex);
            
            auto predicate = [name](AttributeBase const* attr)
            {
                return (attr->getName() == name);
            };
            
            const auto it = find_if(m_attrs.begin(), m_attrs.end(), predicate);
            if(it != m_attrs.cend())
            {
                return *it;
            }
            
            return nullptr;
        }
        
    public:
        
        //! Constructor.
        /** Creates a new attribute manager.
         */
        inline Manager() noexcept {};
        
        //! Destructor.
        /** Free the attributes.
         */
        virtual inline ~Manager() noexcept
        {
            lock_guard<mutex> guard(m_attrs_mutex);
            m_attributes.clear();
        }
        
        // static flip declare method.
        static void declare()
        {
            Model::declare<Manager>()
            .name("cicm.kiwi.AttributeBase.Manager")
            .member<flip::Collection<AttributeBase>, &AttributeBase::Manager::m_attributes>("attributes");
        }
        
        //! Retrieve an attribute value.
        /** The function retrieves an attribute value.
         @param name the name of the attribute.
         @return The value of the attribute as a vector or an empty vector if the attribute doesn't exist.
         */
        inline Atom getAttributeValue(const sTag name) const noexcept
        {
            const AttributeBase* attr = getAttributeBase(name);
            if(attr)
            {
                return attr->getValue();
            }
            
            return Atom();
        }
		
        //! Set an attribute value.
        /** The function sets an attribute value.
         @param name the name of the attribute.
         @param value The new attribute value.
         */
        inline void setAttributeValue(const sTag name, Atom const& atom) noexcept
        {
            AttributeBase* attr = getAttributeBase(name);
            if(attr)
            {
                if(attr->getValue() != atom)
                {
                    attr->setValue(atom);
                    if(this->notify(attr))
                    {
                        const auto listeners = attr->getListeners();
                        for(auto it : listeners)
                        {
                            //it->attrChanged(shared_from_this(), attr);
                        }
                    }
                }
            }
        }
        
        inline bool attributeChanged() const noexcept { return m_attributes.changed(); }
        
        //! Write the attributes in a dico.
        /** The function writes the attributes in a dico.
         @param dico The dico.
         */
        void write(Dico& dico) const;
        
        //! Write the attributes in a dico.
        /** The function writes the attributes in a dico.
         @param dico The dico.
         */
        void read(Dico const& dico);
        
        //! Add an attribute listener in the binding list of the attribute manager.
        /** The function adds an attribute listener in the binding list of the attribute manager. The attribute listener can specifies the names of the attributes, an empty vector means it will be attached to all the attributes.
         @param listener  The listener.
         @param names     The names of the attibutes.
         */
        void addListener(sListener listener, sTag name);
        
        //! Add an attribute listener in the binding list of the attribute manager.
        /** The function adds an attribute listener in the binding list of the attribute manager. The attribute listener can specifies the names of the attributes, an empty vector means it will be attached to all the attributes.
         @param listener  The listener.
         @param names     The names of the attibutes.
         */
        void addListener(sListener listener, vector<sTag> const& names = vector<sTag>());
        
        //! Remove a listener from the binding list of the attribute.
        /** The function removes a listener from the binding list of the attribute. The attribute listener can specifies the names of the attributes, an empty vector means it will be detached from all the attributes.
         @param listener  The listener.
         @param names     The names of the attibutes.
         */
        void removeListener(sListener listener, vector<sTag> const& names = vector<sTag>());
    
    protected:
        
        //! Notify the manager that the values of an attribute has changed.
        /** The function notifies the manager that the values of an attribute has changed.
         @param attr An attribute.
         @return pass true to notify changes to listeners, false if you don't want them to be notified
         */
        virtual bool notify(AttributeBase* attr) {return true;};
        
        //! Constructor.
        /** Allocate and initialize the member values.
         @param name			The name of the attribute.
         @param label			A short description of the attribute in a human readable style.
         @param category		A named category that the attribute fits into.
         @param order			The attribute order.
         @param behavior		A combination of the flags which define the attribute's behavior.
         */
        template<class T> inline void createAttribute(const sTag name, string const& label,
                                                 string const& category,
                                                 T const& value,
                                                 const ulong behavior = 0ul,
                                                 const ulong order = 0ul)
        {
            lock_guard<mutex> guard(m_attrs_mutex);
            m_attributes.emplace<Attribute<T>>(name, label, category, value, behavior, order);
        }
        
        //! Constructor.
        /** Allocate and initialize the member values.
         @param name			The name of the attribute.
         @param label			A short description of the attribute in a human readable style.
         @param category		A named category that the attribute fits into.
         @param order			The attribute order.
         @param behavior		A combination of the flags which define the attribute's behavior.
         */
        template<class T> inline AttributeBase& addAttr(const sTag name)
        {
            auto attr = m_attributes.emplace<Attribute<T>>(name);
            return *attr;
        }
        
        //! Constructor.
        /** Allocate and initialize the member values.
         @param name			The name of the attribute.
         @param label			A short description of the attribute in a human readable style.
         @param category		A named category that the attribute fits into.
         @param order			The attribute order.
         @param behavior		A combination of the flags which define the attribute's behavior.
         */
        template<class T> inline void addAttr(AttributeBase* attr, const sTag name, string const& label,
                            string const& category,
                            T const& value,
                            const ulong behavior = 0ul,
                            const ulong order = 0ul)
        {
            attr->setName(name);
            attr->setLabel(label);
            attr->setDefault(value);
            
            m_attrs.push_back(attr);
        }
    };
}

#endif


