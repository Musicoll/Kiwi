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

#ifndef __DEF_KIWI_MODELS_FLIP_ATOM__
#define __DEF_KIWI_MODELS_FLIP_ATOM__

#include "KiwiFlipTypes.h"

namespace Kiwi
{
    class FlipAtom;
    typedef vector<FlipAtom>        FlipAtomVector;
    typedef map<sTag, FlipAtom>     FlipAtomDico;
    
    // ================================================================================ //
    //                                      ATOM                                        //
    // ================================================================================ //
    
    //! The atom class
    /**
     The atom is a base class that you should inherite from if you want to be able to pass your class in an atom vector or in a dico. The default atoms are the long, the double, the tag, the dico and the object.
     */
    class FlipAtom : public flip::Object
    {
    public:
        
        enum AtomType
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

        struct Quark : public flip::Object
        {
            Quark() = default;
            virtual inline ~Quark() noexcept {}
            virtual inline AtomType getType() const noexcept {return UNDEFINED;}
            inline bool isUndefined() const noexcept {return getType() == UNDEFINED;}
            inline bool isBool() const noexcept {return getType() == BOOLEAN;}
            inline bool isLong() const noexcept {return getType() == LONG;}
            inline bool isDouble() const noexcept {return getType() == DOUBLE;}
            inline bool isNumber() const noexcept {return isLong() || isDouble() || isBool();}
            inline bool isTag() const noexcept {return getType() == TAG;}
            inline bool isDico() const noexcept{return getType() == DICO;}
            inline bool isVector() const noexcept {return getType() == VECTOR;}
            virtual inline bool getBool() const noexcept {return false;}
            virtual inline long getLong() const noexcept {return 0ul;}
            virtual inline double getDouble() const noexcept {return 0.;}
            virtual inline sTag getTag() const noexcept {return Tags::_empty;}
            virtual inline FlipAtomVector getVector() const noexcept {return FlipAtomVector();}
            virtual inline FlipAtomDico getDico() const noexcept {return FlipAtomDico();}
        };

        /*
        struct QuarkUndefined : public Quark
        {
            QuarkBool() = default;
            inline QuarkBool(QuarkBool const& _val) noexcept : val(_val.val) {}
            inline QuarkBool(bool const& _val) noexcept : val(_val) {}
            inline AtomType getType() const noexcept override {return BOOLEAN;}
            inline bool getBool() const noexcept override {return val;}
            inline long getLong() const noexcept override {return long(val);}
            inline double getDouble() const noexcept override {return double(val);}
        };
        */
        
        struct QuarkBool : public Quark
        {
            flip::Bool val;
            QuarkBool() = default;
            inline QuarkBool(QuarkBool const& _val) noexcept : val(_val.val) {}
            inline QuarkBool(bool const& _val) noexcept : val(_val) {}
            inline AtomType getType() const noexcept override {return BOOLEAN;}
            inline bool getBool() const noexcept override {return val;}
            inline long getLong() const noexcept override {return long(val);}
            inline double getDouble() const noexcept override {return double(val);}
        };
        
        struct QuarkLong : public Quark
        {
            flip::Int val;
            QuarkLong() = default;
            inline QuarkLong(QuarkLong const& _val) noexcept : val(_val.val) {}
            inline QuarkLong(long const& _val) noexcept : val(_val) {}
            inline QuarkLong(long long const& _val) noexcept : val(_val) {}
            inline AtomType getType() const noexcept override {return LONG;}
            inline bool getBool() const noexcept override {return bool(val);}
            inline long getLong() const noexcept override {return val;}
            inline double getDouble() const noexcept override {return double(val);}
        };
        
        struct QuarkDouble : public Quark
        {
            flip::Float val;
            QuarkDouble() = default;
            inline QuarkDouble(QuarkDouble const& _val) noexcept : val(_val.val) {}
            inline QuarkDouble(double const& _val) noexcept : val(_val) {}
            inline AtomType getType() const noexcept override {return DOUBLE;}
            inline bool getBool() const noexcept override {return bool(val);}
            inline long getLong() const noexcept override {return long(val);}
            inline double getDouble() const noexcept override {return val;}
        };
        
