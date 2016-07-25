/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "../catch.hpp"

#include <KiwiDsp/KiwiDsp_Chain.hpp>

#include "Processors.hpp"

using namespace kiwi;
using namespace dsp;

// ==================================================================================== //
//                                      TEST CHAIN                                      //
// ==================================================================================== //

//@todo Link is duplicated and Processor is duplicated
TEST_CASE("Dsp - Chain", "[Dsp, Chain]")
{
    const size_t samplerate = 44100ul;
    const size_t vectorsize = 64ul;
    
    Chain chain;
    std::set<Processor*> processes;
    std::set<Link*> links;
    
    Sig sig1(1.3);
    Sig sig2(2.7);
    PlusScalar plus_scalar(1.);
    PlusSignal plus_signal;
    
    Link link1(sig1, 0, plus_scalar, 0);
    Link link2(plus_scalar, 0, plus_signal, 0);
    Link link3(sig2, 0, plus_signal, 1);
    
    SECTION("Chain released not compiled")
    {
        chain.release();
    }
    
    SECTION("Chain compiled")
    {
        processes.insert(&sig1);
        processes.insert(&plus_scalar);
        links.insert(&link1);
        
        REQUIRE_NOTHROW(chain.compile(samplerate, vectorsize, processes, links));
        
        chain.release();
    }
    
    SECTION("Fanning Inlet process")
    {
        Sig sig_1(1.);
        Sig sig_2(2.);
        Sig sig_3(3.);
        
        PlusScalar plus_scalar_0(0.);
        
        Link link_1(sig_1, 0, plus_scalar_0, 0);
        Link link_2(sig_2, 0, plus_scalar_0, 0);
        Link link_3(sig_3, 0, plus_scalar_0, 0);
        
        processes.insert(&sig_1);
        processes.insert(&sig_2);
        processes.insert(&sig_3);
        processes.insert(&plus_scalar_0);
        links.insert(&link_1);
        links.insert(&link_2);
        links.insert(&link_3);
        
        REQUIRE_NOTHROW(chain.compile(samplerate, vectorsize, processes, links));
        
        chain.release();
    }
    
    SECTION("empty Chain compiled")
    {
        REQUIRE_NOTHROW(chain.compile(samplerate, vectorsize, processes, links));
        
        chain.release();
    }
    
    SECTION("Loop Detected")
    {
        processes.insert(&sig1);
        processes.insert(&sig2);
        processes.insert(&plus_scalar);
        processes.insert(&plus_signal);
        links.insert(&link1);
        links.insert(&link2);
        links.insert(&link3);
        
        Link link_loop(plus_signal, 0, plus_scalar, 0);
        links.insert(&link_loop);
        
        REQUIRE_THROWS_AS(chain.compile(samplerate, vectorsize, processes, links), Error);
        
        chain.release();
    }
    
    SECTION("Chain compile failed - Processor already used")
    {
        ;
    }
    
    SECTION("Processor Throw Based on Infos")
    {
        CopyThrow copy_throw;
        Link link_throw(plus_signal, 0, copy_throw, 0);
        
        processes.insert(&sig1);
        processes.insert(&sig2);
        processes.insert(&plus_scalar);
        processes.insert(&plus_signal);
        links.insert(&link1);
        links.insert(&link2);
        links.insert(&link3);
        
        processes.insert(&copy_throw);
        links.insert(&link_throw);
        
        REQUIRE_THROWS_AS(chain.compile(samplerate, 128ul, processes, links), Error);
        
        chain.release();
    }
    
    SECTION("Chain tick - WIP")
    {
        Sig sig(1.);
        PlusScalar plus(19.);
        std::string result;
        Print print(result);
        //PlusScalar plus_unused(1.);
        
        Link link_1(sig, 0, plus, 0);
        Link link_2(plus, 0, print, 0);
        //Link link_unused(sig, 0, plus_unused, 0);
        
        processes.insert(&sig);
        processes.insert(&plus);
        processes.insert(&print);
        //processes.insert(&plus_unused);
        
        links.insert(&link_1);
        links.insert(&link_2);
        //links.insert(&link_unused);
        
        REQUIRE_NOTHROW(chain.compile(samplerate, 4ul, processes, links));
        
        chain.tick();
        chain.tick();
        
        CHECK(result == "[20.000000, 20.000000, 20.000000, 20.000000]");
        
        //std::cout << result << std::endl;
        
        chain.release();
    }
    
    SECTION("Chain tick - fanning inlet (inlet add)")
    {
        Sig sig_1(1.);
        Sig sig_2(2.);
        Sig sig_3(3.);
        std::string result;
        Print print(result);
        
        Link link_1(sig_1, 0, print, 0);
        Link link_2(sig_2, 0, print, 0);
        Link link_3(sig_3, 0, print, 0);
        
        processes.insert(&sig_1);
        processes.insert(&sig_2);
        processes.insert(&sig_3);
        processes.insert(&print);
        
        links.insert(&link_1);
        links.insert(&link_2);
        links.insert(&link_3);
        
        REQUIRE_NOTHROW(chain.compile(samplerate, 4ul, processes, links));
        
        chain.tick();
        chain.tick();
        
        CHECK(result == "[6.000000, 6.000000, 6.000000, 6.000000]");
        
        //std::cout << result << std::endl;
        
        chain.release();
    }
    
    SECTION("Chain tick - fanning outlet (outlet copy)")
    {
        Sig sig(1.111111);
        
        std::string result_1;
        Print print_1(result_1);
        std::string result_2;
        Print print_2(result_2);
        std::string result_3;
        Print print_3(result_3);
        
        Link link_1(sig, 0, print_1, 0);
        Link link_2(sig, 0, print_2, 0);
        Link link_3(sig, 0, print_3, 0);
        
        processes.insert(&sig);
        processes.insert(&print_1);
        processes.insert(&print_2);
        processes.insert(&print_3);
        
        links.insert(&link_1);
        links.insert(&link_2);
        links.insert(&link_3);
        
        REQUIRE_NOTHROW(chain.compile(samplerate, 4ul, processes, links));
        
        chain.tick();
        chain.tick();
        
        CHECK(result_1 == "[1.111111, 1.111111, 1.111111, 1.111111]");
        CHECK(result_1 == result_2);
        CHECK(result_1 == result_3);
        
        chain.release();
    }
    
    SECTION("Chain tick - count example")
    {
        Count count;
        
        std::string result;
        Print print(result);
        
        Link link(count, 0, print, 0);
        
        processes.insert(&count);
        processes.insert(&print);
        
        links.insert(&link);
        
        REQUIRE_NOTHROW(chain.compile(samplerate, 4ul, processes, links));
        
        chain.tick();
        
        CHECK(result == "[0.000000, 1.000000, 2.000000, 3.000000]");
        
        chain.tick();
        
        CHECK(result == "[4.000000, 5.000000, 6.000000, 7.000000]");
        
        chain.tick();
        
        CHECK(result == "[8.000000, 9.000000, 10.000000, 11.000000]");
        
        chain.release();
    }
    
    SECTION("Chain tick - count example 2")
    {
        Count count;
        
        PlusSignal plus;
        
        std::string result;
        Print print(result);
        
        Link link_1(count, 0, plus, 0);
        Link link_2(count, 0, plus, 1);
        Link link_3(plus, 0, print, 0);
        
        processes.insert(&count);
        processes.insert(&plus);
        processes.insert(&print);
        
        links.insert(&link_1);
        links.insert(&link_2);
        links.insert(&link_3);
        
        REQUIRE_NOTHROW(chain.compile(samplerate, 4ul, processes, links));
        
        chain.tick();
        
        CHECK(result == "[0.000000, 2.000000, 4.000000, 6.000000]");
        
        chain.tick();
        
        CHECK(result == "[8.000000, 10.000000, 12.000000, 14.000000]");
        
        chain.tick();
        
        CHECK(result == "[16.000000, 18.000000, 20.000000, 22.000000]");
        
        chain.release();
    }
}
