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
    //! The Atom can hold a boolean value, an integer, a float, a tag,
    //! a vector of Atom or a dictionary of key/Atom pair.
    class Atom
    {
    public:
        
        //! @brief The type of an integer number in the Atom class
        using integer_t = int64_t;
        
        //! @brief The type of a floating-point number in the Atom class
        using float_t = double;
        
        //! @brief The type of a Symbol in the Atom class
        using symbol_t = Symbol;
        
        //! @brief Enum of Atom value types
        enum class Type : uint8_t
        {
            Null        = 0,
            Int         = 1,
            Float       = 2,
            Symbol      = 3
        };
        
        // ================================================================================ //
        //                                  CONSTRUCTORS                                    //
        // ================================================================================ //
        
        //! @brief Construct an Atom of type Null
        inline Atom() noexcept : m_quark(new Quark()) {}
        
        //! @brief Move constructor.
        //! Constructs an Atom value with the contents of another Atom using move semantics.
        //! It "steals" the resources from the other and leaves it as a Null Atom.
        //! @param other The other Atom.
        inline Atom(Atom&& other) noexcept : m_quark(std::move(other.m_quark))
        {
            other.m_quark = new Quark();
        }
        
        //! @brief Copy constructor.
        //! Constructs an Atom by copying the contents of an other Atom.
        //! @param other The other Atom.
        Atom(Atom const& other) noexcept;
        
        //! @brief Constructs a boolean_t Atom (explicit).
        //! @param value The value.
        inline explicit Atom(bool value) noexcept : m_quark(new QuarkInt(value)) {}
        
        //! @brief Constructs an integer_t Atom (explicit)
        //! @param value The value.
        inline explicit Atom(const integer_t value) noexcept : m_quark(new QuarkInt(value)) {}
        
    private:
        //! @internal Utility to target a compatible integer value.
        template <class IntegerType>
        using EnableIfCompatibleIntTypeFrom = typename std::enable_if<
        std::is_constructible<integer_t, IntegerType>::value
        && std::numeric_limits<IntegerType>::is_signed
        && (! std::is_same<integer_t, IntegerType>::value)
        && (sizeof(IntegerType) <= sizeof(integer_t))
        && (   std::is_same<short,      IntegerType>::value
            || std::is_same<int,        IntegerType>::value
            || std::is_same<long,       IntegerType>::value
            || std::is_same<long long,  IntegerType>::value), IntegerType>;
        
        //! @internal Utility to target a compatible integer value.
        template <class IntegerType>
        using EnableIfCompatibleIntTypeTo = typename std::enable_if<
        std::is_constructible<IntegerType, integer_t>::value
        && std::numeric_limits<IntegerType>::is_signed
        && (! std::is_same<integer_t, IntegerType>::value)
        && (sizeof(integer_t) <= sizeof(IntegerType))
        && (   std::is_same<short,      IntegerType>::value
            || std::is_same<int,        IntegerType>::value
            || std::is_same<long,       IntegerType>::value
            || std::is_same<long long,  IntegerType>::value), IntegerType>;
        
        //! @internal Utility to target a compatible floating-point value.
        template <class FloatType>
        using EnableIfCompatibleFloatTypeFrom = typename std::enable_if<
        std::is_constructible<float_t, FloatType>::value
        && std::is_floating_point<FloatType>::value
        && (sizeof(FloatType) <= sizeof(float_t))>;
        
        //! @internal Utility to target a compatible floating-point value.
        template <class FloatType>
        using EnableIfCompatibleFloatTypeTo = typename std::enable_if<
        std::is_constructible<FloatType, float_t>::value
        && std::is_floating_point<FloatType>::value
        && (sizeof(float_t) <= sizeof(FloatType))>;
    
    public:
        
        //! @brief Constructs a integer_t Atom with a compatible integer value.
        //! @detail compatible integer type are short, int, long, long long.
        //! @param value The value.
        template<class IntegerType,
        typename EnableIfCompatibleIntTypeFrom<IntegerType>::type = 0>
        Atom(const IntegerType value) noexcept
        : Atom(static_cast<integer_t>(value)) {}
        
        //! @brief Constructs a float_t Atom (explicit).
        //! @details infinty and NaN value both produce a Null Atom type.
        //! @param value The value.
        explicit Atom(const float_t value) noexcept : m_quark(new QuarkFloat(value))
        {
            // infinity and NAN produce an Null Atom type
            if (! std::isfinite(value))
            {
                delete m_quark;
                m_quark = new Quark();
            }
        }
        
        //! @brief Constructs a float_t Atom value with a compatible floating-point value.
        //! @param value The value.
        template<class FloatType,
        typename = typename EnableIfCompatibleFloatTypeFrom<FloatType>::type>
        Atom(const FloatType value) noexcept
        : Atom(static_cast<float_t>(value)) {}
        
        //! @brief Constructs a symbol_t Atom (explicit).
        //! @param tag The tag.
        inline Atom(const symbol_t tag) noexcept
        : m_quark(new QuarkSymbol(tag)) {}

        //! @brief Constructs a symbol_t Atom.
        //! @param tag The tag
        inline Atom(char const* tag) noexcept
        : Atom(Symbol(tag)) {}
        
        //! @brief Constructs a symbol_t Atom with a string.
        //! @param tag The tag
        inline Atom(std::string const& tag) noexcept
        : Atom(Symbol(tag)) {}
        
        //! @brief Constructs a symbol_t Atom by moving a string.
        //! @param tag The tag
        inline Atom(std::string&& tag) noexcept
        : Atom(Symbol(std::forward<std::string>(tag))) {}
        
        //! Destructor.
        inline ~Atom() noexcept {delete m_quark;}
        
        //! @brief Get the type of the Atom.
        //! @return The Type of the atom as a Type.
        //! @see  isNull(), isBool(), isInt(), isFloat(), isNumber(), isSymbol(), isVector(), isDico()
        inline Type getType() const noexcept {return m_quark->getType();}
        
        //! @brief Returns true if the Atom is Null.
        //! @return true if the Atom is Null.
        //! @see getType(), isInt(), isFloat(), isNumber(), isSymbol()
        inline bool isNull() const noexcept {return m_quark->isNull();}
        
        //! @brief Returns true if the Atom is an integer_t.
        //! @return true if the Atom is an integer_t.
        //! @see getType(), isNull(), isFloat(), isNumber(), isSymbol()
        inline bool isInt() const noexcept {return m_quark->isInt();}
        
        //! @brief Returns true if the Atom is a float_t.
        //! @return true if the Atom is an float_t.
        //! @see getType(), isNull(), isInt(), isNumber(), isSymbol()
        inline bool isFloat() const noexcept {return m_quark->isFloat();}
        
        //! @brief Returns true if the Atom is a boolean_t, an integer_t, or a float_t.
        //! @return true if the Atom is a boolean_t, an integer_t, or a float_t.
        //! @see getType(), isNull(), isInt(), isFloat(), isSymbol()
        inline bool isNumber() const noexcept {return m_quark->isNumber();}
        
        //! @brief Returns true if the Atom is a symbol_t.
        //! @return true if the Atom is a symbol_t.
        //! @see getType(), isNull(), isInt(), isFloat(), isNumber()
        inline bool isSymbol() const noexcept {return m_quark->isSymbol();}
        
        //! @brief Retrieves The Atom value as an integer_t value.
        //! @return An integer value if the atom is a number otherwise 0.
        //! @see isNumber()
        inline operator integer_t() const noexcept {return m_quark->getInt();}
        
        //! @brief Retrieves The Atom value as an integer value.
        //! @return An integer value if the atom is a number otherwise 0.
        //! @see isNumber()
        template<class IntegerType,
        typename EnableIfCompatibleIntTypeTo<IntegerType>::type = 0>
        operator IntegerType() const noexcept
        {
            return static_cast<IntegerType>(m_quark->getInt());
        }
        
        //! Cast the atom to a float_t.
        /** The function casts the atom to a float_t.
         @return A float_t value if the atom is a digit otherwise 0.
         */
        inline operator float_t() const noexcept {return m_quark->getFloat();}
        
        template<class FloatType,
        typename = typename EnableIfCompatibleFloatTypeTo<FloatType>::type>
        inline operator FloatType() const noexcept
        {return static_cast<FloatType>(m_quark->getFloat());}
        
        //! Cast the atom to a tag.
        /** The function casts the atom to a tag.
         @return A tag if the atom is a tag otherwise a nullptr.
         */
        inline operator symbol_t() const noexcept {return m_quark->getSymbol();}
        
        //! Set up the atom with another atom.
        /** The function sets up the atom with another atom.
         @param other   The other atom.
         @return An atom.
         */
        Atom& operator=(Atom const& other) noexcept;
        
        //! Set up the atom with another atom.
        /** The function sets up the atom with another atom.
         @param other   The other atom.
         @return An atom.
         */
        Atom& operator=(Atom&& other) noexcept
        {
            std::swap(m_quark, other.m_quark);
            return *this;
        }
        
        //! Set up the atom with a boolean value.
        /** The function sets up the atom with a long value created with aboolean value.
         @param value   The boolean value.
         @return An atom.
         */
        inline Atom& operator=(const bool value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkInt(value);
            return *this;
        }
        
        //! Set up the atom with a long value.
        /** The function sets up the atom with a long value.
         @param value   The long value.
         @return An atom.
         */
        inline Atom& operator=(const integer_t value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkInt(value);
            return *this;
        }
        
        template<class IntegerType,
        typename EnableIfCompatibleIntTypeFrom<IntegerType>::type = 0>
        inline Atom& operator=(const IntegerType value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkInt(static_cast<integer_t>(value));
            return *this;
        }

        //! Set up the atom with a float_t value.
        /** The function sets up the atom with a float_t value.
         @param value   The float_t value.
         @return An atom.
         */
        inline Atom& operator=(const float_t value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkFloat(value);
            return *this;
        }
        
        template<class FloatType,
        typename = typename EnableIfCompatibleFloatTypeFrom<FloatType>::type>
        Atom& operator=(const FloatType value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkFloat(static_cast<float_t>(value));
            return *this;
        }
        
        //! Set up the atom with a string.
        /** The function sets up the atom with string.
         @param tag   The string.
         @return An atom.
         */
        inline Atom& operator=(char const* tag)
        {
            delete m_quark;
            m_quark = new QuarkSymbol(Symbol(tag));
            return *this;
        }
        
        //! Set up the atom with a string.
        /** The function sets up the atom with string.
         @param tag   The string.
         @return An atom.
         */
        inline Atom& operator=(std::string const& tag)
        {
            delete m_quark;
            m_quark = new QuarkSymbol(Symbol(tag));
            return *this;
        }
        
        //! Set up the atom with a string.
        /** The function sets up the atom with string.
         @param tag   The string.
         @return An atom.
         */
        inline Atom& operator=(std::string&& tag) noexcept
        {
            delete m_quark;
            m_quark = new QuarkSymbol(Symbol(std::forward<std::string>(tag)));
            return *this;
        }
        
        //! Set up the atom with a tag.
        /** The function sets up the atom with a tag.
         @param tag   The tag.
         @return An atom.
         */
        inline Atom& operator=(symbol_t tag) noexcept
        {
            delete m_quark;
            m_quark = new QuarkSymbol(tag);
            return *this;
        }
        
        //! Compare the atom with another.
        /** The function compares the atom with another.
         @param other The other atom.
         @return true if the atoms hold the same value otherwise false.
         */
        inline bool operator==(Atom const& other) const noexcept
        {
            if(other.isNull() && isNull())
            {
                return true;
            }
            else if(other.isInt() && isNumber())
            {
                return m_quark->getInt() == other.m_quark->getInt();
            }
            else if(other.isFloat() && isNumber())
            {
                return m_quark->getFloat() == other.m_quark->getFloat();
            }
            else if(other.isSymbol() && isSymbol())
            {
                return m_quark->getSymbol() == other.m_quark->getSymbol();
            }
            
            return false;
        }
        
        //! Compare the atom with a long value.
        /** The function compares the atom with a long.
         @param value   The long value.
         @return true if the atom hold the same long value otherwise false.
         */
        inline bool operator==(const integer_t value) const noexcept
        {
            if(isNumber())
            {
                return value == m_quark->getInt();
            }
            else
            {
                return false;
            }
        }
        
        template<typename IntegerType, typename
        std::enable_if<
        std::is_constructible<IntegerType, integer_t>::value &&
        std::numeric_limits<IntegerType>::is_integer &&
        std::numeric_limits<IntegerType>::is_signed, IntegerType>::type
        = 0>
        bool operator==(const IntegerType value) const noexcept
        {
            if(isNumber())
            {
                return value == static_cast<IntegerType>(m_quark->getInt());
            }
            else
            {
                return false;
            }
        }
        
        template<typename UnsignedType, typename
        std::enable_if<
        std::is_constructible<UnsignedType, integer_t>::value &&
        std::numeric_limits<UnsignedType>::is_integer &&
        !std::numeric_limits<UnsignedType>::is_signed, UnsignedType>::type
        = 0>
        bool operator==(const UnsignedType value) const noexcept
        {
            if(isNumber())
            {
                return value == static_cast<UnsignedType>(m_quark->getInt());
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a float_t value.
        /** The function compares the atom with a float_t value.
         @param value   The float_t value.
         @return true if the atom hold the same float_t value otherwise false.
         */
        inline bool operator==(const float_t value) const noexcept
        {
            if(isNumber())
            {
                return value == m_quark->getFloat();
            }
            else
            {
                return false;
            }
        }
        
        template<class FloatType,
        typename = typename EnableIfCompatibleFloatTypeFrom<FloatType>::type>
        bool operator==(const FloatType value) const noexcept
        {
            if(isNumber())
            {
                std::cout << static_cast<float_t>(m_quark->getFloat()) << " == " << static_cast<float_t>(value) <<std::endl;
                return m_quark->getFloat() == static_cast<float_t>(value);
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom hold the same tag create with the std::string otherwise false.
         */
        bool operator==(char const* tag) const noexcept;
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom hold the same tag create with the std::string otherwise false.
         */
        bool operator==(std::string const& tag) const noexcept;
        
        //! Compare the atom with a tag.
        /** The function compares the atom with a tag.
         @param tag   The tag.
         @return true if the atom hold the same tag otherwise false.
         */
        bool operator==(symbol_t tag) const noexcept
        {
            if(isSymbol())
            {
                return m_quark->getSymbol() == tag;
            }
            
            return false;
        }
        
        //! Compare the atom with another.
        /** The function compares the atom with another.
         @param other The other atom.
         @return true if the atoms differ otherwise false.
         */
        inline bool operator!=(const Atom& other) const noexcept
        {
            return !(*this == other);
        }
        
        //! Compare the atom with a boolean value.
        /** The function compares the atom with a boolean value.
         @param value   The boolean value.
         @return true if the atom differ from the boolean value otherwise false.
         */
        inline bool operator!=(const boolean_t value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the atom with a long value.
        /** The function compares the atom with a long.
         @param value   The long value.
         @return true if the atom differ from the long value otherwise false.
         */
        inline bool operator!=(const integer_t value) const noexcept
        {
            return !(*this == value);
        }
        template<typename IntegerType, typename
        std::enable_if<
        std::is_constructible<IntegerType, integer_t>::value &&
        std::numeric_limits<IntegerType>::is_integer &&
        std::numeric_limits<IntegerType>::is_signed, IntegerType>::type
        = 0>
        inline bool operator!=(const IntegerType value) const noexcept
        {
            return !(*this == value);
        }
        
        template<typename UnsignedType, typename
        std::enable_if<
        std::is_constructible<UnsignedType, integer_t>::value &&
        std::numeric_limits<UnsignedType>::is_integer &&
        !std::numeric_limits<UnsignedType>::is_signed, UnsignedType>::type
        = 0>
        inline bool operator!=(const UnsignedType value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the atom with a float_t value.
        /** The function compares the atom with a float_t value.
         @param value   The float_t value.
         @return true if the atom differ from the float_t value otherwise false.
         */
        inline bool operator!=(const float_t value) const noexcept
        {
            return !(*this == value);
        }
        
        template<class FloatType,
        typename = typename EnableIfCompatibleFloatTypeTo<FloatType>::type>
        inline bool operator!=(const FloatType value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom differ from the tag create with the std::string otherwise false.
         */
        inline bool operator!=(char const* tag) const noexcept
        {
            return !(*this == tag);
        }
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom differ from the tag create with the std::string otherwise false.
         */
        inline bool operator!=(std::string const& tag) const noexcept
        {
            return !(*this == tag);
        }
        
        //! Compare the atom with a tag.
        /** The function compares the atom with a tag.
         @param value   The tag.
         @return true if the atom differ from the tag otherwise false.
         */
        inline bool operator!=(const Symbol tag) const noexcept
        {
            return !(*this == tag);
        }
                
        //! Parse a std::string into a vector of atoms.
        /** Parse a std::string into a vector of atoms.
         @param     text	The std::string to parse.
         @return    The vector of atoms.
         @remark    For example, the std::string : "foo \"bar 42\" 1 2 3.14" will parsed into a vector of 5 atoms.
         The atom types will be determined automatically as 2 #Atom::Type::Symbol atoms, 2 #Atom::Type::Int atoms,
         && 1 #Atom::Type::Float atom.
         */
        static std::vector<Atom> parse(std::string const& text);
        
    private:
        
        //! @brief The type of value an Atom can be.
        using value_t = Atom::Type;
        
        //! @internal Base class of all internal Quark types
        class Quark
        {
        public:
            constexpr inline Quark() noexcept {}
            virtual inline ~Quark() noexcept {}
            // Type checks
            virtual inline value_t getType() const noexcept {return value_t::Null;}
            inline bool isNull() const noexcept             {return getType() == value_t::Null;}
            inline bool isInt() const noexcept              {return getType() == value_t::Int;}
            inline bool isFloat() const noexcept            {return getType() == value_t::Float;}
            inline bool isNumber() const noexcept           {return isInt() || isFloat();}
            inline bool isSymbol() const noexcept           {return getType() == value_t::Symbol;}
            // Getters
            virtual inline boolean_t getBool() const noexcept   {return false;}
            virtual inline integer_t getInt() const noexcept    {return 0ll;}
            virtual inline float_t   getFloat() const noexcept  {return 0.;}
            virtual inline symbol_t  getSymbol() const noexcept {return Symbol("");}
        };
        
        //! @internal A Quark that hold an integer number
        class QuarkInt : public Quark
        {
        public:
            const integer_t val;
            inline QuarkInt() noexcept : val(0) {}
            inline QuarkInt(QuarkInt const& _val) noexcept : val(_val.val) {}
            inline QuarkInt(const integer_t& _val) noexcept : val(_val) {}
            inline value_t getType() const noexcept override    {return value_t::Int;}
            inline boolean_t getBool() const noexcept override  {return (val != 0ll);}
            inline integer_t getInt() const noexcept override   {return val;}
            inline float_t getFloat() const noexcept override   {return static_cast<float_t>(val);}
        };
        
        //! @internal A Quark that hold a floating-point number
        class QuarkFloat : public Quark
        {
        public:
            const float_t val;
            inline QuarkFloat(QuarkFloat const& _val) noexcept : val(_val.val) {}
            inline QuarkFloat(float_t const& _val) noexcept : val(_val) {}
            inline value_t getType() const noexcept override    {return value_t::Float;}
            inline boolean_t getBool() const noexcept override  {return (val != 0.);}
            inline integer_t getInt() const noexcept override   {return static_cast<integer_t>(val);}
            inline float_t getFloat() const noexcept override   {return val;}
        };
        
        //! @internal A Quark that hold a tag
        class QuarkSymbol : public Quark
        {
        public:
            const symbol_t val;
            inline QuarkSymbol(QuarkSymbol const& _val) noexcept : val(_val.val) {}
            inline QuarkSymbol(const symbol_t _val) noexcept : val(_val) {}
            inline value_t getType() const noexcept override    {return value_t::Symbol;}
            inline symbol_t getSymbol() const noexcept override       {return val;}
        };
        
        Quark* m_quark;
    };
    
    std::ostream& operator<<(std::ostream& output, Atom const& atom);
}



#endif // KIWI_CORE_ATOM_HPP_INCLUDED
