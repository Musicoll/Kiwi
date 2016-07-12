/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_MODEL_ATOM_HPP_INCLUDED
#define KIWI_MODEL_ATOM_HPP_INCLUDED

#include <cassert>
#include <iostream>
#include <sstream>
#include <cstring>
#include <memory>
#include <vector>

namespace kiwi
{
    // ================================================================================ //
    //                                      ATOM                                        //
    // ================================================================================ //
    
    //! @brief The Atom can dynamically hold different types of value
    //! @details The Atom can hold an integer, a float or a string.
    class Atom
    {
    public: // methods
        
        // ================================================================================ //
        //                                      Types                                       //
        // ================================================================================ //
        
        //! @brief The type of a signed integer number in the Atom class.
        using int_t     = int64_t;
        
        //! @brief The type of a floating-point number in the Atom class.
        using float_t   = double;
        
        //! @brief The type of a string type in the Atom class.
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
        
    private: // methods
        
        // ================================================================================ //
        //                                      VALUE                                       //
        // ================================================================================ //
        
        //! @internal Exception-safe object creation helper
        static string_t* create_string_pointer(string_t const& v)
        {
            std::allocator<string_t> alloc;
            auto deleter = [&alloc](string_t * object) { alloc.deallocate(object, 1); };
            std::unique_ptr<string_t, decltype(deleter)> object(alloc.allocate(1), deleter);
            alloc.construct(object.get(), v);
            return object.release();
        }
        
        static string_t* create_string_pointer(string_t&& v)
        {
            std::allocator<string_t> alloc;
            auto deleter = [&alloc](string_t * object) { alloc.deallocate(object, 1); };
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
        
    private: // methods
        
        //! @internal Atom Type (Null by default)
        Type        m_type = Type::Null;
        
        //! @internal Atom value
        atom_value  m_value = {};
    };
    
    // ================================================================================ //
    //                                  STRING HELPER                                   //
    // ================================================================================ //
    
    //! @brief An std::string helper class.
    struct StringHelper
    {
        //! @brief unescape a string
        static std::string unescape(std::string const& text)
        {
            bool state = false;
            std::ostringstream ss;
            for(const auto& iter : text)
            {
                if(state)
                {
                    switch(iter)
                    {
                        case '"': ss << '\"'; break;
                        case '/': ss << '/'; break;
                        case 'b': ss << '\b'; break;
                        case 'f': ss << '\f'; break;
                        case 'n': ss << '\n'; break;
                        case 'r': ss << '\r'; break;
                        case 't': ss << '\t'; break;
                        case '\\': ss << '\\'; break;
                        default: ss << iter; break;
                    }
                    state = false;
                }
                else
                {
                    switch(iter)
                    {
                        case '"':   { return ss.str(); }
                        case '\\':  { state = true; break; }
                        default:    { ss << iter; break; }
                    }
                }
            }
            return ss.str();
        }
    };
    
    // ================================================================================ //
    //                                    ATOM HELPER                                   //
    // ================================================================================ //
    
