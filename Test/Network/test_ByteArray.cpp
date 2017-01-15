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

#include <KiwiNetwork/KiwiNetwork_ByteArray.hpp>

using namespace kiwi;

TEST_CASE("Constructor, Destructor, Operators", "[ByteArray]")
{
    SECTION("Default constructor")
    {
        network::ByteArray array;
        CHECK(array.getSize() == 0);
        CHECK(array.getData() == nullptr);
    }
    
    SECTION("Fill constructor")
    {
        network::ByteArray array(5);
        CHECK(array.getSize() == 5);
        
        for(size_t i = 0; i < array.getSize(); ++i)
        {
            CHECK(array[i] == '\0');
        }
    }
    
    SECTION("Copy constructor")
    {
        network::ByteArray array_1(5);
        array_1[0] = 'a';
        network::ByteArray array_2(array_1);
        
        CHECK(array_1.getSize() == array_2.getSize());
        
        for(size_t i = 0; i < array_1.getSize(); ++i)
        {
            CHECK(array_1[i] == array_2[i]);
        }
    }
    
    SECTION("Equality operators")
    {
        network::ByteArray array_1(5);
        network::ByteArray array_2(5);
        
        array_1[0] = 'a';
        array_2[0] = 'a';
        
        CHECK(array_1 == array_2);
        CHECK(!(array_1 != array_2));
        
        array_2[3] = 'c';
        
        CHECK(!(array_1 == array_2));
        CHECK(array_1 != array_2);
    }
    
    SECTION("Assignment operator")
    {
        network::ByteArray array_1(5);
        network::ByteArray array_2(3);
        
        array_1[3] = 'd';
        array_2 = array_1;
        CHECK(array_2 == array_1);
        REQUIRE_THROWS_AS(array_2[5], network::ByteArrayError);
    }
}

TEST_CASE("Writting, reading data", "[ByteArray]")
{
    SECTION("Writting and reading bytes")
    {
        network::ByteArray array(4);
        array[0] = 'a';
        array[1] = 'z';
        
        char* buffer_write = static_cast<char*>(malloc(3 * sizeof(char)));
        
        buffer_write[0] = 'b';
        buffer_write[1] = 'c';
        buffer_write[2] = 'd';
        
        REQUIRE_THROWS_AS(array.writeData(buffer_write, 3, 2), network::ByteArrayError);
        
        array.writeData(buffer_write, 1, 3);
        
        char* buffer_read = static_cast<char*>(malloc(2 * sizeof(char)));
        
        REQUIRE_THROWS_AS(array.readData(buffer_read, 2, 3), network::ByteArrayError);
        
        array.readData(buffer_read, 2, 2);
        
        CHECK(buffer_read[0] == 'c');
        CHECK(buffer_read[1] == 'd');
        
        std::free(buffer_write);
        std::free(buffer_read);
    }
    
    SECTION("Writting, Reading integer values")
    {
        network::ByteArray array(sizeof(uint64_t));
        
        uint64_t integer_write = 12;
        
        array.writeData(&integer_write, 0, sizeof(uint64_t));
        
        uint64_t integer_read = 0;
        
        array.readData(&integer_read, 0,  sizeof(uint64_t));
        
        CHECK(integer_read == 12);
        
    }
    
    SECTION("Using string and stream operators")
    {
        network::ByteArray array(2);
        array[0] = 'h';
        array[1] = 'e';
        array << "llo world";
        
        std::string read_string = "Eliott says ";
        array >> read_string;
        
        CHECK(read_string == "Eliott says hello world");
    }
}