        struct QuarkTag : public Quark
        {
            FlipTag val;
            QuarkTag() = default;
            inline QuarkTag(QuarkTag const& _val) noexcept : val(_val.val) {}
            inline QuarkTag(const sTag _val) noexcept : val(_val) {}
            inline AtomType getType() const noexcept override {return TAG;}
            inline sTag getTag() const noexcept override {return val;}
        };
        
        struct QuarkVector : public Quark
        {
            FlipAtomVector val;
            QuarkVector() = default;
            inline QuarkVector(QuarkVector const& _val) noexcept : val(_val.val) {}
            inline QuarkVector(FlipAtomVector const& _val) noexcept : val(_val) {}
            inline QuarkVector(FlipAtomVector::iterator first, FlipAtomVector::iterator last) noexcept : val(first, last) {}
            inline QuarkVector(FlipAtomVector&& _val) noexcept {swap(val, _val);}
            inline QuarkVector(initializer_list<FlipAtom> il) noexcept : val(il) {}
            inline ~QuarkVector() noexcept {val.clear();}
            inline AtomType getType() const noexcept override {return VECTOR;}
            inline FlipAtomVector getVector() const noexcept override {return val;}
        };
        
        struct QuarkDico : public Quark
        {
            FlipAtomDico val;
            QuarkDico() = default;
            inline QuarkDico(QuarkDico const& _val) noexcept : val(_val.val) {}
            inline QuarkDico(FlipAtomDico const& _val) noexcept : val(_val) {}
            inline QuarkDico(FlipAtomDico::iterator first, FlipAtomDico::iterator last) noexcept : val(first, last) {}
            inline QuarkDico(FlipAtomDico&& _val) noexcept {swap(val, _val);}
            inline QuarkDico(initializer_list<pair<const sTag, FlipAtom>> il) noexcept : val(il) {}
            inline ~QuarkDico() noexcept {val.clear();}
            inline AtomType getType() const noexcept override {return DICO;}
            inline FlipAtomDico getDico() const noexcept override {return val;}
        };
        
        flip::Optional<Quark> m_quark;
        
        //! Retrieve the quark of the atom.
        inline Quark& getQuark() const noexcept { return m_quark.operator Quark &(); }
        
    public:
        
        //! flip static declare method
        static void declare()
        {
            // Quark declaration
            Model::declare<FlipAtom::Quark>()
            .name("cicm.kiwi.FlipAtom.Quark");
            
            // QuarkBool declaration
            Model::declare<FlipAtom::QuarkBool>()
            .name("cicm.kiwi.FlipAtom.QuarkBool")
            .inherit<FlipAtom::Quark>()
            .member<flip::Bool, &FlipAtom::QuarkBool::val>("value");
            
            // QuarkLong declaration
            Model::declare<FlipAtom::QuarkLong>()
            .name("cicm.kiwi.FlipAtom.QuarkLong")
            .inherit<FlipAtom::Quark>()
            .member<flip::Int, &FlipAtom::QuarkLong::val>("value");
            
            // QuarkDouble declaration
            Model::declare<FlipAtom::QuarkDouble>()
            .name("cicm.kiwi.FlipAtom.QuarkDouble")
            .inherit<FlipAtom::Quark>()
            .member<flip::Float, &FlipAtom::QuarkDouble::val>("value");
            
            // QuarkTag declaration
            Model::declare<FlipAtom::QuarkTag>()
            .name("cicm.kiwi.FlipAtom.QuarkTag")
            .inherit<FlipAtom::Quark>()
            .member<FlipTag, &FlipAtom::QuarkTag::val>("value");
            
            // FlipAtom declaration
            Model::declare<FlipAtom>()
            .name("cicm.kiwi.FlipAtom")
            .member<flip::Optional<Quark>, &FlipAtom::m_quark>("quark");
        }
        
        // ================================================================================ //
        //                                      ATOM                                        //
        // ================================================================================ //
        
        //! Flip Default Constructor.
        /** The function does nothing.
         */
        //inline FlipAtom(flip::Default&) noexcept {}
        
        //! Constructor.
        /** The function allocates an undefined atom.
         */
        inline FlipAtom() noexcept
        {
            //m_quark.reset<Quark>();
        }
        
        //! Constructor with another atom.
        /** The function allocates the atom with an atom.
         */
        FlipAtom(FlipAtom const& other) noexcept;
        
