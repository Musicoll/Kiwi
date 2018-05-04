/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
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

#include <KiwiDsp/KiwiDsp_Chain.h>
#include <KiwiDsp/KiwiDsp_Misc.h>

#include "Processors.h"

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
        const size_t sr = 2ul;
        const size_t vs = 2ul;
        
        std::shared_ptr<Processor> sig_1(new Sig(1.));
        std::shared_ptr<Processor> plus_scalar(new PlusScalar(2.));
        
        chain.addProcessor(sig_1);
        chain.addProcessor(plus_scalar);
        
        chain.connect(*sig_1, 0, *plus_scalar, 0);
        
        // Adding processor twice
        chain.addProcessor(sig_1);
        REQUIRE_THROWS_AS(chain.prepare(sr, vs), Error);
        
        std::shared_ptr<Processor> scalar_out(new PlusScalar(0.));
        
        // Connect with non existing node.
        chain.connect(*sig_1, 0, *scalar_out, 0);
        REQUIRE_THROWS_AS(chain.prepare(sr, vs), Error);
        
        // Disconnect non existing node
        chain.disconnect(*sig_1, 0, *scalar_out, 0);
        REQUIRE_THROWS_AS(chain.prepare(sr, vs), Error);
        
        chain.release();
    }
    
    SECTION("Chain compiled")
    {
        Chain chain;
        
        std::shared_ptr<Processor> sig_1(new Sig(1.3));
        std::shared_ptr<Processor> plus_scalar(new PlusScalar(1.));
        
        chain.addProcessor(sig_1);
        chain.addProcessor(plus_scalar);
        chain.connect(*sig_1, 0, *plus_scalar, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        chain.release();
    }
    
    SECTION("Fanning Inlet process")
    {
        Chain chain;
        
        std::shared_ptr<Processor> sig_1(new Sig(1.));
        std::shared_ptr<Processor> sig_2(new Sig(2.));
        std::shared_ptr<Processor> sig_3(new Sig(3.));
        
        std::shared_ptr<Processor> plus_scalar_0(new PlusScalar(0.));
        
        chain.addProcessor(sig_1);
        chain.addProcessor(sig_2);
        chain.addProcessor(sig_3);
        chain.addProcessor(plus_scalar_0);
        
        chain.connect(*sig_1, 0, *plus_scalar_0, 0);
        chain.connect(*sig_2, 0, *plus_scalar_0, 0);
        chain.connect(*sig_3, 0, *plus_scalar_0, 0);
        
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
        
        std::shared_ptr<Processor> sig_1(new Sig(1.3));
        std::shared_ptr<Processor> sig_2(new Sig(2.7));
        std::shared_ptr<Processor> plus_scalar(new PlusScalar(1.));
        std::shared_ptr<Processor> plus_signal(new PlusSignal());
        
        chain.addProcessor(sig_1);
        chain.addProcessor(sig_2);
        chain.addProcessor(plus_scalar);
        chain.addProcessor(plus_signal);
        
        chain.connect(*sig_1, 0, *plus_scalar, 0);
        chain.connect(*plus_scalar, 0, *plus_signal, 0);
        chain.connect(*sig_2, 0, *plus_signal, 1);
        
        // Adding a connnection that creates a loop
        chain.connect(*plus_signal, 0, *plus_scalar, 0);
        
        REQUIRE_THROWS_AS(chain.prepare(0, 0), LoopError);
    }
    
    SECTION("non-connected inlets and outlets share same signals")
    {   
        Chain chain;
        
        std::shared_ptr<Processor> sig_1(new Sig(1.));
        std::shared_ptr<Processor> checker_1(new SharedSignalsChecker());
        std::shared_ptr<Processor> checker_2(new SharedSignalsChecker());
        
        chain.addProcessor(sig_1);
        chain.addProcessor(checker_1);
        chain.addProcessor(checker_2);
        
        chain.connect(*sig_1, 0, *checker_1, 1);
        chain.connect(*checker_1, 1, *checker_2, 1);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        chain.tick();
        
        Buffer const& in_1 = *dynamic_cast<SharedSignalsChecker&>(*checker_1).m_input;
        Buffer const& out_1 = *dynamic_cast<SharedSignalsChecker&>(*checker_1).m_output;
        Buffer const& in_2 = *dynamic_cast<SharedSignalsChecker&>(*checker_2).m_input;
        Buffer const& out_2 = *dynamic_cast<SharedSignalsChecker&>(*checker_2).m_output;
        
        // check that all unconnected inlet share the same signal pointer.
        CHECK(in_1[0].data() == in_1[2].data());
        CHECK(in_1[0].data() == in_2[0].data());
        CHECK(in_1[0].data() == in_2[2].data());
        // different than connected inlet signal
        CHECK_FALSE(in_1[0].data() == in_1[1].data());
        
        // check that all unconnected outlet of same index share the same signal pointer.
        CHECK(out_1[0].data() == out_2[0].data());
        CHECK(out_1[2].data() == out_2[2].data());
        CHECK_FALSE(out_1[0].data() == out_2[2].data());
        CHECK_FALSE(out_1[1].data() == out_2[1].data());
        
        REQUIRE_NOTHROW(chain.release());
    }
    
    SECTION("Updating after first prepare")
    {
        Chain chain;
        const size_t sr = 2ul;
        const size_t vs = 4ul;
        
        std::shared_ptr<Processor> sig_1(new Sig(1.));
        std::shared_ptr<Processor> sig_2(new Sig(2.));
        std::shared_ptr<Processor> plus_signal(new PlusSignal());
        std::string result;
        std::shared_ptr<Processor> print(new Print(result));
        
        chain.addProcessor(sig_1);
        chain.addProcessor(plus_signal);
        chain.addProcessor(print);
        
        chain.connect(*sig_1, 0, *plus_signal, 0);
        chain.connect(*plus_signal, 0, *print, 0);
        
        chain.prepare(sr, vs);
        
        chain.tick();
        
        CHECK(result == "[1.000000, 1.000000, 1.000000, 1.000000]");
        
        // Adding a processors
        
        chain.addProcessor(sig_2);
        
        chain.update();
        
        chain.tick();
        
        CHECK(result == "[1.000000, 1.000000, 1.000000, 1.000000]");
        
        // Connecting processor
        
        chain.connect(*sig_2, 0, *plus_signal, 1);
        
        chain.update();
        
        chain.tick();
        
        CHECK(result == "[3.000000, 3.000000, 3.000000, 3.000000]");
        
        // Disconnecting processor
        
        chain.disconnect(*sig_1, 0, *plus_signal, 0);
        
        chain.update();
        
        chain.tick();
        
        CHECK(result == "[2.000000, 2.000000, 2.000000, 2.000000]");
        
        // Removing processor
        
        chain.removeProcessor(*plus_signal);
        
        chain.update();
        
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
        
        std::shared_ptr<Processor> sig_1(new Sig(1.3));
        std::shared_ptr<Processor> sig_2(new Sig(2.7));
        std::shared_ptr<Processor> plus_scalar(new PlusScalar(1.));
        std::shared_ptr<Processor> plus_signal(new PlusSignal());
        
        std::shared_ptr<Processor> copy_throw(new CopyThrow());
        
        chain.addProcessor(sig_1);
        chain.addProcessor(sig_2);
        chain.addProcessor(plus_scalar);
        chain.addProcessor(plus_signal);
        
        chain.connect(*sig_1, 0, *plus_scalar, 0);
        chain.connect(*plus_scalar, 0, *plus_signal, 0);
        chain.connect(*sig_2, 0, *plus_signal, 1);
        
        chain.addProcessor(copy_throw);
        chain.connect(*plus_signal, 0, *copy_throw, 0);
        
        REQUIRE_THROWS_AS(chain.prepare(samplerate, 128ul), Error);
        
        chain.release();
    }
    
    SECTION("Chain tick - WIP")
    {
        Chain chain;
        
        std::shared_ptr<Processor> sig_1(new Sig(1.));
        std::shared_ptr<Processor> plus(new PlusScalar(19.));
        std::string result;
        std::shared_ptr<Processor> print(new Print(result));
        
        chain.addProcessor(sig_1);
        chain.addProcessor(plus);
        chain.addProcessor(print);
        
        chain.connect(*sig_1, 0, *plus, 0);
        chain.connect(*plus, 0, *print, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, 4ul));
        
        chain.tick();
        chain.tick();
        
        CHECK(result == "[20.000000, 20.000000, 20.000000, 20.000000]");
        
        chain.release();
    }
    
    SECTION("Chain tick - fanning inlet (inlet add)")
    {
        Chain chain;
        
        std::shared_ptr<Processor> sig_1(new Sig(1.));
        std::shared_ptr<Processor> sig_2(new Sig(2.));
        std::shared_ptr<Processor> sig_3(new Sig(3.));
        std::string result;
        std::shared_ptr<Processor> print(new Print(result));
        
        chain.addProcessor(sig_1);
        chain.addProcessor(sig_2);
        chain.addProcessor(sig_3);
        chain.addProcessor(print);
        
        chain.connect(*sig_1, 0, *print, 0);
        chain.connect(*sig_2, 0, *print, 0);
        chain.connect(*sig_3, 0, *print, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, 4ul));
        
        chain.tick();
        
        CHECK(result == "[6.000000, 6.000000, 6.000000, 6.000000]");
        
        chain.release();
    }
    
    SECTION("Chain tick - fanning outlet (outlet copy)")
    {
        Chain chain;
        
        std::shared_ptr<Processor> sig(new Sig(1.111111));
        
        std::string result_1;
        std::shared_ptr<Processor> print_1(new Print(result_1));
        std::string result_2;
        std::shared_ptr<Processor> print_2(new Print(result_2));
        std::string result_3;
        std::shared_ptr<Processor> print_3(new Print(result_3));
        
        chain.addProcessor(sig);
        chain.addProcessor(print_1);
        chain.addProcessor(print_2);
        chain.addProcessor(print_3);
        
        chain.connect(*sig, 0, *print_1, 0);
        chain.connect(*sig, 0, *print_2, 0);
        chain.connect(*sig, 0, *print_3, 0);
        
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
        
        std::shared_ptr<Processor> count(new Count());
        
        std::string result;
        std::shared_ptr<Processor> print(new Print(result));
        
        chain.addProcessor(count);
        chain.addProcessor(print);
        
        chain.connect(*count, 0, *print, 0);
        
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
        
        std::shared_ptr<Processor> count(new Count());
        std::shared_ptr<Processor> plus(new PlusSignal());
        std::string result;
        std::shared_ptr<Processor> print(new Print(result));
        
        chain.addProcessor(count);
        chain.addProcessor(plus);
        chain.addProcessor(print);
        
        chain.connect(*count, 0, *plus, 0);
        chain.connect(*count, 0, *plus, 1);
        chain.connect(*plus, 0, *print, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, 4ul));
        
        chain.tick();
        
        CHECK(result == "[0.000000, 2.000000, 4.000000, 6.000000]");
        
        chain.tick();
        
        CHECK(result == "[8.000000, 10.000000, 12.000000, 14.000000]");
        
        chain.tick();
        
        CHECK(result == "[16.000000, 18.000000, 20.000000, 22.000000]");
        
        chain.release();
    }
    
    SECTION("Chain processor without perform")
    {
        Chain chain;
        
        std::shared_ptr<Processor> sig1(new Sig(1));
        std::shared_ptr<Processor> sig2(new Sig(2));
        std::shared_ptr<Processor> plus_remove(new PlusScalarRemover());
        std::string result;
        std::shared_ptr<Processor> print(new Print(result));
        
        chain.addProcessor(sig1);
        chain.addProcessor(sig2);
        chain.addProcessor(plus_remove);
        chain.addProcessor(print);
        
        chain.connect(*sig2, 0, *plus_remove, 1);
        chain.connect(*plus_remove, 0, *print, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, 4ul));
        
        chain.tick();
        
        CHECK(result == "[0.000000, 0.000000, 0.000000, 0.000000]");
        
        chain.connect(*sig1, 0, *plus_remove, 0);
    
        chain.update();
        
        chain.tick();
        
        CHECK(result == "[3.000000, 3.000000, 3.000000, 3.000000]");
        
        chain.release();
    }
}
