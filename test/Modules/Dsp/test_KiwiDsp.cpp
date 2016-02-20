//
//  main.cpp
//  KiwiDspTest
//
//  Created by Guillot Pierre on 02/02/2016.
//  Copyright © 2016 CICM. All rights reserved.
//

#include "test_class.hpp"
#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"

#define SAMPLE_RATE 44100ul
#define VECTOR_SIZE 64ul

TEST_CASE("Chain", "[Chain]")
{
    std::vector<Processor*> processes;
    std::vector<Link*> links;
    
    std::unique_ptr<Processor> sig1(new Sig(1.3f));
    std::unique_ptr<Processor> sig2(new Sig(2.7f));
    std::unique_ptr<Processor> plus_scalar(new PlusScalar(1.f));
    std::unique_ptr<Processor> plus_signal(new PlusSignal());
    
    
    std::unique_ptr<Link> link1(new Link(*sig1.get(), 0, *plus_scalar.get(), 0));
    std::unique_ptr<Link> link2(new Link(*plus_scalar.get(), 0, *plus_signal.get(), 0));
    std::unique_ptr<Link> link3(new Link(*sig2.get(), 0, *plus_signal.get(), 1));
    std::unique_ptr<Link> link_loop(new Link(*plus_signal.get(), 0, *plus_scalar.get(), 1));
    std::unique_ptr<Link> link_false(new Link(*sig2.get(), 0, *plus_scalar.get(), 1));
    
    SECTION("Wrong Link")
    {
        Chain chain;
        processes.push_back(sig1.get());
        processes.push_back(sig2.get());
        processes.push_back(plus_scalar.get());
        processes.push_back(plus_signal.get());
        links.push_back(link1.get());
        links.push_back(link2.get());
        links.push_back(link3.get());
        links.push_back(link_false.get());
        REQUIRE_THROWS_AS(chain.compile(44100ul, 64ul, processes, links), Error);
    }
    
    SECTION("Processor Already Used")
    {
        Chain chain1, chain2;
        processes.push_back(sig1.get());
        processes.push_back(sig2.get());
        processes.push_back(plus_scalar.get());
        processes.push_back(plus_signal.get());
        links.push_back(link1.get());
        links.push_back(link2.get());
        links.push_back(link3.get());
        REQUIRE_NOTHROW(chain1.compile(44100ul, 64ul, processes, links));
        REQUIRE_THROWS_AS(chain2.compile(44100ul, 64ul, processes, links), Error);
    }
    
    SECTION("Loop Detected")
    {
        Chain chain;
        processes.push_back(sig1.get());
        processes.push_back(sig2.get());
        processes.push_back(plus_scalar.get());
        processes.push_back(plus_signal.get());
        links.push_back(link1.get());
        links.push_back(link2.get());
        links.push_back(link3.get());
        links.push_back(link_loop.get());
        REQUIRE_THROWS_AS(chain.compile(44100ul, 64ul, processes, links), Error);
    }
    
    SECTION("Chain Compiled")
    {
        Chain chain;
        processes.push_back(sig1.get());
        processes.push_back(sig2.get());
        processes.push_back(plus_scalar.get());
        processes.push_back(plus_signal.get());
        links.push_back(link1.get());
        links.push_back(link2.get());
        links.push_back(link3.get());
        
        REQUIRE_NOTHROW(chain.compile(44100ul, 64ul, processes, links));
    }
}

