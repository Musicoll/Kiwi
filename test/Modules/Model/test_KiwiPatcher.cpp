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
#include <vector>

#include "flip/Document.h"
#include "flip/DocumentObserver.h"

#include "flip/History.h"
#include "flip/HistoryStoreMemory.h"
#include "flip/DataConsumerMemory.h"
#include "flip/BackEndBinary.h"
#include "flip/BackEndMl.h"

using namespace kiwi;

TEST_CASE("model", "[model]")
{
    auto instance = controller::Instance::create(123456789ULL, "kiwi");
    auto& patcher = instance->createPatcher();
    
    patcher.beginTransaction("Add Object \"plus\"");
    
    auto obj_plus = patcher.addObject("plus");
    CHECK(obj_plus->getText() == "");
    CHECK(obj_plus->getNumberOfInlets() == 2);
    
    //obj_plus->receive(0, {30});
    //obj_plus->receive(1, {100});
    //obj_plus->receive(0, {400});
    
    auto obj_plus_2 = patcher.addObject("plus", "10");
    //CHECK(obj_plus_2->getText() == "10");
    //CHECK(obj_plus_2->getNumberOfInlets() == 1);
    
    auto link_1 = patcher.addLink(*obj_plus, 0, *obj_plus_2, 0);
    
    obj_plus->receive(0, {"bang"}); // output 10
    
    obj_plus->receive(1, {10});
    
    obj_plus->receive(0, {"bang"}); // output 20
    
    auto link_2 = patcher.addLink(*obj_plus_2, 0, *obj_plus, 1);
    
    obj_plus->receive(0, {"bang"}); // output 20
    obj_plus->receive(0, {"bang"}); // output 30
    obj_plus->receive(0, {"bang"}); // output 40
    obj_plus->receive(0, {"bang"}); // output 50
    
    patcher.removeLink(link_2);
    
    obj_plus->receive(0, {"bang"}); // output 60
    obj_plus->receive(0, {"bang"}); // output 60
    obj_plus->receive(0, {"bang"}); // output 60
    
    patcher.endTransaction();
    
    patcher.undo(true);
    patcher.redo(true);
}
