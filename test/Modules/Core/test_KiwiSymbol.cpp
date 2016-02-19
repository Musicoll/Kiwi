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

#include "../../catch.hpp"
#include "../../KiwiBenchmark.hpp"

using namespace kiwi;

// ================================================================================ //
//                                      SYMBOL                                      //
// ================================================================================ //

TEST_CASE("Symbol", "[Symbol]")
{
    auto original_symtable_size = Symbol::size();
    Symbol sym_1("foo");
    CHECK(Symbol::size() == original_symtable_size + 1);
    Symbol sym_2("foo");
    CHECK(Symbol::size() == original_symtable_size + 1);
    Symbol sym_3("jojo");
    Symbol sym_4("jojo");
    CHECK(Symbol::size() == original_symtable_size + 2);
    
    CHECK(sym_1 == sym_2);
    CHECK(sym_1 == sym_1);
    CHECK_FALSE(sym_1 != sym_2);
    CHECK(sym_1 != sym_3);
    
    // copy construct
    std::string str = "bar";
    Symbol sym_5(str);
    CHECK(sym_5 == "bar");
    CHECK(sym_5.toString() == "bar");
    
    
    std::vector<Symbol> sims {"jojo", "jaja", "juju"};
    CHECK(Symbol::size() == original_symtable_size + 5);
    
    /*
    // copy construct
    std::string str2 = "bar";
    std::cout << str2 << '\n';
    Symbol sym_6(str2);
    std::cout << str2 << '\n';
    
    Symbol ss = "ss";
    */
    
    Benchmark bench;
    
    bench.startTestCase("Creation (same) (10000x)");
    
    {
        bench.startUnit("Symbols");
        for(int i = 0; i < 10000; ++i) Symbol("test_same");
        bench.endUnit();
        
        bench.startUnit("Tag");
        for(int i = 0; i < 10000; ++i) Tag::create("test_same");
        bench.endUnit();
        
        bench.startUnit("std::string");
        for(int i = 0; i < 10000; ++i) std::string("test_same");
        bench.endUnit();
    }
    
    bench.endTestCase();
    
    bench.startTestCase("Creation (Different) (10000x)");
    
    {
        bench.startUnit("Symbols");
        for(int i = 0; i < 10000; ++i) Symbol(std::to_string(i));
        bench.endUnit();
        
        bench.startUnit("Tag");
        for(int i = 0; i < 10000; ++i) Tag::create(std::to_string(i));
        bench.endUnit();
        
        bench.startUnit("std::string");
        for(int i = 0; i < 10000; ++i) std::string(std::to_string(i));
        bench.endUnit();
    }
    
    bench.endTestCase();
    
    bench.startTestCase("Equality compare (100000x)");
    
    {
        Symbol      bench_sym_1("bench_equality_compare_1");
        sTag        bench_tag_1 = Tag::create("bench_equality_compare_1");
        std::string bench_str_1("bench_equality_compare_1");
        
        Symbol      bench_sym_2("bench_equality_compare_2");
        sTag        bench_tag_2 = Tag::create("bench_equality_compare_2");
        std::string bench_str_2("bench_equality_compare_2");
        
        bench.startUnit("Symbols (==)");
        for(int i = 0; i < 100000; ++i) auto r = (bench_sym_1 == bench_sym_2);
        bench.endUnit();
        
        bench.startUnit("Tag (==)");
        for(int i = 0; i < 10000; ++i) auto r = (bench_tag_1 == bench_tag_2);
        bench.endUnit();
        
        bench.startUnit("std::string (==)");
        for(int i = 0; i < 100000; ++i) auto r = (bench_str_1 == bench_str_2);
        bench.endUnit();
        
        bench.startUnit("Symbols (!=)");
        for(int i = 0; i < 100000; ++i) auto r = (bench_sym_1 != bench_sym_2);
        bench.endUnit();
        
        bench.startUnit("Tag (!=)");
        for(int i = 0; i < 10000; ++i) auto r = (bench_tag_1 != bench_tag_2);
        bench.endUnit();
        
        bench.startUnit("std::string (!=)");
        for(int i = 0; i < 100000; ++i) auto r = (bench_str_1 != bench_str_2);
        bench.endUnit();
    }
    
    bench.endTestCase();
}
