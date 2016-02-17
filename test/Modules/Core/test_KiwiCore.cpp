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

#include "../../../Modules/KiwiCore/KiwiAtom.h"

#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"

using namespace kiwi;

// ================================================================================ //
//                                      ATOM                                        //
// ================================================================================ //

TEST_CASE("Atom Constructors", "[Atom]")
{
    SECTION("temp")
    {
//        std::cout << "max<long long>: " << std::numeric_limits<long long>::max() << std::endl;
//        
//        std::cout << "max<unsigned long>: " << std::numeric_limits<unsigned long>::max() << std::endl;
//        
//        std::cout << "max<unsigned long long>: " << std::numeric_limits<unsigned long long>::max() << std::endl;
        /*
        std::cout << std::endl;
        std::cout << "is_integral: " << std::endl;
        std::cout << "bool: " << std::is_integral<bool>::value << std::endl;
        std::cout << "int: " << std::is_integral<int>::value << std::endl;
        std::cout << "unsigned int: " << std::is_integral<unsigned int>::value << std::endl;
        std::cout << "unsigned long: " << std::is_integral<unsigned long>::value << std::endl;
        std::cout << "double: " << std::is_integral<double>::value << std::endl;
        std::cout << "float: " << std::is_integral<float>::value << std::endl;
        
        std::cout << "is_signed: " << std::endl;
        std::cout << "long: " << std::numeric_limits<long>::is_signed << std::endl;
        std::cout << "unsigned long: " << std::numeric_limits<unsigned long>::is_signed << std::endl;
        std::cout << std::endl;
        */
        
        //std::cout << "bool: " << Atom::test(true) << std::endl;
        //std::cout << "int: " << Atom::test<int>(1) << std::endl;
    }
    
    SECTION("Null type")
    {
        Atom a;
        CHECK(a.getType() == Atom::Type::Null);
        CHECK(a.isNull());
        CHECK(Atom().getType() == Atom::Type::Null);
        CHECK(Atom(INFINITY).getType() == Atom::Type::Null);    // infinite produces Null
        CHECK(Atom(NAN).getType() == Atom::Type::Null);         // NaN produces Null
    }
    
    SECTION("Boolean types")
    {
        CHECK(Atom(true).getType() == Atom::Type::Boolean);
        CHECK(Atom(false).getType() == Atom::Type::Boolean);
    }
    
    SECTION("Signed Integral types")
    {
        CHECK(Atom(short(1)).getType() == Atom::Type::Int);
        CHECK(Atom(1).getType() == Atom::Type::Int);
        CHECK(Atom(1l).getType() == Atom::Type::Int);
        CHECK(Atom(1ll).getType() == Atom::Type::Int);
        CHECK(Atom(0xFFFFFF).getType() == Atom::Type::Int);  // hexadecimal
        CHECK(Atom(0113).getType() == Atom::Type::Int);      // octal
        
        //Atom::test(short(1));
        //Atom::test(1);
        //Atom::test(1l);
    }
    
    SECTION("Unsigned Integral types are unsupported")
    {
        /*
        CHECK(Atom(1u).getType() == Atom::Type::Int);        // unsigned (int)
        CHECK(Atom(1ul).getType() == Atom::Type::Int);       // unsigned long
        CHECK(Atom(1lu).getType() == Atom::Type::Int);       // unsigned long
        CHECK(Atom(1ull).getType() == Atom::Type::Int);      // unsigned long long
        CHECK(Atom(1llu).getType() == Atom::Type::Int);      // unsigned long long
        
        //int64_t i64 = std::numeric_limits<uint64_t>::max();
        
        //uint64_t i64 = std::numeric_limits<uint64_t>::max();
        
        Atom a_unsigned(42ull);
        int64_t a_i = a_unsigned;
        CHECK(a_i == 42ll);
        
        Atom b_unsigned(std::numeric_limits<uint64_t>::max());
        Atom bbb = Atom(std::numeric_limits<uint64_t>::max());
        int64_t b_i = b_unsigned;
        CHECK(b_i == std::numeric_limits<uint64_t>::max());
        */
    }
    
    SECTION("Floating-Point types")
    {
        CHECK(Atom(3.14f).getType() == Atom::Type::Float);    // float
        CHECK(Atom(3.14).getType() == Atom::Type::Float);     // double
        //CHECK(Atom(3.14l).getType() == Atom::Type::Float);  // long double not supported
        CHECK(Atom(6.02e23).getType() == Atom::Type::Float);  // 6.02 x 10^23 (Avogadro constant)
        CHECK(Atom(1.6e-19).getType() == Atom::Type::Float);  // 1.6 x 10^-19 (electric charge of an electron)
        
        // check for infinity and NaN
        CHECK_FALSE(Atom(INFINITY).getType() == Atom::Type::Float);    // infinite produces Null
        CHECK_FALSE(Atom(NAN).getType() == Atom::Type::Float);         // NaN produces Null
    }
    
    SECTION("Tag types")
    {
        CHECK(Atom("c").getType() == Atom::Type::Tag);
        CHECK(Atom(Tag::create("tag")).getType() == Atom::Type::Tag);
        sTag tag = Tag::create("tag");
        CHECK(Atom(tag).getType() == Atom::Type::Tag);
        
        //WARN("Atom('c').getType() == Atom::Type::Tag");
        //CHECK(Atom('c').getType() == Atom::Type::Tag);
        //CHECK_FALSE(Atom('c').getType() == Atom::Type::Int);       //
    }
}

