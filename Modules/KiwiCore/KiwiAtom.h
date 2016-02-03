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

#ifndef __DEF_KIWI_CORE_ATOM__
#define __DEF_KIWI_CORE_ATOM__

#include "KiwiTag.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      ATOM                                        //
    // ================================================================================ //
    
    //! The atom class
    /**
     The atom is a base class that you should inherite from if you want to be able to pass your class in an atom vector or in a dico. The default atoms are the long, the double, the tag, the dico and the object.
     */
    class Atom
    {
    public:
        
        //! @Todo : replace CAPS with Min and type the enum
        enum Type
        {
            UNDEFINED = 0,
            BOOLEAN   = 1,
            LONG      = 2,
            DOUBLE    = 3,
            TAG       = 4,
            VECTOR    = 5,
            DICO      = 6
        };
        
    private:

        class Quark
        {
        public:
            constexpr inline Quark() noexcept {}
            virtual inline ~Quark() noexcept {}
            virtual inline Type getType() const noexcept {return UNDEFINED;}
            inline bool isUndefined() const noexcept {return getType() == UNDEFINED;}
            inline bool isBool() const noexcept {return getType() == BOOLEAN;}
            inline bool isLong() const noexcept {return getType() == LONG;}
            inline bool isDouble() const noexcept {return getType() == DOUBLE;}
            inline bool isNumber() const noexcept {return isLong() || isDouble() || isBool();}
            inline bool isTag() const noexcept {return getType() == TAG;}
            inline bool isDico() const noexcept{return getType() == DICO;}
            inline bool isVector() const noexcept {return getType() == VECTOR;}
            virtual inline bool getBool() const noexcept {return false;}
            virtual inline int64_t getLong() const noexcept {return 0ul;}
            virtual inline double getDouble() const noexcept {return 0.;}
            virtual inline sTag getTag() const noexcept {return Tags::_empty;}
            virtual inline Vector getVector() const noexcept {return Vector();}
            virtual inline Dico getDico() const noexcept {return Dico();}
        };
        
        class QuarkBool : public Quark
        {
        public:
            const bool val;
            inline QuarkBool(QuarkBool const& _val) noexcept : val(_val.val) {}
            inline QuarkBool(bool const& _val) noexcept : val(_val) {}
            inline Type getType() const noexcept override {return BOOLEAN;}
            inline bool getBool() const noexcept override {return val;}
            inline int64_t getLong() const noexcept override {return long(val);}
            inline double getDouble() const noexcept override {return double(val);}
        };
        
        class QuarkLong : public Quark
        {
        public:
            const int64_t val;
            inline QuarkLong(QuarkLong const& _val) noexcept : val(_val.val) {}
            inline QuarkLong(const long& _val) noexcept : val(int64_t(_val)) {}
            inline QuarkLong(const int64_t& _val) noexcept : val(_val) {}
            inline Type getType() const noexcept override {return LONG;}
            inline bool getBool() const noexcept override {return bool(val);}
            inline int64_t getLong() const noexcept override {return val;}
            inline double getDouble() const noexcept override {return double(val);}
        };
        
        class QuarkDouble : public Quark
        {
        public:
            const double val;
            inline QuarkDouble(QuarkDouble const& _val) noexcept : val(_val.val) {}
            inline QuarkDouble(double const& _val) noexcept : val(_val) {}
            inline Type getType() const noexcept override {return DOUBLE;}
            inline bool getBool() const noexcept override {return bool(val);}
            inline int64_t getLong() const noexcept override {return long(val);}
            inline double getDouble() const noexcept override {return val;}
        };
        
        class QuarkTag : public Quark
        {
        public:
            const sTag val;
            inline QuarkTag(QuarkTag const& _val) noexcept : val(_val.val) {}
            inline QuarkTag(const sTag _val) noexcept : val(_val) {}
            inline Type getType() const noexcept override {return TAG;}
            inline sTag getTag() const noexcept override {return val;}
        };
        
        class QuarkVector : public Quark
        {
        public:
            Vector val;
            inline QuarkVector(QuarkVector const& _val) noexcept : val(_val.val) {}
            inline QuarkVector(Vector const& _val) noexcept : val(_val) {}
            inline QuarkVector(Vector::iterator first, Vector::iterator last) noexcept : val(first, last) {}
            inline QuarkVector(Vector&& _val) noexcept {swap(val, _val);}
            inline QuarkVector(initializer_list<Atom> il) noexcept : val(il) {}
            inline ~QuarkVector() noexcept {val.clear();}
            inline Type getType() const noexcept override {return VECTOR;}
            inline Vector getVector() const noexcept override {return val;}
        };
        
        class QuarkDico : public Quark
        {
        public:
            Dico val;
            inline QuarkDico(QuarkDico const& _val) noexcept : val(_val.val) {}
            inline QuarkDico(Dico const& _val) noexcept : val(_val) {}
            inline QuarkDico(Dico::iterator first, Dico::iterator last) noexcept : val(first, last) {}
            inline QuarkDico(Dico&& _val) noexcept {swap(val, _val);}
            inline QuarkDico(initializer_list<pair<const sTag, Atom>> il) noexcept : val(il) {}
            inline ~QuarkDico() noexcept {val.clear();}
            inline Type getType() const noexcept override {return DICO;}
            inline Dico getDico() const noexcept override {return val;}
        };
        
        Quark* m_quark; // use an unique_ptr<> instead ?
        
    public:
        
        // ================================================================================ //
        //                                      ATOM                                        //
        // ================================================================================ //
        
        //! Constructor.
        /** The function allocates an undefined atom.
         */
        inline Atom() noexcept : m_quark(new Quark()) {}
        
        //! Constructor with another atom.
        /** The function allocates the atom with an atom.
         */
        inline Atom(Atom&& other) noexcept : m_quark(move(other.m_quark)) {other.m_quark = new Quark();}
        
        //! Constructor with another atom.
        /** The function allocates the atom with an atom.
         */
        Atom(Atom const& other) noexcept;
        
        //! Constructor with a boolean value.
        /** The function allocates the atom with a long value created with a boolean value.
         @param value The value.
         */
        inline Atom(bool value) noexcept : m_quark(new QuarkBool(value)) {}
        
        //! Constructor with a long value.
        /** The function allocates the atom with a long value.
         @param value The value.
         */
        inline Atom(const int value) noexcept : m_quark(new QuarkLong(long(value))) {}
        
        //! Constructor with a long value.
        /** The function allocates the atom with a long value.
         @param value The value.
         */
        inline Atom(const long value) noexcept : m_quark(new QuarkLong(int64_t(value))) {}
        
        //! Constructor with a long long value.
        /** The function allocates the atom with a long long value.
         @param value The value.
         */
        inline Atom(const int64_t value) noexcept : m_quark(new QuarkLong(value)) {}
        
        //! Constructor with a double value.
        /** The function allocates the atom with a double value.
         @param value The value.
         */
        inline Atom(const float value) noexcept : m_quark(new QuarkDouble(double(value))) {}
        
        //! Constructor with a double value.
        /** The function allocates the atom with a double value.
         @param value The value.
         */
        inline Atom(const double value) noexcept : m_quark(new QuarkDouble(value)) {}
        
        //! Constructor with a string.
        /** The function allocates the atom with a tag created with a string.
         @param tag The tag.
         */
        inline Atom(char const* tag) noexcept : m_quark(new QuarkTag(Tag::create(tag))) {}
        
        //! Constructor with a string.
        /** The function allocates the atom with a tag created with a string.
         @param tag The tag.
         */
        inline Atom(string const& tag) noexcept : m_quark(new QuarkTag(Tag::create(tag))) {}
        
        //! Constructor with a string.
        /** The function allocates the atom with a tag created with a string.
         @param tag The tag.
         */
        inline Atom(string&& tag) noexcept : m_quark(new QuarkTag(Tag::create(forward<string>(tag)))) {}
        
        //! Constructor with a tag.
        /** The function allocates the atom with a tag.
         */
        inline Atom(const sTag tag) noexcept : m_quark(new QuarkTag(tag)) {}
        
        //! Constructor with a vector of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline Atom(Vector const& atoms) noexcept : m_quark(new QuarkVector(atoms)) {}
        
        //! Constructor with a vector of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline Atom(Vector&& atoms) noexcept : m_quark(new QuarkVector(forward<Vector>(atoms))) {}
        
        //! Constructor with a vector of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline Atom(Vector::iterator first, Vector::iterator last) noexcept : m_quark(new QuarkVector(first, last)) {}
        
        //! Constructor with a vector of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline Atom(initializer_list<Atom> il) noexcept : m_quark(new QuarkVector(il)) {}
        
        //! Constructor with a map of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline Atom(Dico const& atoms) noexcept : m_quark(new QuarkDico(atoms)) {}
        
        //! Constructor with a map of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline Atom(Dico&& atoms) noexcept  : m_quark(new QuarkDico(forward<Dico>(atoms))) {}
        
        //! Constructor with a map of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline Atom(Dico::iterator first, Dico::iterator last) noexcept : m_quark(new QuarkDico(first, last)) {}
        
        //! Constructor with a map of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline Atom(initializer_list<pair<const sTag, Atom>> il) noexcept : m_quark(new QuarkDico(il)) {}
        
        //! Destructor.
        /** Doesn't perform anything.
         */
        inline ~Atom() noexcept {delete m_quark;}
        
        //! Retrieve the type of the atom.
        /** The function retrieves the type of the atom.
         @return The type of the atom as a type.
         */
        inline Type getType() const noexcept {return m_quark->getType();}
        
        //! Check if the atom is undefined.
        /** The function checks if the atom is undefined.
         @return    true if the atom is undefined.
         */
        inline bool isUndefined() const noexcept {return m_quark->isUndefined();}
        
        //! Check if the atom is of type bool.
        /** The function checks if the atom is of type bool.
         @return    true if the atom is a bool.
         */
        inline bool isBool() const noexcept {return m_quark->isBool();}
        
        //! Check if the atom is of type long.
        /** The function checks if the atom is of type long.
         @return    true if the atom is a long.
         */
        inline bool isLong() const noexcept {return m_quark->isLong();}
        
        //! Check if the atom is of type double.
        /** The function checks if the atom is of type double.
         @return    true if the atom is a double.
         */
        inline bool isDouble() const noexcept {return m_quark->isDouble();}
        
        //! Checks if the atom is of type long or double.
        /** The function checks if the atom is of type long or double.
         @return    true if the atom is a long or a double.
         */
        inline bool isNumber() const noexcept {return m_quark->isNumber();}
        
        //! Check if the atom is of type tag.
        /** The function checks if the atom is of type tag.
         @return    true if the atom is a tag.
         */
        inline bool isTag() const noexcept {return m_quark->isTag();}
        
        //! Check if the atom is of type vector.
        /** The function checks if the atom is of type vector.
         @return    true if the atom is a vector.
         */
        inline bool isVector() const noexcept {return m_quark->isVector();}
        
        //! Check if the atom is of type map.
        /** The function checks if the atom is of type map.
         @return    true if the atom is a map.
         */
        inline bool isDico() const noexcept {return m_quark->isDico();}
        
        //! Cast the atom to a boolean.
        /** The function casts the atom to a boolean.
         @return An boolean value if the atom is a digit otherwise 0.
         */
        inline operator bool() const noexcept {return m_quark->getBool();}
        
        //! Cast the atom to an int.
        /** The function casts the atom to an int.
         @return An int value if the atom is a digit otherwise 0.
         */
        inline operator int() const noexcept {return int(m_quark->getLong());}
        
        //! Cast the atom to a long.
        /** The function casts the atom to a long.
         @return A long value if the atom is a digit otherwise 0.
         */
        inline operator long() const noexcept {return m_quark->getLong();}
        
        //! Cast the atom to a long long.
        /** The function casts the atom to a long long.
         @return A long value if the atom is a digit otherwise 0.
         */
        inline operator int64_t() const noexcept {return m_quark->getLong();}
        
        //! Cast the atom to a long.
        /** The function casts the atom to a long.
         @return A long value if the atom is a digit otherwise 0.
         */
        inline operator ulong() const noexcept {return ulong(m_quark->getLong());}
        
        //! Cast the atom to a float.
        /** The function casts the atom to a float.
         @return A float value if the atom is a digit otherwise 0.
         */
        inline operator float() const noexcept {return float(m_quark->getDouble());}
        
        //! Cast the atom to a double.
        /** The function casts the atom to a double.
         @return A double value if the atom is a digit otherwise 0.
         */
        inline operator double() const noexcept {return m_quark->getDouble();}
        
        //! Cast the atom to a tag.
        /** The function casts the atom to a tag.
         @return A tag if the atom is a tag otherwise a nullptr.
         */
        inline operator sTag() const noexcept {return m_quark->getTag();}
        
        //! Cast the atom to a vector of atoms.
        /** The function casts the atom to a vector of atoms.
         @return A vector of atoms.
         */
        inline operator Vector() const noexcept {return m_quark->getVector();}
        
        //! Cast the atom to a map of atoms.
        /** The function casts the atom to a map of atoms.
         @return A map of atoms.
         */
        inline operator Dico() const noexcept {return m_quark->getDico();}
        
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
            swap(m_quark, other.m_quark);
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
            m_quark = new QuarkBool(value);
            return *this;
        }
        
        //! Set up the atom with a long value.
        /** The function sets up the atom with a long value.
         @param value   The long value.
         @return An atom.
         */
        inline Atom& operator=(const int value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkLong((long)value);
            return *this;
        }
        
        //! Set up the atom with a long value.
        /** The function sets up the atom with a long value.
         @param value   The long value.
         @return An atom.
         */
        inline Atom& operator=(const long value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkLong(int64_t(value));
            return *this;
        }
        
        //! Set up the atom with a long value.
        /** The function sets up the atom with a long value.
         @param value   The long value.
         @return An atom.
         */
        inline Atom& operator=(const int64_t value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkLong(value);
            return *this;
        }
        
        //! Set up the atom with a double value.
        /** The function sets up the atom with a double value.
         @param value   The double value.
         @return An atom.
         */
        inline Atom& operator=(const float value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkDouble((float)value);
            return *this;
        }
        
        //! Set up the atom with a double value.
        /** The function sets up the atom with a double value.
         @param value   The double value.
         @return An atom.
         */
        inline Atom& operator=(const double value) noexcept
        {
            delete m_quark;
            m_quark = new QuarkDouble(value);
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
        inline Atom& operator=(string const& tag)
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
        inline Atom& operator=(string&& tag) noexcept
        {
            delete m_quark;
            m_quark = new QuarkTag(Tag::create(forward<string>(tag)));
            return *this;
        }
        
        //! Set up the atom with a tag.
        /** The function sets up the atom with a tag.
         @param tag   The tag.
         @return An atom.
         */
        inline Atom& operator=(sTag tag) noexcept
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
        inline Atom& operator=(Vector const& atoms) noexcept
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
        inline Atom& operator=(Vector&& atoms) noexcept
        {
            delete m_quark;
            m_quark = new QuarkVector(forward<Vector>(atoms));
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(initializer_list<Atom> il) noexcept
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
        inline Atom& operator=(Dico const& atoms) noexcept
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
        inline Atom& operator=(Dico&& atoms) noexcept
        {
            delete m_quark;
            m_quark = new QuarkDico(forward<Dico>(atoms));
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline Atom& operator=(initializer_list<pair<const sTag, Atom>> il) noexcept
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
            if(other.isUndefined() && isUndefined())
            {
                return true;
            }
            else if(other.isBool() && isNumber())
            {
                return m_quark->getBool() == other.m_quark->getBool();
            }
            else if(other.isLong() && isNumber())
            {
                return m_quark->getLong() == other.m_quark->getLong();
            }
            else if(other.isDouble() && isNumber())
            {
                return m_quark->getDouble() == other.m_quark->getDouble();
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
        inline bool operator==(const bool value) const noexcept
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
        
        //! Compare the atom with a integer value.
        /** The function compares the atom with a integer value.
         @param value   The integer value.
         @return true if the atom hold the same integer value otherwise false.
         */
        inline bool operator==(const int value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getLong() == (long)value;
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
        inline bool operator==(const long value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getLong() == value;
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
        inline bool operator==(const long long value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getLong() == long(value);
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
        inline bool operator!=(const long long value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getLong() != long(value);
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a float value.
        /** The function compares the atom with a float value.
         @param value   The float value.
         @return true if the atom hold the same float value otherwise false.
         */
        inline bool operator==(const float value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getDouble() == (double)value;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a double value.
        /** The function compares the atom with a double value.
         @param value   The double value.
         @return true if the atom hold the same double value otherwise false.
         */
        inline bool operator==(const double value) const noexcept
        {
            if(isNumber())
            {
                return m_quark->getDouble() == value;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom hold the same tag create with the string otherwise false.
         */
        bool operator==(char const* tag) const noexcept;
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom hold the same tag create with the string otherwise false.
         */
        bool operator==(string const& tag) const noexcept;
        
        //! Compare the atom with a tag.
        /** The function compares the atom with a tag.
         @param tag   The tag.
         @return true if the atom hold the same tag otherwise false.
         */
        bool operator==(sTag tag) const noexcept;
        
        //! Compare the atom with a vector.
        /** The function compares the atom with a vector.
         @param vector   The vector.
         @return true if the atom hold the same vector otherwise false.
         */
        inline bool operator==(Vector const& vector) const noexcept
        {
            if(isVector())
            {
                return m_quark->getVector() == vector;
            }
            else
            {
                return false;
            }
        }
        
        //! Compare the atom with a dico.
        /** The function compares the atom with a dico.
         @param dico   The dico.
         @return true if the atom hold the same dico otherwise false.
         */
        inline bool operator==(Dico const& dico) const noexcept
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
        inline bool operator!=(const bool value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the atom with a long value.
        /** The function compares the atom with a long.
         @param value   The long value.
         @return true if the atom differ from the long value otherwise false.
         */
        inline bool operator!=(const long value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the atom with a double value.
        /** The function compares the atom with a double value.
         @param value   The double value.
         @return true if the atom differ from the double value otherwise false.
         */
        inline bool operator!=(const double value) const noexcept
        {
            return !(*this == value);
        }
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom differ from the tag create with the string otherwise false.
         */
        inline bool operator!=(char const* tag) const noexcept
        {
            return !(*this == tag);
        }
        
        //! Compare the atom with a string.
        /** The function compares the atom with a string.
         @param tag   The string.
         @return true if the atom differ from the tag create with the string otherwise false.
         */
        inline bool operator!=(string const& tag) const noexcept
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
        inline bool operator!=(Vector const& vector) const noexcept
        {
            return !(*this == vector);
        }
        
        //! Compare the atom with a dico.
        /** The function compares the atom with a dico.
         @param dico   The dico.
         @return true if the atom differ from the dico otherwise false.
         */
        inline bool operator!=(Dico const& dico) const noexcept
        {
            return !(*this == dico);
        }
        
        static ostream& toJson(ostream &output, const Atom &atom, ulong& indent);
        
        //! Parse a string into a vector of atoms.
        /** Parse a string into a vector of atoms.
         @param     text	The string to parse.
         @return    The vector of atoms.
         @remark    For example, the string : "foo \"bar 42\" 1 2 3.14" will parsed into a vector of 5 atoms.
         The atom types will be determined automatically as 2 #Atom::Type::TAG atoms, 2 #Atom::Type::LONG atoms,
         and 1 #Atom::Type::DOUBLE atom.
         */
        static Vector parse(string const& text);
    };
    
    ostream& operator<<(ostream &output, const Atom &atom);
}



#endif


