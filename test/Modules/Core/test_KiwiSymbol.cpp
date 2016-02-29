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

#include "../../../Modules/KiwiCore/KiwiSymbol.hpp"

#include "../../catch.hpp"
#include "../../KiwiBenchmark.hpp"

using namespace kiwi;

// ================================================================================ //
//                                      SYMBOL                                      //
// ================================================================================ //

TEST_CASE("Symbol", "[Symbol]")
{
    SECTION("Default Constructor")
    {
        Symbol sym;
        CHECK(sym == Symbols::empty);
        CHECK(sym == Symbols::empty);
    }
    
    SECTION("(std::string const&) Constructor")
    {
        const std::string str1 = "foo";
        const std::string str2 = "jojo";
        Symbol sym_1(str1);
        Symbol sym_2(str1);
        Symbol sym_3(str2);
        Symbol sym_4(str2);
        
        CHECK(sym_1 == sym_2);
        CHECK(sym_1 == sym_1);
        CHECK_FALSE(sym_1 != sym_2);
        CHECK(sym_1 != sym_3);
    }
    
    SECTION("(std::string&&) Constructor")
    {
        Symbol sym_1("foo");
        Symbol sym_2("foo");
        Symbol sym_3("jojo");
        Symbol sym_4("jojo");
        
        CHECK(sym_1 == sym_2);
        CHECK(sym_1 == sym_1);
        CHECK_FALSE(sym_1 != sym_2);
        CHECK(sym_1 != sym_3);
        
        const char* str1 = "str1";
        Symbol sym_5(str1);
        
        auto str2 = "str2";
        Symbol sym_6(str2);
        
        const auto str3 = "str3";
        Symbol sym_7(str3);
    }
    
    SECTION("Copy assignment operator")
    {
        Symbol sym_1("foo");
        Symbol sym_2("jojo");
        CHECK(sym_1 != sym_2);
        sym_2 = sym_1;
        CHECK(sym_1 == sym_2);
    }
    
    SECTION("Symbol swap")
    {
        Symbol sym_1("foo");
        Symbol sym_2("jojo");
        CHECK(sym_1.toString() == "foo");
        CHECK(sym_2.toString() == "jojo");
        
        sym_1.swap(sym_2);
        CHECK(sym_1.toString() == "jojo");
        CHECK(sym_2.toString() == "foo");
        
        std::swap(sym_1, sym_2);
        CHECK(sym_1.toString() == "foo");
        CHECK(sym_2.toString() == "jojo");
    }

    SECTION("toString()")
    {
        const std::string str = "jujube";
        Symbol sym(str);
        CHECK(sym.toString() == str);
    }
    
    SECTION("Equality operator (==)")
    {
        Symbol sym_empty_1("");
        Symbol sym_empty_2("");
        Symbol sym_foo("foo");
        Symbol sym_bar("bar");
        
        CHECK(sym_empty_1 == sym_empty_2);
        CHECK(sym_empty_1 == Symbols::empty);
        CHECK_FALSE(sym_empty_1 == sym_foo);
        CHECK_FALSE(sym_foo == sym_bar);
        CHECK(sym_foo == sym_foo);
        CHECK(sym_bar == sym_bar);
    }
    
    SECTION("Equality operator (!=)")
    {
        Symbol sym_empty_1("");
        Symbol sym_empty_2("");
        Symbol sym_foo("foo");
        Symbol sym_bar("bar");
        
        CHECK_FALSE(sym_empty_1 != sym_empty_2);
        CHECK_FALSE(sym_empty_1 != Symbols::empty);
        CHECK(sym_empty_1 != sym_foo);
        CHECK(sym_foo != sym_bar);
        CHECK_FALSE(sym_foo != sym_foo);
        CHECK_FALSE(sym_bar != sym_bar);
    }
    
    SECTION("std::vector<Symbol>")
    {
        std::vector<Symbol> syms {Symbol("jojo"), Symbol("jaja"), Symbol("juju")};
        CHECK(syms.size() == 3);
        CHECK(syms[0].toString() == "jojo");
        CHECK(syms[1].toString() == "jaja");
        CHECK(syms[2].toString() == "juju");
    }
}

/*
TEST_CASE("Symbol Benchmark", "[Symbol, Benchmark]")
{
    Benchmark bench;
    const uint64_t iter = 100000;
    
    bench.startTestCase("Creation (same) (10000x)", Benchmark::Sort::ByPerfAsc);
    {
        Symbol("test_same");
        Symbol("test_same");
        
        bench.startUnit("Symbols");
        for(uint64_t i = 0; i < iter; ++i) Symbol("test_same");
        bench.endUnit();
        
        bench.startUnit("std::string");
        for(uint64_t i = 0; i < iter; ++i) std::string("test_same");
        bench.endUnit();
    }
    bench.endTestCase();
    
    bench.startTestCase("Creation (Different) (10000x)", Benchmark::Sort::ByPerfAsc);
    {
        bench.startUnit("Symbols");
        for(uint64_t i = 0; i < iter; ++i) Symbol(std::to_string(i));
        bench.endUnit();
        
        bench.startUnit("std::string");
        for(uint64_t i = 0; i < iter; ++i) std::string(std::to_string(i));
        bench.endUnit();
    }
    bench.endTestCase();
    
    
    bench.startTestCase("Equality compare (100000x)", Benchmark::Sort::ByPerfAsc);
    {
        std::vector<Symbol> v_symbols;
        std::vector<std::string> v_strings;
        const auto cstr = "Oh bladi Oh blada Eh Oh bladiblada";
        for(uint64_t i = 0; i < iter; ++i)
        {
            const auto rdstr = cstr + std::to_string(std::rand());
            v_symbols.push_back(rdstr);
            v_strings.push_back(rdstr);
        }
        
        bench.startUnit("v_symbols (==)");
        for(uint64_t i = 0; i < iter - 1; ++i) {
            if((v_symbols[i] == v_symbols[i+1])) {}
        }
        bench.endUnit();
        
        bench.startUnit("v_strings (==)");
        for(uint64_t i = 0; i < iter - 1; ++i) {
            if((v_strings[i] == v_strings[i+1])) {}
        }
        bench.endUnit();
        
        bench.startUnit("v_symbols (!=)");
        for(uint64_t i = 0; i < iter - 1; ++i) {
            if((v_symbols[i] != v_symbols[i+1])) {}
        }
        bench.endUnit();
        
        bench.startUnit("v_strings (!=)");
        for(uint64_t i = 0; i < iter - 1; ++i) {
            if((v_strings[i] != v_strings[i+1])) {}
        }
        bench.endUnit();
    }
    
    bench.endTestCase();
}
*/
