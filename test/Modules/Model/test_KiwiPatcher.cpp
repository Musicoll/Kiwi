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


#include "../../../Modules/KiwiModel/KiwiModels.hpp"
#include "../../catch.hpp"
#include <vector>

using namespace kiwi;

// ================================================================================ //
//                                        Model                                     //
// ================================================================================ //


TEST_CASE("model", "[model]")
{
    // define current model version :
    Model::version("unit_test_model_01");
    
    // basic types declaration :
    FlipRGBA::declare();
    FlipPoint::declare();
    
    // attributes declaration :
    AttributeBase::declare();
    Attribute<flip::Bool>::declare("cicm.kiwi.Attribute.Bool");
    Attribute<flip::Int>::declare("cicm.kiwi.Attribute.Int");
    Attribute<flip::Float>::declare("cicm.kiwi.Attribute.Float");
    Attribute<flip::String>::declare("cicm.kiwi.Attribute.String");
    Attribute<FlipRGBA>::declare("cicm.kiwi.Attribute.RGBA");
    
    // patcher elements declaration :
    kiwi::Object::declare();
    Link::declare();
    Patcher::declare();
    
    auto document_uptr = std::unique_ptr<flip::Document>(new flip::Document(Model::use (), 123456789ULL, uint32_t ('cicm'), uint32_t('kiwi')));
    
    flip::Document& document = *document_uptr;
    
    Patcher& song = document.root <Patcher>();
    
    document_uptr.reset();
}