TEST_CASE("Atom Null", "[Atom]")
{
    Atom atom;
    REQUIRE(atom.getType()          == Atom::Type::Null);
    REQUIRE(atom.isNull()      == true);
    REQUIRE(atom.isBool()           == false);
    REQUIRE(atom.isNumber()         == false);
    REQUIRE(atom.isInt()           == false);
    REQUIRE(atom.isFloat()         == false);
    REQUIRE(atom.isTag()            == false);
    REQUIRE(atom.isVector()         == false);
    REQUIRE(atom.isDico()           == false);
    
    REQUIRE_FALSE(atom == false);
    REQUIRE_FALSE(atom == true);
    
    Atom a1 = true;
    Atom a2 = 1;
    REQUIRE(a1 == a2); // ok ?
}

TEST_CASE("Atom Boolean", "[Atom]")
{
    Atom atom(true);
    CHECK(atom.getType()        == Atom::Type::Boolean);
    CHECK(atom.isNull()         == false);
    CHECK(atom.isBool()         == true);
    
    // for now a bool type Atom is a number
    CHECK(atom.isNumber()       == true);
    CHECK(atom.isInt()          == false);
    CHECK(atom.isFloat()        == false);
    CHECK(atom.isTag()          == false);
    CHECK(atom.isVector()       == false);
    CHECK(atom.isDico()         == false);
    CHECK(atom == true);
    
    REQUIRE(Atom(false) == Atom(false));
    REQUIRE(Atom(true) == Atom(true));
}

TEST_CASE("Atom Int", "[Atom]")
{
    Atom atom(1);
    CHECK(atom.getType()          == Atom::Type::Int);
    CHECK(atom.isNull()      == false);
    
    // Atom(1) is not a bool (?)
    CHECK(atom.isBool()           == false);
    CHECK(atom.isNumber()         == true);
    CHECK(atom.isInt()           == true);
    CHECK(atom.isFloat()         == false);
    CHECK(atom.isTag()            == false);
    CHECK(atom.isVector()         == false);
    CHECK(atom.isDico()           == false);
    CHECK(atom == true);
    CHECK(atom == 1);
    CHECK(atom == 1.);
    
    CHECK(Atom(std::numeric_limits<int64_t>::max()) == std::numeric_limits<int64_t>::max());
    //CHECK(Atom(std::numeric_limits<uint64_t>::max()) == std::numeric_limits<uint64_t>::max());
    
    // do we need to support all equality operators ?
    //CHECK(atom == 1ul);
    
    Atom atom2{1}; // call initializer_list ctor => VECTOR type
    CHECK(atom2.getType()         == Atom::Type::Vector);
    
    Atom to_move(42);
    Atom moved(std::move(to_move));
    CHECK(to_move.isNull()          == true);
    CHECK(moved.isNumber()         == true);
    CHECK(moved.isInt()           == true);
    CHECK(moved == 42);
}