        //! Constructor with a boolean value.
        /** The function allocates the atom with a long value created with a boolean value.
         @param value The value.
         */
        inline FlipAtom(const bool value) noexcept
        {
            m_quark.reset<QuarkBool>(value);
        }
        
        //! Constructor with a long value.
        /** The function allocates the atom with a long value.
         @param value The value.
         */
        inline FlipAtom(const int value) noexcept
        {
            m_quark.reset<QuarkLong>(static_cast<long long>(value));
        }
        
        //! Constructor with a long value.
        /** The function allocates the atom with a long value.
         @param value The value.
         */
        inline FlipAtom(const long value) noexcept
        {
            m_quark.reset<QuarkLong>(static_cast<long long>(value));
        }
        
        //! Constructor with a long long value.
        /** The function allocates the atom with a long long value.
         @param value The value.
         */
        inline FlipAtom(const long long value) noexcept
        {
            m_quark.reset<QuarkLong>(value);
        }
        
        //! Constructor with a double value.
        /** The function allocates the atom with a double value.
         @param value The value.
         */
        inline FlipAtom(const float value) noexcept
        {
            m_quark.reset<QuarkDouble>(static_cast<double>(value));
        }
        
        //! Constructor with a double value.
        /** The function allocates the atom with a double value.
         @param value The value.
         */
        inline FlipAtom(const double value) noexcept
        {
            m_quark.reset<QuarkDouble>(value);
        }
        
        //! Constructor with a string.
        /** The function allocates the atom with a tag created with a string.
         @param tag The tag.
         */
        inline FlipAtom(const char* tag) noexcept
        {
            m_quark.reset<QuarkTag>(Tag::create(tag));
        }
        
        //! Constructor with a string.
        /** The function allocates the atom with a tag created with a string.
         @param tag The tag.
         */
        inline FlipAtom(string const& tag) noexcept
        {
            m_quark.reset<QuarkTag>(Tag::create(tag));
        }
        
        //! Constructor with a string.
        /** The function allocates the atom with a tag created with a string.
         @param tag The tag.
         */
        //inline FlipAtom(string&& tag) noexcept : m_quark(new QuarkTag(Tag::create(forward<string>(tag)))) {}
        
        //! Constructor with a tag.
        /** The function allocates the atom with a tag.
         */
        inline FlipAtom(const sTag tag) noexcept
        {
            m_quark.reset<QuarkTag>(tag);
        }
        
        //! Constructor with a vector of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline FlipAtom(FlipAtomVector const& atoms) noexcept
        {
            m_quark.reset<QuarkVector>(atoms);
        }
        
        //! Constructor with a vector of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        //inline FlipAtom(FlipAtomVector&& atoms) noexcept : m_quark(new QuarkVector(forward<FlipAtomVector>(atoms))) {}
        
        //! Constructor with a vector of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline FlipAtom(FlipAtomVector::iterator first, FlipAtomVector::iterator last) noexcept
        {
            m_quark.reset<QuarkVector>(first, last);
        }
        
        //! Constructor with a vector of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline FlipAtom(initializer_list<FlipAtom> il) noexcept
        {
            m_quark.reset<QuarkVector>(il);
        }
        
        //! Constructor with a map of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline FlipAtom(FlipAtomDico const& atoms) noexcept
        {
            m_quark.reset<QuarkDico>(atoms);
        }
        
        //! Constructor with a map of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        //inline FlipAtom(FlipAtomDico&& atoms) noexcept  : m_quark(new QuarkDico(forward<FlipAtomDico>(atoms))) {}
        
        //! Constructor with a map of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline FlipAtom(FlipAtomDico::iterator first, FlipAtomDico::iterator last) noexcept
        {
            m_quark.reset<QuarkDico>(first, last);
        }
        
        //! Constructor with a map of atoms.
        /** The function allocates the atom with a vector of atoms.
         */
        inline FlipAtom(initializer_list<pair<const sTag, FlipAtom>> il) noexcept
        {
            m_quark.reset<QuarkDico>(il);
        }
        
        //! Destructor.
        /** Doesn't perform anything.
         */
        inline ~FlipAtom() noexcept {}
        
