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

class PatcherObserver : public flip::DocumentObserver<Patcher>
{
public:
    void document_changed(Patcher& patcher) override
    {
        std::cout << "  Document changed :" << '\n';
        
        if(patcher.getObjects().changed())
        {
            std::cout << "\tpatcher object container changed :" << '\n';
            
            const auto& objects = patcher.getObjects();
            
            for(const auto& obj : objects)
            {
                const auto change_status_str = (obj.changed() ? "changed" : "no change");
                std::cout << "\t\t- object \"" << obj.getName() << "\" (" << change_status_str << ")\n";
                
                const auto status_str = (obj.resident() ? "resident" : (obj.added() ? "added" : "removed"));
                std::cout << "\t\t\t- status : " << status_str << '\n';
                
                if(obj.changed())
                {
                    std::cout << "\t\t\t- name : " << obj.getName() << '\n';
                }
            }
        }

        {
            std::cout << "\tpatcher attributes :" << '\n';
            
            std::vector<Atom> color = patcher.getAttributeValue("bgcolor");
            
            std::cout << "\t\t- bgcolor : " << to_string(color) << '\n';
            
            auto gridsize = patcher.getAttributeValue("gridsize");
            std::cout << "\t\t- gridSize : " << to_string(gridsize) << '\n';
            
            auto attr = patcher.getAttribute("bgcolor");
            if (attr && attr->getType() == Attribute::Type::RGBA)
            {
                Attribute::RGBA* attr_rgba = dynamic_cast<Attribute::RGBA*>(attr);
                FlipRGBA rgba = attr_rgba->get();
            }
        }
        
        std::cout << "- - - - - - - - - - - - - - - - - - -\n";
    }
};

TEST_CASE("model", "[model]")
{
    Model::init("unit_test_model_01");
    kiwi::model::initializeBasicObjects();
    
    PatcherObserver observer;
    
    auto document = std::unique_ptr<flip::Document>(new flip::Document(Model::use(), observer, 123456789ULL, uint32_t ('cicm'), uint32_t('kiwi')));
    
    // Set up an history for this document
    auto history = std::unique_ptr<flip::History<flip::HistoryStoreMemory>>(new flip::History<flip::HistoryStoreMemory>(*document.get()));
    
    Patcher& patcher = document->root<Patcher>();
    patcher.init();
    auto tx = document->commit();
    
    const auto commitWithUndoStep = [&tx, &document, &history] (std::string const& label)
    {
        std::cout << "* " << label << '\n';
        document->set_label(label);
        tx = document->commit();
        history->add_undo_step(tx);
    };
    
    const auto Undo = [&document, &history] ()
    {
        std::cout << "* " << "Undo" << '\n';
        history->execute_undo();
        document->commit();
    };
    
    const auto Redo = [&document, &history] ()
    {
        std::cout << "* " << "Redo" << '\n';
        history->execute_redo();
        document->commit();
    };
    
    model::Object* obj_plus = patcher.addObject("plus", "1");
    commitWithUndoStep("Add Object \"plus\"");
    
    Undo();
    Redo();
    
    model::Object* obj_plus_alias = patcher.addObject("+", "42");
    commitWithUndoStep("Add Object \"+\"");
    
    Undo();
    Redo();
    
    patcher.setAttributeValue("bgcolor", {0., 1., 0., 1.});
    patcher.setAttributeValue("gridsize", {40});
    commitWithUndoStep("Change Patcher attributes value #1");
    
    Undo();
    Redo();
    
    patcher.setAttributeValue("bgcolor", {0.6666, 0.7777, 0.8888, 1.});
    patcher.setAttributeValue("gridsize", {25});
    commitWithUndoStep("Change Patcher attributes value #2");
    
    document.reset();
}


