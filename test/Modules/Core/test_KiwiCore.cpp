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

#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"

#include "../../../Modules/KiwiCore/KiwiAtom.h"

using namespace kiwi;

// ================================================================================ //
//                                      ATOM                                        //
// ================================================================================ //

TEST_CASE("Atom Undefined", "[Atom]")
{
    Atom atom;
    REQUIRE(atom.getType()          == Atom::UNDEFINED);
    REQUIRE(atom.isUndefined()      == true);
    REQUIRE(atom.isBool()           == false);
    REQUIRE(atom.isNumber()         == false);
    REQUIRE(atom.isLong()           == false);
    REQUIRE(atom.isDouble()         == false);
    REQUIRE(atom.isTag()            == false);
    REQUIRE(atom.isVector()         == false);
    REQUIRE(atom.isDico()           == false);
    
    REQUIRE_FALSE(atom == false);
    REQUIRE_FALSE(atom == true);
    REQUIRE(Atom(false) == Atom(false));
    REQUIRE(Atom(true) == Atom(true));
    
    Atom a1 = true;
    Atom a2 = 1;
    REQUIRE(a1 == a2); // ok ?
}

TEST_CASE("Atom Boolean", "[Atom]")
{
    Atom atom(true);
    CHECK(atom.getType()          == Atom::BOOLEAN);
    CHECK(atom.isUndefined()      == false);
    CHECK(atom.isBool()           == true);
    
    // for now a bool type Atom is a number
    CHECK(atom.isNumber()         == true);
    CHECK(atom.isLong()           == false);
    CHECK(atom.isDouble()         == false);
    CHECK(atom.isTag()            == false);
    CHECK(atom.isVector()         == false);
    CHECK(atom.isDico()           == false);
    CHECK(atom == true);
}

TEST_CASE("Atom Long", "[Atom]")
{
    Atom atom(1);
    CHECK(atom.getType()          == Atom::LONG);
    CHECK(atom.isUndefined()      == false);
    
    // Atom(1) is not a bool (?)
    CHECK(atom.isBool()           == false);
    CHECK(atom.isNumber()         == true);
    CHECK(atom.isLong()           == true);
    CHECK(atom.isDouble()         == false);
    CHECK(atom.isTag()            == false);
    CHECK(atom.isVector()         == false);
    CHECK(atom.isDico()           == false);
    CHECK(atom == true);
    CHECK(atom == 1);
    CHECK(atom == 1.);
    
    // do we need to support all equality operators ?
    //CHECK(atom == 1ul);
    
    Atom atom2{1}; // call initializer_list ctor => VECTOR type
    CHECK(atom2.getType()         == Atom::VECTOR);
    
    Atom to_move(42);
    Atom moved(std::move(to_move));
    CHECK(to_move.isUndefined()    == true);
    CHECK(moved.isNumber()         == true);
    CHECK(moved.isLong()           == true);
    CHECK(moved == 42);
}

TEST_CASE("Atom Double", "[Atom]")
{
    Atom atom(1.);
    CHECK(atom.getType()          == Atom::DOUBLE);
    CHECK(atom.isUndefined()      == false);
    CHECK(atom.isBool()           == false);
    CHECK(atom.isNumber()         == true);
    CHECK(atom.isLong()           == false);
    CHECK(atom.isDouble()         == true);
    CHECK(atom.isTag()            == false);
    CHECK(atom.isVector()         == false);
    CHECK(atom.isDico()           == false);
    CHECK(atom == 1.);
    CHECK(atom == 1.f);
    CHECK(atom == 1);
    CHECK(atom == true);
    
    Atom atom2{1.}; // call initializer_list ctor => VECTOR type
    CHECK(atom2.getType()         == Atom::VECTOR);
    
    Atom to_move(42.42);
    Atom moved(std::move(to_move));
    CHECK(to_move.isUndefined()    == true);
    CHECK(moved.isNumber()         == true);
    CHECK(moved.isDouble()         == true);
    CHECK(moved == 42.42);
}

TEST_CASE("Atom Tag", "[Atom]")
{
    Atom atom("foo");
    CHECK(atom.getType()          == Atom::TAG);
    CHECK(atom.isUndefined()      == false);
    CHECK(atom.isBool()           == false);
    CHECK(atom.isNumber()         == false);
    CHECK(atom.isLong()           == false);
    CHECK(atom.isDouble()         == false);
    CHECK(atom.isTag()            == true);
    CHECK(atom.isVector()         == false);
    CHECK(atom.isDico()           == false);
    
    CHECK(atom == Atom("foo"));
    CHECK(atom != Atom("jojo"));
    
    Atom atom2{"foo"}; // call initializer_list ctor => VECTOR type
    CHECK(atom2.getType()         == Atom::VECTOR);
    
    Atom to_move("foo");
    Atom moved(std::move(to_move));
    CHECK(to_move.isUndefined()   == true);
    CHECK(moved.isTag()           == true);
}