        /*
        inline FlipAtom(Atom const& atom)
        {
            if(atom.isBool())
            {
                m_quark.reset<QuarkBool>(static_cast<bool>(atom));
            }
            else if(atom.isLong())
            {
                m_quark.reset<QuarkLong>(static_cast<long>(atom));
            }
            else if(atom.isDouble())
            {
                m_quark.reset<QuarkDouble>(static_cast<double>(atom));
            }
            else if(atom.isTag())
            {
                m_quark.reset<QuarkTag>(static_cast<sTag>(atom));
            }
            else if(atom.isVector())
            {
                int todo;
            }
            else if(atom.isDico())
            {
                ;
            }
            
            m_quark.reset();
        }
        */
        operator Atom() const
        {
            if(m_quark)
            {
                if(isBool())
                {
                    return Atom(getQuark().getBool());
                }
                else if(isLong())
                {
                    return Atom(getQuark().getLong());
                }
                else if(isDouble())
                {
                    return Atom(getQuark().getDouble());
                }
                else if(isTag())
                {
                    return Atom(getQuark().getTag());
                }
                else if(isVector())
                {
                    int todo;
                    //return Atom(getQuark().getVector());
                }
                else if(isDico())
                {
                    //return Atom(getQuark().getDico());
                }
            }
            
            return Atom();
        }
        
        
        //! Retrieve the type of the atom.
        /** The function retrieves the type of the atom.
         @return The type of the atom as a type.
         */
        inline AtomType getType() const noexcept
        {
            return m_quark.operator Quark &().getType();
        }
        
        //! Check if the atom is undefined.
        /** The function checks if the atom is undefined.
         @return    true if the atom is undefined.
         */
        inline bool isUndefined() const noexcept
        {
            return (m_quark) ? getQuark().isUndefined() : true;
        }
        
        //! Check if the atom is of type bool.
        /** The function checks if the atom is of type bool.
         @return    true if the atom is a bool.
         */
        inline bool isBool() const noexcept {return (m_quark) ? getQuark().isBool() : false;}
        
        //! Check if the atom is of type long.
        /** The function checks if the atom is of type long.
         @return    true if the atom is a long.
         */
        inline bool isLong() const noexcept {return (m_quark) ? getQuark().isLong() : false;}
        
        //! Check if the atom is of type double.
        /** The function checks if the atom is of type double.
         @return    true if the atom is a double.
         */
        inline bool isDouble() const noexcept {return (m_quark) ? getQuark().isDouble() : false;}
        
        //! Checks if the atom is of type long or double.
        /** The function checks if the atom is of type long or double.
         @return    true if the atom is a long or a double.
         */
        inline bool isNumber() const noexcept {return (m_quark) ? getQuark().isNumber() : false;}
        
        //! Check if the atom is of type tag.
        /** The function checks if the atom is of type tag.
         @return    true if the atom is a tag.
         */
        inline bool isTag() const noexcept {return (m_quark) ? getQuark().isTag() : false;}
        
        //! Check if the atom is of type vector.
        /** The function checks if the atom is of type vector.
         @return    true if the atom is a vector.
         */
        inline bool isVector() const noexcept {return (m_quark) ? getQuark().isVector() : false;}
        
        //! Check if the atom is of type map.
        /** The function checks if the atom is of type map.
         @return    true if the atom is a map.
         */
        inline bool isDico() const noexcept {return (m_quark) ? getQuark().isDico() : false;}
        
        //! Cast the atom to a boolean.
        /** The function casts the atom to a boolean.
         @return An boolean value if the atom is a digit otherwise 0.
         */
        inline operator bool() const noexcept {return (m_quark) ? getQuark().getBool() : false;}
        
        //! Cast the atom to an int.
        /** The function casts the atom to an int.
         @return An int value if the atom is a digit otherwise 0.
         */
        inline operator int() const noexcept {return (m_quark) ? int(getQuark().getLong()) : 0;}
        
        //! Cast the atom to a long.
        /** The function casts the atom to a long.
         @return A long value if the atom is a digit otherwise 0.
         */
        inline operator long() const noexcept {return (m_quark) ? getQuark().getLong() : 0;}
        
        //! Cast the atom to a long long.
        /** The function casts the atom to a long long.
         @return A long value if the atom is a digit otherwise 0.
         */
        inline operator long long() const noexcept {return (m_quark) ? getQuark().getLong() : 0ll;}
        
