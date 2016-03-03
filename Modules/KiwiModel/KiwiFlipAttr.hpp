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
    class AttributeBase : public flip::Object
    {
    public:
        
        class Manager;
        
        //! @internal flip static declare method
        static void declare()
        {
            Model::declare<AttributeBase>()
            .name("cicm.kiwi.AttributeBase")
            .member<flip::String, &AttributeBase::m_name> ("name");
        }
        
    protected:
        
        flip::String    m_name;  ///< The name of the attribute.
        
        //! @brief Sets the attribute value with an atom.
        //! @details This method must be implemented in subclasses.
        //! @param atom The atom.
        //virtual void setValue(Atom const&)          { assert(true); };
        
        //! Set the attribute default value with an atom.
        /** The function sets the attribute default value with an atom.
         @param atom The atom.
         */
        //virtual void setDefaultValue(Atom const&)   { assert(true); }
        
        //! Resets the value to its default state.
        virtual void resetDefault()  {};
        
    public:
        
        //! Default constructor
        AttributeBase() = default;
        
        //! @brief Constructs an AttributeBase object.
        //! @param name The name of the attribute (usually only letters and undescore characters).
        AttributeBase(std::string const& name) noexcept : m_name(name) {};
        
        //! @brief Move constructor.
        //! @param name The name of the attribute (usually only letters and undescore characters).
        inline AttributeBase(std::string&& name) noexcept : m_name(std::forward<std::string>(name)) {};
        
        //! @brief Destructor.
        virtual inline ~AttributeBase() noexcept {};
        
        //! Retrieve the type index of the attribute.
        //! @return The type index of the attribute.
        virtual std::type_index getTypeIndex() const noexcept { return typeid(this); };
        
        //! Retrieve the attribute value an atom.
        /** The function retrieves the attribute value as  an atom.
         @return The atom.
         */
        //virtual inline Atom getValue() const noexcept { return Atom(); };
        
        //! Retrieve if the attribute is from a specific template.
        /** The function retrieves if the attribute is from a specific template.
         @return true if the attribute is from a specific template.
         */
        template<class T>
        inline bool isType() const noexcept {return (std::type_index)typeid(T) == getTypeIndex();}
        
        //! Retrieve the name of the attribute.
        /** The function retrieves the name of the attribute.
         @return The name of the attribute.
         */
        inline std::string getName() const { return m_name; }
        
        //! Set the attribute name.
        /** The function sets the attribute name.
         @param label The attribute name.
         */
        inline AttributeBase& setName(std::string const& name) noexcept
        {
            m_name = name;
            return *this;
        }
        
        //! Set the attribute label.
        /** The function sets the attribute label.
         @param label The attribute label.
         */
        inline AttributeBase& setDefault(const Atom& atom) noexcept
        {
            //setDefaultValue(atom);
            resetDefault();
            return *this;
        }
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE TYPED                                 //
    // ================================================================================ //
    
    template <class ValueType>
    class Attribute : public AttributeBase
    {
    public:
        using value_t = ValueType;
        
    protected:
        value_t m_default;
        value_t m_value;
        
    public:
        
        //! @brief Default constructor.
        inline Attribute() = default;
        
        //! @brief Constructs an Attribute.
        Attribute(std::string const& name, value_t const& value) :
            AttributeBase(name),
            m_default(value),
            m_value(value)
        {
            static_assert(std::is_base_of<flip::Type, value_t>::value, "The class T must inherit from flip::Type");
        }
        
        //! @brief Destructor.
        inline ~Attribute() noexcept {}
        
        // static flip declare method.
        static void declare(const std::string& classname)
        {
            Model::declare<Attribute<value_t>>()
            .name(classname.c_str())
            .template inherit<AttributeBase>()
            .template member<value_t, &Attribute::m_default>("default_value")
            .template member<value_t, &Attribute::m_value>("value");
        }
        
        //! Retrieve the type index of the attribute.
        /** The function retrieves the type index of the attribute.
         @return The type index of the attribute.
         */
        inline std::type_index getTypeIndex() const noexcept override {return typeid(value_t);}
    
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
        //Atom getValue() const noexcept override {return Atom(m_value);}
        
    private:
        
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
        //virtual void setValue(Atom const& atom) override { m_value = atom; }
        
        //! Set the attribute default value with an atom.
        /** The function sets the attribute default value with an atom.
         @param atom The atom.
         */
        //virtual void setDefaultValue(Atom const& atom) override { m_default = atom; }
        
        //! Resets the value to its default state.
        /** Resets the value to its default state.
         */
        inline void resetDefault() override  {set(m_default);}
    };
    
    // ================================================================================ //
    //                                  ATTRIBUTE MANAGER                               //
    // ================================================================================ //
    
    //! The attribute manager manages a set of attributes.
    /** it allows the setting and the getting of their values 
     and to retrieve them by name or by category.
     @see AttributeBaseTyped
     */
    class AttributeBase::Manager
    {
    private:
        std::vector<AttributeBase*>         m_attrs;
        mutable std::mutex                  m_attrs_mutex;
        
        //! Retrieves an attribute.
        /** The function retrieves an attribute.
         @param name the name of the attribute.
         @return The attribute.
         */
        inline AttributeBase* getAttributeBase(std::string const& name) const noexcept
        {
            std::lock_guard<std::mutex> guard(m_attrs_mutex);
            
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
            std::lock_guard<std::mutex> guard(m_attrs_mutex);
            m_attrs.clear();
        }
        
        //! Retrieve an attribute value.
        /** The function retrieves an attribute value.
         @param name the name of the attribute.
         @return The value of the attribute as a vector or an empty vector if the attribute doesn't exist.
         */
        inline Atom getAttributeValue(std::string const& name) const noexcept
        {
            const AttributeBase* attr = getAttributeBase(name);
            if(attr)
            {
                //return attr->getValue();
            }
            
            return Atom();
        }
		
        //! Set an attribute value.
        /** The function sets an attribute value.
         @param name the name of the attribute.
         @param value The new attribute value.
         */
        inline void setAttributeValue(std::string const& name, Atom const& atom) noexcept
        {
            AttributeBase* attr = getAttributeBase(name);
            if(attr)
            {
                //attr->setValue(atom);
            }
        }
        
    protected:
        
        //! @brief Constructor.
        //! @param name The name of the attribute.
        //! @param value The value of the attribute.
        template<class T>
        void addAttr(AttributeBase* attr, std::string const& name, T const& value)
        {
            attr->setName(name);
            //attr->setDefault(value);
            m_attrs.push_back(attr);
        }
    };
}

#endif
