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

#include "../catch.hpp"

#include <KiwiDsp/KiwiDsp_Signal.h>

using namespace kiwi;
using namespace dsp;

// ================================================================================ //
//                                       SIGNAL                                     //
// ================================================================================ //

TEST_CASE("Dsp - Signal", "[Dsp, Signal]")
{
    SECTION("Signal ctor - throws as bad alloc")
    {
        REQUIRE_THROWS_AS(Signal(-1), std::bad_alloc);
    }
    
    SECTION("Signal - move ctor")
    {
        const size_t size = 64;
        Signal sig1(size, 1.);
        
        REQUIRE(sig1.size() == size);
        REQUIRE(sig1.data() != nullptr);
        
        Signal sig2 = std::move(sig1);
        
        REQUIRE(sig1.size() == 0ul);
        REQUIRE(sig1.data() == nullptr);
        
        REQUIRE(sig2.size() == size);
        REQUIRE(sig2.data() != nullptr);
        
        for(int j = 0; j < size; ++j)
        {
            CHECK(sig2[j] == 1.);
        }
    }
    
    SECTION("Signal - copy")
    {
        Signal sig1(64, 0.);
        Signal sig2(64, 0.5);
        sig2.copy(sig1);
        
        for(int j = 0; j < sig2.size(); ++j)
        {
            CHECK(sig2[j] == sig1[j]);
        }
    }
    
    SECTION("Signal - move assignment")
    {
        size_t size = 64;
        Signal sig1(size, 0.5);
        Signal sig2(size);
        sig2 = std::move(sig1);
        
        CHECK(sig1.size() == 0);
        CHECK(sig1.data() == nullptr);
        
        REQUIRE(sig2.size() == size);
        REQUIRE(sig2.data() != nullptr);
        
        for(int j = 0; j < size; ++j)
        {
            CHECK(sig2[j] == 0.5);
        }
    }
    
    SECTION("Signal - double move assignment support")
    {
        size_t size = 64;
        Signal sig1(size, 0.5);
        Signal sig2 = std::move(sig1);
        
        CHECK(sig1.size() == 0);
        CHECK(sig1.data() == nullptr);
        
        REQUIRE(sig2.size() == size);
        REQUIRE(sig2.data() != nullptr);
        
        for(int j = 0; j < size; ++j)
        {
            CHECK(sig2[j] == 0.5);
        }
        
        sig2 = std::move(sig1);
        
        CHECK(sig1.size() == 0);
        CHECK(sig1.data() == nullptr);
        CHECK(sig2.size() == 0);
        CHECK(sig2.data() == nullptr);
    }
    
    SECTION("Signal::fill(sample_t const&)")
    {
        const size_t size = 64;
        Signal sig(size);
        sig.fill(1.);
        
        for(int j = 0; j < size; ++j)
        {
            CHECK(sig[j] == 1.);
        }
    }
    
    SECTION("Signal - add += signal")
    {
        const size_t size = 64;
        
        Signal sig1(size);
        for (int i = 0; i < size; ++i)
        {
            sig1[i] = 4 * i;
        }
        
        Signal sig2(size, 1.);
        
        sig1.add(sig2);
        
        for(int j = 0; j < size; ++j)
        {
            CHECK(sig1[j] == 4 * j + 1.);
        }
    }
    
    SECTION("Signal - add +")
    {
        const size_t size = 64;
        
        Signal sig1(size);
        for (int i = 0; i < size; ++i)
        {
            sig1[i] = 4 * i;
        }
        
        Signal sig2(size, 1.);
        Signal result(size, 0);
        
        Signal::add(sig1, sig2, result);
        
        for(int j = 0; j < result.size(); ++j)
        {
            CHECK(result[j] == 4 * j + 1.);
        }
    }
}
