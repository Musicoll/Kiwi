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

#include "flip/Document.h"
#include "flip/DocumentServer.h"
#include "flip/PortDirect.h"
#include "flip/CarrierDirect.h"

#include <KiwiTool/KiwiTool_Atom.h>

#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_PatcherUser.h>
#include <KiwiModel/KiwiModel_PatcherValidator.h>
#include <KiwiModel/KiwiModel_Factory.h>

using namespace kiwi;

// ==================================================================================== //
//                                          VALIDATOR                                   //
// ==================================================================================== //

TEST_CASE("Model Validator", "[Validator]")
{
    SECTION("Concurrent object deletion and link creation (deletion first)")
    {
        model::PatcherValidator validator;
        flip::DocumentServer server (model::DataModel::use(), validator, 123456789ULL);
        
        server.root<model::Patcher>().addObject(model::Factory::create(tool::AtomHelper::parse("+")));
        server.root<model::Patcher>().addObject(model::Factory::create(tool::AtomHelper::parse("+")));
        
        server.commit();
        
        // Connect client 1
        flip::PortDirect port_01;
        server.port_factory_add (port_01);
        
        flip::Document document_01 (model::DataModel::use(), validator, 11, 23, 24);
        flip::CarrierDirect carrier_01 (document_01, port_01);
        
        document_01.pull();
        
        // Connect client 2
        flip::PortDirect port_02;
        server.port_factory_add (port_02);
        
        flip::Document document_02 (model::DataModel::use(), validator, 12, 23, 24);
        flip::CarrierDirect carrier_02 (document_02, port_02);
        
        document_02.pull();
        
        // client 1 delete objects
        model::Patcher& patcher_01 = document_01.root<model::Patcher>();
        patcher_01.removeObject(*patcher_01.getObjects().begin());
        patcher_01.removeObject(*(++patcher_01.getObjects().begin()));
        
        // client 2 create link
        model::Patcher& patcher_02 = document_02.root<model::Patcher>();
        patcher_02.addLink(*patcher_02.getObjects().begin(), 0, *(++patcher_02.getObjects().begin()), 0);
        
        // client 1 commit and push
        document_01.commit();
        document_01.push();
        
        CHECK(server.root<model::Patcher>().getObjects().count_if([](model::Object &){return true;}) == 0);
        
        // client 2 commit and push (server reject link creation)
        document_02.commit();
        document_02.push();
        
        CHECK(server.root<model::Patcher>().getLinks().count_if([](model::Link &){return true;}) == 0);
        
        // client 1 pull
        document_01.pull();
        CHECK(patcher_01.getObjects().count_if([](model::Object &){return true;}) == 0);
        CHECK(patcher_01.getLinks().count_if([](model::Link &){return true;}) == 0);
        
        // client 2 pull (stack unwind plus link creation rejected)
        document_02.pull();
        CHECK(patcher_02.getObjects().count_if([](model::Object &){return true;}) == 0);
        CHECK(patcher_02.getLinks().count_if([](model::Link &){return true;}) == 0);
        
        server.port_factory_remove (port_01);
        server.port_factory_remove (port_02);
    }
    
    SECTION("Concurrent object deletion and link creation (creation first)")
    {
        model::PatcherValidator validator;
        flip::DocumentServer server (model::DataModel::use(), validator, 123456789ULL);
        
        server.root<model::Patcher>().addObject(model::Factory::create(tool::AtomHelper::parse("+")));
        server.root<model::Patcher>().addObject(model::Factory::create(tool::AtomHelper::parse("+")));
        
        server.commit();
        
        // Connect client 1
        flip::PortDirect port_01;
        server.port_factory_add (port_01);
        
        flip::Document document_01 (model::DataModel::use(), validator, 11, 23, 24);
        flip::CarrierDirect carrier_01 (document_01, port_01);
        
        document_01.pull();
        
        // Connect client 2
        flip::PortDirect port_02;
        server.port_factory_add (port_02);
        
        flip::Document document_02 (model::DataModel::use(), validator, 12, 23, 24);
        flip::CarrierDirect carrier_02 (document_02, port_02);
        
        document_02.pull();
        
        // client 1 delete objects
        model::Patcher& patcher_01 = document_01.root<model::Patcher>();
        patcher_01.removeObject(*patcher_01.getObjects().begin());
        patcher_01.removeObject(*(++patcher_01.getObjects().begin()));
        
        // client 2 create link
        model::Patcher& patcher_02 = document_02.root<model::Patcher>();
        patcher_02.addLink(*patcher_02.getObjects().begin(), 0, *(++patcher_02.getObjects().begin()), 0);
        
        // client 2 commit and push
        document_02.commit();
        document_02.push();
        
        CHECK(server.root<model::Patcher>().getLinks().count_if([](model::Link &){return true;}) == 1);
        
        // client 1 commit and push (transaction rejected)
        document_01.commit();
        document_01.push();
        
        CHECK(server.root<model::Patcher>().getObjects().count_if([](model::Object &){return true;}) == 2);
        
        // client 1 pull (stack unwind plus deletion rejected)
        document_01.pull();
        CHECK(patcher_01.getObjects().count_if([](model::Object &){return true;}) == 2);
        CHECK(patcher_01.getLinks().count_if([](model::Link &){return true;}) == 1);
        
        // client 2 pull
        document_02.pull();
        CHECK(patcher_02.getObjects().count_if([](model::Object &){return true;}) == 2);
        CHECK(patcher_02.getLinks().count_if([](model::Link &){return true;}) == 1);
        
        server.port_factory_remove (port_01);
        server.port_factory_remove (port_02);
    }
}