        //! Cast the atom to a long.
        /** The function casts the atom to a long.
         @return A long value if the atom is a digit otherwise 0.
         */
        inline operator ulong() const noexcept {return (m_quark) ? ulong(getQuark().getLong()) : 0ul;}
        
        //! Cast the atom to a float.
        /** The function casts the atom to a float.
         @return A float value if the atom is a digit otherwise 0.
         */
        inline operator float() const noexcept {return (m_quark) ? float(getQuark().getDouble()) : 0.f;}
        
        //! Cast the atom to a double.
        /** The function casts the atom to a double.
         @return A double value if the atom is a digit otherwise 0.
         */
        inline operator double() const noexcept {return (m_quark) ? getQuark().getDouble() : 0.;}
        
        //! Cast the atom to a tag.
        /** The function casts the atom to a tag.
         @return A tag if the atom is a tag otherwise a nullptr.
         */
        inline operator sTag() const noexcept {return (m_quark) ? getQuark().getTag() : Tags::_empty;}
        
        //! Cast the atom to a vector of atoms.
        /** The function casts the atom to a vector of atoms.
         @return A vector of atoms.
         */
        inline operator FlipAtomVector() const noexcept {return (m_quark) ? getQuark().getVector() : FlipAtomVector();}
        
        //! Cast the atom to a map of atoms.
        /** The function casts the atom to a map of atoms.
         @return A map of atoms.
         */
        inline operator FlipAtomDico() const noexcept {return (m_quark) ? getQuark().getDico() : FlipAtomDico();}
        
        //! Set up the atom with another atom.
        /** The function sets up the atom with another atom.
         @param other   The other atom.
         @return An atom.
         */
        FlipAtom& operator=(FlipAtom const& other) noexcept;
        
        //! Set up the atom with another atom.
        /** The function sets up the atom with another atom.
         @param other   The other atom.
         @return An atom.
         */
        /*
        FlipAtom& operator=(FlipAtom&& other) noexcept
        {
            swap(m_quark, other.m_quark);
            return *this;
        }
        */
        
        //! Set up the atom with a boolean value.
        /** The function sets up the atom with a long value created with aboolean value.
         @param value   The boolean value.
         @return An atom.
         */
        inline FlipAtom& operator=(const bool value) noexcept
        {
            m_quark.reset<QuarkBool>(value);
            return *this;
        }
        
        //! Set up the atom with a long value.
        /** The function sets up the atom with a long value.
         @param value   The long value.
         @return An atom.
         */
        inline FlipAtom& operator=(const int value) noexcept
        {
            m_quark.reset<QuarkLong>(static_cast<long long>(value));
            return *this;
        }
        
        //! Set up the atom with a long value.
        /** The function sets up the atom with a long value.
         @param value   The long value.
         @return An atom.
         */
        inline FlipAtom& operator=(const long value) noexcept
        {
            m_quark.reset<QuarkLong>(value);
            return *this;
        }
        
        //! Set up the atom with a double value.
        /** The function sets up the atom with a double value.
         @param value   The double value.
         @return An atom.
         */
        inline FlipAtom& operator=(const float value) noexcept
        {
            m_quark.reset<QuarkDouble>((float)value);
            return *this;
        }
        
        //! Set up the atom with a double value.
        /** The function sets up the atom with a double value.
         @param value   The double value.
         @return An atom.
         */
        inline FlipAtom& operator=(const double value) noexcept
        {
            m_quark.reset<QuarkDouble>(value);
            return *this;
        }
        
        //! Set up the atom with a string.
        /** The function sets up the atom with string.
         @param tag   The string.
         @return An atom.
         */
        inline FlipAtom& operator=(char const* tag)
        {
            m_quark.reset<QuarkTag>(Tag::create(tag));
            return *this;
        }
        
        //! Set up the atom with a string.
        /** The function sets up the atom with string.
         @param tag   The string.
         @return An atom.
         */
        inline FlipAtom& operator=(string const& tag)
        {
            m_quark.reset<QuarkTag>(Tag::create(tag));
            return *this;
        }
        
        //! Set up the atom with a string.
        /** The function sets up the atom with string.
         @param tag   The string.
         @return An atom.
         */
        inline FlipAtom& operator=(string&& tag) noexcept
        {
            m_quark.reset<QuarkTag>(Tag::create(forward<string>(tag)));
            return *this;
        }
        
