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
        Patcher::Patcher(Instance& instance) noexcept :
        m_instance(instance),
        m_document(model::Model::use(), *this, m_instance.getUserId(), 'cicm', 'kpat'),
        m_history(m_document)
        {
            ;
        }
        
        Patcher::~Patcher()
        {
            m_links.clear();
            m_objects.clear();
        }
        
        std::unique_ptr<Patcher> Patcher::create(Instance& instance)
        {
            std::unique_ptr<Patcher> patcher(new Patcher(instance));
            return patcher;
        }
        
        ID Patcher::addObject(std::string const& name, std::string const& text)
        {
            if(name == "plus" || name == "+")
            {
                return getModel().addObject(std::unique_ptr<model::ObjectPlus>(new model::ObjectPlus({name, text})));
            }
            else if(name == "print")
            {
                return getModel().addObject(std::unique_ptr<model::ObjectPrint>(new model::ObjectPrint({name, text})));
            }
            
            return ID();
        }
        
        ID Patcher::addLink(ID const& from, const uint32_t outlet, ID const& to, const uint32_t inlet)
        {
            auto* from_model = getObjectModel(from);
            auto* to_model = getObjectModel(to);
            if(from_model && to_model)
            {
                return getModel().addLink(std::unique_ptr<model::Link>(new model::Link(*from_model, outlet, *to_model, inlet)));
            }
            
            return ID();
        }
        
        void Patcher::removeObject(ID const& object_id)
        {
            getModel().removeObject(object_id);
        }
        
        void Patcher::removeLink(ID const& link_id)
        {
            getModel().removeLink(link_id);
        }
        
        void Patcher::beginTransaction(std::string const& label)
        {
            m_document.set_label(label);
            std::cout << "* " << label << '\n';
        }
        
        void Patcher::endTransaction()
        {
            auto tx = m_document.commit();
            m_history.add_undo_step(tx);
        }
        
        void Patcher::undo(const bool commit)
        {
            const auto last_undo = m_history.last_undo();
            
            if(last_undo != m_history.end())
            {
                std::cout << "* " << "Undo \"" << last_undo->label() << "\"\n";
                m_history.execute_undo();
                if(commit)
                {
                    m_document.commit();
                }
            }
            else
            {
                std::cout << "* Undo impossible\n";
            }
        }
        
        void Patcher::redo(const bool commit)
        {
            const auto first_redo = m_history.first_redo();
            
            if(first_redo != m_history.end())
            {
                std::cout << "* " << "Redo \"" << m_history.first_redo()->label() << "\"\n";
                m_history.execute_redo();
                
                if(commit)
                {
                    m_document.commit();
                }
            }
            else
            {
                std::cout << "* Redo impossible\n";
            }
        }
        
        void Patcher::sendToObject(ID const& object_id, const uint32_t inlet, std::vector<Atom> args)
        {
            auto* object_ctrl = getObjectController(object_id);
            if(object_ctrl)
            {
                object_ctrl->receive(inlet, args);
            }
        }
        
        // ================================================================================ //
        //                                  DOCUMENT OBSERVER                               //
        // ================================================================================ //
        
        void Patcher::document_changed(model::Patcher& patcher)
        {
            debug_document(patcher);
            
            if(patcher.changed())
            {
                const bool link_changed = patcher.linksChanged();
                
                // check links before objects
                if(link_changed)
                {
                    for(auto& link : patcher.getLinks())
                    {
                        if(link.changed() && link.removed())
                        {
                            linkWillBeRemoved(link);
                        }
                    }
                }

                if(patcher.objectsChanged())
                {
                    for(auto& object : patcher.getObjects())
                    {
                        if(object.changed())
                        {
                            if(object.added())
                            {
                                objectHasBeenAdded(object);
                            }
                            else if(object.removed())
                            {
                                objectWillBeRemoved(object);
                            }
                            else
                            {
                                objectChanged(object);
                            }
                        }
                    }
                }
                
                // check links before objects
                if(link_changed)
                {
                    for(auto& link : patcher.getLinks())
                    {
                        if(link.changed())
                        {
                            if(link.added())
                            {
                                linkHasBeenAdded(link);
                            }
                            else if(link.resident())
                            {
                                linkChanged(link);
                            }
                        }
                    }
                }
            }
        }

        void Patcher::objectHasBeenAdded(model::Object& object)
        {
            const auto name = object.getName();
            const auto args = AtomHelper::parse(object.getText());
            
            if(name == "plus" || name == "+")
            {
                addObjectController<controller::ObjectPlus, model::ObjectPlus>(object, args);
            }
            else if(name == "print")
            {
                addObjectController<controller::ObjectPrint, model::ObjectPrint>(object, args);
            }
        }

        void Patcher::objectChanged(model::Object& /*object*/)
        {
            
        }

        void Patcher::objectWillBeRemoved(model::Object& object)
        {
            const auto it = std::find_if(m_objects.begin(), m_objects.end(), [&object](objects_t::value_type const& ctrl)
            {
                return (&ctrl.get()->m_model == &object);
            });
            
            if(it != m_objects.cend())
            {
                m_objects.erase(it);
            }
        }

        void Patcher::linkHasBeenAdded(model::Link& link)
        {
            auto* from = getObjectController(link.getObjectFrom());
            auto* to = getObjectController(link.getObjectTo());
            
            if(from && to)
            {
                m_links.emplace_back(links_t::value_type(new controller::Link(link, from, to)));
            }
        }
        
        void Patcher::linkChanged(model::Link& /*link*/)
        {
            ;
        }
        
        void Patcher::linkWillBeRemoved(model::Link& link)
        {
            //std::cout << "linkWillBeRemoved" << '\n';
            const auto it = std::find_if(m_links.begin(), m_links.end(), [&link](links_t::value_type const& ctrl)
            {
                return (&ctrl.get()->getModel() == &link);
            });
            
            if(it != m_links.cend())
            {
                //std::cout << "linkWillBeRemoved found" << '\n';
                m_links.erase(it);
            }
        }
        
        // ================================================================================ //
        //                                  PATCHER DEBUG                                   //
        // ================================================================================ //
        
        void Patcher::debug_document(model::Patcher& patcher)
        {
            if(!m_instance.isInDebugMode()) return;
            
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
                
                auto const& objects = patcher.getObjects();
                
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
                
                auto const& links = patcher.getLinks();
                
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
                    else if(link.removed())
                    {
                        const auto& from = link.getObjectFromBefore();
                        const auto& to = link.getObjectToBefore();
                        
                        indent(3); std::cout    << "- from object (before) : \""
                        << from.getName() << "\" ("
                        << link.getOutletIndex() << ")" << '\n';
                        
                        indent(3); std::cout    << "- to object (before) : \""
                        << to.getName() << "\" ("
                        << link.getInletIndex() << ")" << '\n';
                    }
                }
            }
            
            std::cout << "- - - - - - - - - - - - - - - - - - -\n";
        }
    }
}
