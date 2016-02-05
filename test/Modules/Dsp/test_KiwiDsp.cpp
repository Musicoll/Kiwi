//
//  main.cpp
//  KiwiDspTest
//
//  Created by Guillot Pierre on 02/02/2016.
//  Copyright © 2016 CICM. All rights reserved.
//

#define KIWI_DSP_FLOAT
#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"
#include "../../../Modules/KiwiDsp/KiwiDspSignal.hpp"

using namespace kiwi::dsp;

static inline bool is_equal(size_t const size, sample const& value, sample* in) noexcept
{
    for(size_t i = 0; i < size; ++i)
    {
        if(*in++ != value)
        {
            return false;
        }
    }
    return true;
}

TEST_CASE("Samples Class Tests", "[Samples]")
{
    size_t size = 512;
    sample* vec = Samples<sample>::allocate(size);
    REQUIRE(vec != nullptr);
    
    SECTION("Initialization Section")
    {
        SECTION("Fill")
        {
            Samples<sample>::fill(size, 1.2f, vec);
            CHECK(is_equal(size, 1.2f, vec));
        }
        
        SECTION("Clear")
        {
            Samples<sample>::clear(size, vec);
            CHECK(is_equal(size, 0.f, vec));
        }
    }
    
    SECTION("Algebra Section")
    {
        ;
    }
    
    vec = Samples<sample>::release(vec);
    REQUIRE(vec == nullptr);
}
