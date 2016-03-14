/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

// Catch main function

#define CATCH_CONFIG_RUNNER
#include "../../catch.hpp"

int main( int argc, char* const argv[] )
{
    // global setup...
    std::cout << "running Unit-Tests - Kiwi Model ..." << '\n'
    << "*************************************\n\n";
    
    int result = Catch::Session().run( argc, argv );
    
    // global clean-up...
    
    return result;
}
