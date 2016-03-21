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
#include "../../catch.hpp"
#include <vector>

#include "flip/Document.h"
#include "flip/DocumentObserver.h"

#include "flip/History.h"
#include "flip/HistoryStoreMemory.h"
#include "flip/DataConsumerMemory.h"
#include "flip/BackEndBinary.h"
#include "flip/BackEndMl.h"

#include "flip/Signal.h"

using namespace kiwi;

flip_DISABLE_WARNINGS_FOUR_CHAR_CONSTANTS

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

class PatcherObserver : public flip::DocumentObserver<Patcher>
{
public:
    
    void indent(const int level)
    {
        if(level >= 1) for(int i = 0; i < level; ++i) std::cout << "  |---";
    }
    
    void processAttributes(Attribute::Manager const& obj, const int indent_level = 0)
    {
        indent(indent_level);
        std::cout << "- attrs";
        std::vector<Attribute*> attrs = obj.getAttributes();
        std::cout << " (" << attrs.size() << ") :\n";
        
        for(auto* attr : obj.getAttributes())
        {
            indent(indent_level + 1);
            std::cout << "- " << attr->getName();
            std::vector<Atom> value = attr->getValue();
            std::cout << " : " << to_string(value) << '\n';
        }
    }
    
    void document_changed(Patcher& patcher) override
    {
        std::cout << "  Patcher changed :" << '\n';
        
        //if(patcher.getObjects().changed())
        if(true)
        {
            const auto objs_change_status_str = (patcher.getObjects().changed() ? "changed" : "no change");
            indent(1);
            std::cout << "- Objects : (" << objs_change_status_str << ")\n";
            
            const auto& objects = patcher.getObjects();
            
            for(const auto& obj : objects)
            {
                const auto change_status_str = (obj.changed() ? "changed" : "no change");
                indent(2);
                std::cout << "- object \"" << obj.getName() << "\" (" << change_status_str << ")\n";
                
                const auto type_str = [&obj]()
                {
                    switch (obj.getType())
                    {
                        case Object::ObjectType::Default:   { return "Default"; }
                        case Object::ObjectType::Classic:   { return "Classic"; }
                        case Object::ObjectType::Gui:       { return "Gui"; }
                        case Object::ObjectType::Dsp:       { return "Dsp"; }
                        case Object::ObjectType::DspGui:    { return "DspGui"; }
                        default: return "undefined";
                    }
                };
                
                indent(3); std::cout << "- type : " << type_str() << '\n';
                
                const auto status_str = (obj.resident() ? "resident" : (obj.added() ? "added" : "removed"));
                
                indent(3); std::cout << "- status : " << status_str << '\n';
                indent(3); std::cout << "- text : " << obj.getText() << '\n';
                
                processAttributes(obj, 3);
            }
        }
        
        //if(patcher.getLinks().changed())
        if(true)
        {
            const auto links_change_status_str = (patcher.getLinks().changed() ? "changed" : "no change");
            indent(1);
            std::cout << "- Links : (" << links_change_status_str << ")\n";
            
            const auto& links = patcher.getLinks();
            
            for(const auto& link : links)
            {
                const auto change_status_str = (link.changed() ? "changed" : "no change");
                
                const auto type_str = [&link]()
                {
                    switch (link.getType())
                    {
                        case Link::LinkType::Invalid:   { return "Invalid"; }
                        case Link::LinkType::Control:   { return "Control"; }
                        case Link::LinkType::Dsp:       { return "Dsp"; }
                        default: return "undefined";
                    }
                };
                
                indent(3); std::cout << "- type : " << type_str() << '\n';
                
                const auto status_str = (link.resident() ? "resident" : (link.added() ? "added" : "removed"));
                
                indent(3); std::cout << "- status : " << status_str << '\n';
                
                //indent(3); std::cout << "- from : " << link.getObjectFrom()->getName() << '\n';
                //indent(3); std::cout << "- to : " << link.getObjectTo()->getName() << '\n';
                
                processAttributes(link, 3);
            }
        }

        processAttributes(patcher, 1);
        
        std::cout << "- - - - - - - - - - - - - - - - - - -\n";
    }
};

