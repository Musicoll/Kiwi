//
//  main.cpp
//  KiwiDspTest
//
//  Created by Guillot Pierre on 02/02/2016.
//  Copyright © 2016 CICM. All rights reserved.
//

#include "../../../Modules/KiwiDsp/KiwiDsp.hpp"

using namespace kiwi::dsp;

// ==================================================================================== //
//                                  CLASSES FOR TESTS                                   //
// ==================================================================================== //

class Sig : public Processor
{
public:
    Sig(sample_t value) noexcept : m_value(value) {setNumberOfInlets(0); setNumberOfOutlets(1);}
    ~Sig()  noexcept {}
private:
    bool prepare(Infos const& infos) final {return infos.isOutputConnected(0ul);}
    
    void perform(Buffer const& /*input*/, Buffer& output) noexcept final{
        Samples< sample_t >::fill(output.getVectorSize(), m_value, output[0ul]);}
    sample_t m_value;
};

class PlusScalar : public Processor
{
public:
    PlusScalar(sample_t value) noexcept : m_value(value){setNumberOfInlets(1);setNumberOfOutlets(1);}
    ~PlusScalar() noexcept{}
private:
    bool prepare(Infos const& infos) final {
        return infos.isInputConnected(0ul) && infos.isOutputConnected(0ul);}
    
    void perform(Buffer const& input, Buffer& output) noexcept final {
        Samples< sample_t >::add(input.getVectorSize(), m_value, input[0ul], output[0ul]);}
    sample_t m_value;
};

class PlusSignal : public Processor
{
public:
    PlusSignal() noexcept {setNumberOfInlets(2); setNumberOfOutlets(1);}
    ~PlusSignal()  noexcept{}
private:
    bool prepare(Infos const& infos) final {
        return infos.isInputConnected(0ul) && infos.isInputConnected(1ul) && infos.isOutputConnected(0ul);}
    
    void perform(Buffer const& input, Buffer& output) noexcept final{
        Samples< sample_t >::add(input.getVectorSize(), input[0ul], input[1ul], output[0ul]);}
};

#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"

#define SAMPLE_RATE 44100ul
#define VECTOR_SIZE 64ul

// ==================================================================================== //
//                                          TESTS                                       //
// ==================================================================================== //
//@todo Link is duplicated and Processor is duplicated
TEST_CASE("Chain", "[Chain]")
{
    std::unique_ptr<Processor> sig1(new Sig(1.3f));
    std::unique_ptr<Processor> sig2(new Sig(2.7f));
    std::unique_ptr<Processor> plus_scalar(new PlusScalar(1.f));
    std::unique_ptr<Processor> plus_signal(new PlusSignal());
    
    
    std::unique_ptr<Link> link1(new Link(*sig1.get(), 0, *plus_scalar.get(), 0));
    std::unique_ptr<Link> link2(new Link(*plus_scalar.get(), 0, *plus_signal.get(), 0));
    std::unique_ptr<Link> link3(new Link(*sig2.get(), 0, *plus_signal.get(), 1));
    std::unique_ptr<Link> link_loop(new Link(*plus_signal.get(), 0, *plus_scalar.get(), 0));
    
    SECTION("Link Is Duplicated")
    {
        Chain chain;
        std::set<Processor*> processes;
        std::set<Link*> links;
        processes.insert(sig1.get());
        processes.insert(sig2.get());
        processes.insert(plus_scalar.get());
        processes.insert(plus_signal.get());
        links.insert(link1.get());
        links.insert(link2.get());
        links.insert(link3.get());
        links.insert(link2.get());
        REQUIRE_THROWS_AS(chain.compile(44100ul, 64ul, processes, links), Error);
    }
    
    SECTION("Processor Already Used")
    {
        Chain chain1, chain2;
        std::set<Processor*> processes;
        std::set<Link*> links;
        processes.insert(sig1.get());
        processes.insert(sig2.get());
        processes.insert(plus_scalar.get());
        processes.insert(plus_signal.get());
        links.insert(link1.get());
        links.insert(link2.get());
        links.insert(link3.get());
        REQUIRE_NOTHROW(chain1.compile(44100ul, 64ul, processes, links));
        REQUIRE_THROWS_AS(chain2.compile(44100ul, 64ul, processes, links), Error);
    }
    
    SECTION("Loop Detected")
    {
        Chain chain;
        std::set<Processor*> processes;
        std::set<Link*> links;
        processes.insert(sig1.get());
        processes.insert(sig2.get());
        processes.insert(plus_scalar.get());
        processes.insert(plus_signal.get());
        links.insert(link1.get());
        links.insert(link2.get());
        links.insert(link3.get());
        links.insert(link_loop.get());
        REQUIRE_THROWS_AS(chain.compile(44100ul, 64ul, processes, links), Error);
    }
    
    SECTION("Chain Compiled")
    {
        Chain chain;
        std::set<Processor*> processes;
        std::set<Link*> links;
        processes.insert(sig1.get());
        processes.insert(sig2.get());
        processes.insert(plus_scalar.get());
        processes.insert(plus_signal.get());
        links.insert(link1.get());
        links.insert(link2.get());
        links.insert(link3.get());
        
        REQUIRE_NOTHROW(chain.compile(44100ul, 64ul, processes, links));
    }
}

