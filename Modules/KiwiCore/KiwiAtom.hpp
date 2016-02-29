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

#ifndef KIWI_CORE_ATOM_HPP_INCLUDED
#define KIWI_CORE_ATOM_HPP_INCLUDED

#include <assert.h>
#include <iostream>
#include <cstring>
#include <memory>


namespace kiwi
{
    // ================================================================================ //
    //                                      ATOM                                        //
    // ================================================================================ //
    
    //! @brief The Atom can dynamically hold different types of value
    //! @details The Atom can hold an integer, a float or a string.
    class Atom
    {
    public:
        
        // ================================================================================ //
        //                                      Types                                       //
        // ================================================================================ //
        
        //! @brief The type of a signed integer number in the Atom class.
        using int_t     = int64_t;
        
        //! @brief The type of a floating-point number in the Atom class.
        using float_t   = double;
        
        //! @brief The type of a std::string in the Atom class.
        using string_t  = std::string;
        
        //! @brief Enum of Atom value types
        //! @see getType(), isNull(), isInt(), isFloat(), isNumber(), isString()
        enum class Type : uint8_t
        {
            Null        = 0,
            Int         = 1,
            Float       = 2,
            String      = 3
        };
        
        // ================================================================================ //
        //                                  CONSTRUCTORS                                    //
        // ================================================================================ //
        
        //! @brief Default constructor.
        //! @details Constructs an Atom of type Null.
        Atom() noexcept :
            m_type(Type::Null),
            m_value()
        {
            ;
        }

        //! @brief Constructs an int_t Atom.
        //! @details The integer value will be 1 or 0 depending on the bool value.
        //! @param value The value.
        Atom(const bool value) noexcept :
            m_type(Type::Int),
            m_value(value ? int_t(1) : int_t(0))
        {
            ;
        }
        
        //! @brief Constructs an int_t Atom.
        //! @param value The value.
        Atom(const int value) noexcept :
            m_type(Type::Int),
            m_value(static_cast<int_t>(value))
        {
            ;
        }
        
        //! @brief Constructs an int_t Atom.
        //! @param value The value.
        Atom(const long value) noexcept :
            m_type(Type::Int),
            m_value(static_cast<int_t>(value))
        {
            ;
        }
        
        //! @brief Constructs an int_t Atom.
        //! @param value The value.
        Atom(const long long value) noexcept :
            m_type(Type::Int),
            m_value(static_cast<int_t>(value))
        {
            ;
        }
        
        //! @brief Constructs a float_t Atom.
        //! @details infinty and NaN value both produce a Null Atom type.
        //! @param value The value.
        Atom(const float value) noexcept :
            m_type(Type::Float),
            m_value(static_cast<float_t>(value))
        {
            ;
        }
        
        //! @brief Constructs a float_t Atom.
        //! @details infinty and NaN value both produce a Null Atom type.
        //! @param value The value.
        Atom(const double value) noexcept :
            m_type(Type::Float),
            m_value(static_cast<float_t>(value))
        {
            ;
        }
        
        //! @brief Constructs a string_t Atom.
        //! @param sym The value.
        Atom(string_t const& sym) :
            m_type(Type::String),
            m_value(sym)
        {
            ;
        }
        
        //! @brief Constructs a string_t Atom.
        //! @param sym The value.
        Atom(string_t&& sym) :
        m_type(Type::String),
        m_value(std::move(sym))
        {
            ;
        }
        
        //! @brief Constructs a string_t Atom.
        //! @param sym The value.
        Atom(char const* sym) :
        m_type(Type::String),
        m_value(std::move(std::string(sym)))
        {
            ;
        }
        
        //! @brief Copy constructor.
        //! @details Constructs an Atom by copying the contents of an other Atom.
        //! @param other The other Atom.
        Atom(Atom const& other) : m_type(other.m_type)
        {
            switch(m_type)
            {
                case Type::Int:     { m_value = other.m_value.int_v; break; }
                case Type::Float:   { m_value = other.m_value.float_v; break; }
                case Type::String:
                {
                    assert(other.m_value.string_v != nullptr);
                    m_value = *other.m_value.string_v;
                }
                    
                default: break;
            }
        }
        
        //! @brief Move constructor.
        //! @details Constructs an Atom value by stealing the contents of an other Atom
        //! using move semantics, leaving the other as a Null value Atom.
        //! @param other The other Atom value.
        Atom(Atom&& other) :
            m_type(std::move(other.m_type)),
            m_value(std::move(other.m_value))
        {
            // leave the other as a Null value Atom
            other.m_type = Type::Null;
            other.m_value = {};
        }
        
        //! @brief Destructor.
        inline ~Atom()
        {
            if(isString())
            {
                std::allocator<string_t> alloc;
                alloc.destroy(m_value.string_v);
                alloc.deallocate(m_value.string_v, 1);
            }
        }
        
        //! @brief Copy assigment operator.
        //! @details Copies an Atom value.
        //! @param other The Atom object to copy.
        Atom& operator=(Atom const& other)
        {
            if(!other.isString())
            {
                if(isString())
                {
                    std::allocator<string_t> alloc;
                    alloc.destroy(m_value.string_v);
                    alloc.deallocate(m_value.string_v, 1);
                }
                
                m_value = other.m_value;
            }
            else
            {
                if(isString())
                {
                    *m_value.string_v = *other.m_value.string_v;
                }
                else
                {
                    m_value.string_v = create_string_pointer(*other.m_value.string_v);
                }
            }
            
            m_type = other.m_type;
            
            return *this;
        }
        
