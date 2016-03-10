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
    class TModel {};
    // ================================================================================ //
    //                                      ATTRIBUTE                                   //
    // ================================================================================ //
    
    //! @brief The Attribute class is an abstract class that holds a set of values of differents kinds and differents sizes.
    //! @details The attribute manages a set of values that should be displayed in an inspector.
    //! The setter and getter must be overritten.
    class Attribute : public flip::Object
    {
    public:
        
        class Manager;
        
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
        
        //! @brief Default constructor
        Attribute() = default;
        
        //! @brief Constructs an Attribute object.
        //! @param name The name of the attribute (usually only letters and undescore characters).
        Attribute(std::string const& name) noexcept : m_name(name) {};
        
        //! @brief Move constructor.
        //! @param name The name of the attribute (usually only letters and undescore characters).
        inline Attribute(std::string&& name) noexcept : m_name(std::forward<std::string>(name)) {};
        
        //! @brief Destructor.
        virtual inline ~Attribute() noexcept {};
        
        //! @brief Returns the attribute value as a vector of atoms.
        //! @details This method must be overwritten by subclasses.
        //! @return The attribute value as a vector of atoms.
        virtual inline std::vector<Atom> getValue() const = 0;
        
        //! @brief Sets the attribute value with a vector of atoms.
        //! @details This method must be overwritten by subclasses.
        //! @param atoms The vector of atoms.
        virtual void setValue(std::vector<Atom> const&) = 0;
        
        //! @brief Sets the attribute default value with a vector of atoms.
        //! @details This method must be overwritten by subclasses.
        //! @param atoms The vector of atoms.
        virtual void setDefaultValue(std::vector<Atom> const&) = 0;
        
        //! @brief Returns the attribute default value as a vector of atoms.
        //! @details This method must be overwritten by subclasses.
        //! @return A vector of atoms.
        virtual std::vector<Atom> getDefaultValue() = 0;
        
        //! @brief Resets the value to its default state.
        //! @details This method must be overwritten by subclasses.
        virtual void resetDefault() = 0;
        
        //! @brief Returns the Type of the attribute.
        //! @return The Type of the attribute.
        virtual Type getType() const noexcept = 0;
        
        //! @brief Returns the name of the attribute.
        //! @return The name of the attribute.
        inline std::string getName() const { return m_name; }
        
        //! @internal flip static declare method
        template<class TModel>
        static void declare()
        {
            if(TModel::template has<Attribute>()) return;
            
            TModel::template declare<Attribute>()
            .template name("cicm.kiwi.Attribute")
            .template member<flip::String, &Attribute::m_name>("name");
        }
        
    protected:
        flip::String    m_name;  ///< The name of the attribute.
        
    private:
        template<class ValueType> class Typed;
        friend class Manager;
        friend class AttrFloat;
        friend class AttrRGBA;
        
        //! @brief Set the attribute name.
        //! @param name The attribute name.
        inline Attribute& setName(std::string const& name)
        {
            m_name = name;
            return *this;
        }
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE TYPED                                 //
    // ================================================================================ //
    
    template<class ValueType>
    class Attribute::Typed : public Attribute
    {
    public:
        using value_t = ValueType;
        
    protected:
        value_t m_default;
        value_t m_value;
        
    public:
        //! @brief Default constructor.
        Typed() = default;
        
        //! @brief Constructs an Attribute.
        Typed(std::string const& name, value_t const& value) :
            Attribute(name), m_default(value), m_value(value) {}
        
        //! @brief Destructor.
        virtual ~Typed() noexcept {}
        
        //! @internal flip static declare method
        template<class TModel, class TSubclass>
        static void declare(std::string const& subclass_name)
        {
            if(TModel::template has<Typed<value_t>>()) return;
            
            const std::string typed_name_str = "cicm.kiwi.AttrTyped." + subclass_name;
            const std::string sublass_name_str = "cicm.kiwi." + subclass_name;
            
            TModel::template declare<Typed<value_t>>()
            .name(typed_name_str.c_str())
            .template inherit<Attribute>()
            .template member<value_t, &Typed::m_default>("default_value")
            .template member<value_t, &Typed::m_value>("value");
            
            TModel::template declare<TSubclass>()
            .template name(sublass_name_str.c_str())
            .template inherit<Attribute::Typed<value_t>>();
        }
        
        //! Retrieves the values.
        /** The current values.
         @return The current values.
         */
        virtual inline value_t get() const          { return m_value; }
        
        //! Retrieves the default value.
        /** Retrieve the default value.
         @return The the default value.
         */
        virtual inline value_t getDefault() const   {return m_default;}
        
    private:
        
        friend Attribute::Manager;
    
    protected:
        
        //! Sets the values.
        /** The function sets the current value.
         @param elements The vector of elements.
         @see get
         */
        virtual void set(value_t const& value) { m_value = value; }
        
        //! Resets the value to its default state.
        /** Resets the value to its default state.
         */
        virtual void resetDefault() override  { set(m_default); }
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE FLOAT                                 //
    // ================================================================================ //
    
    class AttrFloat : public Attribute::Typed<flip::Float>
    {
    public:
        
        //! @brief Default constructor.
        AttrFloat() = default;
        
        //! @brief Constructs an Attribute.
        AttrFloat(std::string const& name, value_t const& value) :
            Attribute::Typed<value_t>(name, value) {}
        
        // static flip declare method.
        template<class TModel>
        static void declare()
        {
            Attribute::Typed<value_t>::declare<TModel, AttrFloat>("AttrFloat");
        }
        
        inline Type getType() const noexcept override { return Type::Float; };
        
        //! Retrieve the attribute value as a vector of atoms.
        /** The function retrieves the attribute value as a vector of atoms.
         @return The vector of atoms.
         */
        std::vector<Atom> getValue() const noexcept override {return {m_value.value()};}
        
        //! Set the attribute value with an atom.
        /** The function sets the attribute value with an atom.
         @param atom The atom.
         */
        void setValue(std::vector<Atom> const& atom) final
        {
            if(atom.size() >= 1 && atom[0].isNumber())
            {
                m_value = atom[0].getFloat();
            }
        }
        
        //! Set the attribute default value with an atom.
        /** The function sets the attribute default value with an atom.
         @param atom The atom.
         */
        void setDefaultValue(std::vector<Atom> const& atom) override
        {
            if(atom.size() >= 1 && atom[0].isNumber())
            {
                m_value = atom[0].getFloat();
            }
        }
        
        //! @brief Returns the attribute default value as a vector of atoms.
        //! @details This method must be overwritten by subclasses.
        //! @return A vector of atoms.
        std::vector<Atom> getDefaultValue() override { return {m_default.value()}; };
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE RGBA                                  //
    // ================================================================================ //
    
    class AttrRGBA : public Attribute::Typed<FlipRGBA>
    {
    public:
        
        //! @brief Default constructor.
        AttrRGBA() = default;
        
        //! @brief Constructs an Attribute.
        AttrRGBA(std::string const& name, value_t const& value) :
            Attribute::Typed<FlipRGBA>(name, value) {}
        
        //! @brief Destructor.
        inline ~AttrRGBA() noexcept {}
        
        //! @internal flip static declare method
        template<class TModel>
        static void declare()
        {
            Attribute::Typed<value_t>::declare<TModel, AttrRGBA>("AttrRGBA");
        }
        
        inline Type getType() const noexcept override {return Type::RGBA;}
        
        //! Retrieve the attribute value as a vector of atoms.
        /** The function retrieves the attribute value as a vector of atoms.
         @return The vector of atoms.
         */
        std::vector<Atom> getValue() const noexcept override {return m_value;}
        
        //! @brief Returns the attribute default value as a vector of atoms.
        //! @details This method must be overwritten by subclasses.
        //! @return A vector of atoms.
        std::vector<Atom> getDefaultValue() override {return m_default;}
        
    private:
        
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
        
        //! @internal flip static declare method
        template<class TModel>
        static void declare()
        {
            if(TModel::template has<AttrInt>()) return;
            
            TModel::template declare<AttrInt>()
            .template name("cicm.kiwi.AttrInt")
            .template inherit<Attribute>()
            .template member<value_t, &AttrInt::m_default>("default_value")
            .template member<value_t, &AttrInt::m_value>("value");
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
        
        //! @brief Returns the attribute default value as a vector of atoms.
        //! @details This method must be overwritten by subclasses.
        //! @return A vector of atoms.
        std::vector<Atom> getDefaultValue() override { return {m_default.value()}; };
        
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
            attr->setDefaultValue(default_value);
            attr->setValue(default_value);
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
