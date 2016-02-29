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
//                                 ATOM CONSTRUCTORS                                //
// ================================================================================ //

TEST_CASE("Atom Constructors", "[Atom]")
{
    SECTION("Default (Atom::Type::Null)")
    {
        Atom atom;
        CHECK(atom.getType() == Atom::Type::Null);
        CHECK(atom.isNull());
        
        CHECK(Atom().getType() == Atom::Type::Null);
    }
    
    SECTION("bool (Atom::Type::Int)")
    {
        Atom atom_true(true);
        REQUIRE(atom_true.isInt());
        CHECK(atom_true.getInt() == 1);
        
        Atom atom_false(false);
        REQUIRE(atom_false.isInt());
        CHECK(atom_false.getInt() == 0);
    }
    
    SECTION("Signed Integral types")
    {
        CHECK(Atom('c').isInt());           // char
        CHECK(Atom(true).isInt());          // bool
        CHECK(Atom(false).isInt());         // bool
        CHECK(Atom(short(1)).isInt());      // short
        CHECK(Atom(1).isInt());             // int
        CHECK(Atom(1l).isInt());            // long
        CHECK(Atom(1ll).isInt());           // long long
        CHECK(Atom(0xFFFFFF).isInt());      // hexadecimal
        CHECK(Atom(0113).isInt());          // octal
    }
    
    SECTION("int (Atom::Type::Int)")
    {
        Atom atom_1(440);
        REQUIRE(atom_1.isInt());
        CHECK(atom_1.getInt() == 440);
        
        Atom atom_2(-440);
        REQUIRE(atom_2.isInt());
        CHECK(atom_2.getInt() == -440);
        
        Atom atom_3(std::numeric_limits<int>::max());
        REQUIRE(atom_3.isInt());
        CHECK(atom_3.getInt() == std::numeric_limits<int>::max());
        
        Atom atom_4(std::numeric_limits<int>::lowest());
        REQUIRE(atom_4.isInt());
        CHECK(atom_4.getInt() == std::numeric_limits<int>::lowest());
    }
    
    SECTION("long (Atom::Type::Int)")
    {
        Atom atom_1(440l);
        REQUIRE(atom_1.isInt());
        CHECK(atom_1.getInt() == 440l);
        
        Atom atom_2(-440l);
        REQUIRE(atom_2.isInt());
        CHECK(atom_2.getInt() == -440l);
        
        Atom atom_3(std::numeric_limits<long>::max());
        REQUIRE(atom_3.isInt());
        CHECK(atom_3.getInt() == std::numeric_limits<long>::max());
        
        Atom atom_4(std::numeric_limits<long>::lowest());
        REQUIRE(atom_4.isInt());
        CHECK(atom_4.getInt() == std::numeric_limits<long>::lowest());
    }
    
    SECTION("long long (Atom::Type::Int)")
    {
        Atom atom_1(440ll);
        REQUIRE(atom_1.isInt());
        CHECK(atom_1.getInt() == 440ll);
        
        Atom atom_2(-440ll);
        REQUIRE(atom_2.isInt());
        CHECK(atom_2.getInt() == -440ll);
        
        Atom atom_3(std::numeric_limits<long long>::max());
        REQUIRE(atom_3.isInt());
        CHECK(atom_3.getInt() == std::numeric_limits<long long>::max());
        
        Atom atom_4(std::numeric_limits<long long>::lowest());
        REQUIRE(atom_4.isInt());
        CHECK(atom_4.getInt() == std::numeric_limits<long long>::lowest());
    }
    
    /*
    SECTION("Unsigned Integral types are unsupported")
    {
        CHECK(Atom(1u).getType() == Atom::Type::Int);           // unsigned (int)
        CHECK(Atom(1ul).getType() == Atom::Type::Int);          // unsigned long
        CHECK(Atom(1lu).getType() == Atom::Type::Int);          // unsigned long
        CHECK(Atom(1ull).getType() == Atom::Type::Int);         // unsigned long long
        CHECK(Atom(1llu).getType() == Atom::Type::Int);         // unsigned long long
    }
    */
    
    SECTION("Floating-Point types")
    {
        CHECK(Atom(3.14f).getType() == Atom::Type::Float);      // float
        CHECK(Atom(3.14).getType() == Atom::Type::Float);       // double
        //CHECK(Atom(3.14l).getType() == Atom::Type::Float);    // long double are not supported
        CHECK(Atom(6.02e23).getType() == Atom::Type::Float);    // 6.02 x 10^23 (Avogadro constant)
        CHECK(Atom(1.6e-19).getType() == Atom::Type::Float);    // 1.6 x 10^-19 (electric charge of an electron)
    }
    
    SECTION("Check for infinity and NaN")
    {
        // NaN produces Null
        CHECK(Atom(double(NAN)).isNull());
        CHECK_FALSE(Atom(float(NAN)).isFloat());
        
        // infinite produces Null
        CHECK(Atom(double(INFINITY)).isNull());
        CHECK_FALSE(Atom(float(INFINITY)).isFloat());
    }
    
    SECTION("float (Atom::Type::Float)")
    {
        Atom atom_1(3.14f);
        REQUIRE(atom_1.isFloat());
        CHECK(atom_1.getFloat() == 3.14f);
        
        Atom atom_2(-3.14f);
        REQUIRE(atom_2.isFloat());
        CHECK(atom_2.getFloat() == -3.14f);
        
        Atom atom_3(std::numeric_limits<float>::max());
        REQUIRE(atom_3.isFloat());
        CHECK(atom_3.getFloat() == std::numeric_limits<float>::max());
        
        Atom atom_4(std::numeric_limits<float>::lowest());
        REQUIRE(atom_4.isFloat());
        CHECK(atom_4.getFloat() == std::numeric_limits<float>::lowest());
    }
    
    SECTION("double (Atom::Type::Float)")
    {
        Atom atom_1(3.14);
        REQUIRE(atom_1.isFloat());
        CHECK(atom_1.getFloat() == 3.14);
        
        Atom atom_2(-3.14);
        REQUIRE(atom_2.isFloat());
        CHECK(atom_2.getFloat() == -3.14);
        
        Atom atom_3(std::numeric_limits<double>::max());
        REQUIRE(atom_3.isFloat());
        CHECK(atom_3.getFloat() == std::numeric_limits<double>::max());
        
        Atom atom_4(std::numeric_limits<double>::lowest());
        REQUIRE(atom_4.isFloat());
        CHECK(atom_4.getFloat() == std::numeric_limits<double>::lowest());
    }
    
    SECTION("Symbol const& (Atom::Type::Symbol)")
    {
        const std::string str("symbol");
        Symbol sym(str);
        Atom a_sym(sym);
        CHECK(a_sym.getType() == Atom::Type::Symbol);
        CHECK(a_sym.isSymbol());
        CHECK(a_sym.getSymbol() == sym);
        CHECK(a_sym.getSymbol().toString() == str);
        CHECK(a_sym.getSymbol() == sym);
        CHECK(a_sym.getSymbol().toString() == str);
    }
    
    SECTION("char const* (Atom::Type::Symbol)")
    {
        const std::string str("symbol");
        Symbol sym(str);
        Atom a_sym("symbol");
        CHECK(a_sym.getType() == Atom::Type::Symbol);
        CHECK(a_sym.isSymbol());
        CHECK(a_sym.getSymbol() == sym);
        CHECK(a_sym.getSymbol().toString() == str);
        CHECK(a_sym.getSymbol() == sym);
        CHECK(a_sym.getSymbol().toString() == str);
    }
    
    SECTION("std::string const& (Atom::Type::Symbol)")
    {
        const std::string str("symbol");
        Symbol sym(str);
        Atom a_sym(str);
        CHECK(a_sym.getType() == Atom::Type::Symbol);
        CHECK(a_sym.isSymbol());
        CHECK(a_sym.getSymbol() == sym);
        CHECK(a_sym.getSymbol().toString() == str);
    }
    
    SECTION("Copy constructor + Copy assigment operator")
    {
        Atom a_null;
        Atom a_int(42);
        Atom a_float(3.14);
        Atom a_symbol("symbol");
        
        Atom a_temp;
        CHECK(a_temp.getType()       == Atom::Type::Null);
        a_temp = a_int;
        CHECK(a_temp.getType()       == Atom::Type::Int);
        a_temp = a_float;
        CHECK(a_temp.getType()       == Atom::Type::Float);
        a_temp = a_symbol;
        CHECK(a_temp.getType()       == Atom::Type::Symbol);
        a_temp = a_null;
        CHECK(a_temp.getType()       == Atom::Type::Null);
    }
    
    SECTION("Move constructor")
    {
        SECTION("Null")
        {
            Atom to_move;
            Atom moved(std::move(to_move));
            CHECK(to_move.isNull()          == true);
            CHECK(moved.isNull()            == true);
        }
        
        SECTION("Int")
        {
            Atom to_move(42);
            Atom moved(std::move(to_move));
            CHECK(to_move.isNull()          == true);
            CHECK(moved.isNumber()          == true);
            CHECK(moved.isInt()             == true);
            CHECK(moved.getInt() == 42);
        }
        
        SECTION("Float")
        {
            Atom to_move(3.14);
            Atom moved(std::move(to_move));
            CHECK(to_move.isNull()          == true);
            CHECK(moved.isNumber()          == true);
            CHECK(moved.isFloat()           == true);
            CHECK(moved.getFloat() == 3.14);
        }
        
        SECTION("Symbol")
        {
            Atom to_move("symbol");
            Atom moved(std::move(to_move));
            CHECK(to_move.isNull()          == true);
            CHECK(moved.isSymbol()          == true);
            CHECK(moved.getSymbol().toString() == "symbol");
        }
    }
}