TEST_CASE("Atom Float", "[Atom]")
{
    Atom atom(1.123);
    CHECK(atom.getType()          == Atom::Type::Float);
    CHECK(atom.isNull()           == false);
    CHECK(atom.isBool()           == false);
    CHECK(atom.isNumber()         == true);
    CHECK(atom.isInt()            == false);
    CHECK(atom.isFloat()          == true);
    CHECK(atom.isTag()            == false);
    CHECK(atom.isVector()         == false);
    CHECK(atom.isDico()           == false);
    CHECK(atom == 1.123);
    CHECK(atom == Approx(1.123f));
    // Should this fail ??
    CHECK(atom == 1);
    CHECK(atom == true);
    
    CHECK_FALSE(Atom(1.123) == 3.14);
    
    Atom atom2{1.}; // call initializer_list ctor => VECTOR type
    CHECK(atom2.getType()         == Atom::Type::Vector);
    
    Atom to_move(42.42);
    Atom moved(std::move(to_move));
    CHECK(to_move.isNull()    == true);
    CHECK(moved.isNumber()         == true);
    CHECK(moved.isFloat()         == true);
    CHECK(moved == 42.42);
}

TEST_CASE("Atom Tag", "[Atom]")
{
    Atom atom("foo");
    CHECK(atom.getType()          == Atom::Type::Tag);
    CHECK(atom.isNull()      == false);
    CHECK(atom.isBool()           == false);
    CHECK(atom.isNumber()         == false);
    CHECK(atom.isInt()           == false);
    CHECK(atom.isFloat()         == false);
    CHECK(atom.isTag()            == true);
    CHECK(atom.isVector()         == false);
    CHECK(atom.isDico()           == false);
    
    CHECK(atom == Atom("foo"));
    CHECK(atom != Atom("jojo"));
    
    Atom atom2{"foo"}; // call initializer_list ctor => VECTOR type
    CHECK(atom2.getType()         == Atom::Type::Vector);
    
    Atom to_move("foo");
    Atom moved(std::move(to_move));
    CHECK(to_move.isNull()   == true);
    CHECK(moved.isTag()           == true);
}

