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
#include <KiwiDsp/KiwiDsp_Misc.hpp>

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
}


TEST_CASE("Dsp - Chain ordering", "[Dsp, Chain]")
{
    using PutFirstProcessor = PassThrough<1, Processor::GraphOrder::PutFirst>;
    using PutLastProcessor = PassThrough<1, Processor::GraphOrder::PutLast>;
    using UnorderedProcessor = PassThrough<1, Processor::GraphOrder::Unordered>;
    
    const size_t samplerate = 2ul;
    const size_t vectorsize = 4ul;
    
    auto putfirst_1 = std::make_shared<PutFirstProcessor>();
    auto unordered_1 = std::make_shared<UnorderedProcessor>();
    auto putlast_1 = std::make_shared<PutLastProcessor>();
    
    Chain chain;
    
    SECTION("Chain - Get node index with a processor that does not exist returns 0")
    {
        chain.addProcessor(unordered_1);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        CHECK(chain.getNodeIndex(*unordered_1)  == 1);
        CHECK(chain.getNodeIndex(*putlast_1)    == 0);
        CHECK(chain.getNodeIndex(*putfirst_1)   == 0);
    }
    
    SECTION("Chain - Adding Unordered nodes sort them in adding order")
    {
        auto unordered_2 = std::make_shared<UnorderedProcessor>();
        auto unordered_3 = std::make_shared<UnorderedProcessor>();
        
        chain.addProcessor(unordered_1);
        chain.addProcessor(unordered_2);
        chain.addProcessor(unordered_3);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        CHECK(chain.getNodeIndex(*unordered_1) == 1);
        CHECK(chain.getNodeIndex(*unordered_2) == 2);
        CHECK(chain.getNodeIndex(*unordered_3) == 3);
    }
    
    SECTION("Chain - PutFirst nodes are always first")
    {
        chain.addProcessor(unordered_1);
        chain.addProcessor(putlast_1);
        chain.addProcessor(putfirst_1);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        CHECK(chain.getNodeIndex(*putfirst_1)   == 1);
        CHECK(chain.getNodeIndex(*putlast_1)    == 2);
        CHECK(chain.getNodeIndex(*unordered_1)  == 3);
    }
    
    SECTION("Chain - PutLast Nodes chaining")
    {
        auto putlast_2 = std::make_shared<PutLastProcessor>();
        auto putlast_3 = std::make_shared<PutLastProcessor>();
        
        chain.addProcessor(putlast_1);
        chain.addProcessor(putlast_2);
        chain.addProcessor(putlast_3);
        
        chain.connect(*putlast_1, 0, *putlast_2, 0);
        chain.connect(*putlast_2, 0, *putlast_3, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        CHECK(chain.getNodeIndex(*putlast_1)    == 1);
        CHECK(chain.getNodeIndex(*putlast_2)    == 2);
        CHECK(chain.getNodeIndex(*putlast_3)    == 3);
    }
    
    SECTION("Chain - PutFirst Nodes chaining")
    {
        auto putfirst_2 = std::make_shared<PutFirstProcessor>();
        auto putfirst_3 = std::make_shared<PutFirstProcessor>();
        
        chain.addProcessor(putfirst_1);
        chain.addProcessor(putfirst_2);
        chain.addProcessor(putfirst_3);
        
        chain.connect(*putfirst_1, 0, *putfirst_2, 0);
        chain.connect(*putfirst_2, 0, *putfirst_3, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        CHECK(chain.getNodeIndex(*putfirst_1)    == 1);
        CHECK(chain.getNodeIndex(*putfirst_2)    == 2);
        CHECK(chain.getNodeIndex(*putfirst_3)    == 3);
    }
    
    SECTION("Chain - PutFirst are indexed after upstream connected nodes")
    {
        chain.addProcessor(unordered_1);
        chain.addProcessor(putlast_1);
        chain.addProcessor(putfirst_1);
        
        chain.connect(*unordered_1, 0, *putfirst_1, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        CHECK(chain.getNodeIndex(*unordered_1)  == 1);
        CHECK(chain.getNodeIndex(*putfirst_1)   == 2);
        CHECK(chain.getNodeIndex(*putlast_1)    == 3);
    }
    
    SECTION("Chain - PutLast are indexed before downstream connected nodes")
    {
        chain.addProcessor(unordered_1);
        chain.addProcessor(putlast_1);
        chain.addProcessor(putfirst_1);
        
        chain.connect(*putlast_1, 0, *unordered_1, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        CHECK(chain.getNodeIndex(*putfirst_1)   == 1);
        CHECK(chain.getNodeIndex(*putlast_1)    == 2);
        CHECK(chain.getNodeIndex(*unordered_1)  == 3);
    }
    
    SECTION("Chain - PutFirst with PutFirst and Unordered Node example 1")
    {
        auto sig = std::make_shared<Sig>(1);
        
        chain.addProcessor(putlast_1);
        chain.addProcessor(putfirst_1);
        chain.addProcessor(sig);
        
        chain.connect(*sig, 0, *putfirst_1, 0);
        chain.connect(*putlast_1, 0, *putfirst_1, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        CHECK(chain.getNodeIndex(*sig)          == 1);
        CHECK(chain.getNodeIndex(*putfirst_1)   == 2);
        CHECK(chain.getNodeIndex(*putlast_1)    == 3);
    }
    
    SECTION("Chain - PutFirst with PutFirst and Unordered Node example 2")
    {
        auto sig = std::make_shared<Sig>(1);
        
        chain.addProcessor(putlast_1);
        chain.addProcessor(unordered_1);
        chain.addProcessor(putfirst_1);
        chain.addProcessor(sig);
        
        chain.connect(*sig, 0, *putfirst_1, 0);
        chain.connect(*putlast_1, 0, *unordered_1, 0);
        chain.connect(*unordered_1, 0, *putfirst_1, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vectorsize));
        
        CHECK(chain.getNodeIndex(*sig)          == 1);
        CHECK(chain.getNodeIndex(*putfirst_1)   == 2);
        CHECK(chain.getNodeIndex(*putlast_1)    == 3);
        CHECK(chain.getNodeIndex(*unordered_1)  == 4);
    }
    
    SECTION("Chain - PutLast PutFirst Loop")
    {
        chain.addProcessor(putlast_1);
        chain.addProcessor(putfirst_1);
        
        chain.connect(*putlast_1, 0, *putfirst_1, 0);
        chain.connect(*putfirst_1, 0, *putlast_1, 0);
        
        REQUIRE_THROWS_AS(chain.prepare(samplerate, vectorsize), Error);
        
        CHECK(chain.getNodeIndex(*putfirst_1)   == 0);
        CHECK(chain.getNodeIndex(*putlast_1)    == 0);
    }
    
    SECTION("Chain - Send Receive performing WITHOUT ordering")
    {
        const auto vs = 4ul;
        const auto signal_sptr = std::make_shared<Signal>(vs);
        
        // Unordered send and receive
        auto send = std::make_shared<Send>(signal_sptr, false);
        auto receive = std::make_shared<Receive>(signal_sptr, false);
        auto sig = std::make_shared<Sig>(1);
        
        std::string result;
        auto print = std::make_shared<Print>(result);
        
        // Adding unordered processors in a bad order
        chain.addProcessor(receive);
        chain.addProcessor(print);
        chain.addProcessor(sig);
        chain.addProcessor(send);
        
        chain.connect(*sig, 0, *send, 0);
        chain.connect(*receive, 0, *print, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vs));
        
        REQUIRE(chain.getNodeIndex(*receive)    == 1);
        REQUIRE(chain.getNodeIndex(*print)      == 2);
        REQUIRE(chain.getNodeIndex(*sig)        == 3);
        REQUIRE(chain.getNodeIndex(*send)       == 4);
        
        // introduce a block size of delay
        chain.tick();
        CHECK(result == "[0.000000, 0.000000, 0.000000, 0.000000]");
        
        chain.tick();
        CHECK(result == "[1.000000, 1.000000, 1.000000, 1.000000]");
    }
    
    SECTION("Chain - Send Receive performing WITH ordering")
    {
        const auto vs = 4ul;
        const auto signal_sptr = std::make_shared<Signal>(vs);
        
        // PutFirst Send and PutLast Receive
        auto send = std::make_shared<Send>(signal_sptr, true);
        auto receive = std::make_shared<Receive>(signal_sptr, true);
        auto sig = std::make_shared<Sig>(1);
        
        std::string result;
        auto print = std::make_shared<Print>(result);
        
        chain.addProcessor(receive);
        chain.addProcessor(print);
        chain.addProcessor(sig);
        chain.addProcessor(send);
        
        chain.connect(*sig, 0, *send, 0);
        chain.connect(*receive, 0, *print, 0);
        
        REQUIRE_NOTHROW(chain.prepare(samplerate, vs));
        
        REQUIRE(chain.getNodeIndex(*sig)        == 1);
        REQUIRE(chain.getNodeIndex(*send)       == 2);
        REQUIRE(chain.getNodeIndex(*receive)    == 3);
        REQUIRE(chain.getNodeIndex(*print)      == 4);
        
        chain.tick();
        
        // compute without introducing additionnal delay
        CHECK(result == "[1.000000, 1.000000, 1.000000, 1.000000]");
    }
    
    chain.release();
}
