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

// ==================================================================================== //
//                                          BUFFER                                      //
// ==================================================================================== //

TEST_CASE("Dsp - Buffer", "[Dsp, Buffer]")
{
    SECTION("Buffer - Default empty ctor")
    {
        Buffer buffer;
        
        auto channels = buffer.getNumberOfChannels();
        auto vectorsize = buffer.getVectorSize();
        
        REQUIRE(channels == 0);
        REQUIRE(vectorsize == 0);
    }
    
    SECTION("Buffer - uninitialized signals ctor")
    {
        Buffer buffer(2);
        
        auto channels = buffer.getNumberOfChannels();
        auto vectorsize = buffer.getVectorSize();
        
        REQUIRE(channels == 2);
        REQUIRE(vectorsize == 0);
        
        CHECK(buffer[0].empty());
        CHECK(buffer[1].empty());
        CHECK(buffer[0].data() == nullptr);
        CHECK(buffer[1].data() == nullptr);
    }
    
    SECTION("Buffer - filled ctor")
    {
        Buffer buffer(2, 128, 1.);
        
        auto channels = buffer.getNumberOfChannels();
        auto vectorsize = buffer.getVectorSize();
        
        REQUIRE(channels == 2);
        REQUIRE(vectorsize == 128);
        
        for(int i = 0; i < channels; ++i)
        {
            for(int j = 0; j < vectorsize; ++j)
            {
                CHECK(buffer[i][j] == 1.);
            }
        }
    }
    
    SECTION("Buffer - move ctor")
    {
        Buffer buffer_1(2, 128, 1.);
        
        REQUIRE(buffer_1.getNumberOfChannels() == 2);
        REQUIRE(buffer_1.getVectorSize() == 128);
        
        Buffer buffer_2(std::move(buffer_1));
        
        REQUIRE(buffer_1.empty());
        REQUIRE(buffer_1.getNumberOfChannels() == 0);
        REQUIRE(buffer_1.getVectorSize() == 0);
        
        REQUIRE(buffer_2.getNumberOfChannels() == 2);
        REQUIRE(buffer_2.getVectorSize() == 128);
        
        auto channels = buffer_2.getNumberOfChannels();
        auto vectorsize = buffer_2.getVectorSize();
        
        for(int i = 0; i < channels; ++i)
        {
            for(int j = 0; j < vectorsize; ++j)
            {
                CHECK(buffer_2[i][j] == 1.);
            }
        }
    }
    
    SECTION("Test")
    {
        Buffer buffer(2, 128, 1.);
        
        auto channels = buffer.getNumberOfChannels();
        auto vectorsize = buffer.getVectorSize();
        
        REQUIRE(channels == 2);
        REQUIRE(vectorsize == 128);
        
        sample_t* left = buffer[0].data();
        sample_t* right = buffer[1].data();
        
        while(vectorsize--)
        {
            *left++ = 0.5;
            *right++ = 0.5;
        }
        
        for(int i = 0; i < channels; ++i)
        {
            for(int j = 0; j < buffer.getVectorSize(); ++j)
            {
                CHECK(buffer[i][j] == 0.5);
            }
        }
    }
}