TEST_CASE("Atom Copy assignment", "[Atom]")
{
    Atom a_temp;
    CHECK(a_temp.getType()       == Atom::Type::Null);
    
    a_temp = 123456789;
    CHECK(a_temp.getType()       == Atom::Type::Int);
    CHECK(a_temp.getInt() == 123456789);
    
    a_temp = 123456789l;
    CHECK(a_temp.getType()       == Atom::Type::Int);
    CHECK(a_temp.getInt() == 123456789l);
    
    a_temp = 123456789ll;
    CHECK(a_temp.getType()       == Atom::Type::Int);
    CHECK(a_temp.getInt() == 123456789ll);
    
    a_temp = 1.23456789f;
    CHECK(a_temp.getType()       == Atom::Type::Float);
    CHECK(a_temp.getFloat() == 1.23456789f);
    
    a_temp = 1.23456789;
    CHECK(a_temp.getType()       == Atom::Type::Float);
    CHECK(a_temp.getFloat() == 1.23456789);
    
    Symbol sym("jujube");
    a_temp = sym;
    CHECK(a_temp.getType()       == Atom::Type::Symbol);
    CHECK(a_temp.getSymbol() == sym);
    
    a_temp = "kiwi";
    CHECK(a_temp.getType()       == Atom::Type::Symbol);
    CHECK(a_temp.getSymbol().toString() == "kiwi");
    
    a_temp = {};
    CHECK(a_temp.getType()       == Atom::Type::Null);
}

