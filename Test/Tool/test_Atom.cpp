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

#include <vector>

#include "../catch.hpp"

#include <KiwiTool/KiwiTool_Atom.h>

using namespace kiwi::tool;

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
    
    SECTION("char const* (Atom::Type::String)")
    {
        const std::string str("string");
        Atom a_sym("string");
        CHECK(a_sym.getType() == Atom::Type::String);
        CHECK(a_sym.isString());
        CHECK(a_sym.getString() == str);
    }
    
    SECTION("std::string const& (Atom::Type::String)")
    {
        const std::string str("string");
        Atom a_sym(str);
        CHECK(a_sym.getType() == Atom::Type::String);
        CHECK(a_sym.isString());
        CHECK(a_sym.getString() == str);
    }
    
    SECTION("Copy constructor")
    {
        Atom a_null;
        Atom a_null_copy = a_null;
        CHECK(a_null_copy.isNull());
        
        Atom a_int(42);
        Atom a_int_copy = a_int;
        CHECK(a_int_copy.isInt());
        CHECK(a_int_copy.getInt() == a_int.getInt());
        
        Atom a_float(3.14);
        Atom a_float_copy = a_float;
        CHECK(a_float_copy.isFloat());
        CHECK(a_float_copy.getFloat() == a_float.getFloat());
        
        Atom a_string("string");
        Atom a_string_copy = a_string;
        CHECK(a_string_copy.isString());
        CHECK(a_string_copy.getString() == a_string.getString());
        
        Atom a_comma = Atom::Comma();
        Atom a_comma_copy = a_comma;
        CHECK(a_comma_copy.getType()== Atom::Type::Comma);
        
        Atom a_dollar = Atom::Dollar(1);
        Atom a_dollar_copy = a_dollar;
        CHECK(a_dollar_copy.isDollar());
        CHECK(a_dollar_copy.getDollarIndex() == a_dollar.getDollarIndex());
    }
    
    SECTION("Copy assigment operator")
    {
        Atom a_null;
        Atom a_int(42);
        Atom a_float(3.14);
        Atom a_string("string");
        Atom a_string_2("string_2");
        
        Atom a_temp;
        CHECK(a_temp.getType()       == Atom::Type::Null);
        a_temp = a_int;
        CHECK(a_temp.getType()       == Atom::Type::Int);
        a_temp = a_float;
        CHECK(a_temp.getType()       == Atom::Type::Float);
        a_temp = a_string;
        CHECK(a_temp.getType()       == Atom::Type::String);
        a_temp = a_string_2;
        CHECK(a_temp.getType()       == Atom::Type::String);
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
        
        SECTION("String")
        {
            Atom to_move("string");
            Atom moved(std::move(to_move));
            CHECK(to_move.isNull()          == true);
            CHECK(moved.isString()          == true);
            CHECK(moved.getString() == "string");
        }
        
        SECTION("std::string move ctor")
        {
            std::string str_to_move("string");
            Atom moved(std::move(str_to_move));
            CHECK(moved.isString()          == true);
            CHECK(moved.getString() == "string");
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
    
    std::string str("jujube");
    a_temp = str;
    CHECK(a_temp.getType()       == Atom::Type::String);
    CHECK(a_temp.getString() == str);
    
    a_temp = "kiwi";
    CHECK(a_temp.getType()       == Atom::Type::String);
    CHECK(a_temp.getString() == "kiwi");
    
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
        CHECK(atom.isString()       == false);
        CHECK(atom.isBang()         == false);
        CHECK(atom.isComma()        == false);
        CHECK(atom.isDollar()       == false);
    }
    
    SECTION("Check Types (Int)")
    {
        Atom atom(42);
        CHECK(atom.getType()        == Atom::Type::Int);
        CHECK(atom.isNull()         == false);
        CHECK(atom.isNumber()       == true);
        CHECK(atom.isInt()          == true);
        CHECK(atom.isFloat()        == false);
        CHECK(atom.isString()       == false);
        CHECK(atom.isBang()         == false);
        CHECK(atom.isComma()        == false);
        CHECK(atom.isDollar()       == false);
    }
    
    SECTION("Check Types (Float)")
    {
        Atom atom(3.14);
        CHECK(atom.getType()        == Atom::Type::Float);
        CHECK(atom.isNull()         == false);
        CHECK(atom.isNumber()       == true);
        CHECK(atom.isInt()          == false);
        CHECK(atom.isFloat()        == true);
        CHECK(atom.isString()       == false);
        CHECK(atom.isBang()         == false);
        CHECK(atom.isComma()        == false);
        CHECK(atom.isDollar()       == false);
    }
    
    SECTION("Check Types (String)")
    {
        Atom atom("foo");
        CHECK(atom.getType()        == Atom::Type::String);
        CHECK(atom.isNull()         == false);
        CHECK(atom.isNumber()       == false);
        CHECK(atom.isInt()          == false);
        CHECK(atom.isFloat()        == false);
        CHECK(atom.isString()       == true);
        CHECK(atom.isBang()         == false);
        CHECK(atom.isComma()        == false);
        CHECK(atom.isDollar()       == false);
    }
    
    SECTION("Check Types (String) - bang")
    {
        Atom atom("bang");
        CHECK(atom.getType()        == Atom::Type::String);
        CHECK(atom.isNull()         == false);
        CHECK(atom.isNumber()       == false);
        CHECK(atom.isInt()          == false);
        CHECK(atom.isFloat()        == false);
        CHECK(atom.isString()       == true);
        CHECK(atom.isBang()         == true);
        CHECK(atom.isComma()        == false);
        CHECK(atom.isDollar()       == false);
    }
    
    SECTION("Check Types (Comma)")
    {
        Atom atom = Atom::Comma();
        CHECK(atom.getType()        == Atom::Type::Comma);
        CHECK(atom.isNull()         == false);
        CHECK(atom.isNumber()       == false);
        CHECK(atom.isInt()          == false);
        CHECK(atom.isFloat()        == false);
        CHECK(atom.isString()       == false);
        CHECK(atom.isBang()         == false);
        CHECK(atom.isComma()        == true);
        CHECK(atom.isDollar()       == false);
    }
    
    SECTION("Check Types (Dollar)")
    {
        Atom atom = Atom::Dollar(1);
        CHECK(atom.getType()        == Atom::Type::Dollar);
        CHECK(atom.isNull()         == false);
        CHECK(atom.isNumber()       == false);
        CHECK(atom.isInt()          == false);
        CHECK(atom.isFloat()        == false);
        CHECK(atom.isString()       == false);
        CHECK(atom.isBang()         == false);
        CHECK(atom.isComma()        == false);
        CHECK(atom.isDollar()       == true);
    }
}

