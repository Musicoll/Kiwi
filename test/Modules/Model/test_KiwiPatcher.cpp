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

#include "../../../Modules/KiwiEngine/KiwiEngine.hpp"
#include "../../catch.hpp"

using namespace kiwi;

TEST_CASE("Patcher", "[Patcher]")
{
    auto instance = engine::Instance::create(123456789ULL, "kiwi");
    instance->setDebug(true);
    
    auto& patcher = instance->createPatcher();
    
    const std::vector<Atom> bang_msg{"bang"};
    
    patcher.beginTransaction("Add two plus objects, and a print object");
    patcher.addPlus();
    patcher.addPlus();
    patcher.addPrint();
    patcher.endTransaction();
    
    auto objects = patcher.getObjects();
    REQUIRE(objects.size() == 3);
    
    patcher.beginTransaction("connect objects");
    patcher.addLink(*objects[0], 0, *objects[1], 0);
    patcher.addLink(*objects[1], 0, *objects[2], 0);
    patcher.endTransaction();
    
    REQUIRE(patcher.getLinks().size() == 2);
    
    patcher.sendToObject(*objects[0], 1, {10});
    patcher.sendToObject(*objects[0], 0, {10});
    
    patcher.beginTransaction("remove object plus 2 (and links too)");
    patcher.removeObject(*objects[1]);
    patcher.endTransaction();
    
    CHECK(patcher.getObjects().size() == 2);
    CHECK(patcher.getLinks().size() == 0);
    
    patcher.undo(true);
    REQUIRE(patcher.getObjects().size() == 3);
    REQUIRE(patcher.getLinks().size() == 2);
    
    objects = patcher.getObjects();
    
    // Warning object container index changed and is not coherent with model (to be fixed)
    patcher.beginTransaction("add recursive link");
    patcher.addLink(*objects[1], 0, *objects[0], 0);
    patcher.endTransaction();
    
    REQUIRE(patcher.getLinks().size() == 3);
    
    patcher.sendToObject(*objects[0], 1, {1});
    patcher.sendToObject(*objects[1], 1, {1});
    
    patcher.sendToObject(*objects[0], 0, bang_msg);
}