TEST_CASE("Check Types", "[Atom]")
{
    SECTION("Check Types (Null)")
    {
        Atom atom;
        CHECK(atom.getType()        == Atom::Type::Null);
        CHECK(atom.isNull()         == true);
        CHECK(atom.isNumber()       == false);
        CHECK(atom.isInt()          == false);
        CHECK(atom.isFloat()        == false);
        CHECK(atom.isSymbol()       == false);
    }
    
    SECTION("Check Types (Int)")
    {
        Atom atom(42);
        CHECK(atom.getType()        == Atom::Type::Int);
        CHECK(atom.isNull()         == false);
        CHECK(atom.isNumber()       == true);
        CHECK(atom.isInt()          == true);
        CHECK(atom.isFloat()        == false);
        CHECK(atom.isSymbol()       == false);
    }
    
    SECTION("Check Types (Float)")
    {
        Atom atom(3.14);
        CHECK(atom.getType()        == Atom::Type::Float);
        CHECK(atom.isNull()         == false);
        CHECK(atom.isNumber()       == true);
        CHECK(atom.isInt()          == false);
        CHECK(atom.isFloat()        == true);
        CHECK(atom.isSymbol()       == false);
    }
    
    SECTION("Check Types (Symbol)")
    {
        Atom atom("foo");
        CHECK(atom.getType()        == Atom::Type::Symbol);
        CHECK(atom.isNull()         == false);
        CHECK(atom.isNumber()       == false);
        CHECK(atom.isInt()          == false);
        CHECK(atom.isFloat()        == false);
        CHECK(atom.isSymbol()       == true);
    }
}

TEST_CASE("Value getters", "[Atom]")
{
    SECTION("When Atom is of Type::Null")
    {
        Atom a_null;
        CHECK(a_null.getInt() == 0);
        CHECK(a_null.getFloat() == 0.0);
        CHECK(a_null.getSymbol() == Symbols::empty);
    }
    
    SECTION("When Atom is of Type::Int")
    {
        Atom a_int(42);
        CHECK(a_int.getInt() == 42);
        CHECK(a_int.getFloat() == 42.);
        CHECK(a_int.getSymbol() == Symbols::empty);
    }
    
    SECTION("When Atom is of Type::Float")
    {
        Atom a_float(3.14f);
        CHECK(a_float.getInt() == 3);
        CHECK(a_float.getFloat() == 3.14f);
        CHECK(a_float.getSymbol() == Symbols::empty);
        
        Atom a_double(3.99);
        CHECK(a_double.getInt() == 3);
        CHECK(a_double.getFloat() == 3.99);
        CHECK(a_double.getSymbol() == Symbols::empty);
    }
    
    SECTION("When Atom is of Type::Symbol")
    {
        Atom a_sym("foo");
        CHECK(a_sym.getInt() == 0);
        CHECK(a_sym.getFloat() == 0.0);
        CHECK(a_sym.getSymbol() == Symbol("foo"));
    }
}

