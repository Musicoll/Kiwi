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

#include <KiwiModel/KiwiModel_Factory.h>

using namespace kiwi;

// ==================================================================================== //
//                                          FACTORY                                     //
// ==================================================================================== //

TEST_CASE("Model Factory", "[Names]")
{
    SECTION("One special character")
    {
        std::string kiwi_name = "¨";
        
        std::string model_name  = model::Factory::toModelName(kiwi_name);
        
        CHECK(kiwi_name == model::Factory::toKiwiName(model_name));
    }
    
    SECTION("More complex character chain")
    {
        std::string kiwi_name = "¨ç*$$sd$sdf$@#2342ééé";
        
        std::cout << kiwi_name.size() << std::endl;
        
        std::string model_name  = model::Factory::toModelName(kiwi_name);
        
        CHECK(kiwi_name == model::Factory::toKiwiName(model_name));
    }
}
