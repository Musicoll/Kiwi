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


#include "../../../Modules/KiwiCore/KiwiAtom.hpp"
#include "../../catch.hpp"

using namespace kiwi;

// ================================================================================ //
//                                      ATOM                                        //
// ================================================================================ //

TEST_CASE("Atom Constructors", "[Atom]")
{
    SECTION("Null type")
    {
        Atom a;
        CHECK(a.getType() == Atom::Type::Null);
        CHECK(a.isNull());
        CHECK(Atom().getType() == Atom::Type::Null);
        CHECK(Atom(INFINITY).getType() == Atom::Type::Null);    // infinite produces Null
        CHECK(Atom(NAN).getType() == Atom::Type::Null);         // NaN produces Null
    }
    
    SECTION("Signed Integral types")
    {
        CHECK(Atom(true).isInt());          // bool
        CHECK(Atom(false).isInt());         // bool
        CHECK(Atom(short(1)).isInt());      // short
        CHECK(Atom(1).isInt());             // int
        CHECK(Atom(1l).isInt());            // long
        CHECK(Atom(1ll).isInt());           // long long
        CHECK(Atom(0xFFFFFF).isInt());      // hexadecimal
        CHECK(Atom(0113).isInt());          // octal
    }
    
    /*
    SECTION("Unsigned Integral types are unsupported")
    {
        CHECK(Atom(1u).getType() == Atom::Type::Int);        // unsigned (int)
        CHECK(Atom(1ul).getType() == Atom::Type::Int);       // unsigned long
        CHECK(Atom(1lu).getType() == Atom::Type::Int);       // unsigned long
        CHECK(Atom(1ull).getType() == Atom::Type::Int);      // unsigned long long
        CHECK(Atom(1llu).getType() == Atom::Type::Int);      // unsigned long long
    }
    */
    
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
    
    SECTION("Symbol types")
    {
        CHECK(Atom("c").getType() == Atom::Type::Symbol);
        CHECK(Atom(Symbol("tag")).getType() == Atom::Type::Symbol);
        Symbol tag = Symbol("tag");
        CHECK(Atom(tag).getType() == Atom::Type::Symbol);
        
        //WARN("Atom('c').getType() == Atom::Type::Symbol");
        //CHECK(Atom('c').getType() == Atom::Type::Symbol);
        //CHECK_FALSE(Atom('c').getType() == Atom::Type::Int);       //
    }
}

TEST_CASE("Atom Null", "[Atom]")
{
    Atom atom;
    REQUIRE(atom.getType()          == Atom::Type::Null);
    REQUIRE(atom.isNull()      == true);
    REQUIRE(atom.isNumber()         == false);
    REQUIRE(atom.isInt()           == false);
    REQUIRE(atom.isFloat()         == false);
    REQUIRE(atom.isSymbol()            == false);
    
    //REQUIRE_FALSE(atom == false);
    //REQUIRE_FALSE(atom == true);
}

TEST_CASE("Atom Int", "[Atom]")
{
    Atom atom(1);
    CHECK(atom.getType()          == Atom::Type::Int);
    CHECK(atom.isNull()      == false);
    
    CHECK(atom.isNumber()         == true);
    CHECK(atom.isInt()           == true);
    CHECK(atom.isFloat()         == false);
    CHECK(atom.isSymbol()            == false);
    //CHECK(atom == true);
    //CHECK(atom == 1);
    //CHECK(atom == 1.);
    
    //CHECK(Atom(std::numeric_limits<int64_t>::max()) == std::numeric_limits<int64_t>::max());
    //CHECK(Atom(std::numeric_limits<uint64_t>::max()) == std::numeric_limits<uint64_t>::max());
    
    // do we need to support all equality operators ?
    //CHECK(atom == 1ul);
    
    Atom to_move(42);
    Atom moved(std::move(to_move));
    CHECK(to_move.isNull()          == true);
    CHECK(moved.isNumber()         == true);
    CHECK(moved.isInt()           == true);
    //CHECK(moved == 42);
}

TEST_CASE("Atom Float", "[Atom]")
{
    Atom atom(1.123);
    CHECK(atom.getType()          == Atom::Type::Float);
    CHECK(atom.isNull()           == false);
    CHECK(atom.isNumber()         == true);
    CHECK(atom.isInt()            == false);
    CHECK(atom.isFloat()          == true);
    CHECK(atom.isSymbol()         == false);
    //CHECK(atom == 1.123);
    //CHECK(atom == Approx(1.123f));
    // Should this fail ??
    //CHECK(atom == 1);
    //CHECK(atom == true);
    
    //CHECK_FALSE(Atom(1.123) == 3.14);
    
    Atom to_move(42.42);
    Atom moved(std::move(to_move));
    CHECK(to_move.isNull()    == true);
    CHECK(moved.isNumber()         == true);
    CHECK(moved.isFloat()         == true);
    //CHECK(moved == 42.42);
}

TEST_CASE("Atom Symbol", "[Atom]")
{
    Atom atom("foo");
    CHECK(atom.getType()          == Atom::Type::Symbol);
    CHECK(atom.isNull()      == false);
    CHECK(atom.isNumber()         == false);
    CHECK(atom.isInt()           == false);
    CHECK(atom.isFloat()         == false);
    CHECK(atom.isSymbol()            == true);
    
    CHECK(atom == Atom("foo"));
    CHECK(atom != Atom("jojo"));
    
    Atom to_move("foo");
    Atom moved(std::move(to_move));
    CHECK(to_move.isNull()   == true);
    CHECK(moved.isSymbol()   == true);
    
    //Atom mmmm;
    //mmmm = Symbol("zaza");
}

TEST_CASE("Atom string parser", "[Atom]")
{
    const std::string message = "foo \"bar 42\" 1 -4 3.14";
    
    std::vector<Atom> atom_vec(Atom::parse(message));
    REQUIRE(atom_vec.size() == 5);
    CHECK(atom_vec[0].isSymbol());
    CHECK(atom_vec[1].isSymbol());
    CHECK(atom_vec[2].isInt());
    CHECK(atom_vec[3].isInt());
    CHECK(atom_vec[4].isFloat());
}


