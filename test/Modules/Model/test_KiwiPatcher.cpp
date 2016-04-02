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

// ================================================================================ //
//                                     UTILITIES                                    //
// ================================================================================ //

std::string to_string(Atom const& atom);
std::string to_string(Atom const& atom)
{
    std::string output;
    if(atom.isInt())
    {
        output += std::to_string(atom.getInt());
    }
    else if(atom.isFloat())
    {
        output += std::to_string(atom.getFloat());
    }
    else if(atom.isString())
    {
        output += atom.getString();
    }
    
    return output;
}

std::string to_string(std::vector<Atom>& atom_vec);
std::string to_string(std::vector<Atom>& atom_vec)
{
    std::string output;
    if(!atom_vec.empty())
    {
        if(atom_vec.size() == 1)
        {
            output += to_string(atom_vec[0]);
        }
        else
        {
            output += '[';
            for(std::vector<Atom>::size_type i = 0; i < atom_vec.size();)
            {
                output += to_string(atom_vec[i]);
                if(++i != atom_vec.size())
                {
                    output += ", ";
                }
            }
            output += ']';
        }
    }
    
    return output;
}

// ================================================================================ //
//                                        Model                                     //
// ================================================================================ //

class PatcherObserver : public flip::DocumentObserver<model::Patcher>
{
public:
    
    void indent(const int level)
    {
        if(level >= 1) for(int i = 0; i < level; ++i) std::cout << "  |---";
    }
    
    void document_changed(model::Patcher& patcher) override
    {
        std::cout << "  Patcher changed :" << '\n';
        
        //if(patcher.getObjects().changed())
        if(true)
        {
            const auto objs_change_status_str = (patcher.objectsChanged() ? "changed" : "no change");
            indent(1);
            std::cout << "- Objects : (" << objs_change_status_str << ")\n";
            
            const auto& objects = patcher.getObjects();
            
            for(const auto& obj : objects)
            {
                const auto change_status_str = (obj.changed() ? "changed" : "no change");
                indent(2);
                std::cout << "- object \"" << obj.getName() << "\" (" << change_status_str << ")\n";
                
                const auto status_str = (obj.resident() ? "resident" : (obj.added() ? "added" : "removed"));
                
                indent(3); std::cout << "- status : " << status_str << '\n';
                indent(3); std::cout << "- text : " << obj.getText() << '\n';
            }
        }
        
        //if(patcher.getLinks().changed())
        if(true)
        {
            const auto links_change_status_str = (patcher.linksChanged() ? "changed" : "no change");
            indent(1);
            std::cout << "- Links : (" << links_change_status_str << ")\n";
            
            const auto& links = patcher.getLinks();
            
            for(const auto& link : links)
            {
                const auto change_status_str = (link.changed() ? "changed" : "no change");
                
                const auto status_str = (link.resident() ? "resident" : (link.added() ? "added" : "removed"));
                
                indent(3); std::cout << "- status : " << status_str << '\n';
                
                if(link.resident() || link.added())
                {
                    const auto& from = link.getObjectFrom();
                    const auto& to = link.getObjectTo();
                    
                    indent(3); std::cout    << "- from object : \""
                                            << from.getName() << "\" ("
                                            << link.getOutletIndex() << ")" << '\n';
                    
                    indent(3); std::cout    << "- to object : \""
                                            << to.getName() << "\" ("
                                            << link.getInletIndex() << ")" << '\n';
                }
            }
        }
        
        std::cout << "- - - - - - - - - - - - - - - - - - -\n";
    }
};

class PatcherValidator : public flip::DocumentValidator<model::Patcher>
{
public:
    
