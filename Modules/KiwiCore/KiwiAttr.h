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

#ifndef KIWI_ATTR_H_INCLUDED
#define KIWI_ATTR_H_INCLUDED

#include "KiwiAtom.h"
#include "KiwiListenerSet.h"

namespace kiwi
{
    class Attr;
    typedef std::shared_ptr<Attr>       sAttr;
    typedef std::weak_ptr<Attr>         wAttr;
    typedef std::shared_ptr<const Attr> scAttr;
    typedef std::weak_ptr<const Attr>   wcAttr;
    
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
    //! The attribute is an abstract class that holds a set of values of differents kinds and differents sizes.
    /** The attribute manages a set of values that should be displayed in an inspector. The setter and getter must be override.
     */
    class Attr : public std::enable_shared_from_this<Attr>
    {
    public:
        class Manager;
        typedef std::shared_ptr<Manager>         sManager;
        typedef std::weak_ptr<Manager>           wManager;
        typedef std::shared_ptr<const Manager>   scManager;
        typedef std::weak_ptr<const Manager>     swManager;
        
        class Listener;
        typedef std::shared_ptr<Listener>        sListener;
        typedef std::weak_ptr<Listener>          wListener;
        typedef std::shared_ptr<const Listener>  scListener;
        typedef std::weak_ptr<const Listener>    wcListener;
        
        template <class T> class Typed;
        
        /** Flags describing the behavior of the attribute.
         @see setInvisible, setDisabled, setSaveable, setNotifyChanges
         */
        enum Behavior
        {
            Invisible			= 1<<0,///< Indicates that the attribute is invisible.
            Disabled			= 1<<1,///< Indicates that the attribute can't be changed.
            Unsaved             = 1<<2,///< Indicates that the attribute is not saved.
            Silent              = 1<<3,///< Indicates that the attribute should not notify its changes.
            All                 = Invisible | Disabled | Unsaved | Silent
        };
        
    protected:
        const sTag      m_name;				///< The name of the attribute.
        const std::string    m_label;			///< The label of the attribute.
        const std::string    m_category;			///< The category of the attribute.
        const ulong		m_order;			///< The order of the attribute.
        ulong           m_behavior;			///< The behavior of the attribute.
        bool            m_frozen;           ///< The frozen state of the attribute.
        ListenerSet<Listener>
                        m_listeners;        ///< The listeners.
        
        //! Sets the attribute value with an atom.
        /** The function sets the attribute value with an atom.
         @param atom The atom.
         */
        virtual inline void setValue(Atom const& atom) = 0;
        
        //! Freezes or unfreezes the current value.
        /** Freezes or unfreezes the current value.
         @param frozen If true the attribute will be frozen, if false it will be unfrozen.
         */
        virtual inline void freeze(const bool frozen) = 0;
        
        //! Resets the value to its default state.
        /** Resets the value to its default state.
         */
        virtual inline void resetDefault()  = 0;
        
        //! Resets the attribute values to frozen values.
        /** Resets the attribute values to its frozen values.
         */
        virtual inline void resetFrozen() = 0;
        
        //! Sets the whole behavior flags field of the attribute.
        /** The function sets the whole behavior flags field of the attribute.
         @param behavior	A combination of the flags which define the attribute's behaviors.
         */
        inline void setBehavior(const ulong behavior) noexcept {if(m_behavior != behavior) {m_behavior = behavior;}}
        
        //! Sets if the attribute is visible or not.
        /** The function sets if the attribute is visible or not.
         @param state If true, the attribute will be invisible, otherwise it will be visible.
         */
        inline void setInvisible(const bool state) noexcept {state ? m_behavior |= Invisible : m_behavior &= ~Invisible;}
        
        //! Sets if the attribute is disabled or not.
        /** The function sets if the attribute is disabled or not.
         @param state If true, the attribute will be disabled, otherwise it will be enabled.
         */
        inline void setDisabled(const bool state) noexcept {state ? m_behavior |= Disabled : m_behavior &= ~Disabled;}
        
        //! Sets if the attribute is saved or not.
        /** The function sets if the attribute is saved or not.
         @param state If true, the attribute will be saved, otherwise it won't be saved.
         */
        inline void setUnsaved(const bool state) noexcept {state ? m_behavior |= Unsaved : m_behavior &= ~Unsaved;}
        
        //! Sets if the attribute is notifier or not.
        /** The function sets if the attribute is notifier or not.
         @param state If true, the attribute will notify changes, otherwise it won't notify changes.
         */
        inline void setSilent(const bool state) noexcept {state ? m_behavior |= Silent : m_behavior &= ~Silent;};
        
