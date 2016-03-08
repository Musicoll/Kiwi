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

#include "KiwiFlipTypes.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
    //! The attribute is an abstract class that holds a set of values of differents kinds and differents sizes.
    /** The attribute manages a set of values that should be displayed in an inspector. The setter and getter must be overriden.
     */
    class Attribute : public flip::Object
    {
    public:
        
        //! @brief Enum of Attribute types
        enum class Type : uint8_t
        {
            Invalid = 0,
            Int,
            Float,
            String,
            RGBA,
            Enum
        };
        
        class Manager;
        
        //! @brief Sets the attribute value with an atom.
        //! @details This method must be implemented in subclasses.
        //! @param atom The atom.
        virtual void setValue(std::vector<Atom> const&)          { assert(true); };
        
        //! Set the attribute default value with an atom.
        /** The function sets the attribute default value with an atom.
         @param atom The atom.
         */
        virtual void setDefaultValue(std::vector<Atom> const&)   { assert(true); }
        
        //! Resets the value to its default state.
        virtual void resetDefault()  {};
        
    public:
        
        //! Default constructor
        Attribute() = default;
        
        //! @brief Constructs an Attribute object.
        //! @param name The name of the attribute (usually only letters and undescore characters).
        Attribute(std::string const& name) noexcept : m_name(name) {};
        
        //! @brief Move constructor.
        //! @param name The name of the attribute (usually only letters and undescore characters).
        inline Attribute(std::string&& name) noexcept : m_name(std::forward<std::string>(name)) {};
        
        //! @brief Destructor.
        virtual inline ~Attribute() noexcept {};
        
        //! Retrieve the Type of the attribute.
        //! @return The Type of the attribute.
        virtual Type getType() const noexcept = 0;
        
        //! Retrieve the attribute value an atom.
        /** The function retrieves the attribute value as  an atom.
         @return The atom.
         */
        virtual inline std::vector<Atom> getValue() const noexcept { return std::vector<Atom>(); };
        
        //! Retrieve the name of the attribute.
        /** The function retrieves the name of the attribute.
         @return The name of the attribute.
         */
        inline std::string getName() const { return m_name; }
        
        //! Set the attribute name.
        /** The function sets the attribute name.
         @param label The attribute name.
         */
        inline Attribute& setName(std::string const& name) noexcept
        {
            m_name = name;
            return *this;
        }
        
        //! Set the attribute label.
        /** The function sets the attribute label.
         @param label The attribute label.
         */
        inline Attribute& setDefault(std::vector<Atom> const& atom) noexcept
        {
            setDefaultValue(atom);
            resetDefault();
            return *this;
        }
        
        //! @internal flip static declare method
        static void declare()
        {
            Model::declare<Attribute>()
            .name("cicm.kiwi.Attribute")
            .member<flip::String, &Attribute::m_name>("name");
        }
        
    protected:
        
        flip::String    m_name;  ///< The name of the attribute.
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE RGBA                                  //
    // ================================================================================ //
    
    class AttrRGBA : public Attribute
    {
    public:
        using value_t = FlipRGBA;
        
    protected:
        value_t m_default;
        value_t m_value;
        
    public:
        
        //! @brief Default constructor.
        AttrRGBA() = default;
        
        //! @brief Constructs an Attribute.
        AttrRGBA(std::string const& name, value_t const& value) :
            Attribute(name),
            m_default(value),
            m_value(value)
        {
            ;
        }
        
        //! @brief Destructor.
        inline ~AttrRGBA() noexcept {}
        
        // static flip declare method.
        static void declare()
        {
            Model::declare<AttrRGBA>()
            .name("cicm.kiwi.AttrRGBA")
            .inherit<Attribute>()
            .member<value_t, &AttrRGBA::m_default>("default_value")
            .member<value_t, &AttrRGBA::m_value>("value");
        }
        
        inline Type getType() const noexcept override {return Type::RGBA;}
    
        //! Retrieves the values.
        /** The current values.
         @return The current values.
         */
        inline value_t get() const {return m_value;}
        
        //! Retrieves the default value.
        /** Retrieve the default value.
         @return The the default value.
         */
        inline value_t getDefault() const {return m_default;}
        
        //! Retrieve the attribute value as a vector of atoms.
        /** The function retrieves the attribute value as a vector of atoms.
         @return The vector of atoms.
         */
        std::vector<Atom> getValue() const noexcept override {return std::vector<Atom>(m_value);}
        
    private:
        
        friend Attribute::Manager;
        
        //! Sets the values.
        /** The function sets the current value.
         @param elements The vector of elements.
         @see get
         */
        inline void set(value_t const& value) { m_value = value; }
        
        //! Set the attribute value with an atom.
        /** The function sets the attribute value with an atom.
         @param atom The atom.
         */
        virtual void setValue(std::vector<Atom> const& atom) override { m_value = atom; }
        
        //! Set the attribute default value with an atom.
        /** The function sets the attribute default value with an atom.
         @param atom The atom.
         */
        virtual void setDefaultValue(std::vector<Atom> const& atom) override { m_default = atom; }
        
        //! Resets the value to its default state.
        /** Resets the value to its default state.
         */
        inline void resetDefault() override  { set(m_default); }
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE RGBA                                  //
    // ================================================================================ //
    
    class AttrInt : public Attribute
    {
    public:
        using value_t = flip::Int;
        
    protected:
        value_t m_default;
        value_t m_value;
        
    public:
        
        //! @brief Default constructor.
        AttrInt() = default;
        
        //! @brief Constructs an Attribute.
        AttrInt(std::string const& name, value_t const& value) :
        Attribute(name),
        m_default(value),
        m_value(value)
        {
            ;
        }
        
        //! @brief Destructor.
        inline ~AttrInt() noexcept {}
        
        // static flip declare method.
        static void declare()
        {
            Model::declare<AttrInt>()
            .name("cicm.kiwi.AttrInt")
            .inherit<Attribute>()
            .member<value_t, &AttrInt::m_default>("default_value")
            .member<value_t, &AttrInt::m_value>("value");
        }
        
        //! Retrieve the type index of the attribute.
        //! The function retrieves the type index of the attribute.
        //! @return The type index of the attribute.
        inline Type getType() const noexcept override {return Type::Int;}
        
        //! Retrieves the values.
        /** The current values.
         @return The current values.
         */
        inline value_t get() const {return m_value;}
        
        //! Retrieves the default value.
        /** Retrieve the default value.
         @return The the default value.
         */
        inline value_t getDefault() const {return m_default;}
        
        //! Retrieve the attribute value as a vector of atoms.
        /** The function retrieves the attribute value as a vector of atoms.
         @return The vector of atoms.
         */
        std::vector<Atom> getValue() const noexcept override {return {m_value.value()};}
        
    private:
        
        friend Attribute::Manager;
        
        //! Sets the values.
        /** The function sets the current value.
         @param elements The vector of elements.
         @see get
         */
        inline void set(value_t const& value) { m_value = value; }
        
        //! Set the attribute value with an atom.
        /** The function sets the attribute value with an atom.
         @param atom The atom.
         */
        virtual void setValue(std::vector<Atom> const& atom) override
        {
            if(atom.size() >= 1 && atom[0].isNumber())
            {
                m_value = atom[0].getInt();
            }
        }
        
        //! Set the attribute default value with an atom.
        /** The function sets the attribute default value with an atom.
         @param atom The atom.
         */
        virtual void setDefaultValue(std::vector<Atom> const& atom) override
        {
            if(atom.size() >= 1 && atom[0].isNumber())
            {
                m_default = atom[0].getInt();
            }
        }
        
        //! Resets the value to its default state.
        /** Resets the value to its default state.
         */
        inline void resetDefault() override  { set(m_default); }
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE MANAGER                               //
    // ================================================================================ //
    
    //! The attribute manager manages a set of attributes.
    /** it allows the setting and the getting of their values 
     and to retrieve them by name or by category.
     @see AttributeTyped
     */
    class Attribute::Manager
    {
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
        inline std::vector<Atom> getAttributeValue(std::string const& name) const noexcept
        {
            const Attribute* attr = getAttribute(name);
            if(attr)
            {
                return attr->getValue();
            }
            
            return std::vector<Atom>();
        }
		
        //! Set an attribute value.
        /** The function sets an attribute value.
         @param name the name of the attribute.
         @param value The new attribute value.
         */
        void setAttributeValue(std::string const& name, std::vector<Atom> const& value) noexcept
        {
            Attribute* attr = getAttribute(name);
            if(attr != nullptr)
            {
                attr->setValue(value);
            }
        }
        
    protected:
        
        //! @brief Constructor.
        //! @param name The name of the attribute.
        //! @param value The value of the attribute.
        void addAttr(Attribute* attr, std::string const& name, std::vector<Atom> const& default_value)
        {
            attr->setName(name);
            attr->setDefault(default_value);            
            m_attrs[name] = attr;
        }
        
    private:
        //! Retrieves an attribute.
        /** The function retrieves an attribute.
         @param name the name of the attribute.
         @return The attribute.
         */
        Attribute* getAttribute(std::string const& name) const noexcept
        {
            std::lock_guard<std::mutex> guard(m_attrs_mutex);
            
            const auto it = m_attrs.find(name);
            if(it != m_attrs.cend())
            {
                return it->second;
            }
            
            return nullptr;
        }
        
        std::map<std::string, Attribute*>   m_attrs;
        mutable std::mutex                  m_attrs_mutex;
    };
}

#endif
