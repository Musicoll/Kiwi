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

#include "KiwiSymbol.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      ATOM                                        //
    // ================================================================================ //
    
    //! @brief The Atom can dynamically hold different types of value
    //! @details The Atom can hold an integer, a float or a Symbol.
    class Atom
    {
    public:
        
        // ================================================================================ //
        //                                      Types                                       //
        // ================================================================================ //
        
        //! @brief The type of a signed integer number in the Atom class.
        using integer_t = int64_t;
        
        //! @brief The type of a floating-point number in the Atom class.
        using float_t = double;
        
        //! @brief The type of a Symbol in the Atom class.
        using symbol_t = Symbol;
        
        //! @brief Enum of Atom value types
        //! @see getType(), isNull(), isInt(), isFloat(), isNumber(), isSymbol()
        enum class Type : uint8_t
        {
            Null        = 0,
            Int         = 1,
            Float       = 2,
            Symbol      = 3
        };
        
    private:
        
        // ================================================================================ //
        //                                      VALUE                                       //
        // ================================================================================ //
        
        //! @internal Exception-safe object creation helper
        template<typename T, typename... Args>
        static T* create(Args&& ... args)
        {
            std::allocator<T> alloc;
            auto deleter = [&](T * object) { alloc.deallocate(object, 1); };
            std::unique_ptr<T, decltype(deleter)> object(alloc.allocate(1), deleter);
            alloc.construct(object.get(), std::forward<Args>(args)...);
            return object.release();
        }
        
        //! @internal The actual storage union for an Atom value.
        union atom_value
        {
            //! @brief number (integer).
            integer_t   int_v;
            
            //! @brief number (floating-point).
            float_t     float_v;
            
            //! @brief symbol.
            symbol_t*   sym_v;
            
            //! @brief default constructor (for null values).
            atom_value() = default;
            
            //! @brief constructor for numbers (integer).
            atom_value(integer_t v) noexcept : int_v(v) {}
            
            //! @brief constructor for numbers (floating-point).
            atom_value(float_t v) noexcept : float_v(v) {}
            
            //! @brief constructor for symbols
            atom_value(symbol_t const& value) : sym_v(create<symbol_t>(value)) {}
        };
        
        //! @internal Atom Type (Null by default)
        Type        m_type = Type::Null;
        
        //! @internal Atom value
        atom_value  m_value = {};
        
    public:
        
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

        //! @brief Constructs an integer_t Atom.
        //! @details The integer value will be 1 or 0 depending on the bool value.
        //! @param value The value.
        Atom(bool value) noexcept :
            m_type(Type::Int),
            m_value(value ? integer_t(1) : integer_t(0))
        {
            ;
        }
        
        //! @brief Constructs an integer_t Atom.
        //! @param value The value.
        Atom(int value) noexcept :
            m_type(Type::Int),
            m_value(static_cast<integer_t>(value))
        {
            ;
        }
        
        //! @brief Constructs an integer_t Atom.
        //! @param value The value.
        Atom(long value) noexcept :
            m_type(Type::Int),
            m_value(static_cast<integer_t>(value))
        {
            ;
        }
        
        //! @brief Constructs an integer_t Atom.
        //! @param value The value.
        Atom(long long value) noexcept :
            m_type(Type::Int),
            m_value(static_cast<integer_t>(value))
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
            // infinity and NAN produce an Null Atom type
            if (! std::isfinite(value))
            {
                m_type = Type::Null;
                m_value.float_v = 0.0;
            }
        }
        
        //! @brief Constructs a float_t Atom.
        //! @details infinty and NaN value both produce a Null Atom type.
        //! @param value The value.
        Atom(const double value) noexcept :
            m_type(Type::Float),
            m_value(static_cast<float_t>(value))
        {
            // infinity and NAN produce an Null Atom type
            if (! std::isfinite(value))
            {
                m_type = Type::Null;
                m_value.float_v = 0.0;
            }
        }
        
        //! @brief Constructs a symbol_t Atom.
        //! @param sym The Symbol value.
        Atom(symbol_t const& sym) :
            m_type(Type::Symbol),
            m_value(sym)
        {
            ;
        }

        //! @brief Constructs a symbol_t Atom with a string literal.
        //! @param sym The Symbol value.
        Atom(char const* sym) : Atom(Symbol(sym)) {}
        
        //! @brief Constructs a symbol_t Atom with a string.
        //! @param sym The Symbol value.
        Atom(std::string const& sym) : Atom(Symbol(sym)) {}
        
        //! @brief Constructs a symbol_t Atom by moving a string.
        //! @param sym The Symbol value.
        Atom(std::string&& sym) : Atom(Symbol(std::forward<std::string>(sym))) {}
        
        //! @brief Copy constructor.
        //! @details Constructs an Atom by copying the contents of an other Atom.
        //! @param other The other Atom.
        Atom(Atom const& other) : m_type(other.m_type)
        {
            switch(m_type)
            {
                case Type::Int:     { m_value = other.m_value.int_v; break; }
                case Type::Float:   { m_value = other.m_value.float_v; break; }
                case Type::Symbol:
                {
                    assert(other.m_value.sym_v != nullptr);
                    m_value = *other.m_value.sym_v;
                }
                    
                default: break;
            }
        }
        
        //! @brief Move constructor.
        //! @details Constructs an Atom value by stealing the contents of an other Atom using move semantics,
        //! leaving the other as a Null value Atom.
        //! @param other The other Atom value.
        Atom(Atom&& other) :
            m_type(std::move(other.m_type)),
            m_value(std::move(other.m_value))
        {
            // leave the other as a Null value Atom
            other.m_type = Type::Null;
            other.m_value = {};
        }
        
        //! @brief Copy assigment operator.
        //! @details Copies an Atom value with the "copy and swap" method.
        //! @param other The Atom object to copy.
        Atom& operator=(Atom other)
        noexcept (std::is_nothrow_move_constructible<Type>::value
                  && std::is_nothrow_move_assignable<Type>::value
                  && std::is_nothrow_move_constructible<atom_value>::value
                  && std::is_nothrow_move_assignable<atom_value>::value)
        {
            std::swap(m_type, other.m_type);
            std::swap(m_value, other.m_value);
            return *this;
        }
        
        //! @brief Destructor.
        inline ~Atom()
        {
            if(isSymbol())
            {
                std::allocator<symbol_t> alloc;
                alloc.destroy(m_value.sym_v);
                alloc.deallocate(m_value.sym_v, 1);
            }
        }
        
        // ================================================================================ //
        //                                   Type Getters                                   //
        // ================================================================================ //
        
        //! @brief Get the type of the Atom.
        //! @return The Type of the atom as a Type.
        //! @see isNull(), isInt(), isFloat(), isNumber(), isSymbol()
        inline Type getType() const noexcept    { return m_type; }
        
        //! @brief Returns true if the Atom is Null.
        //! @return true if the Atom is Null.
        //! @see getType(), isInt(), isFloat(), isNumber(), isSymbol()
        inline bool isNull() const noexcept     { return (m_type == Type::Null); }
        
        //! @brief Returns true if the Atom is an integer_t.
        //! @return true if the Atom is an integer_t.
        //! @see getType(), isNull(), isFloat(), isNumber(), isSymbol()
        inline bool isInt() const noexcept      { return m_type == Type::Int; }
        
        //! @brief Returns true if the Atom is a float_t.
        //! @return true if the Atom is an float_t.
        //! @see getType(), isNull(), isInt(), isNumber(), isSymbol()
        inline bool isFloat() const noexcept    { return m_type == Type::Float; }
        
        //! @brief Returns true if the Atom is a bool, an integer_t, or a float_t.
        //! @return true if the Atom is a bool, an integer_t, or a float_t.
        //! @see getType(), isNull(), isInt(), isFloat(), isSymbol()
        inline bool isNumber() const noexcept   { return (isInt() || isFloat()); }
        
        //! @brief Returns true if the Atom is a symbol_t.
        //! @return true if the Atom is a symbol_t.
        //! @see getType(), isNull(), isInt(), isFloat(), isNumber()
        inline bool isSymbol() const noexcept   { return m_type == Type::Symbol; }
        
        // ================================================================================ //
        //                                   Value Getters                                  //
        // ================================================================================ //
        
        //! @brief Retrieves the Atom value as an integer_t value.
        //! @return The current integer atom value if it is a number otherwise 0.
        //! @see getType(), isNumber(), isInt(), getFloat()
        integer_t getInt() const noexcept
        {
            if(isInt())
            {
                return m_value.int_v;
            }
            else if(isFloat())
            {
                return static_cast<integer_t>(m_value.float_v);
            }
            
            return integer_t(0);
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
        
        //! @brief Retrieves the Atom value as a Symbol value.
        //! @return The current Symbol atom value if it is a Symbol otherwise an empty Symbol.
        //! @see getType(), isSymbol(), getInt(), getFloat()
        symbol_t getSymbol() const noexcept
        {
            if(isSymbol())
            {
                return *m_value.sym_v;
            }
            
            return symbol_t();
        }
        
        // ================================================================================ //
        //                                Equality Operators                                //
        // ================================================================================ //
        
        //! @brief Compare the atom with another.
        //! @details Two Atom objects are equal if and only if :
        //! - their types are equals.
        //! - their values are equals
        //! @param other The other atom.
        //! @return True if the two Atom objects are equals otherwise false.
        bool operator==(Atom const& other) const noexcept
        {
            if(other.isNull() && isNull())
            {
                return true;
            }
            else if(other.isInt() && isInt())
            {
                return (m_value.int_v == other.m_value.int_v);
            }
            else if(other.isFloat() && isFloat())
            {
                return (m_value.float_v == other.m_value.float_v);
            }
            else if(other.isSymbol() && isSymbol())
            {
                return (*m_value.sym_v == *other.m_value.sym_v);
            }
            
            return false;
        }
        
        //! @brief Compare the atom with another.
        //! @param other The other atom.
        //! @return True if the two Atom objects differ otherwise false.
        inline bool operator!=(const Atom& other) const noexcept
        {
            return !(*this == other);
        }
        
        // ================================================================================ //
        //                                    Utilities                                     //
        // ================================================================================ //
                
        //! @brief Parse an std::string into an std::vector of Atom objects.
        //! @param text	The std::string to parse.
        //! @return A vector of Atom objects.
        //! @remark For example, the std::string : "foo \"bar 42\" 1 2 3.14"
        //! will parsed into an std::vector of 5 Atom objects.
        //! The atom types will be determined automatically as
        //! - 2 #Atom::Type::Symbol,
        //! - 2 #Atom::Type::Int,
        //! - 1 #Atom::Type::Float.
        static std::vector<Atom> parse(std::string const& text);
    };
    
    std::ostream& operator<<(std::ostream& output, Atom const& atom);
}

#endif // KIWI_CORE_ATOM_HPP_INCLUDED