class PatcherValidator : public flip::DocumentValidator<Patcher>
{
public:
    
    void validate(Patcher& patcher)
    {
        
        std::cout << "  Document Validate :" << '\n';
        
        //throw std::runtime_error("ERROOOOOORRRR !!!");
        
        if(patcher.getObjects().changed())
        {
            //throw std::runtime_error("ERROOOOOORRRR !!!");
        }
    };
};

TEST_CASE("model", "[model]")
{
    Model::init("unit_test_model_01");
    kiwi::model::initializeBasicObjects();
    
    PatcherObserver     observer;
    PatcherValidator    validator;
    
    auto document = std::unique_ptr<flip::Document>(new flip::Document(Model::use(), observer, validator, 123456789ULL, uint32_t ('cicm'), uint32_t('kiwi')));
    
    // Set up an history for this document
    auto history = std::unique_ptr<flip::History<flip::HistoryStoreMemory>>(new flip::History<flip::HistoryStoreMemory>(*document.get()));
    
    Patcher& patcher = document->root<Patcher>();
    patcher.init();
    auto tx = document->commit();
    
    const auto commitWithUndoStep = [&tx, &document, &history] (std::string const& label)
    {
        std::cout << "* " << label << '\n';
        
        document->set_label(label);
        
        try
        {
            tx = document->commit();
        }
        catch (std::runtime_error e)
        {
            std::cerr << e.what();
        }
        
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
    
    patcher.setAttributeValue("bgcolor", {0., 1., 0., 1.});
    patcher.setAttributeValue("gridsize", {40});
    commitWithUndoStep("Change Patcher attributes value #1");
    
    Undo();
    Undo(); // test impossible undo use case
    Redo();
    Redo(); // test impossible redo use case
    
    patcher.setAttributeValue("bgcolor", {0.6666, 0.7777, 0.8888, 1.});
    patcher.setAttributeValue("gridsize", {25});
    commitWithUndoStep("Change Patcher attributes value #2");
    
    Undo();
    Redo();
    
    model::Object* obj_plus_ptr = patcher.addObject("plus", "1");
    auto obj_plus_ref = obj_plus_ptr->ref();
    obj_plus_ptr->setAttributeValue("bgcolor", {0.6666, 0.7777, 0.8888, 1.});
    obj_plus_ptr->setAttributeValue("color", {1., 0.0, 1., 1.});
    commitWithUndoStep("Add Object \"plus\"");
    
    Undo();
    Redo();
    
    obj_plus_ptr = document->object_ptr<model::Object>(obj_plus_ref);
    if(obj_plus_ptr)
    {
        obj_plus_ptr->setAttributeValue("bgcolor", {.123456, 0.7777, 0.8888, 1.});
    }
    
    commitWithUndoStep("Change \"plus\" object bgcolor");
    
    patcher.remove(obj_plus_ptr);
    commitWithUndoStep("Remove Object \"plus\"");
    Undo();
    
    model::Object* obj_plus_alias = patcher.addObject("+", "42");
    auto obj_plus_alias_ref = obj_plus_alias->ref();
    commitWithUndoStep("Add Object \"+\"");
    
    Undo();
    Redo();
    
    obj_plus_ptr = document->object_ptr<model::Object>(obj_plus_ref);
    auto obj_plus_alias_ptr = document->object_ptr<model::Object>(obj_plus_alias_ref);
        
    Link* plus_link = patcher.addLink(obj_plus_ptr, 0, obj_plus_alias_ptr, 0);
    commitWithUndoStep("Add Link between plus objects");
    
    Undo();
    Redo();
    
    history.reset();
    document.reset();
}