TEST_CASE("Value getters", "[Atom]")
{
    SECTION("When Atom is of Type::Null")
    {
        Atom a_null;
        CHECK(a_null.getInt() == 0);
        CHECK(a_null.getFloat() == 0.0);
        CHECK(a_null.getString() == "");
    }
    
    SECTION("When Atom is of Type::Int")
    {
        Atom a_int(42);
        CHECK(a_int.getInt() == 42);
        CHECK(a_int.getFloat() == 42.);
        CHECK(a_int.getString() == "");
    }
    
    SECTION("When Atom is of Type::Float")
    {
        Atom a_float(3.14f);
        CHECK(a_float.getInt() == 3);
        CHECK(a_float.getFloat() == 3.14f);
        CHECK(a_float.getString() == "");
        
        Atom a_double(3.99);
        CHECK(a_double.getInt() == 3);
        CHECK(a_double.getFloat() == 3.99);
        CHECK(a_double.getString() == "");
    }
    
    SECTION("When Atom is of Type::String")
    {
        Atom a_sym("foo");
        CHECK(a_sym.getInt() == 0);
        CHECK(a_sym.getFloat() == 0.0);
        CHECK(a_sym.getString() == "foo");
    }
}

// -------
// Parser
// -------

TEST_CASE("Atom Parse", "[Atom]")
{
    SECTION("basic atom parsing")
    {
        const auto atoms = AtomHelper::parse("foo \"bar 42\" 1 -2 3.14 -3.14");
        REQUIRE(atoms.size() == 6);
        
        CHECK(atoms[0].getString() == "foo");
        CHECK(atoms[1].getString() == "bar 42");
        
        CHECK((atoms[2].isInt() && atoms[3].isInt()));
        
        CHECK(atoms[4].isFloat());
        CHECK(atoms[4].getFloat() == 3.14);
        
        CHECK(atoms[5].isFloat());
        CHECK(atoms[5].getFloat() == -3.14);
    }
    
    SECTION("parse integer beginning with multiple zeros")
    {
        const auto atoms = AtomHelper::parse("000101");
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isInt());
        CHECK(atoms[0].getInt() == 101);
    }
    
    SECTION("parse float beginning with multiple zeros")
    {
        const auto atoms = AtomHelper::parse("000.101");
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isFloat());
        CHECK(atoms[0].getFloat() == 0.101);
    }
    
    SECTION("simple dot is a string")
    {
        const auto atoms = AtomHelper::parse(".");
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isString());
    }
    
    SECTION("dot following by digits is a Float")
    {
        const auto atoms = AtomHelper::parse(".001");
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isFloat());
    }
    
    SECTION("float values are trimmed")
    {
        const auto atoms = AtomHelper::parse(".001000");
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isFloat());
        CHECK(AtomHelper::toString(atoms[0]) == "0.001");
    }
    
    SECTION("negative sign following by a dot and digits is a Float")
    {
        const auto atoms = AtomHelper::parse("-.001");
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isFloat());
        CHECK(atoms[0].getFloat() == -.001);
    }
    
    SECTION("exponent is a Float")
    {
        const auto atoms = AtomHelper::parse("6.02e23");
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isFloat());
        CHECK(atoms[0].getFloat() == 6.02e23);
    }
    
    SECTION("digits with more than one dot is a String")
    {
        const auto atoms = AtomHelper::parse("0.001.");
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isString());
    }
    
    SECTION("skip whitespaces")
    {
        const auto atoms = AtomHelper::parse("   toto   44");
        CHECK(atoms.size() == 2);
        CHECK(atoms[0].isString());
        CHECK(atoms[0].getString() == "toto");
        CHECK(atoms[1].isInt());
        CHECK(atoms[1].getInt() == 44);
    }
    
    SECTION("skip special whitespace characters")
    {
        const auto text = "\f\n\r\t\v  \f  \n  \r  \t  \v ";
        const auto atoms = AtomHelper::parse(text);
        REQUIRE(atoms.size() == 0);
    }
    
    SECTION("preserve special whitespace characters in double-quoted text sequence")
    {
        const auto text = "\"\f\n\r\t\v  \f  \n  \r  \t  \v \"";
        const auto atoms = AtomHelper::parse(text);
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isString());
        CHECK(AtomHelper::toString(atoms) == text);
        CHECK(AtomHelper::toString(atoms[0]) == text);
        CHECK(atoms[0].getString() == "\f\n\r\t\v  \f  \n  \r  \t  \v ");
    }
    
    SECTION("strip slashes")
    {
        const auto text = R"(\0\a\b\c\d\e\f\g\h\i\j\k\l\m\n\o\p\q\r\s\t\u\v\w\x\y\z)";
        const auto text_result = "0abcdefghijklmnopqrstuvwxyz";
        const auto atoms = AtomHelper::parse(text);
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isString());
        CHECK(AtomHelper::toString(atoms) == text_result);
        CHECK(AtomHelper::toString(atoms[0]) == text_result);
        CHECK(atoms[0].getString() == text_result);
    }
    
    SECTION("strip slashes double slash")
    {
        const auto text = R"(\\a\\b\\c\\d)";
        const auto normal_text = "\\\\a\\\\b\\\\c\\\\d";
        REQUIRE(text == normal_text);
        
        const auto atoms = AtomHelper::parse(text);
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isString());
        
        /*
         CHECK(AtomHelper::toString(atoms) == text_result);
         CHECK(AtomHelper::toString(atoms[0]) == text_result);
         CHECK(atoms[0].getString() == text_result);
         */
    }
    
    SECTION("foo \"bar 42\" 1 -2 3.14")
    {
        const auto atoms = AtomHelper::parse("foo \"bar 42\" 1 -2 3.14");
        REQUIRE(atoms.size() == 5);
        CHECK(atoms[0].getString() == "foo");
        CHECK(atoms[1].getString() == "bar 42");
        CHECK((atoms[2].isInt() && atoms[3].isInt()));
        CHECK(atoms[4].isFloat());
    }
    
    SECTION("test escaping")
    {
        const auto original_text = R"(foo "bar 42" 1 -2 3.14)";
        const auto atoms = AtomHelper::parse(original_text);
        const auto text = AtomHelper::toString(atoms);
        CHECK(text == original_text);
    }
}