        //! Adds a listener.
        /** The functions adds a listener to the attribute.
         @param listener The listener.
         */
        inline void addListener(sListener listener) noexcept {m_listeners.add(listener);}
        
        //! Removes a listener.
        /** The functions removes a listener from the attribute.
         @param listener The listener.
         */
        inline void removeListener(sListener listener) noexcept {m_listeners.remove(listener);}
        
        //! Gets the listeners.
        /** The functions gets the liteners from the attribute and removes the deprecated listeners.
         @return The listeners.
         */
        inline std::vector<sListener> getListeners() noexcept { return m_listeners.getListeners(); }
        
    public:
        
        //! Constructor.
        /** Allocate and initialize the member values.
         @param name			The name of the attribute (usually only letters and undescore characters).
         @param label			A short description of the attribute in a human readable style.
         @param category		A named category that the attribute fits into.
         @param order			The attribute order.
         @param behavior		A combination of the flags which define the attribute's behavior.
         */
        inline Attr(const sTag name, std::string const& label, std::string const& category, const ulong behavior, const ulong order) noexcept :
        m_name(name), m_label(label), m_category(category), m_order(order), m_behavior(behavior), m_frozen(false) {}
        
        //! Constructor.
        /** Allocate and initialize the member values.
         @param name			The name of the attribute (usually only letters and undescore characters).
         @param label			A short description of the attribute in a human readable style.
         @param category		A named category that the attribute fits into.
         @param order			The attribute order.
         @param behavior		A combination of the flags which define the attribute's behavior.
         */
        inline Attr(sTag&& name, std::string&& label, std::string&& category, const ulong behavior, const ulong order) noexcept :
        m_name(std::forward<sTag>(name)), m_label(std::forward<std::string>(label)), m_category(std::forward<std::string>(category)), m_order(order), m_behavior(behavior), m_frozen(false) {}
        
        
        //! Destructor.
        /** Clear the attribute.
         */
        virtual inline ~Attr() noexcept {m_listeners.clear();};
        
        //! Retrieve the type index of the attribute.
        /** The function retrieves the type index of the attribute.
         @return The type index of the attribute.
         */
        virtual std::type_index getTypeIndex() const noexcept = 0;
        
        //! Retrieve the attribute value an atom.
        /** The function retrieves the attribute value as  an atom.
         @return The atom.
         */
        virtual inline Atom getValue() const noexcept = 0;
        
        //! Retrieve if the attribute is from a specific template.
        /** The function retrieves if the attribute is from a specific template.
         @return true if the attribute is from a specific template.
         */
        template<class T> inline bool isType() const noexcept {return (std::type_index)typeid(T) == getTypeIndex();}
        
        //! Retrieve the name of the attribute.
        /** The function retrieves the name of the attribute.
         @return The name of the attribute.
         */
        inline sTag getName() const noexcept {return m_name;}
        
        //! Retrieve the attribute label.
        /** The function retrieves the attribute label.
         @return The attribute label.
         */
        inline std::string getLabel() const noexcept {return m_label;}
        
        //! Retrieve the attribute category.
        /** The function retrieves the attribute category.
         @return The attribute category.
         */
        inline std::string getCategory() const noexcept {return m_category;}
        
        //! Retrieve the attribute order.
        /** The function retrieves the attribute order.
         @return The attribute order.
         */
        inline ulong getOrder() const noexcept {return m_order;}
        
        //! Retrieves the whole behavior flags field of the attribute.
        /** The function retrieves the whole behavior flags field of the attribute.
         @return behavior	A combination of the flags which define the attribute's behaviors.
         */
        inline ulong getBehavior() const noexcept{return m_behavior;}
        
        //! Retrieve if the attribute is invisible.
        /** The function retrieves if the attribute is invisible.
         @return True if the attribute is invisible otherwise false.
         */
        inline bool isInvisible() const noexcept {return m_behavior & Invisible;}
        
        //! Retrieve if the attribute is disable.
        /** The function retrieves if the attribute is disable.
         @return True if the attribute is disabled otherwise false.
         */
        inline bool isDisabled() const noexcept {return m_behavior & Disabled;}
        
        //! Retrieve if the attribute is saved.
        /** The function retrieves if the attribute is saved.
         @return True if the attribute is saveable otherwise false.
         */
        inline bool isUnsaved() const noexcept {return m_behavior & Unsaved;}
        
        //! Retrieve if the attribute should notify changes.
        /** The function retrieves if the attribute should notify changes.
         @return True if the attribute should notify changes otherwise false.
         */
        inline bool isSilent() const noexcept {return m_behavior & Silent;}
        
