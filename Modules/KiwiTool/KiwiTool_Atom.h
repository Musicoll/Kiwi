/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include <cassert>
#include <iostream>
#include <sstream>
#include <cstring>
#include <memory>
#include <vector>

namespace kiwi { namespace tool {
    
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
        //! @see getType(), isNull(), isInt(), isFloat(), isNumber(), isString(), isComma(), isDollar()
        enum class Type : uint8_t
        {
            // basic types:
            Null = 0,
            Int,
            Float,
            String,
            
            // special types:
            Comma,
            Dollar,
        };
        
        // ================================================================================ //
        //                                  CONSTRUCTORS                                    //
        // ================================================================================ //
        
        //! @brief Default constructor.
        //! @details Constructs an Atom of type Null.
        Atom() noexcept;
        
        //! @brief Constructs an int_t Atom.
        //! @details The integer value will be 1 or 0 depending on the bool value.
        //! @param value The value.
        Atom(const bool value) noexcept;
        
        //! @brief Constructs an int_t Atom.
        //! @param value The value.
        Atom(const int value) noexcept;
        
        //! @brief Constructs an int_t Atom.
        //! @param value The value.
        Atom(const long value) noexcept;
        
        //! @brief Constructs an int_t Atom.
        //! @param value The value.
        Atom(const long long value) noexcept;
        
        //! @brief Constructs a float_t Atom.
        //! @details infinty and NaN value both produce a Null Atom type.
        //! @param value The value.
        Atom(const float value) noexcept;
        
        //! @brief Constructs a float_t Atom.
        //! @details infinty and NaN value both produce a Null Atom type.
        //! @param value The value.
        Atom(const double value) noexcept;
        
        //! @brief Constructs a string_t Atom.
        //! @param sym The value.
        Atom(string_t const& sym);
        
        //! @brief Constructs a string_t Atom.
        //! @param sym The value.
        Atom(string_t&& sym);
        
        //! @brief Constructs a string_t Atom.
        //! @param sym The value.
        Atom(char const* sym);
        
        //! @brief Constructs a Comma Atom.
        static Atom Comma();
        
        //! @brief Constructs a Dollar Atom.
        //! @param index must be between 1 and 9
        //! @return A Dollar Atom or a Null Atom if out of range.
        static Atom Dollar(int_t index);
        
        //! @brief Copy constructor.
        //! @details Constructs an Atom by copying the contents of an other Atom.
        //! @param other The other Atom.
        Atom(Atom const& other);
        
        //! @brief Move constructor.
        //! @details Constructs an Atom value by stealing the contents of an other Atom
        //! using move semantics, leaving the other as a Null value Atom.
        //! @param other The other Atom value.
        Atom(Atom&& other);
        
        //! @brief Destructor.
        ~Atom();
        
        //! @brief Copy assigment operator.
        //! @details Copies an Atom value.
        //! @param other The Atom object to copy.
        Atom& operator=(Atom const& other);
        
        //! @brief Copy assigment operator.
        //! @details Copies an Atom value with the "copy and swap" method.
        //! @param other The Atom object to copy.
        Atom& operator=(Atom&& other) noexcept;
        
        // ================================================================================ //
        //                                   Type Getters                                   //
        // ================================================================================ //
        
        //! @brief Get the type of the Atom.
        //! @return The Type of the atom as a Type.
        //! @see isNull(), isInt(), isFloat(), isNumber(), isString()
        Type getType() const noexcept;
        
        //! @brief Returns true if the Atom is Null.
        //! @return true if the Atom is Null.
        //! @see getType(), isInt(), isFloat(), isNumber(), isString()
        bool isNull() const noexcept;
        
        //! @brief Returns true if the Atom is an int_t.
        //! @return true if the Atom is an int_t.
        //! @see getType(), isNull(), isFloat(), isNumber(), isString()
        bool isInt() const noexcept;
        
        //! @brief Returns true if the Atom is a float_t.
        //! @return true if the Atom is an float_t.
        //! @see getType(), isNull(), isInt(), isNumber(), isString()
        bool isFloat() const noexcept;
        