    //! @brief An Atom helper class.
    struct AtomHelper
    {
        //! @brief Parse a string into a vector of atoms.
        //! @details Parse a string into a vector of atoms.
        //! @param text The string to parse.
        //! @return The vector of atoms.
        //! @remark For example, the string : "foo \"bar 42\" 1 2 3.14" will parsed into a vector of 5 atoms.
        //! The atom types will be determined automatically as 2 #Atom::Type::TAG atoms, 2 #Atom::Type::LONG atoms, and 1 #Atom::Type::DOUBLE atom.
        static std::vector<Atom> parse(std::string const& text)
        {
            std::vector<Atom> atoms;
            const auto textlen = text.length();
            auto pos = text.find_first_not_of(' ', 0);
            
            while(pos < textlen)
            {
                std::string word;
                word.reserve(20);
                bool is_tag      = false;
                bool is_number   = false;
                bool is_float    = false;
                bool is_negative = false;
                bool is_quoted   = false;
                
                while(pos < textlen)
                {
                    const char c = text[pos];
                    
                    if(c == ' ')
                    {
                        if(!is_quoted)
                        {
                            // preserve white space in quoted tags, otherwise skip them
                            if(word.empty())
                            {
                                pos++;
                                continue;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else if(c == '\"')
                    {
                        // closing quote
                        if(is_quoted)
                        {
                            pos++;
                            break;
                        }
                        
                        // opening quote
                        if(word.empty())
                        {
                            pos++;
                            
                            // ignore if it can not be closed
                            if(text.find_first_of('\"', pos) != std::string::npos)
                                is_quoted = is_tag = true;
                            
                            continue;
                        }
                    }
                    else if(!is_tag)
                    {
                        if(word.empty() && c == '-')
                        {
                            is_negative = true;
                        }
                        else if(!is_float && (word.empty() || is_number || is_negative) && c == '.')
                        {
                            is_float = true;
                        }
                        else if(isdigit(c) && (is_number || (word.empty() || is_negative || is_float)))
                        {
                            is_number = true;
                        }
                        else
                        {
                            is_tag = true;
                            is_number = is_negative = is_float = false;
                        }
                    }
                    
                    word += c;
                    pos++;
                }
                
                if(!word.empty())
                {
                    if(is_number)
                    {
                        if(is_float)
                        {
                            atoms.emplace_back(std::stod(word.c_str()));
                        }
                        else
                        {
                            long value = 0;
                            try
                            {
                                value = std::stol(word.c_str());
                            }
                            catch(std::out_of_range const& err)
                            {
                                // print error here
                            }
                            catch(std::invalid_argument const& err)
                            {
                                // print error here
                            }
                            
                            atoms.emplace_back(value);
                        }
                    }
                    else
                    {
                        atoms.emplace_back(StringHelper::unescape(word));
                    }
                }
            }
            
            return atoms;
        }
        
        static std::string trimDecimal(std::string& text)
        {
            std::string::size_type pos = text.find('.');
            
            if(pos != std::string::npos)
            {
                std::string::size_type size = text.size();
                while(size > pos && text[size - 1] == '0')
                {
                    text.pop_back();
                    size--;
                }
            }
            
            return text;
        }
        
        //! @brief Convert an Atom into a string.
        static std::string toString(Atom const& atom)
        {
            std::string output;
            if(atom.isInt())
            {
                output += std::to_string(atom.getInt());
            }
            else if(atom.isFloat())
            {
                std::string str = std::to_string(atom.getFloat());
                output += trimDecimal(str);
            }
            else if(atom.isString())
            {
                output += atom.getString();
            }
            
            return output;
        }
        
        //! @brief Convert a vector of Atom into a string.
        static std::string toString(std::vector<Atom> const& atoms, const char delimiter = ' ')
        {
            std::string output;
            if(!atoms.empty())
            {
                for(std::vector<Atom>::size_type i = 0; i < atoms.size();)
                {
                    output += toString(atoms[i]);
                    if(++i != atoms.size())
                    {
                        output += delimiter;
                    }
                }
            }
            
            return output;
        }
        
        //! @brief Convert a vector of Atom into a string.
        static std::string toJsonString(std::vector<Atom> const& atoms)
        {
            std::string output;
            if(!atoms.empty())
            {
                if(atoms.size() == 1)
                {
                    output += toString(atoms[0]);
                }
                else
                {
                    output += '[';
                    for(std::vector<Atom>::size_type i = 0; i < atoms.size();)
                    {
                        output += toString(atoms[i]);
                        if(++i != atoms.size())
                        {
                            output += ", ";
                        }
                    }
                    output += ']';
                }
            }
            
            return output;
        }
    };
}

#endif // KIWI_MODEL_ATOM_HPP_INCLUDED