TEST_CASE("Equality operators", "[Atom]")
{
    Atom a_null_1, a_null_2;
    Atom a_int_1(42), a_int_2(-26);
    Atom a_float_1(3.14), a_float_2(666.666);
    Atom a_symbol_1("symbol_1"), a_symbol_2("symbol_2");
    
    SECTION("Atom::Type::Null")
    {
        // Null value are equal
        CHECK(a_null_1 == a_null_2);
        CHECK_FALSE(a_null_1 != a_null_2);
        
        CHECK_FALSE(a_null_1 == a_int_1);
        CHECK(a_null_1 != a_int_1);
        
        CHECK_FALSE(a_null_1 == a_float_1);
        CHECK(a_null_1 != a_float_1);
        
        CHECK_FALSE(a_null_1 == a_symbol_1);
        CHECK(a_null_1 != a_symbol_1);
    }
    
    SECTION("Atom::Type::Int")
    {
        CHECK_FALSE(a_int_1 == a_null_1);
        CHECK(a_int_1 != a_null_1);
        
        // Int with same value are equal
        CHECK(a_int_1 == a_int_1);
        CHECK_FALSE(a_int_1 != a_int_1);
        
        // Int with different values are NOT equal
        CHECK_FALSE(a_int_1 == a_int_2);
        CHECK(a_int_1 != a_int_2);
        
        // Int are NOT equal to Float
        CHECK_FALSE(a_int_1 == a_float_1);
        CHECK(a_int_1 != a_float_1);
        
        // Int are NOT equal to Symbols
        CHECK_FALSE(a_int_1 == a_symbol_1);
        CHECK(a_int_1 != a_symbol_1);
    }
    
    SECTION("Atom::Type::Float")
    {
        CHECK_FALSE(a_float_1 == a_null_1);
        CHECK(a_float_1 != a_null_1);
        
        // Float with same value are equal
        CHECK(a_float_1 == a_float_1);
        CHECK_FALSE(a_float_1 != a_float_1);
        
        // Float with different values are NOT equal
        CHECK_FALSE(a_float_1 == a_float_2);
        CHECK(a_float_1 != a_float_2);
        
        // Float are NOT equal to Int
        CHECK_FALSE(a_float_1 == a_int_1);
        CHECK(a_float_1 != a_int_1);
        
        // Float are NOT equal to Symbols
        CHECK_FALSE(a_float_1 == a_symbol_1);
        CHECK(a_float_1 != a_symbol_1);
    }
    
    SECTION("Atom::Type::Symbol")
    {
        CHECK_FALSE(a_symbol_1 == a_null_1);
        CHECK(a_symbol_1 != a_null_1);
        
        // Symbol objects with same value are equal
        CHECK(a_symbol_1 == a_symbol_1);
        CHECK_FALSE(a_symbol_1 != a_symbol_1);
        
        // Symbol objects with different values are NOT equal
        CHECK_FALSE(a_symbol_1 == a_symbol_2);
        CHECK(a_float_1 != a_symbol_2);
        
        // Symbol objects are NOT equal to Int
        CHECK_FALSE(a_symbol_1 == a_int_1);
        CHECK(a_symbol_1 != a_int_1);
        
        // Symbol objects are NOT equal to Float
        CHECK_FALSE(a_symbol_1 == a_float_1);
        CHECK(a_symbol_1 != a_float_1);
    }
}

TEST_CASE("Atom string parser", "[Atom]")
{
    SECTION("foo 42 3.14")
    {
        const std::string message = "foo 42 3.14";
        
        auto atom_vec = Atom::parse(message);
        REQUIRE(atom_vec.size() == 3);
        CHECK(atom_vec[0].isSymbol());
        CHECK(atom_vec[1].isInt());
        CHECK(atom_vec[2].isFloat());
    }
    
    SECTION("foo~ \"bar 42\" 1 -4 3.14")
    {
        const std::string message = "foo~ \"bar 42\" 1 -4 3.14";
        
        auto atom_vec = Atom::parse(message);
        REQUIRE(atom_vec.size() == 5);
        CHECK(atom_vec[0].isSymbol());
        CHECK(atom_vec[1].isSymbol());
        CHECK(atom_vec[2].isInt());
        CHECK(atom_vec[3].isInt());
        CHECK(atom_vec[4].isFloat());
    }
}

TEST_CASE("Atom std::ostream::operator<<", "[Atom]")
{
    Atom a_null;
    Atom a_sym("foo bar");
    Atom a_int(42);
    Atom a_float(3.14);
    
    std::ostringstream stream;
    
    const std::string expected_str = "\"foo bar\" 42 3.14";
    stream << a_null << a_sym << " " << a_int << " " << a_float;
    const std::string result_str = stream.str();
    
    CHECK(expected_str == result_str);
}
