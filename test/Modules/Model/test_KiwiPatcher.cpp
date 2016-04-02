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

TEST_CASE("Patcher", "[patcher]")
{
    auto instance = controller::Instance::create(123456789ULL, "kiwi");
    auto& patcher = instance->createPatcher();
    
    const std::vector<Atom> bang_msg{"bang"};
    
    patcher.beginTransaction("Add Object \"plus\"");
    
    auto* plus_obj_1 = patcher.addObject("plus");
    auto* plus_obj_2 = patcher.addObject("plus", "10.");
    auto* print_obj  = patcher.addObject("print", "result");
    
    auto* plus_link_1 = patcher.addLink(*plus_obj_1, 0, *plus_obj_2, 0);
    auto* print_link = patcher.addLink(*plus_obj_2, 0, *print_obj, 0);
    
    plus_obj_1->receive(0, bang_msg);
    plus_obj_1->receive(1, {10.});
    plus_obj_1->receive(0, bang_msg);
    
    auto* recursive_plus_link = patcher.addLink(*plus_obj_2, 0, *plus_obj_1, 1);
    
    plus_obj_1->receive(0, bang_msg);
    plus_obj_1->receive(0, bang_msg);
    plus_obj_1->receive(0, bang_msg);
    plus_obj_1->receive(0, bang_msg);
    
    patcher.removeLink(recursive_plus_link);
    
    plus_obj_1->receive(0, bang_msg);
    plus_obj_1->receive(0, bang_msg);
    plus_obj_1->receive(0, bang_msg);
    
    patcher.endTransaction();
    
    patcher.undo(true);
    patcher.redo(true);
}