TEST_CASE("Atom Vector", "[Atom]")
{
    // Empty atom vector
    Vector empty_vec;
    Atom atom(empty_vec);
    CHECK(atom.isUndefined()      == false);
    CHECK(atom.isVector()         == true);
    
    // It would be nice to simply have to do :
    //Atom atom({}); // implicit empty vector
    //CHECK(atom.isUndefined()      == false); // true
    //CHECK(atom.isVector()         == true);  // false
    
    // One element atom vector
    Atom atom_1{1};
    CHECK(atom_1.isVector()         == true);
    Vector atom_vec = atom_1;
    REQUIRE(atom_vec.size() == 1);
    CHECK(atom_vec[0].isLong()      == true);
    CHECK(atom_vec[0] == 1);
    
    // Four elements atom vector
    Atom atom_2{true, 1, 3.14, "tag"};
    CHECK(atom_2.isVector()         == true);
    Vector atom_2_vec = atom_2;
    REQUIRE(atom_2_vec.size() == 4);
    CHECK(atom_2_vec[0].isBool()     == true);
    CHECK(atom_2_vec[0] == true);
    CHECK(atom_2_vec[1].isLong()     == true);
    CHECK(atom_2_vec[1] == 1);
    CHECK(atom_2_vec[2].isDouble()   == true);
    CHECK(atom_2_vec[2] == 3.14);
    CHECK(atom_2_vec[3].isTag()      == true);
    CHECK(atom_2_vec[3] == Atom("tag"));
    
    // Do we want to support direct vector access (?)
    //CHECK(atom_2.isVector() == true);
    //CHECK(atom_2.getSize() == 4);
    //CHECK(atom_2[0].isBool() == true);
    
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
    CHECK(atom_4_vec_2[0].isLong() == true);
    CHECK(atom_4_vec_2[1].isBool() == true);
    
    Vector atom_4_vec_3 = atom_4_vec[3];
    REQUIRE(atom_4_vec_3.size() == atom_2_vec.size());
    
    // Do we want :
    Vector atom_4_vec_4 = atom_4_vec[4];
    REQUIRE(atom_4_vec_4.size() == atom_2_vec.size());
    // or do we want : ?
    //REQUIRE(atom_4_vec_4.size() == 1);
    //Vector atom_4_vec_4_vec = atom_4_vec_4;
    //REQUIRE(atom_4_vec_4_vec.size() == atom_2_vec.size());
    
    Vector atom_4_vec_5 = atom_4_vec[5];
    REQUIRE(atom_4_vec_5.size() == 2);
    REQUIRE(atom_4_vec_5[0].isVector() == true);
    REQUIRE(atom_4_vec_5[1].isDouble() == true);
    Vector atom_4_vec_5_vec = atom_4_vec_5[0];
    REQUIRE(atom_4_vec_5_vec.size() == atom_2_vec.size());
    
    // check atoms equality
    Atom eq_atom_1{22, "foo", 3.14};
    Atom eq_atom_2{22, "bar", 3.14};
    Atom eq_atom_3{22, "foo", 3.14, "bar"};
    
    // deep comparison equality operator
    CHECK(eq_atom_1 == eq_atom_1);
    CHECK_FALSE(eq_atom_1 == eq_atom_2);
    CHECK_FALSE(eq_atom_1 == eq_atom_3);
    
    Vector eq_atom_1_vec = eq_atom_1;
    eq_atom_1_vec[1] = "bar";
    CHECK_FALSE(eq_atom_1 == eq_atom_2);
    eq_atom_1 = eq_atom_1_vec;
    CHECK(eq_atom_1 == eq_atom_2);
    
    // It would be better to allow to do
    //eq_atom_1[1] = "bar";
    //CHECK(eq_atom_1 == eq_atom_2);
}

TEST_CASE("Atom Dico", "[Atom]")
{
    Atom atom_vector{true, 1, 3.14, "tag"};
    
    Dico temp_dico;
    temp_dico[Tag::create("long")] = 12;
    temp_dico[Tag::create("object")] = atom_vector;
    
    Atom atom_dico(temp_dico);
    REQUIRE(atom_dico.isDico()         == true);
    Dico dico = atom_dico;
    REQUIRE(dico.size()                == 2);
    REQUIRE(dico[Tag::create("long")].isLong() == true);
    REQUIRE(dico[Tag::create("object")].isVector() == true);
    
    //std::cout << atom_dico << '\n';
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
    CHECK(atom_vec[2].isLong());
    CHECK(atom_vec[3].isLong());
    CHECK(atom_vec[4].isDouble());
}


