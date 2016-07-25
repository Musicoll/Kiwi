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

#include <KiwiDsp/KiwiDsp_Buffer.hpp>

using namespace kiwi;
using namespace dsp;

// ================================================================================ //
//                                       SIGNAL                                     //
// ================================================================================ //

TEST_CASE("Dsp - Signal", "[Dsp, Signal]")
{
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
    
    SECTION("Signal - copy ctor")
    {
        const size_t size = 64;
        Signal sig1(size, 0.5);
        Signal sig2 = sig1;
        
        CHECK(sig1.size() == sig2.size());
        
        for(int j = 0; j < size; ++j)
        {
            CHECK(sig1[j] == sig2[j]);
        }
    }
    
    SECTION("Signal - empty ctor and copy")
    {
        Signal sig1;
        Signal sig2(64, 0.5);
        sig2 = sig1;
        
        CHECK(sig1.size() == 0ul);
        CHECK(sig1.data() == nullptr);
        
        CHECK(sig2.size() == 0ul);
        CHECK(sig2.data() == nullptr);
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
    
    SECTION("Signal - swap")
    {
        const size_t size = 64;
        Signal sig1(size, 0.5);
        Signal sig2(size, 1.);
        
        std::swap(sig1, sig2);
        
        for(int j = 0; j < size; ++j)
        {
            CHECK(sig1[j] == 1.);
            CHECK(sig2[j] == 0.5);
        }
    }
    
    SECTION("Signal - swap (different size)")
    {
        Signal sig1(64, 0.5);
        Signal sig2(128, 1.);
        
        std::swap(sig1, sig2);
        
        for(int j = 0; j < 64; ++j)
        {
            CHECK(sig2[j] == 0.5);
        }
        
        for(int j = 0; j < 128; ++j)
        {
            CHECK(sig1[j] == 1.);
        }
        
        std::swap(sig1, sig2);
        
        for(int j = 0; j < 64; ++j)
        {
            CHECK(sig1[j] == 0.5);
        }
        
        for(int j = 0; j < 128; ++j)
        {
            CHECK(sig2[j] == 1.);
        }
    }
    
    SECTION("Signal::fillWith(sample_t const&)")
    {
        const size_t size = 64;
        Signal sig(size);
        sig.fillWith(1.);
        
        for(int j = 0; j < size; ++j)
        {
            CHECK(sig[j] == 1.);
        }
    }
    
    SECTION("Signal - add")
    {
        const size_t size = 64;
        Signal sig1(size, 0.5);
        Signal sig2(size, 1.5);
        Signal result = Signal::add(sig1, sig2);
        
        for(int j = 0; j < result.size(); ++j)
        {
            CHECK(result[j] == 2.);
        }
    }
}