TEST_CASE("Atom Parse Quoted", "[Atom]")
{
    SECTION("basic")
    {
        const auto atoms = AtomHelper::parse("\"0 10\"");
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isString());
        CHECK(atoms[0].getString() == "0 10");
    }
    
    SECTION("with comma")
    {
        const auto atoms = AtomHelper::parse(R"("0, 10")");
        REQUIRE(atoms.size() == 1);
        CHECK(atoms[0].isString());
        CHECK(atoms[0].getString() == "0, 10");
    }
    
    SECTION("with escaped quote")
    {
        const auto typed_text = R"("name: \"toto\"")";
        const auto formated_text = R"("name: \"toto\"")";
        const auto display_text = R"(name: "toto")";
        
        const auto atoms = AtomHelper::parse(typed_text);
        REQUIRE(atoms.size() == 1);
        REQUIRE(atoms[0].isString());
        
        CHECK(atoms[0].getString() == display_text);
        CHECK(AtomHelper::toString(atoms[0]) == formated_text);
        CHECK(AtomHelper::toString(atoms[0], false) == display_text);
    }
}

TEST_CASE("Atom Parse Comma", "[Atom]")
{
    SECTION("parse \"0, 10\" without comma option")
    {
        const auto atoms = AtomHelper::parse("0, 10");
        REQUIRE(atoms.size() == 2);
        CHECK(atoms[0].isString());
        CHECK(atoms[0].getString() == "0,");
        CHECK(atoms[1].isInt());
    }
    
    SECTION("parse \"0, 10\" without flag then toString")
    {
        const auto typed_text = R"(0, 10)";
        const auto text_to_display = R"(0, 10)";
        const auto atoms = AtomHelper::parse(typed_text);
        REQUIRE(atoms.size() == 2);
        CHECK(atoms[0].getString() == "0,");
        CHECK(AtomHelper::toString(atoms[0]) == "0,");
        const auto formated_text = AtomHelper::toString(atoms);
        CHECK(text_to_display == formated_text);
    }
    
    SECTION("parse with comma flag")
    {
        const auto atoms = AtomHelper::parse("0, 10", AtomHelper::ParsingFlags::Comma);
        REQUIRE(atoms.size() == 3);
        CHECK(atoms[0].isInt());
        CHECK(atoms[1].isComma());
        CHECK(atoms[2].isInt());
    }
    
    SECTION("parse \"0, 10\" with comma flag then toString")
    {
        const auto original_text = "0, 10";
        const auto atoms = AtomHelper::parse(original_text, AtomHelper::ParsingFlags::Comma);
        REQUIRE(atoms.size() == 3);
        const auto text = AtomHelper::toString(atoms);
        CHECK(text == original_text);
    }
    
    SECTION("parse comma without space after it")
    {
        const auto original_text = "0,10";
        const auto atoms = AtomHelper::parse(original_text, AtomHelper::ParsingFlags::Comma);
        REQUIRE(atoms.size() == 3);
        CHECK(atoms[0].isInt());
        CHECK(atoms[1].isComma());
        CHECK(atoms[2].isInt());
        const auto text = AtomHelper::toString(atoms);
        CHECK(text == "0, 10");
    }
    
    SECTION("parse several commas")
    {
        const auto original_text = ",,,";
        const auto atoms = AtomHelper::parse(original_text, AtomHelper::ParsingFlags::Comma);
        REQUIRE(atoms.size() == 3);
        CHECK(atoms[0].isComma());
        CHECK(atoms[1].isComma());
        CHECK(atoms[2].isComma());
        const auto text = AtomHelper::toString(atoms);
        CHECK(text == ",,,");
    }
}

