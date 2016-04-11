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

#include "../../../Modules/KiwiModel/KiwiModel.hpp"
#include "../../../Modules/KiwiController/KiwiController.hpp"
#include "../../catch.hpp"

using namespace kiwi;

TEST_CASE("Patcher - testcase : counter", "[Patcher]")
{
    auto instance = controller::Instance::create(123456789ULL, "kiwi");
    //instance->setDebug(true);
    
    auto& patcher = instance->createPatcher();
    
    const std::vector<Atom> bang_msg{"bang"};
    
    patcher.beginTransaction("Add two plus objects, a print object, and link them together");
    
    auto plus_obj_1 = patcher.addPlus();
    auto plus_obj_2 = patcher.addPlus();
    auto print_obj  = patcher.addPrint();
    
    auto plus_link_id = patcher.addLink(plus_obj_1, 0, plus_obj_2, 0);
    auto print_link_id = patcher.addLink(plus_obj_2, 0, print_obj, 0);
    
    patcher.endTransaction();
    
    patcher.sendToObject(plus_obj_1, 0, bang_msg);
    
    CHECK(patcher.getNumberOfObjects() == 3);
    CHECK(patcher.getNumberOfLinks() == 2);
    
    patcher.beginTransaction("remove object plus 2 (and links too)");
    patcher.removeObject(plus_obj_2);
    patcher.endTransaction();
    CHECK(patcher.getNumberOfObjects() == 2);
    CHECK(patcher.getNumberOfLinks() == 0);
    
    patcher.undo(true);
    CHECK(patcher.getNumberOfObjects() == 3);
    CHECK(patcher.getNumberOfLinks() == 2);
    
    patcher.sendToObject(plus_obj_1, 0, {92});
    
    // test stack overflow :
    patcher.beginTransaction("add recursive link");
    patcher.addLink(plus_obj_2, 0, plus_obj_1, 0);
    patcher.endTransaction();
    
    patcher.sendToObject(plus_obj_2, 1, {1});
    patcher.sendToObject(plus_obj_1, 0, {10});
}