        //! @brief Returns true if the Atom is a bool, an int_t, or a float_t.
        //! @return true if the Atom is a bool, an int_t, or a float_t.
        //! @see getType(), isNull(), isInt(), isFloat(), isString()
        bool isNumber() const noexcept;
        
        //! @brief Returns true if the Atom is a string_t.
        //! @return true if the Atom is a string_t.
        //! @see getType(), isNull(), isInt(), isFloat(), isNumber()
        bool isString() const noexcept;
        
        //! @brief Returns true if the Atom is a string_t that contains the special "bang" keyword.
        //! @return true if the Atom is a string_t that contains the special "bang" keyword.
        //! @see getType(), isNull(), isInt(), isFloat(), isString()
        bool isBang() const;
        
        //! @brief Returns true if the Atom is an comma_t.
        //! @return true if the Atom is a comma_t.
        //! @see getType(), isNull(), isInt(), isFloat(), isNumber(), isString()
        bool isComma() const noexcept;
        
        //! @brief Returns true if the Atom is a dollar or a dollar typed.
        //! @return true if the Atom is a dollar_t.
        //! @see getType(), isNull(), isInt(), isFloat(), isNumber(), isString()
        bool isDollar() const noexcept;
        
        // ================================================================================ //
        //                                   Value Getters                                  //
        // ================================================================================ //
        
        //! @brief Retrieves the Atom value as an int_t value.
        //! @return The current integer atom value if it is a number otherwise 0.
        //! @see getType(), isNumber(), isInt(), getFloat()
        int_t getInt() const noexcept;
        
        //! @brief Retrieves the Atom value as a float_t value.
        //! @return The current floating-point atom value if it is a number otherwise 0.0.
        //! @see getType(), isNumber(), isFloat(), getInt()
        float_t getFloat() const noexcept;
        
        //! @brief Retrieves the Atom value as a string_t value.
        //! @return The current string atom value if it is a string otherwise an empty string.
        //! @see getType(), isString(), getInt(), getFloat()
        string_t const& getString() const;
        
        //! @brief Retrieves the Dollar index value if the Atom is a dollar type.
        //! @return The Dollar index if the Atom is a dollar, 0 otherwise.
        //! @see getType(), isDollar(), isDollarTyped()
        int_t getDollarIndex() const;
        
    private: // methods
        
        // ================================================================================ //
        //                                      VALUE                                       //
        // ================================================================================ //
        
        //! @internal Exception-safe object creation helper
        static string_t* create_string_pointer(string_t const& v);
        
        static string_t* create_string_pointer(string_t&& v);
        
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
    //                                    ATOM HELPER                                   //
    // ================================================================================ //
    
    //! @brief An Atom helper class.
    struct AtomHelper
    {
        struct ParsingFlags { enum Flags {
            Comma = 0x01,
            Dollar = 0x02,
        }; };
        
        //! @brief Parse a string into a vector of atoms.
        //! @param text The string to parse.
        //! @param flags The flags as a set of #ParsingFlags.
        //! @return The vector of atoms.
        //! @details The parsing method can be altered by the #ParsingFlags \flags passed as parameter.
        //! If the ParsingFlags::Comma flag is set, it will create a Comma atom type
        //! for each ',' char of the string (except if the text is in double quotes)
        //! If the ParsingFlags::Dollar flag is set, it will create a Dollar atom type
        //! for each '$' char followed by a digit between 1 and 9.
        //! @example The string : "foo \"bar 42\" 1 -2 3.14" will be parsed into a vector of 5 Atom.
        //! The atom types will be determined automatically as :
        //! 2 #Atom::Type::String, 2 #Atom::Type::Int, and 1 #Atom::Type::Float.
        static std::vector<Atom> parse(std::string const& text, int flags = 0);
        
        //! @brief Convert an Atom into a string.
        static std::string toString(Atom const& atom, const bool add_quotes = true);
        
        //! @brief Convert a vector of Atom into a string.
        //! @details This method will call the toString static method for each atom
        //! of the vector and output a whitespace between each one
        //! (except for the special Atom::Type::Comma that is stuck to the previous Atom).
        static std::string toString(std::vector<Atom> const& atoms, const bool add_quotes = true);
        
        static std::string trimDecimal(std::string const& text);
    };
    
}}