        //! Set up the atom with a tag.
        /** The function sets up the atom with a tag.
         @param tag   The tag.
         @return An atom.
         */
        inline FlipAtom& operator=(sTag tag) noexcept
        {
            m_quark.reset<QuarkTag>(tag);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline FlipAtom& operator=(FlipAtomVector const& atoms) noexcept
        {
            m_quark.reset<QuarkVector>(atoms);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline FlipAtom& operator=(FlipAtomVector&& atoms) noexcept
        {
            m_quark.reset<QuarkVector>(forward<FlipAtomVector>(atoms));
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline FlipAtom& operator=(initializer_list<FlipAtom> il) noexcept
        {
            m_quark.reset<QuarkVector>(il);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline FlipAtom& operator=(FlipAtomDico const& atoms) noexcept
        {
            m_quark.reset<QuarkDico>(atoms);
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline FlipAtom& operator=(FlipAtomDico&& atoms) noexcept
        {
            m_quark.reset<QuarkDico>(forward<FlipAtomDico>(atoms));
            return *this;
        }
        
        //! Set up the atom with a vector of atoms.
        /** The function sets up the atom with a vector of atoms.
         @param atoms   The vector of atoms.
         @return An atom.
         */
        inline FlipAtom& operator=(initializer_list<pair<const sTag, FlipAtom>> il) noexcept
        {
            m_quark.reset<QuarkDico>(il);
            return *this;
        }
        
        //! Compare the atom with another.
        /** The function compares the atom with another.
         @param other The other atom.
         @return true if the atoms hold the same value otherwise false.
         */
        inline bool operator==(FlipAtom const& other) const noexcept
        {
            if(!(other.m_quark) || !(m_quark))
            {
                return false;
            }
            else if(other.isUndefined() && isUndefined())
            {
                return true;
            }
            else if(other.isBool() && isNumber())
            {
                return getQuark().getBool() == other.getQuark().getBool();
            }
            else if(other.isLong() && isNumber())
            {
                return getQuark().getLong() == other.getQuark().getLong();
            }
            else if(other.isDouble() && isNumber())
            {
                return getQuark().getDouble() == other.getQuark().getDouble();
            }
            else if(other.isTag() && isTag())
            {
                return getQuark().getTag() == other.getQuark().getTag();
            }
            else if(other.isVector() && isVector())
            {
                return getQuark().getVector() == other.getQuark().getVector();
            }
            else if(other.isDico() && isDico())
            {
                return getQuark().getDico() == other.getQuark().getDico();
            }
            
            return false;
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
                return getQuark().getBool() == value;
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
                return getQuark().getLong() == (long)value;
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
                return getQuark().getLong() == value;
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
                return getQuark().getDouble() == (double)value;
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
                return getQuark().getDouble() == value;
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
        inline bool operator==(FlipAtomVector const& vector) const noexcept
        {
            if(isVector())
            {
                return getQuark().getVector() == vector;
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
        inline bool operator==(FlipAtomDico const& dico) const noexcept
        {
            if(isDico())
            {
                return getQuark().getDico() == dico;
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
        inline bool operator!=(const FlipAtom& other) const noexcept
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
        inline bool operator!=(FlipAtomVector const& vector) const noexcept
        {
            return !(*this == vector);
        }
        
        //! Compare the atom with a dico.
        /** The function compares the atom with a dico.
         @param dico   The dico.
         @return true if the atom differ from the dico otherwise false.
         */
        inline bool operator!=(FlipAtomDico const& dico) const noexcept
        {
            return !(*this == dico);
        }
        
        static ostream& toJson(ostream &output, const FlipAtom &atom, ulong& indent);
        
        //! Parse a string into a vector of atoms.
        /** Parse a string into a vector of atoms.
         @param     text	The string to parse.
         @return    The vector of atoms.
         @remark    For example, the string : "foo \"bar 42\" 1 2 3.14" will parsed into a vector of 5 atoms.
         The atom types will be determined automatically as 2 #FlipAtom::Type::TAG atoms, 2 #FlipAtom::Type::LONG atoms,
         and 1 #FlipAtom::Type::DOUBLE atom.
         */
        static FlipAtomVector parse(string const& text);
    };
    
    ostream& operator<<(ostream &output, const FlipAtom &atom);
}



#endif


