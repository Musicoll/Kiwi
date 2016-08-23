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

#include <memory>

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
    
    SECTION("Chain released not compiled")
    {
        Chain chain;
        chain.release();
    }
    
    SECTION("Chain modification")
    {
        Chain chain;
        
        Processor * sig = new Sig(1.);
        
        chain.addProcessor(0, std::move(std::unique_ptr<Processor>(sig)));
        chain.addProcessor(1, std::move(std::unique_ptr<Processor>(new PlusScalar(2.))));
        
        // Reinserting processors
        REQUIRE_THROWS_AS(chain.addProcessor(0, std::move(std::unique_ptr<Processor>(new Sig(2.)))), Error);
        REQUIRE_THROWS_AS(chain.addProcessor(1, std::move(std::unique_ptr<Processor>(sig))), Error);
    
        // Connecting processors
        REQUIRE(chain.connect(0, 0, 1, 0));
        REQUIRE(!chain.connect(0, 0, 1, 0));
        
        // Connecting with non existing processor
        REQUIRE_THROWS_AS(chain.connect(1, 0, 2, 0), Error);
        
        // Disconnecting processors
        REQUIRE(chain.discconnect(0, 0, 1, 0));
        REQUIRE(!chain.discconnect(0, 0, 1, 0));
        
        // Removing processors
        REQUIRE_THROWS_AS(chain.removeProcessor(2), Error);
    }
    
    SECTION("Chain compiled")
    {
        Chain chain;
        
        std::unique_ptr<Processor> sig1(new Sig(1.3));
        std::unique_ptr<Processor> plus_scalar(new PlusScalar(1.));
        
        chain.addProcessor(0, std::move(sig1));
        chain.addProcessor(1, std::move(plus_scalar));
        chain.connect(0, 0, 1, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        chain.release();
    }
    
    SECTION("Fanning Inlet process")
    {
        Chain chain;
        
        std::unique_ptr<Processor> sig_1(new Sig(1.));
        std::unique_ptr<Processor> sig_2(new Sig(2.));
        std::unique_ptr<Processor> sig_3(new Sig(3.));
        
        std::unique_ptr<Processor> plus_scalar_0(new PlusScalar(0.));
        
        chain.addProcessor(0, std::move(sig_1));
        chain.addProcessor(1, std::move(sig_2));
        chain.addProcessor(2, std::move(sig_3));
        chain.addProcessor(3, std::move(plus_scalar_0));
        
        chain.connect(0, 0, 3, 0);
        chain.connect(1, 0, 3, 0);
        chain.connect(2, 0, 3, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        chain.release();
    }
    
    SECTION("empty Chain compiled")
    {
        Chain chain;
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        chain.release();
    }
    
    SECTION("Loop Detected")
    {
        Chain chain;
        
        std::unique_ptr<Processor> sig1(new Sig(1.3));
        std::unique_ptr<Processor> sig2(new Sig(2.7));
        std::unique_ptr<Processor> plus_scalar(new PlusScalar(1.));
        std::unique_ptr<Processor> plus_signal(new PlusSignal());
        
        chain.addProcessor(0, std::move(sig1));
        chain.addProcessor(1, std::move(sig2));
        chain.addProcessor(2, std::move(plus_scalar));
        chain.addProcessor(3, std::move(plus_signal));
        
        chain.connect(0, 0, 2, 0);
        chain.connect(2, 0, 3, 0);
        chain.connect(1, 0, 3, 1);
        
        REQUIRE_THROWS_AS(chain.connect(3, 0, 2, 0);, Error);
    }
    
    SECTION("Prepare after updates")
    {
        Chain chain;
        
        chain.addProcessor(0, std::move(std::unique_ptr<Processor>(new Sig(1.))));
        chain.addProcessor(2, std::move(std::unique_ptr<Processor>(new PlusSignal())));
        std::string result;
        chain.addProcessor(3, std::move(std::unique_ptr<Processor>(new Print(result))));
        
        chain.connect(0, 0, 2, 0);
        chain.connect(2, 0, 3, 0);
        
        chain.prepare(samplerate, 4ul);
        
        chain.tick();
        
        CHECK(result == "[1.000000, 1.000000, 1.000000, 1.000000]");
        
        // Adding a processors
        
        chain.addProcessor(1, std::move(std::unique_ptr<Processor>(new Sig(2.))));
        
        chain.prepare();
        
        chain.tick();
        
        CHECK(result == "[1.000000, 1.000000, 1.000000, 1.000000]");
        
        // Connecting processor
        
        chain.connect(1, 0, 2, 1);
        
        chain.prepare();
        
        chain.tick();
        
        CHECK(result == "[3.000000, 3.000000, 3.000000, 3.000000]");
        
        // Disconnecting processor
        
        chain.discconnect(0, 0, 2, 0);
        
        chain.prepare();
        
        chain.tick();
        
        CHECK(result == "[2.000000, 2.000000, 2.000000, 2.000000]");
        
        // Removing processor
        
        chain.removeProcessor(2);
        
        chain.prepare();
        
        chain.tick();
        
        CHECK(result == "[0.000000, 0.000000, 0.000000, 0.000000]");
        
        chain.release();
    }
    
    SECTION("Chain compile failed - Processor already used")
    {
        ;
    }
    
    SECTION("Processor Throw Based on Infos")
    {
        Chain chain;
        
        std::unique_ptr<Processor> sig1(new Sig(1.3));
        std::unique_ptr<Processor> sig2(new Sig(2.7));
        std::unique_ptr<Processor> plus_scalar(new PlusScalar(1.));
        std::unique_ptr<Processor> plus_signal(new PlusSignal());
        
        std::unique_ptr<Processor> copy_throw(new CopyThrow());
        
        chain.addProcessor(0, std::move(sig1));
        chain.addProcessor(1, std::move(sig2));
        chain.addProcessor(2, std::move(plus_scalar));
        chain.addProcessor(3, std::move(plus_signal));
        
        chain.connect(0, 0, 2, 0);
        chain.connect(2, 0, 3, 0);
        chain.connect(1, 0, 3, 1);
        
        chain.addProcessor(4, std::move(copy_throw));
        chain.connect(3, 0, 4, 0);
        
        REQUIRE_THROWS_AS(chain.prepare(samplerate, 128ul), Error);
        
        chain.release();
    }
    
    SECTION("Chain tick - WIP")
    {
        Chain chain;
        
        std::unique_ptr<Processor> sig(new Sig(1.));
        std::unique_ptr<Processor> plus(new PlusScalar(19.));
        std::string result;
        std::unique_ptr<Processor> print(new Print(result));
        
        chain.addProcessor(0, std::move(sig));
        chain.addProcessor(1, std::move(plus));
        chain.addProcessor(2, std::move(print));
        
        chain.connect(0, 0, 1, 0);
        chain.connect(1, 0, 2, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, 4ul));
        
        chain.tick();
        chain.tick();
        
        CHECK(result == "[20.000000, 20.000000, 20.000000, 20.000000]");
        
        chain.release();
    }
    
    SECTION("Chain tick - fanning inlet (inlet add)")
    {
        Chain chain;
        
        std::unique_ptr<Processor> sig_1(new Sig(1.));
        std::unique_ptr<Processor> sig_2(new Sig(2.));
        std::unique_ptr<Processor> sig_3(new Sig(3.));
        std::string result;
        std::unique_ptr<Processor> print(new Print(result));
        
        chain.addProcessor(1, std::move(sig_1));
        chain.addProcessor(2, std::move(sig_2));
        chain.addProcessor(3, std::move(sig_3));
        chain.addProcessor(4, std::move(print));
        
        chain.connect(1, 0, 4, 0);
        chain.connect(2, 0, 4, 0);
        chain.connect(3, 0, 4, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, 4ul));
        
        chain.tick();
        chain.tick();
        
        CHECK(result == "[6.000000, 6.000000, 6.000000, 6.000000]");
        
        chain.release();
    }
    
    SECTION("Chain tick - fanning outlet (outlet copy)")
    {
        Chain chain;
        
        std::unique_ptr<Processor> sig(new Sig(1.111111));
        
        std::string result_1;
        std::unique_ptr<Processor> print_1(new Print(result_1));
        std::string result_2;
        std::unique_ptr<Processor> print_2(new Print(result_2));
        std::string result_3;
        std::unique_ptr<Processor> print_3(new Print(result_3));
        
        chain.addProcessor(0, std::move(sig));
        chain.addProcessor(1, std::move(print_1));
        chain.addProcessor(2, std::move(print_2));
        chain.addProcessor(3, std::move(print_3));
        
        chain.connect(0, 0, 1, 0);
        chain.connect(0, 0, 2, 0);
        chain.connect(0, 0, 3, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, 4ul));
        
        chain.tick();
        chain.tick();
        
        CHECK(result_1 == "[1.111111, 1.111111, 1.111111, 1.111111]");
        CHECK(result_1 == result_2);
        CHECK(result_1 == result_3);
        
        chain.release();
    }
    
    SECTION("Chain tick - count example")
    {
        Chain chain;
        
        std::unique_ptr<Processor> count(new Count());
        
        std::string result;
        std::unique_ptr<Processor> print(new Print(result));
        
        chain.addProcessor(0, std::move(count));
        chain.addProcessor(1, std::move(print));
        
        chain.connect(0, 0, 1, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, 4ul));
        
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
        Chain chain;
        
        std::unique_ptr<Processor> count(new Count());
        std::unique_ptr<Processor> plus(new PlusSignal());
        std::string result;
        std::unique_ptr<Processor> print(new Print(result));
        
        chain.addProcessor(0, std::move(count));
        chain.addProcessor(1, std::move(plus));
        chain.addProcessor(2, std::move(print));
        
        chain.connect(0, 0, 1, 0);
        chain.connect(0, 0, 1, 1);
        chain.connect(1, 0, 2, 0);;
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, 4ul));
        
        chain.tick();
        
        CHECK(result == "[0.000000, 2.000000, 4.000000, 6.000000]");
        
        chain.tick();
        
        CHECK(result == "[8.000000, 10.000000, 12.000000, 14.000000]");
        
        chain.tick();
        
        CHECK(result == "[16.000000, 18.000000, 20.000000, 22.000000]");
        
        chain.release();
    }
}
