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
    Symbol sym_1("foo");
    Symbol sym_2("foo");
    Symbol sym_3("jojo");
    Symbol sym_4("jojo");
    
    CHECK(sym_1 == sym_2);
    CHECK(sym_1 == sym_1);
    CHECK_FALSE(sym_1 != sym_2);
    CHECK(sym_1 != sym_3);
    
    // copy construct
    std::string str = "bar";
    Symbol sym_5(str);
    //CHECK(sym_5 == "bar");
    CHECK(sym_5.toString() == "bar");
    
    
    std::vector<Symbol> sims {Symbol("jojo"), Symbol("jaja"), Symbol("juju")};
    
    Symbol sym_42("jojo");
    Symbol sym_jojo = sym_42;
    
    CHECK(sym_jojo.toString() == "jojo");
    
    /*
    Benchmark bench;
    
    bench.startTestCase("Creation (same) (10000x)", Benchmark::Sort::ByPerfAsc);
    {
        Symbol("test_same");
        Tag::create("test_same");
        
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
    
    bench.startTestCase("Creation (Different) (10000x)", Benchmark::Sort::ByPerfAsc);
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
    
    
    bench.startTestCase("Equality compare (100000x)", Benchmark::Sort::ByPerfAsc);
    {
        const uint64_t iter = 100000;
        Symbol      bench_sym_1("bench_equality_compare_1");
        sTag        bench_tag_1 = Tag::create("bench_equality_compare_1");
        std::string bench_str_1("bench_equality_compare_1");
        
        Symbol      bench_sym_2("bench_equality_compare_2");
        sTag        bench_tag_2 = Tag::create("bench_equality_compare_2");
        std::string bench_str_2("bench_equality_compare_2");
        
        std::vector<Symbol> v_symbols;
        std::vector<sTag> v_tags;
        std::vector<std::string> v_strings;
        
        std::string str1;
        std::string str2;
        
        std::shared_ptr<std::string> s_str1;
        std::shared_ptr<std::string> s_str2;
        
        bench.startUnit("--string (==)");
        for(uint64_t i = 0; i < iter - 1; ++i) { (str1 == str2) ; }
        bench.endUnit();
        
        bench.startUnit("--stringRef (==)");
        for(uint64_t i = 0; i < iter - 1; ++i) { (&str1 == &str2) ; }
        bench.endUnit();
        
        bench.startUnit("--sString (==)");
        for(uint64_t i = 0; i < iter - 1; ++i) { (s_str1 == s_str1) ; }
        bench.endUnit();
        
        
        for(uint64_t i = 0; i < iter; ++i)
        {
            const auto rdstr = std::to_string(std::rand());
            v_symbols.push_back(rdstr);
            v_tags.push_back(Tag::create(rdstr));
            v_strings.push_back(rdstr);
        }
        
        bool result = false;
        
        bench.startUnit("Symbols (==)");
        for(uint64_t i = 0; i < iter - 1; ++i) {
            if((v_symbols[i] == v_symbols[i+1]))
            {
                
            }
        }
        bench.endUnit();
        
        bench.startUnit("Tag (==)");
        for(uint64_t i = 0; i < iter - 1; ++i) {
            if((v_tags[i] == v_tags[i+1]))
            {
                std::cout << "Et Johnny il ya un problème !";
            }
        }
        bench.endUnit();
        
        bench.startUnit("std::string (==)");
        for(uint64_t i = 0; i < iter - 1; ++i) { if(v_strings[i] == v_strings[i+1]) result = true; }
        bench.endUnit();
        
        bench.startUnit("Symbols (!=)");
        for(int i = 0; i < iter; ++i) auto r = (bench_sym_1 != bench_sym_2);
        bench.endUnit();
        
        bench.startUnit("Tag (!=)");
        for(int i = 0; i < iter; ++i) auto r = (bench_tag_1 != bench_tag_2);
        bench.endUnit();
        
        bench.startUnit("std::string (!=)");
        for(int i = 0; i < iter; ++i) auto r = (bench_str_1 != bench_str_2);
        bench.endUnit();
        
        result = ! result;
    }
    
    bench.endTestCase();
    */
}
