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

#include "KiwiPatcher.hpp"
#include "KiwiInstance.hpp"
#include "Objects/KiwiObjects.hpp"

namespace kiwi
{
    namespace controller
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        //Patcher::Patcher(sInstance instance, PatcherModel& model) noexcept : m_instance(instance), m_model(model)
        Patcher::Patcher(Instance* instance) noexcept : m_instance(instance)
        {
            if(m_instance != nullptr)
            {
                // Set up a document
                m_document = std::unique_ptr<flip::Document>(new flip::Document(model::Model::use(), *this, m_instance->getUserId(), 'cicm', 'kpat'));
                
                // Set up an history for this document
                m_history = std::unique_ptr<flip::History<flip::HistoryStoreMemory>>(new flip::History<flip::HistoryStoreMemory>(*m_document.get()));
                
                // Get our document's root model
                m_model = &m_document->root<model::Patcher>();
                
                m_document->commit();
            }
        }
        
        Patcher::~Patcher()
        {
            m_objects.clear();
            m_links.clear();
        }
        
        std::unique_ptr<Patcher> Patcher::create(Instance* instance)
        {
            std::unique_ptr<Patcher> patcher(new Patcher(instance));
            return patcher;
        }
        
        controller::Object* Patcher::addObject(std::string const& name, std::string const& text)
        {
            std::unique_ptr<model::Object> uptr_object = nullptr;
            
            if(name == "plus" || name == "+")
            {
                model::Object::initInfos infos{name, text};
                uptr_object = std::unique_ptr<model::ObjectPlus>(new model::ObjectPlus(infos));
                auto& obj = *m_model->addObject(std::move(uptr_object));
                
                std::unique_ptr<controller::Object> uptr_object_ctrl = std::unique_ptr<controller::ObjectPlus>(new controller::ObjectPlus(static_cast<model::ObjectPlus&>(obj), infos.args));
                auto saved_uptr_object_ctrl = uptr_object_ctrl.get();
                m_objects.emplace_back(std::move(uptr_object_ctrl));
                return saved_uptr_object_ctrl;
            }
            else if(name == "print")
            {
                //uptr_object = std::unique_ptr<ObjectPrint>(new ObjectPrint(name, text));
            }
            
            return nullptr;
        }
        
        controller::Link* Patcher::addLink(controller::Object& from, const uint32_t outlet, controller::Object& to, const uint32_t inlet)
        {
            if(&from != &to)
            {
                auto& model_from = from.m_model;
                auto& model_to = to.m_model;
                auto& link_model = *m_model->addLink(std::unique_ptr<model::Link>(new model::Link(model_from, outlet, model_to, inlet)));
                
                auto uptr_link_ctrl = std::unique_ptr<controller::Link>(new controller::Link(link_model, &from, &to));
                auto saved_uptr_link_ctrl = uptr_link_ctrl.get();
                m_links.emplace_back(std::move(saved_uptr_link_ctrl));
                return saved_uptr_link_ctrl;
            }
            
            return nullptr;
        }
        
        void Patcher::remove(sObject object)
        {
            ;
        }
        
        void Patcher::undo(const bool commit)
        {
            const auto last_undo = m_history->last_undo();
            
            if(last_undo != m_history->end())
            {
                std::cout << "* " << "Undo \"" << last_undo->label() << "\"\n";
                m_history->execute_undo();
                if(commit)
                {
                    m_document->commit();
                }
            }
            else
            {
                std::cout << "* Undo impossible\n";
            }
        }
        
        void Patcher::redo(const bool commit)
        {
            const auto first_redo = m_history->first_redo();
            
            if(first_redo != m_history->end())
            {
                std::cout << "* " << "Redo \"" << m_history->first_redo()->label() << "\"\n";
                m_history->execute_redo();
                
                if(commit)
                {
                    m_document->commit();
                }
            }
            else
            {
                std::cout << "* Redo impossible\n";
            }
        }
        
        void Patcher::debug_document(model::Patcher& patcher)
        {
            const auto indent = [](const int level)
            {
                if(level >= 1) for(int i = 0; i < level; ++i) std::cout << "  |---";
            };
            
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
                    
                    indent(3); std::cout << "- n°inlet : " << obj.getNumberOfInlets() << '\n';
                    indent(3); std::cout << "- n°outlet : " << obj.getNumberOfOutlets() << '\n';
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
                    
                    indent(2);
                    std::cout << "- link " << "(" << change_status_str << ")\n";
                    
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
        
        void Patcher::document_changed(model::Patcher& patcher)
        {
            debug_document(patcher);
            
            if(patcher.changed())
            {
                if(patcher.objectsChanged())
                {
                    for(auto& obj : patcher.getObjects())
                    {
                        if(obj.added())
                        {
                            auto obj_ptr = getObjectByModel(obj);
                            if(obj_ptr == nullptr)
                            {
                                const auto name = obj.getName();
                                std::vector<Atom> args = AtomHelper::parse(obj.getText());
                                
                                if(name == "plus" || name == "+")
                                {
                                    m_objects.emplace_back(new ObjectPlus(static_cast<model::ObjectPlus&>(obj), args));
                                }
                            }
                            
                            //createControllerForModel(obj);
                        }
                        else if(obj.removed())
                        {
                            auto it = std::find_if(m_objects.begin(), m_objects.end(), [&obj](std::unique_ptr<controller::Object>& ctrl)
                            {
                                return (&ctrl.get()->m_model == &obj);
                            });
                            
                            if(it != m_objects.end())
                            {
                                m_objects.erase(it);
                            }
                        }
                    }
                }
                
                if(patcher.linksChanged())
                {
                    for(auto& link : patcher.getLinks())
                    {
                        if(link.added())
                        {
                            /*
                            auto* from = getCtrlForModel(link.getObjectFrom());
                            auto* to = getCtrlFromID(link.getObjectTo().ref());
                            auto link_ctrl = controller::Link(link, from, to);
                            m_links.push_back(link_ctrl);
                            */
                            //createControllerForModel(link);
                        }
                        else if(link.removed())
                        {
                            //removeControllerForModel(link);
                        }
                    }
                }
            }
        }
    }
}