TEST_CASE("Atom Parse Dollar", "[Atom]")
{
    const int flags = AtomHelper::ParsingFlags::Dollar;
    
    SECTION("parse without dollar flags")
    {
        const auto atoms = AtomHelper::parse("$1 $2 $3 $4 $5 $6 $7 $8 $9");
        REQUIRE(atoms.size() == 9);
        for (auto const& atom : atoms)
        {
            CHECK(atom.isString());
        }
    }
    
    SECTION("invalid dollar atoms")
    {
        const auto atoms = AtomHelper::parse("$0 a$1 $10 $-1 $ $$", AtomHelper::ParsingFlags::Dollar);
        REQUIRE(atoms.size() == 6);
        for (auto const& atom : atoms)
        {
            CHECK(atom.isString());
            CHECK(atom.getDollarIndex() == 0);
        }
    }
    
    SECTION("valid dollar atoms")
    {
        const auto atoms = AtomHelper::parse("$1 $2 $3 $4 $5 $6 $7 $8 $9", AtomHelper::ParsingFlags::Dollar);
        REQUIRE(atoms.size() == 9);
        for (auto const& atom : atoms)
        {
            REQUIRE(atom.isDollar());
            
            static auto count = 0;
            CHECK(++count == atom.getDollarIndex());
        }
    }
    
    SECTION("dollar followed by comma")
    {
        const auto atoms = AtomHelper::parse("$1, $2",
                                             AtomHelper::ParsingFlags::Dollar
                                             | AtomHelper::ParsingFlags::Comma);
        REQUIRE(atoms.size() == 3);
        CHECK(atoms[0].isDollar());
        CHECK(atoms[1].isComma());
        CHECK(atoms[2].isDollar());
    }
    
    SECTION("dollar followed by comma without Comma flag")
    {
        const auto atoms = AtomHelper::parse("$1, $2",
                                             AtomHelper::ParsingFlags::Dollar);
        REQUIRE(atoms.size() == 2);
        CHECK(atoms[0].isString());
        CHECK(atoms[0].getString() == "$1,");
        CHECK(atoms[1].isDollar());
    }
}
