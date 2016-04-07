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
    instance->setDebug(false);
    
    auto& patcher = instance->createPatcher();
    
    const std::vector<Atom> bang_msg{"bang"};
    
    patcher.beginTransaction("Add two plus objects, a print object, and link them together");
    
    ID plus_obj_1 = patcher.addObject("plus");
    ID plus_obj_2 = patcher.addObject("plus", "10.");
    ID print_obj  = patcher.addObject("print", "result");
    
    patcher.addLink(plus_obj_1, 0, plus_obj_2, 0);
    patcher.addLink(plus_obj_2, 0, print_obj, 0);
    
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
}

/*
TEST_CASE("Patcher - testcase : counter", "[Patcher]")
{
    auto instance = controller::Instance::create(123456789ULL, "kiwi");
    instance->setDebug(true);
    
    auto& patcher = instance->createPatcher();
    
    const std::vector<Atom> bang_msg{"bang"};
    
    patcher.beginTransaction("Add two plus objects, a print object, and link them together");
    
    auto* plus_obj_1 = patcher.addObject("plus");
    auto* plus_obj_2 = patcher.addObject("plus", "10.");
    auto* print_obj  = patcher.addObject("print", "result");
    
    auto* plus_link_1 = patcher.addLink(*plus_obj_1, 0, *plus_obj_2, 0);
    auto* print_link = patcher.addLink(*plus_obj_2, 0, *print_obj, 0);
    
    patcher.endTransaction();
    
    std::cout << "plus_obj_1 ref obj : " << plus_obj_1->getModel().ref().obj() << '\n';
    std::cout << "plus_obj_1 ref user : " << plus_obj_1->getModel().ref().user() << '\n';
    std::cout << "plus_obj_1 ref actor : " << plus_obj_1->getModel().ref().actor() << '\n';
    
    std::cout << "plus_obj_2 ref obj : " << plus_obj_2->getModel().ref().obj() << '\n';
    std::cout << "plus_obj_2 ref user : " << plus_obj_2->getModel().ref().user() << '\n';
    std::cout << "plus_obj_2 ref actor : " << plus_obj_2->getModel().ref().actor() << '\n';
    
    std::cout << "print_obj ref obj : " << print_obj->getModel().ref().obj() << '\n';
    std::cout << "print_obj ref user : " << print_obj->getModel().ref().user() << '\n';
    std::cout << "print_obj ref actor : " << print_obj->getModel().ref().actor() << '\n';
    
    
    plus_obj_1->receive(0, bang_msg);
    //plus_obj_1->receive(1, {10.});
    //plus_obj_1->receive(0, bang_msg);
    
    patcher.beginTransaction("Add recursive_plus_link");
    auto* recursive_plus_link = patcher.addLink(*plus_obj_2, 0, *plus_obj_1, 1);
    
    plus_obj_1->receive(0, bang_msg);
    plus_obj_1->receive(0, bang_msg);
    plus_obj_1->receive(0, bang_msg);
    plus_obj_1->receive(0, bang_msg);
    
    patcher.endTransaction();
    
    patcher.undo(true);

    //patcher.beginTransaction("Remove recursive_plus_link");
    //patcher.removeLink(recursive_plus_link);
    //patcher.endTransaction();

    plus_obj_1->receive(0, bang_msg);
    plus_obj_1->receive(0, bang_msg);
    plus_obj_1->receive(0, bang_msg);
    
    //patcher.undo(true);
    //patcher.redo(true);
}
*/

/*
TEST_CASE("Patcher - test Object delete", "[Patcher]")
{
    auto instance = controller::Instance::create(123456789ULL, "kiwi");
    instance->setDebug(false);
    auto& patcher = instance->createPatcher();
    const std::vector<Atom> bang_msg{"bang"};
    
    patcher.beginTransaction("Add two plus objects, a print object, and link them together");
    
    auto* plus_obj_1 = patcher.addObject("plus");
    auto* plus_obj_2 = patcher.addObject("plus", "10.");
    auto* print_obj  = patcher.addObject("print", "result");
    
    auto* plus_link_1 = patcher.addLink(*plus_obj_1, 0, *plus_obj_2, 0);
    auto* print_link = patcher.addLink(*plus_obj_2, 0, *print_obj, 0);
    
    patcher.endTransaction();
    
    CHECK(patcher.getNumberOfObjects() == 3);
    CHECK(patcher.getNumberOfLinks() == 2);
    
    patcher.beginTransaction("remove plus object 2 (should remove links too)");
    patcher.removeObject(plus_obj_2);
    patcher.endTransaction();
    CHECK(patcher.getNumberOfObjects() == 2);
    CHECK(patcher.getNumberOfLinks() == 0);
    
    patcher.undo(true);
    CHECK(patcher.getNumberOfObjects() == 3);
    CHECK(patcher.getNumberOfLinks() == 2);
    
    patcher.beginTransaction("Add another + object");
    auto* plus_obj_3 = patcher.addObject("+");
    patcher.endTransaction();
    
    CHECK(patcher.getNumberOfObjects() == 4);
    CHECK(patcher.getNumberOfLinks() == 2);
    
    patcher.undo(true);
    CHECK(patcher.getNumberOfObjects() == 3);
    CHECK(patcher.getNumberOfLinks() == 2);
    
    patcher.redo(true);
    CHECK(patcher.getNumberOfObjects() == 4);
    CHECK(patcher.getNumberOfLinks() == 2);
    
    patcher.beginTransaction("Add another other + object");
    auto* plus_obj_4 = patcher.addObject("+");
    patcher.endTransaction();
    CHECK(patcher.getNumberOfObjects() == 5);
    CHECK(patcher.getNumberOfLinks() == 2);
    
    patcher.beginTransaction("remove last + object");
    patcher.removeObject(plus_obj_4);
    patcher.endTransaction();
    CHECK(patcher.getNumberOfObjects() == 4);
    CHECK(patcher.getNumberOfLinks() == 2);
}

TEST_CASE("Patcher - test stack overflow", "[Patcher]")
{
    auto instance = controller::Instance::create(123456789ULL, "kiwi");
    instance->setDebug(false);
    auto& patcher = instance->createPatcher();
    const std::vector<Atom> bang_msg{"bang"};
    
    patcher.beginTransaction("Add two plus objects, a print object, and link them together");
    
    auto* plus_obj_1 = patcher.addObject("plus");
    auto* plus_obj_2 = patcher.addObject("plus", "10.");
    auto* print_obj  = patcher.addObject("print", "result");
    
    auto* plus_link_1 = patcher.addLink(*plus_obj_1, 0, *plus_obj_2, 0);
    auto* print_link = patcher.addLink(*plus_obj_2, 0, *print_obj, 0);
    auto* stackoverflow_link = patcher.addLink(*plus_obj_2, 0, *plus_obj_1, 0);
    
    patcher.endTransaction();
    
    CHECK(patcher.getNumberOfObjects() == 3);
    CHECK(patcher.getNumberOfLinks() == 3);
    
    plus_obj_1->receive(0, bang_msg);
}
*/