TEST_CASE("Atom Vector", "[Atom]")
{
    SECTION("Empty Atom vector")
    {
        Vector empty_vec;
        Atom atom(empty_vec);
        CHECK(atom.isNull()      == false);
        CHECK(atom.isVector()    == true);
        
        // It would be nice to simply have to do :
        //Atom atom({}); // implicit empty vector
        //CHECK(atom.isNull()      == false); // true
        //CHECK(atom.isVector()    == true);  // false
    }
    
    SECTION("One element Atom vector")
    {
        Atom atom_1{1};
        CHECK(atom_1.isVector()         == true);
        Vector atom_vec = atom_1;
        REQUIRE(atom_vec.size() == 1);
        CHECK(atom_vec[0].isInt()       == true);
        CHECK(atom_vec[0] == 1);
    }
    
    SECTION("Multiple elements Atom vector")
    {
        Atom atom_2{true, 42, 3.14, "tag"};
        CHECK(atom_2.isVector()         == true);
        Vector atom_2_vec = atom_2;
        REQUIRE(atom_2_vec.size() == 4);
        CHECK(atom_2_vec[0].getType()   == Atom::Type::Boolean);
        CHECK(atom_2_vec[0].isBool()    == true);
        CHECK(atom_2_vec[0] == true);
        CHECK(atom_2_vec[1].isInt()     == true);
        CHECK(atom_2_vec[1] == 42);
        CHECK(atom_2_vec[2].isFloat()   == true);
        CHECK(atom_2_vec[2] == 3.14);
        CHECK(atom_2_vec[3].isTag()     == true);
        CHECK(atom_2_vec[3] == Atom("tag"));
        
        Atom a = {true};
        Vector a_vec = a;
        REQUIRE(a_vec.size() == 1);
        CHECK(atom_2_vec[0].isBool()     == true);
    }
    
    SECTION("Vector of vectors")
    {
        Atom atom_2{true, 42, 3.14, "tag"};
        Vector atom_2_vec = atom_2;
        
        // Vector of vectors
        Atom atom_4
        {
            "foo",
            { "bar" },
            { 1, true },
            atom_2,
            { atom_2 },
            { atom_2, 3.14 }
        };
        
        REQUIRE(atom_4.isVector()       == true);
        Vector atom_4_vec = atom_4;
        REQUIRE(atom_4_vec.size() == 6);
        CHECK(atom_4_vec[0].isTag()     == true);
        REQUIRE(atom_4_vec[1].isVector() == true);
        REQUIRE(atom_4_vec[2].isVector() == true);
        REQUIRE(atom_4_vec[3].isVector() == true);
        REQUIRE(atom_4_vec[4].isVector() == true);
        REQUIRE(atom_4_vec[5].isVector() == true);
        
        Vector atom_4_vec_1 = atom_4_vec[1];
        REQUIRE(atom_4_vec_1.size() == 1);
        CHECK(atom_4_vec_1[0].isTag() == true);
        
        Vector atom_4_vec_2 = atom_4_vec[2];
        REQUIRE(atom_4_vec_2.size() == 2);
        CHECK(atom_4_vec_2[0].isInt() == true);
        CHECK(atom_4_vec_2[1].isBool() == true);
        
        Vector atom_4_vec_3 = atom_4_vec[3];
        REQUIRE(atom_4_vec_3.size() == atom_2_vec.size());
        
        // Do we want :
        Vector atom_4_vec_4 = atom_4_vec[4];
        // @todo important : resolve this issue
        // clang : (atom_4_vec_4.size() == 1) and not 4 !
        //REQUIRE(atom_4_vec_4.size() == atom_2_vec.size());
        
        // or do we want : ?
        //REQUIRE(atom_4_vec_4.size() == 1);
        //Vector atom_4_vec_4_vec = atom_4_vec_4;
        //REQUIRE(atom_4_vec_4_vec.size() == atom_2_vec.size());
        
        Vector atom_4_vec_5 = atom_4_vec[5];
        REQUIRE(atom_4_vec_5.size() == 2);
        REQUIRE(atom_4_vec_5[0].isVector() == true);
        REQUIRE(atom_4_vec_5[1].isFloat() == true);
        Vector atom_4_vec_5_vec = atom_4_vec_5[0];
        REQUIRE(atom_4_vec_5_vec.size() == atom_2_vec.size());
    }
    
    SECTION("Vector equality operators")
    {
        // check atoms equality
        Atom eq_atom_1{22, "foo", 3.14};
        Atom eq_atom_2{22, "bar", 3.14};
        Atom eq_atom_3{22, "foo", 3.14, "bar"};
        
        // deep comparison equality operator
        CHECK(eq_atom_1 == eq_atom_1);
        CHECK_FALSE(eq_atom_1 == eq_atom_2);
        CHECK_FALSE(eq_atom_1 == eq_atom_3);
        
        Atom::vector_t eq_atom_1_vec = eq_atom_1;
        eq_atom_1_vec[1] = "bar";
        CHECK_FALSE(eq_atom_1 == eq_atom_2);
        eq_atom_1 = eq_atom_1_vec;
        CHECK(eq_atom_1 == eq_atom_2);
    }
    
    SECTION("operator []")
    {
        SECTION("On Atom of type vector_t")
        {
            // Read
            Atom vec_1{66, 33.3333, "foo", true};
            REQUIRE(vec_1.isVector());
            CHECK(vec_1[0].isInt());
            CHECK(vec_1[0] == 66);
            CHECK(vec_1[1].isFloat());
            CHECK(vec_1[1] == 33.3333);
            CHECK(vec_1[2].isTag());
            CHECK(vec_1[2] == Tag::create("foo"));
            CHECK(vec_1[3].isBool());
            CHECK(vec_1[3] == true);
            
            // Write
            vec_1[0] = 3.14;
            CHECK(vec_1[0] == 3.14);
            
            // Vector of vector access
            Atom vec_2{66, {55, 22.2222, true}, 33.3333, "foo", true};
            CHECK(vec_2[1][1] == 22.2222);
        }
        
        SECTION("Atom type Null")
        {
            Atom a_null;
            REQUIRE(a_null.isNull());
            a_null[0] = 12;
            CHECK(!a_null.isNull());
            REQUIRE(a_null.isVector());
            CHECK(a_null[0].isInt());
            CHECK(a_null[0] == 12);
        }
        
        SECTION("Bad Atom type")
        {
            Atom a_bool(true);
            REQUIRE(!a_bool.isNull());
            REQUIRE(!a_bool.isVector());
            REQUIRE(a_bool.isBool());
            REQUIRE_THROWS(a_bool[0] = false);
            
            Atom a_int(42);
            REQUIRE(!a_int.isNull());
            REQUIRE(!a_int.isVector());
            REQUIRE(a_int.isInt());
            REQUIRE_THROWS(a_int[0] = false);
            
            Atom a_float(3.14);
            REQUIRE(!a_float.isNull());
            REQUIRE(!a_float.isVector());
            REQUIRE(a_float.isFloat());
            REQUIRE_THROWS(a_float[0] = false);
            
            Atom a_tag("tag");
            REQUIRE(!a_tag.isNull());
            REQUIRE(!a_tag.isVector());
            REQUIRE(a_tag.isTag());
            REQUIRE_THROWS(a_tag[0] = false);
            
            Atom a_dico((Atom::dico_t()));
            REQUIRE(!a_dico.isNull());
            REQUIRE(!a_dico.isVector());
            REQUIRE(a_dico.isDico());
            REQUIRE_THROWS(a_dico[0] = false);
        }
        
        SECTION("Bad Index must return an Atom Null")
        {
            Atom a;
            REQUIRE(a.isNull());
            REQUIRE(a[10].isNull());
            REQUIRE(a.isVector());
            
            Atom b{3.14, 42, true};
            REQUIRE(b.isVector());
            REQUIRE(b[10].isNull());
        }
    }
}

