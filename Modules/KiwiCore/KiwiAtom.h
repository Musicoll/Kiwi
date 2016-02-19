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

#ifndef KIWI_ATOM_H_INCLUDED
#define KIWI_ATOM_H_INCLUDED

#include "KiwiTag.h"

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
        
        //! @brief The type of a boolean in the Atom class
        using boolean_t = bool;
        
        //! @brief The type of an integer number in the Atom class
        using integer_t = int64_t;
        
        //! @brief The type of a floating-point number in the Atom class
        using float_t = double;
        
        //! @brief The type of a Tag in the Atom class
        using tag_t = sTag;
        
        //! @brief The type of a vector of atom in the Atom class
        using vector_t = std::vector<Atom>;
        
        //! @brief The type of a vector of atom in the Atom class
        using dico_t = std::map<tag_t, Atom>;
        
        //! @brief Enum of Atom value types
        enum class Type : uint8_t
        {
            Null        = 0,
            Boolean     = 1,
            Int         = 2,
            Float       = 3,
            Tag         = 4,
            Vector      = 5,
            Dico        = 6
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
        inline Atom(boolean_t value) noexcept : m_quark(new QuarkBool(value)) {}
        
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
        
        //! @brief Constructs a tag_t Atom (explicit).
        //! @param tag The tag.
        inline Atom(const tag_t tag) noexcept
        : m_quark(new QuarkTag(tag)) {}

        //! @brief Constructs a tag_t Atom.
        //! @param tag The tag
        inline Atom(char const* tag) noexcept
        : Atom(Tag::create(tag)) {}
        
        //! @brief Constructs a tag_t Atom with a string.
        //! @param tag The tag
        inline Atom(std::string const& tag) noexcept
        : Atom(Tag::create(tag)) {}
        
        //! @brief Constructs a tag_t Atom by moving a string.
        //! @param tag The tag
        inline Atom(std::string&& tag) noexcept
        : Atom(Tag::create(std::forward<std::string>(tag))) {}
        
        //! @brief Constructs a vector_t Atom (explicit).
        //! @param atoms A vector of atoms
        inline explicit Atom(vector_t const& atoms) noexcept
        : m_quark(new QuarkVector(atoms)) {}
        
        //! @brief Constructs a vector_t Atom using move semantics.
        //! @param atoms A vector of atoms
        inline Atom(vector_t&& atoms) noexcept
        : m_quark(new QuarkVector(std::forward<vector_t>(atoms))) {}
        
        //! @brief Constructs a vector_t Atom with iterators.
        //! @param first First iterator
        //! @param last Last iterator
        inline Atom(vector_t::iterator first, vector_t::iterator last) noexcept
        : m_quark(new QuarkVector(first, last)) {}
        
        //! @brief Constructs a vector_t Atom using an initializer_list.
        //! @param il The initializer_list of atoms
        inline Atom(std::initializer_list<Atom> il) noexcept
        : m_quark(new QuarkVector(il)) {}
        
        //! @brief Constructs a dico_t Atom (explicit).
        //! @param dico A dico_t
        inline explicit Atom(dico_t const& dico) noexcept
        : m_quark(new QuarkDico(dico)) {}
        
        //! @brief Constructs a dico_t Atom using move semantics.
        //! @param dico A dico_t
        inline Atom(dico_t&& atoms) noexcept
        : m_quark(new QuarkDico(std::forward<dico_t>(atoms))) {}
        
        //! @brief Constructs a dico_t Atom using iterators.
        //! @param first The first iterator
        //! @param first The last iterator
        inline Atom(dico_t::iterator first, dico_t::iterator last) noexcept
        : m_quark(new QuarkDico(first, last)) {}
        
        //! @brief Constructs a dico_t Atom using an initializer_list of tag_t/Atom pair.
        //! @param il The initializer_list of pair
        inline Atom(std::initializer_list<std::pair<const tag_t, Atom>> il) noexcept
        : m_quark(new QuarkDico(il)) {}
        
        //! Destructor.
        inline ~Atom() noexcept {delete m_quark;}
        
        //! @brief Get the type of the Atom.
        //! @return The Type of the atom as a Type.
        //! @see  isNull(), isBool(), isInt(), isFloat(), isNumber(), isTag(), isVector(), isDico()
        inline Type getType() const noexcept {return m_quark->getType();}
        
        //! @brief Returns true if the Atom is Null.
        //! @return true if the Atom is Null.
        //! @see getType(), isBool(), isInt(), isFloat(), isNumber(), isTag(), isVector(), isDico()
        inline bool isNull() const noexcept {return m_quark->isNull();}
        
        //! @brief Returns true if the Atom is a boolean_t.
        //! @return true if the Atom is a boolean_t.
        //! @see getType(), isNull(), isInt(), isFloat(), isNumber(), isTag(), isVector(), isDico()
        inline bool isBool() const noexcept {return m_quark->isBool();}
        
        //! @brief Returns true if the Atom is an integer_t.
        //! @return true if the Atom is an integer_t.
        //! @see getType(), isNull(), isBool(), isFloat(), isNumber(), isTag(), isVector(), isDico()
        inline bool isInt() const noexcept {return m_quark->isInt();}
        
        //! @brief Returns true if the Atom is a float_t.
        //! @return true if the Atom is an float_t.
        //! @see getType(), isNull(), isBool(), isInt(), isNumber(), isTag(), isVector(), isDico()
        inline bool isFloat() const noexcept {return m_quark->isFloat();}
        
        //! @brief Returns true if the Atom is a boolean_t, an integer_t, or a float_t.
        //! @return true if the Atom is a boolean_t, an integer_t, or a float_t.
        //! @see getType(), isNull(), isBool(), isInt(), isFloat(), isTag(), isVector(), isDico()
        inline bool isNumber() const noexcept {return m_quark->isNumber();}
        
        //! @brief Returns true if the Atom is a tag_t.
        //! @return true if the Atom is a tag_t.
        //! @see getType(), isNull(), isBool(), isInt(), isFloat(), isNumber(), isVector(), isDico()
        inline bool isTag() const noexcept {return m_quark->isTag();}
        
        //! @brief Returns true if the Atom is a vector_t.
        //! @return true if the Atom is a vector_t.
        //! @see getType(), isNull(), isBool(), isInt(), isFloat(), isNumber(), isTag(), isDico()
        inline bool isVector() const noexcept {return m_quark->isVector();}
        
        //! @brief Returns true if the Atom is a dico_t.
        //! @return true if the Atom is a dico_t.
        //! @see getType(), isNull(), isBool(), isInt(), isFloat(), isNumber(), isTag(), isVector()
        inline bool isDico() const noexcept {return m_quark->isDico();}
        
        //! @brief Retrieves The Atom value as a boolean_t.
        //! @return A boolean value if the atom is a boolean or a digit otherwise false.
        //! @see isBool()
        inline operator boolean_t() const noexcept {return m_quark->getBool();}
        
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
        inline operator tag_t() const noexcept {return m_quark->getTag();}
        
        //! Cast the atom to a vector of atoms.
        /** The function casts the atom to a vector of atoms.
         @return A vector of atoms.
         */
        inline operator vector_t() const noexcept {return m_quark->getVector();}
        
        //! Cast the atom to a map of atoms.
        /** The function casts the atom to a map of atoms.
         @return A map of atoms.
         */
        inline operator dico_t() const noexcept {return m_quark->getDico();}
        
        //! @brief Access specified element of an Atom vector.
        //! @param index The index of the element.
        //! @throw std::domain_error if Atom is not a vector.
        //! @return A reference to the element.
        Atom& operator[](size_t index)
        {
            // implicitly convert null value to an empty vector
            if (isNull())
            {
                delete m_quark;
                m_quark = new QuarkVector();
            }
            
            // operator[] only works for vectors
            if (isVector())
            {
                vector_t& vec = static_cast<QuarkVector*>(m_quark)->getVectorRef();
                
                // fill up array with null values until given idx is reached
                for(size_t i = vec.size(); i <= index; ++i)
                {
                    vec.emplace_back(Atom());
                }
                
                return vec.operator[](index);
            }
            else
            {
                throw std::domain_error("cannot use operator[] with this Atom type");
            }
        }
        
        //! @brief Access specified element of an Atom vector (const).
        //! @param index The index of the element.
        //! @throw std::domain_error if Atom is not a vector.
        //! @return A const reference to the element.
        const Atom& operator[](size_t index) const
        {
            // operator[] only works for vectors
            if (isVector())
            {
                const vector_t& vec = static_cast<QuarkVector*>(m_quark)->getVectorRef();
                return vec.operator[](index);
            }
            else
            {
                throw std::domain_error("cannot use operator[] with this Atom type");
            }
        }
        
        Atom& operator[](const typename dico_t::key_type& key)
        {
            // implicitly convert Atom Null to an Atom dico
            if (isNull())
            {
                delete m_quark;
                m_quark = new QuarkDico();
            }
            
            // operator[] only works for Atom dico
            if (isDico())
            {
                dico_t& dico = static_cast<QuarkDico*>(m_quark)->getDicoRef();
                return dico.operator[](key);
            }
            else
            {
                throw std::domain_error("cannot use operator[] with this Atom type");
            }
        }
        
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
        inline Atom& operator=(const boolean_t value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkBool(value);
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
            m_quark = new QuarkTag(Tag::create(tag));
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
            m_quark = new QuarkTag(Tag::create(tag));
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
            m_quark = new QuarkTag(Tag::create(std::forward<std::string>(tag)));
            return *this;
        }
        
        //! Set up the atom with a tag.
        /** The function sets up the atom with a tag.
         @param tag   The tag.
         @return An atom.
         */
        inline Atom& operator=(tag_t tag) noexcept
        {
            delete m_quark;
            m_quark = new QuarkTag(tag);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(vector_t const& atoms) noexcept
        {
            delete m_quark;
            m_quark = new QuarkVector(atoms);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(vector_t&& atoms) noexcept
        {
            delete m_quark;
            m_quark = new QuarkVector(std::forward<vector_t>(atoms));
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(std::initializer_list<Atom> il) noexcept
        {
            delete m_quark;
            m_quark = new QuarkVector(il);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(dico_t const& atoms) noexcept
        {
            delete m_quark;
            m_quark = new QuarkDico(atoms);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(dico_t&& atoms) noexcept
        {
            delete m_quark;
            m_quark = new QuarkDico(std::forward<dico_t>(atoms));
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(std::initializer_list<std::pair<const tag_t, Atom>> il) noexcept
        {
            delete m_quark;
            m_quark = new QuarkDico(il);
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
            else if(other.isBool() && isNumber())
            {
                return m_quark->getBool() == other.m_quark->getBool();
            }
            else if(other.isInt() && isNumber())
            {
                return m_quark->getInt() == other.m_quark->getInt();
            }
            else if(other.isFloat() && isNumber())
            {
                return m_quark->getFloat() == other.m_quark->getFloat();
            }
            else if(other.isTag() && isTag())
            {
                return m_quark->getTag() == other.m_quark->getTag();
            }
            else if(other.isVector() && isVector())
            {
                return m_quark->getVector() == other.m_quark->getVector();
            }
            else if(other.isDico() && isDico())
            {
                return m_quark->getDico() == other.m_quark->getDico();
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a boolean value.
        /** The function compares the atom with a boolean value.
         @param value   The boolean value.
         @return true if the atom hold the same boolean value otherwise false.
         */
        inline bool operator==(const boolean_t value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getBool() == value;
            }
            else
            {
                return false;
            }
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
        bool operator==(tag_t tag) const noexcept
        {
            if(isTag())
            {
                return m_quark->getTag() == tag;
            }
            
            return false;
        }
        
        //! Compare the atom with a vector.
        /** The function compares the atom with a vector.
         @param vector   The vector.
         @return true if the atom hold the same vector otherwise false.
         */
        inline bool operator==(vector_t const& vector) const noexcept
        {
            if(isVector())
            {
                return m_quark->getVector() == vector;
            }
            
            return false;
        }
        
        //! Compare the atom with a dico.
        /** The function compares the atom with a dico.
         @param dico   The dico.
         @return true if the atom hold the same dico otherwise false.
         */
        inline bool operator==(dico_t const& dico) const noexcept
        {
            if(isDico())
            {
                return m_quark->getDico() == dico;
            }
            else
            {
                return false;
            }
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
        inline bool operator!=(const sTag tag) const noexcept
        {
            return !(*this == tag);
        }
        
        //! Compare the atom with a vector.
        /** The function compares the atom with a vector.
         @param vector   The vector.
         @return true if the atom differ from the vector, otherwise false.
         */
        inline bool operator!=(vector_t const& vector) const noexcept
        {
            return !(*this == vector);
        }
        
        //! Compare the atom with a dico.
        /** The function compares the atom with a dico.
         @param dico   The dico.
         @return true if the atom differ from the dico otherwise false.
         */
        inline bool operator!=(dico_t const& dico) const noexcept
        {
            return !(*this == dico);
        }
        
        static std::ostream& toJson(std::ostream &output, const Atom &atom, ulong& indent);
        
        //! Parse a std::string into a vector of atoms.
        /** Parse a std::string into a vector of atoms.
         @param     text	The std::string to parse.
         @return    The vector of atoms.
         @remark    For example, the std::string : "foo \"bar 42\" 1 2 3.14" will parsed into a vector of 5 atoms.
         The atom types will be determined automatically as 2 #Atom::Type::Tag atoms, 2 #Atom::Type::Int atoms,
         && 1 #Atom::Type::Float atom.
         */
        static Vector parse(std::string const& text);
        
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
            inline bool isBool() const noexcept             {return getType() == value_t::Boolean;}
            inline bool isInt() const noexcept              {return getType() == value_t::Int;}
            inline bool isFloat() const noexcept            {return getType() == value_t::Float;}
            inline bool isNumber() const noexcept           {return isInt() || isFloat() || isBool();}
            inline bool isTag() const noexcept              {return getType() == value_t::Tag;}
            inline bool isDico() const noexcept             {return getType() == value_t::Dico;}
            inline bool isVector() const noexcept           {return getType() == value_t::Vector;}
            // Getters
            virtual inline boolean_t getBool() const noexcept   {return false;}
            virtual inline integer_t getInt() const noexcept    {return 0ll;}
            virtual inline float_t   getFloat() const noexcept  {return 0.;}
            virtual inline tag_t     getTag() const noexcept    {return Tags::_empty;}
            virtual inline vector_t  getVector() const noexcept {return Vector();}
            virtual inline dico_t    getDico() const noexcept   {return Dico();}
        };
        
        //! @internal A Quark that hold a boolean
        class QuarkBool : public Quark
        {
        public:
            const boolean_t val;
            inline QuarkBool() noexcept : val(false) {}
            inline QuarkBool(QuarkBool const& _val) noexcept : val(_val.val) {}
            inline QuarkBool(boolean_t const& _val) noexcept : val(_val) {}
            inline value_t getType() const noexcept override    {return value_t::Boolean;}
            inline boolean_t getBool() const noexcept override  {return val;}
            inline integer_t getInt() const noexcept override   {return val ? 1ll : 0ll;}
            inline float_t getFloat() const noexcept override   {return val ? 1. : 0.;}
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
        class QuarkTag : public Quark
        {
        public:
            const tag_t val;
            inline QuarkTag(QuarkTag const& _val) noexcept : val(_val.val) {}
            inline QuarkTag(const tag_t _val) noexcept : val(_val) {}
            inline value_t getType() const noexcept override    {return value_t::Tag;}
            inline tag_t getTag() const noexcept override       {return val;}
        };
        
        //! @internal A Quark that hold a vector of Atoms
        class QuarkVector : public Quark
        {
        public:
            vector_t val;
            QuarkVector() = default;
            inline QuarkVector(QuarkVector const& _val) noexcept : val(_val.val) {}
            inline QuarkVector(vector_t const& _val) noexcept : val(_val) {}
            inline QuarkVector(vector_t::iterator first, vector_t::iterator last) noexcept
            : val(first, last) {}
            inline QuarkVector(vector_t&& _val) noexcept {std::swap(val, _val);}
            inline QuarkVector(std::initializer_list<Atom> il) noexcept : val(il) {}
            inline ~QuarkVector() noexcept {val.clear();}
            inline value_t getType() const noexcept override    {return value_t::Vector;}
            inline vector_t getVector() const noexcept override {return val;}
            inline vector_t& getVectorRef() noexcept            {return val;}
            inline const vector_t& getVectorRef() const noexcept{return val;}
        };
        
        //! @internal A Quark that hold key/Atom pairs
        class QuarkDico : public Quark
        {
        public:
            dico_t val;
            QuarkDico() = default;
            inline QuarkDico(QuarkDico const& _val) noexcept : val(_val.val) {}
            inline QuarkDico(dico_t const& _val) noexcept : val(_val) {}
            inline QuarkDico(dico_t::iterator first, Dico::iterator last) noexcept : val(first, last) {}
            inline QuarkDico(dico_t&& _val) noexcept {std::swap(val, _val);}
            inline QuarkDico(std::initializer_list<std::pair<const sTag, Atom>> il) noexcept : val(il) {}
            inline ~QuarkDico() noexcept {val.clear();}
            inline value_t getType() const noexcept override {return value_t::Dico;}
            inline dico_t getDico() const noexcept override {return val;}
            inline dico_t& getDicoRef() noexcept {return val;}
            inline const dico_t& getDicoRef() const noexcept {return val;}
        };
        
        Quark* m_quark;
    };
    
    std::ostream& operator<<(std::ostream &output, const Atom &atom);
}



#endif // KIWI_ATOM_H_INCLUDED
