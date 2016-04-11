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
    namespace engine
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
        
        void Patcher::addPlus()
        {
            getModel().addPlus();
        }
        
        void Patcher::addPrint()
        {
            getModel().addPrint();
        }
        
        void Patcher::addLink(Object const& from, const uint32_t outlet, Object const& to, const uint32_t inlet)
        {
            getModel().addLink(from.m_model, outlet, to.m_model, inlet);
        }
        
        void Patcher::removeObject(Object const& object)
        {
            getModel().removeObject(object.m_model);
        }
        
        void Patcher::removeLink(Link const& link)
        {
            getModel().removeLink(link.m_model);
        }
        
        void Patcher::sendToObject(Object& object, const uint32_t inlet, std::vector<Atom> args)
        {
            object.receive(inlet, args);
        }
        
        // ================================================================================ //
        //                              DOCUMENT TRANSACTIONS                               //
        // ================================================================================ //
        
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
                std::cout << "* " << "Redo \"" << first_redo->label() << "\"\n";
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
            
            if(name == "plus")
            {
                m_objects.emplace_back(std::unique_ptr<engine::ObjectPlus>(new engine::ObjectPlus(static_cast<model::ObjectPlus&>(object))));
            }
            else if(name == "print")
            {
                m_objects.emplace_back(std::unique_ptr<engine::ObjectPrint>(new engine::ObjectPrint(static_cast<model::ObjectPrint&>(object))));
            }
        }

        void Patcher::objectChanged(model::Object& /*object*/)
        {
            
        }

        void Patcher::objectWillBeRemoved(model::Object& object)
        {
            const auto it = findController(object);
            if(it != m_objects.cend())
            {
                m_objects.erase(it);
            }
        }

        void Patcher::linkHasBeenAdded(model::Link& link)
        {
            auto from = findController(link.getSenderId());
            auto to = findController(link.getReceiverId());
            
            if(from != m_objects.end()
               && to != m_objects.end())
            {
                const auto it = m_links.emplace(m_links.end(), std::unique_ptr<engine::Link>(new engine::Link(link, *from->get(), *to->get())));
                
                (*from)->addOutputLink(it->get());
            }
        }
        
        void Patcher::linkChanged(model::Link& /*link*/)
        {
            ;
        }
        
        void Patcher::linkWillBeRemoved(model::Link& link)
        {
            const auto it = findController(link);
            if(it != m_links.cend())
            {
                Object& from = (*it)->getSenderObject();
                from.removeOutputLink(it->get());
                m_links.erase(it);
            }
        }
        
        Patcher::objects_t::const_iterator Patcher::findController(model::Object const& object) const
        {
            const auto pred = [&object](std::unique_ptr<engine::Object> const& ctrl)
            {
                return (ctrl->m_model.ref() == object.ref());
            };
            
            return std::find_if(m_objects.begin(), m_objects.end(), pred);
        }
        
        Patcher::links_t::const_iterator Patcher::findController(model::Link const& link) const
        {
            const auto pred = [&link](std::unique_ptr<engine::Link> const& ctrl)
            {
                return (ctrl->m_model.ref() == link.ref());
            };
            
            return std::find_if(m_links.begin(), m_links.end(), pred);
        }
        
        Patcher::objects_t::const_iterator Patcher::findController(Object const& object) const
        {
            const auto pred = [&object](std::unique_ptr<engine::Object> const& ctrl)
            {
                return (ctrl->m_model.ref() == object.m_model.ref());
            };
            
            return std::find_if(m_objects.begin(), m_objects.end(), pred);
        }
        
        Patcher::links_t::const_iterator Patcher::findController(Link const& link) const
        {
            const auto pred = [&link](std::unique_ptr<engine::Link> const& ctrl)
            {
                return (ctrl->m_model.ref() == link.m_model.ref());
            };
            
            return std::find_if(m_links.begin(), m_links.end(), pred);
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
                    
                    auto const& from = link.getSenderId();
                    auto const& to = link.getReceiverId();
                    
                    indent(3); std::cout << "- from object : \""
                    << from.getName() << "\" ("
                    << link.getSenderIndex() << ")" << '\n';
                    
                    indent(3); std::cout << "- to object : \""
                    << to.getName() << "\" ("
                    << link.getReceiverIndex() << ")" << '\n';
                }
            }
            
            std::cout << "- - - - - - - - - - - - - - - - - - -\n";
        }
    }
}