    void validate(model::Patcher& patcher) override
    {
        
        std::cout << "  Document Validate :" << '\n';
        
        //throw std::runtime_error("ERROOOOOORRRR !!!");
        
        /*
        if(patcher.getObjects().changed())
        {
            //throw std::runtime_error("ERROOOOOORRRR !!!");
        }*/
    };
};

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
    
    obj_plus->receive(0, {"bang"}); // output 10
    obj_plus->receive(0, {"bang"}); // output 10
    obj_plus->receive(0, {"bang"}); // output 10
    obj_plus->receive(0, {"bang"}); // output 10
    
    patcher.endTransaction();
    
    patcher.undo(true);
    patcher.redo(true);
    
    /*
    PatcherObserver     observer;
    PatcherValidator    validator;
    
    flip_DISABLE_WARNINGS_FOUR_CHAR_CONSTANTS
    auto document = std::unique_ptr<flip::Document>(new flip::Document(model::Model::use(), observer, validator, 123456789ULL, uint32_t('cicm'), uint32_t('kiwi')));
    flip_RESTORE_WARNINGS
    
    // Set up an history for this document
    auto history = std::unique_ptr<flip::History<flip::HistoryStoreMemory>>(new flip::History<flip::HistoryStoreMemory>(*document.get()));
    
    const auto commitWithUndoStep = [&document, &history] (std::string const& label)
    {
        std::cout << "* " << label << '\n';
        
        document->set_label(label);
        auto tx = document->commit();
        history->add_undo_step(tx);
    };
    
    const auto Undo = [&document, &history] ()
    {
        const auto last_undo = history->last_undo();
        
        if(last_undo != history->end())
        {
            std::cout << "* " << "Undo \"" << last_undo->label() << "\"\n";
            history->execute_undo();
            document->commit();
        }
        else
        {
            std::cout << "* Undo impossible\n";
        }
    };
    
    const auto Redo = [&document, &history] ()
    {
        const auto first_redo = history->first_redo();
        
        if(first_redo != history->end())
        {
            std::cout << "* " << "Redo \"" << history->first_redo()->label() << "\"\n";
            history->execute_redo();
            document->commit();
        }
        else
        {
            std::cout << "* Redo impossible\n";
        }
    };
    
    model::Patcher& patcher = document->root<model::Patcher>();
    document->commit();
    
    model::Object::initInfos("plus", "1");
    
    auto plus_uptr = std::unique_ptr<model::ObjectPlus>(new model::ObjectPlus({"plus", "1"}));
    plus_uptr->setNumberOfInlets(2);
    
    auto obj_plus_ptr = patcher.addObject(std::move(plus_uptr));
    //auto obj_plus_ptr = patcher.addObject(std::unique_ptr<model::ObjectPlus>(new model::ObjectPlus("plus", "1")));
    auto obj_plus_ref = obj_plus_ptr->ref();
    commitWithUndoStep("Add Object \"plus\"");
    
    Undo();
    Redo();
    
    obj_plus_ptr = document->object_ptr<model::Object>(obj_plus_ref);
    if(obj_plus_ptr)
    {
        patcher.removeObject(*obj_plus_ptr);
        commitWithUndoStep("Remove Object \"plus\"");
        Undo();
    }
    
    auto obj_plus_alias = patcher.addObject(std::unique_ptr<model::ObjectPlus>(new model::ObjectPlus({"+", "42"})));
    auto obj_plus_alias_ref = obj_plus_alias->ref();
    commitWithUndoStep("Add Object \"+\"");
    
    Undo();
    Redo();
    
    obj_plus_ptr = document->object_ptr<model::Object>(obj_plus_ref);
    auto obj_plus_alias_ptr = document->object_ptr<model::Object>(obj_plus_alias_ref);
    
    if(obj_plus_ptr && obj_plus_alias_ptr)
    {
        auto link = patcher.addLink(std::unique_ptr<model::Link>(new model::Link(*obj_plus_ptr, 0, *obj_plus_alias_ptr, 0)));
        auto link_ref = link->ref();
        commitWithUndoStep("Add Link between plus objects");
        
        Undo();
        Redo();
        
        auto link_ptr = document->object_ptr<model::Link>(link_ref);
        if(link_ptr)
        {
            patcher.removeLink(*link_ptr);
            commitWithUndoStep("Remove Link");
            Undo();
        }
    }
    
    history.reset();
    document.reset();
    */
}