        //! @brief Copy assigment operator.
        //! @details Copies an Atom value with the "copy and swap" method.
        //! @param other The Atom object to copy.
        Atom& operator=(Atom&& other) noexcept
        {
            std::swap(m_type, other.m_type);
            std::swap(m_value, other.m_value);
            return *this;
        }
        // ================================================================================ //
        //                                   Type Getters                                   //
        // ================================================================================ //
        
        //! @brief Get the type of the Atom.
        //! @return The Type of the atom as a Type.
        //! @see isNull(), isInt(), isFloat(), isNumber(), isString()
        inline Type getType() const noexcept    { return m_type; }
        
        //! @brief Returns true if the Atom is Null.
        //! @return true if the Atom is Null.
        //! @see getType(), isInt(), isFloat(), isNumber(), isString()
        inline bool isNull() const noexcept     { return (m_type == Type::Null); }
        
        //! @brief Returns true if the Atom is an int_t.
        //! @return true if the Atom is an int_t.
        //! @see getType(), isNull(), isFloat(), isNumber(), isString()
        inline bool isInt() const noexcept      { return m_type == Type::Int; }
        
        //! @brief Returns true if the Atom is a float_t.
        //! @return true if the Atom is an float_t.
        //! @see getType(), isNull(), isInt(), isNumber(), isString()
        inline bool isFloat() const noexcept    { return m_type == Type::Float; }
        
        //! @brief Returns true if the Atom is a bool, an int_t, or a float_t.
        //! @return true if the Atom is a bool, an int_t, or a float_t.
        //! @see getType(), isNull(), isInt(), isFloat(), isString()
        inline bool isNumber() const noexcept   { return (isInt() || isFloat()); }
        
        //! @brief Returns true if the Atom is a string_t.
        //! @return true if the Atom is a string_t.
        //! @see getType(), isNull(), isInt(), isFloat(), isNumber()
        inline bool isString() const noexcept   { return m_type == Type::String; }
        
        // ================================================================================ //
        //                                   Value Getters                                  //
        // ================================================================================ //
        
        //! @brief Retrieves the Atom value as an int_t value.
        //! @return The current integer atom value if it is a number otherwise 0.
        //! @see getType(), isNumber(), isInt(), getFloat()
        int_t getInt() const noexcept
        {
            if(isInt())
            {
                return m_value.int_v;
            }
            else if(isFloat())
            {
                return static_cast<int_t>(m_value.float_v);
            }
            
            return int_t(0);
        }
        
        //! @brief Retrieves the Atom value as a float_t value.
        //! @return The current floating-point atom value if it is a number otherwise 0.0.
        //! @see getType(), isNumber(), isFloat(), getInt()
        float_t getFloat() const noexcept
        {
            if(isFloat())
            {
                return m_value.float_v;
            }
            else if(isInt())
            {
                return static_cast<float_t>(m_value.int_v);
            }
            
            return float_t(0.0);
        }
        
        //! @brief Retrieves the Atom value as a string_t value.
        //! @return The current string atom value if it is a string otherwise an empty string.
        //! @see getType(), isString(), getInt(), getFloat()
        string_t getString() const
        {
            if(isString())
            {
                return *m_value.string_v;
            }
            
            return string_t();
        }
        
    private:
        
        // ================================================================================ //
        //                                      VALUE                                       //
        // ================================================================================ //
        
        //! @internal Exception-safe object creation helper
        static string_t* create_string_pointer(string_t const& v)
        {
            std::allocator<string_t> alloc;
            auto deleter = [&](string_t * object) { alloc.deallocate(object, 1); };
            std::unique_ptr<string_t, decltype(deleter)> object(alloc.allocate(1), deleter);
            alloc.construct(object.get(), v);
            return object.release();
        }
        
        static string_t* create_string_pointer(string_t&& v)
        {
            std::allocator<string_t> alloc;
            auto deleter = [&](string_t * object) { alloc.deallocate(object, 1); };
            std::unique_ptr<string_t, decltype(deleter)> object(alloc.allocate(1), deleter);
            alloc.construct(object.get(), std::move(v));
            return object.release();
        }
        
        //! @internal The actual storage union for an Atom value.
        union atom_value
        {
            //! @brief number (integer).
            int_t   int_v;
            
            //! @brief number (floating-point).
            float_t float_v;
            
            //! @brief string.
            string_t*   string_v;
            
            //! @brief default constructor (for null values).
            atom_value() = default;
            
            //! @brief constructor for numbers (integer).
            atom_value(const int_t v) noexcept : int_v(v) {}
            
            //! @brief constructor for numbers (floating-point).
            atom_value(const float_t v) noexcept : float_v(v) {}
            
            //! @brief constructor for strings
            atom_value(string_t const& v) : string_v(create_string_pointer(v)) {}
            
            //! @brief constructor for strings
            atom_value(string_t&& v) : string_v(create_string_pointer(std::move(v))) {}
        };
        
        //! @internal Atom Type (Null by default)
        Type        m_type = Type::Null;
        
        //! @internal Atom value
        atom_value  m_value = {};
    };
}

#endif // KIWI_CORE_ATOM_HPP_INCLUDED