        //! Retrieve if the attribute is frozen.
        /** The function retrieves if the attribute is frozen.
         @return True if the attribute is frozen, false otherwise.
         */
        inline bool isFrozen() const noexcept {return m_frozen;}
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE TYPED                                 //
    // ================================================================================ //
    
    template <class T>
    class Attr::Typed : public Attr
    {
    private:
        friend class Attr::Manager;
        const T m_default;
        T       m_value;
        T       m_freezed;
    public:
        
        //! Constructor.
        /** You should never have to use the function.
         */
        inline Typed(const sTag name,
                     std::string const& label,
                     std::string const& category,
                     T const& value,
                     const ulong behavior,
                     const ulong order)  noexcept
        
            : Attr(name, label, category, behavior, order),
              m_default(value),
              m_value(value)
        {
            ;
        }
        
        //! Constructor.
        /** You should never have to use the function.
         */
        inline Typed(sTag&& name,
                     std::string&& label,
                     std::string&& category,
                     T&& value,
                     const ulong behavior,
                     const ulong order)  noexcept
        
            : Attr(std::forward<sTag>(name),
                   std::forward<std::string>(label),
                   std::forward<std::string>(category),
                   behavior, order),
              m_default(std::forward<T>(value))
        {
            resetDefault();
        }
        
        //! Destructor.
        /** You should never have to use the function.
         */
        inline ~Typed() noexcept {}
        
        //! Retrieve the type index of the attribute.
        /** The function retrieves the type index of the attribute.
         @return The type index of the attribute.
         */
        inline std::type_index getTypeIndex() const noexcept override {return typeid(T);}
    
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
        inline T getFrozen() const {return m_freezed;}
        
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
        inline void set(T const& value){m_value = value;}
        
        //! Sets the values.
        /** The function sets the current value.
         @param elements The vector of elements.
         @see get
         */
        inline void set(T&& value){m_value = std::forward<T>(value);}
        
        //! Set the attribute value with an atom.
        /** The function sets the attribute value with an atom.
         @param atom The atom.
         */
        void setValue(Atom const& atom) override {m_value = atom;}
        
        //! Freezes or unfreezes the current value.
        /** Freezes or unfreezes the current value.
         @param frozen If true the attribute will be frozen, if false it will be unfrozen.
         */
        inline void freeze(const bool frozen) override {m_frozen = frozen; m_freezed = m_value;}
        
        //! Resets the value to its default state.
        /** Resets the value to its default state.
         */
        inline void resetDefault() override  {setValue(m_default);}
        
        //! Resets the attribute values to frozen values.
        /** Resets the attribute values to its frozen values.
         */
        inline void resetFrozen() override  {setValue(m_freezed);}
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE LISTENER                              //
    // ================================================================================ //
    
    //! The attribute manager listener is a virtual class that can be binded to an attribute manager to be notified of various changes.
    /** The attribute manager listener is a very light class that allows to be notified of the attributes modification.
     */
    class Attr::Listener
    {
    public:
        virtual ~Listener() {}
        
        //! Receive the notification that an attribute has changed.
        /** The function must be implement to receive notifications when an attribute is added or removed, or when its value, appearance or behavior changes.
         @param manager     The attribute manager.
         @param attr		The attribute that has been modified.
         */
        virtual void attrChanged(Attr::sManager manager, sAttr attr) = 0;
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE MANAGER                               //
    // ================================================================================ //
    
    //! The attribute manager manages a set of attributes.
    /** The attribute manager manages a set of attributes, it allows the setting and the getting of their values and to retrieve them by name or by category.
     @see AttrTyped
     */
    class Attr::Manager : public inheritable_enable_shared_from_this<Manager>
    {
    private:
        std::map<sTag, sAttr>                m_attrs;
        mutable std::mutex                   m_attrs_mutex;
        
        //! Retrieves an attribute.
        /** The function retrieves an attribute.
         @param name the name of the attribute.
         @return The attribute.
         */
        inline sAttr getAttr(const sTag name) const noexcept
        {
            std::lock_guard<std::mutex> guard(m_attrs_mutex);
            const auto it = m_attrs.find(name);
            if(it != m_attrs.end())
            {
                return it->second;
            }
            return sAttr();
        }
        
        //! Retrieves an attribute.
        /** The function retrieves an attribute.
         @param name the name of the attribute.
         @return The attribute.
         */
        template <class T> inline std::shared_ptr<Typed<T>> getAttr(const sTag name) const noexcept
        {
            std::lock_guard<std::mutex> guard(m_attrs_mutex);
            const auto it = m_attrs.find(name);
            if(it != m_attrs.end())
            {
                return std::dynamic_pointer_cast<Typed<T>>(it->second);
            }
            return std::shared_ptr<Typed<T>>();
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
            std::lock_guard<std::mutex> guard(m_attrs_mutex);
            m_attrs.clear();
        }
        