TEST_CASE("Atom Dico", "[Atom]")
{
    SECTION("Constructor")
    {
        Atom atom_vector{true, 1, 3.14, "tag"};
        
        Dico temp_dico;
        temp_dico[Tag::create("long")] = 12ll;
        temp_dico[Tag::create("object")] = atom_vector;
        
        Atom atom_dico(temp_dico);
        REQUIRE(atom_dico.isDico()         == true);
        Dico dico = atom_dico;
        REQUIRE(dico.size()                == 2);
        REQUIRE(dico[Tag::create("long")].isInt() == true);
        REQUIRE(dico[Tag::create("object")].isVector() == true);
        
        //std::cout << atom_dico << '\n';
    }
    
    SECTION("operator []")
    {
        Atom atom;
        auto key = Tag::create("jojo");
        atom[key] = 24;
        
        CHECK(atom[key].isInt());
        CHECK(atom[key] == 24);
        
        atom[key] = 3.14;
        
        CHECK(atom[key] == 3.14);
    }
}

TEST_CASE("Atom string parser", "[Atom]")
{
    std::string message = "foo \"bar 42\" 1 2 3.14";
    Atom atom = Atom::parse(message);
    
    REQUIRE(atom.isVector() == true);
    
    Vector atom_vec = atom;
    REQUIRE(atom_vec.size() == 5);
    CHECK(atom_vec[0].isTag());
    CHECK(atom_vec[1].isTag());
    CHECK(atom_vec[2].isInt());
    CHECK(atom_vec[3].isInt());
    CHECK(atom_vec[4].isFloat());
}