        //! Retrieve an attribute value.
        /** The function retrieves an attribute value.
         @param name the name of the attribute.
         @return The value of the attribute as a vector or an empty vector if the attribute doesn't exist.
         */
        inline Atom getAttrValue(const sTag name) const noexcept
        {
            const sAttr attr = getAttr(name);
            if(attr)
            {
                return attr->getValue();
            }
            return Atom();
        }
        
        //! Retrieve an attribute value.
        /** The function retrieves an attribute value.
         @param name the name of the attribute.
         @return The value of the attribute or a default value if the attribute doesn't exist.
         */
        template<class T> inline T getAttrValue(const sTag name) const noexcept
        {
            const std::shared_ptr<Typed<T>> attr = getAttr<T>(name);
            if(attr)
            {
                return attr->get();
            }
            return T();
        }
		
        //! Set an attribute value.
        /** The function sets an attribute value.
         @param name the name of the attribute.
         @param value The new attribute value.
         */
        inline void setAttrValue(const sTag name, Atom const& atom) noexcept
        {
            sAttr attr = getAttr(name);
            if(attr)
            {
                if(attr->getValue() != atom)
                {
                    attr->setValue(atom);
                    if(this->notify(attr))
                    {
                        std::vector<sListener> listeners(attr->getListeners());
                        for(auto it : listeners)
                        {
                            it->attrChanged(shared_from_this(), attr);
                        }
                    }
                }
            }
        }
        
		//! Set an attribute value.
		/** The function sets an attribute value.
		 @param name the name of the attribute.
		 @param value The new attribute value.
		 */
		template<class T> inline void setAttrValue(const sTag name, T const& value) noexcept
		{
            std::shared_ptr<Typed<T>> attr = getAttr<T>(name);
            if(attr)
            {
                if(attr->get() != value)
                {
                    attr->set(value);
                    if(this->notify(attr))
                    {
                        std::vector<sListener> listeners(attr->getListeners());
                        for(auto it : listeners)
                        {
                            it->attrChanged(shared_from_this(), attr);
                        }
                    }
                }
            }
		}
        
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
        void addListener(sListener listener, std::vector<sTag> const& names = std::vector<sTag>());
        
        //! Remove a listener from the binding list of the attribute.
        /** The function removes a listener from the binding list of the attribute. The attribute listener can specifies the names of the attributes, an empty vector means it will be detached from all the attributes.
         @param listener  The listener.
         @param names     The names of the attibutes.
         */
        void removeListener(sListener listener, std::vector<sTag> const& names = std::vector<sTag>());
        
    protected:
        
        //! Notify the manager that the values of an attribute has changed.
        /** The function notifies the manager that the values of an attribute has changed.
         @param attr An attribute.
         @return pass true to notify changes to listeners, false if you don't want them to be notified
         */
        virtual bool notify(sAttr attr) {return true;};
        
        //! Constructor.
        /** Allocate and initialize the member values.
         @param name			The name of the attribute.
         @param label			A short description of the attribute in a human readable style.
         @param category		A named category that the attribute fits into.
         @param order			The attribute order.
         @param behavior		A combination of the flags which define the attribute's behavior.
         */
        template<class T> inline void createAttr(const sTag name, std::string const& label,
                                                 std::string const& category,
                                                 T const& value,
                                                 const ulong behavior = 0ul,
                                                 const ulong order = 0ul)
        {
            sAttr attr = std::make_shared<Typed<T>>(name, label, category, value, behavior, order);
            if(attr)
            {
                std::lock_guard<std::mutex> guard(m_attrs_mutex);
                m_attrs[name] = attr;
            }
        }
        
        //! Constructor.
        /** Allocate and initialize the member values.
         @param name			The name of the attribute.
         @param label			A short description of the attribute in a human readable style.
         @param category		A named category that the attribute fits into.
         @param order			The attribute order.
         @param behavior		A combination of the flags which define the attribute's behavior.
         */
        template<class T> inline void createAttr(const sTag name, std::string&& label,
                                                 std::string&& category,
                                                 T&& value,
                                                 const ulong behavior = 0ul,
                                                 const ulong order = 0ul)
        {
            sAttr attr = std::make_shared<Typed<T>>(sTag(name), std::forward<std::string>(label), std::forward<std::string>(category), std::forward<T>(value), behavior, order);
            if(attr)
            {
                std::lock_guard<std::mutex> guard(m_attrs_mutex);
                m_attrs[name] = attr;
            }
        }
    };
}

#endif // KIWI_ATTR_H_INCLUDED
