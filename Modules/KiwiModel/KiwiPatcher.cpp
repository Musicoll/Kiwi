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

#include "KiwiPatcherModel.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                  PATCHER::declare                                //
        // ================================================================================ //
        
        void Patcher::View::Object::declare()
        {
            assert(! PatcherModel::has<Patcher::View::Object>());
            
            PatcherModel::declare<Patcher::View::Object>()
            .name("cicm.kiwi.Patcher.View.Object")
            .template member<flip::ObjectRef<model::Object>, &View::Object::m_ref>("ref");
        }
        
        void Patcher::View::Link::declare()
        {
            assert(! PatcherModel::has<Patcher::View::Link>());
            
            PatcherModel::declare<Patcher::View::Link>()
            .name("cicm.kiwi.Patcher.View.Link")
            .member<flip::ObjectRef<model::Link>, &View::Link::m_ref>("ref");
        }
        
        void Patcher::View::declare()
        {
            assert(! PatcherModel::has<Patcher::View>());
            
            Patcher::View::Object::declare();
            Patcher::View::Link::declare();
            
            PatcherModel::declare<Patcher::View>()
            .name("cicm.kiwi.Patcher.View")
            .member<flip::Collection<View::Object>, &View::m_selected_objects>("selected_objects")
            .member<flip::Collection<View::Link>, &View::m_selected_links>("selected_links")
            .member<flip::Bool, &View::m_is_locked>("locked")
            .member<flip::Float, &View::m_zoom_factor>("zoom_factor");
        }
        
        void Patcher::User::declare()
        {
            assert(! PatcherModel::has<Patcher::User>());
            
            PatcherModel::declare<Patcher::User>()
            .name("cicm.kiwi.Patcher.User")
            .member<flip::Int, &Patcher::User::m_user_id>("user_id")
            .member<flip::Collection<Patcher::View>, &Patcher::User::m_views>("views");
        }
        
        void Patcher::declare()
        {
            assert(! PatcherModel::has<Patcher>());
            
            Patcher::View::declare();
            Patcher::User::declare();
            
            PatcherModel::declare<Patcher>()
            .name("cicm.kiwi.Patcher")
            .member<flip::Array<model::Object>, &Patcher::m_objects>("objects")
            .member<flip::Array<model::Link>, &Patcher::m_links>("links")
            .member<flip::Collection<Patcher::User>, &Patcher::m_users>("users");
        }
        
        // ================================================================================ //
        //                                   PATCHER MODEL                                  //
        // ================================================================================ //
        
        Patcher::Patcher()
        {
            // user changes doesn't need to be stored in an history.
            m_users.disable_in_undo();
        }
        
        Patcher::~Patcher()
        {
            m_links.clear();
            m_objects.clear();
        }
        
        model::Object& Patcher::addObject(std::string const& text)
        {
            //assert(ObjectFactory::has(object_name));
            
            auto object_uptr = ObjectFactory::createModel(text);
            const auto it = m_objects.insert(m_objects.end(), std::move(object_uptr));
            
            /*
            if(it->getName() == "errorbox")
            {
                model::ErrorBox* error_box = dynamic_cast<model::ErrorBox*>(it.operator->());
                
                if(error_box != nullptr)
                {
                    Console::post("error_box");
                }
            }
            */
            
            return *it;
        }
        
        model::Object& Patcher::addObject(flip::Mold const& mold)
        {
            const auto it = m_objects.emplace(m_objects.end(), mold);
            
            /*
            if(it->getName() == "errorbox")
            {
                model::ErrorBox* error_box = dynamic_cast<model::ErrorBox*>(it.operator->());
                
                if(error_box != nullptr)
                {
                    Console::post("error_box");
                }
            }
            */
            
            return *it;
        }
        
        model::Object& Patcher::replaceObjectWith(model::Object& object_to_replace,
                                                  flip::Mold const& mold,
                                                  Patcher::View& view)
        {
            model::Object& new_object = addObject(mold);
            
            /*
            // handle error box case
            if(new_object.getName() == "errorbox")
            {
                model::ErrorBox& error_box = dynamic_cast<model::ErrorBox&>(new_object);
                error_box.setNumberOfInlets(object_to_replace.getNumberOfInlets());
                error_box.setNumberOfOutlets(object_to_replace.getNumberOfOutlets());
            }
            
            
            if(new_object.getName() == "plus")
            {
                model::ObjectPlus* plus_box = dynamic_cast<model::ObjectPlus*>(&new_object);
                Console::post("plus_box : " + std::string(plus_box ? "OK !" : "nullptr"));
                
            }
            */
            
            new_object.setPosition(object_to_replace.getX(), object_to_replace.getY());
            
            // re-link object
            const size_t new_inlets = new_object.getNumberOfInlets();
            const size_t new_outlets = new_object.getNumberOfOutlets();
            
            for(model::Link& link : m_links)
            {
                if(!link.removed())
                {
                    const model::Object& from = link.getSenderObject();
                    const size_t outlet_index = link.getSenderIndex();
                    const model::Object& to = link.getReceiverObject();
                    const size_t inlet_index = link.getReceiverIndex();
                    
                    if(&from == &object_to_replace)
                    {
                        if(outlet_index < new_outlets)
                        {
                            addLink(new_object, outlet_index, to, inlet_index);
                        }
                        else
                        {
                            Console::error("Link removed (outlet out of range)");
                        }
                    }
                    
                    if(&to == &object_to_replace)
                    {
                        if(inlet_index < new_inlets)
                        {
                            addLink(from, outlet_index, new_object, inlet_index);
                        }
                        else
                        {
                            Console::error("Link removed (inlet out of range)");
                        }
                    }
                }
            }
            
            view.unselectObject(object_to_replace);
            removeObject(object_to_replace);
            
            view.selectObject(new_object);
            
            return new_object;
        }
        
        model::Link* Patcher::addLink(model::Object const& from, const size_t outlet, model::Object const& to, const size_t inlet)
        {
            if(canConnect(from, outlet, to, inlet))
            {
                auto link_uptr = std::unique_ptr<model::Link>(new model::Link(from, outlet, to, inlet));
                const auto it = m_links.insert(m_links.end(), std::move(link_uptr));
                
                return it.operator->();
            }
            
            return nullptr;
        }
        
        bool Patcher::canConnect(model::Object const& from, const size_t outlet,
                                 model::Object const& to, const size_t inlet) const
        {
            // check source object
            const auto from_it = findObject(from);
            const bool from_valid = (from_it != m_objects.cend() && from_it->getNumberOfOutlets() > outlet);
            
            // check destination object
            const auto to_it = findObject(to);
            const bool to_valid = (to_it != m_objects.cend() && to_it->getNumberOfInlets() > inlet);
            
            if(from_valid && to_valid)
            {
                // Check if link does not exists.
                const auto find_link = [&from, &outlet, &to, &inlet](model::Link const& link_model)
                {
                    return (link_model.getSenderObject().ref()      == from.ref() &&
                            link_model.getReceiverObject().ref()    == to.ref() &&
                            link_model.getSenderIndex()             == outlet &&
                            link_model.getReceiverIndex()           == inlet);
                };
                
                return (std::find_if(m_links.begin(), m_links.end(), find_link) == m_links.cend());
            }
            
            return false;
        }
        
        void Patcher::removeObject(model::Object const& object, Patcher::View* view)
        {
            auto object_it = findObject(object);
            if(object_it != m_objects.end())
            {
                // first remove links connected to this object
                for(auto link_it = m_links.begin(); link_it != m_links.end();)
                {
                    if(!link_it.removed())
                    {
                        if(link_it->getSenderObject().ref() == object.ref()
                           || link_it->getReceiverObject().ref() == object.ref())
                        {
                            if(view != nullptr)
                            {
                                view->unselectLink(*link_it);
                            }
                            
                            link_it = m_links.erase(link_it);
                        }
                        else
                        {
                            ++link_it;
                        }
                    }
                    else
                    {
                        ++link_it;
                    }
                }
                
                if(view != nullptr)
                {
                    view->unselectObject(*object_it);
                }
                
                m_objects.erase(object_it);
            }
        }
        
        void Patcher::removeLink(model::Link const& link, Patcher::View* view)
        {
            const auto link_it = findLink(link);
            if(link_it != m_links.end())
            {
                if(!link_it.removed())
                {
                    if(view != nullptr)
                    {
                        view->unselectLink(*link_it);
                    }
                    
                    m_links.erase(link_it);
                }
            }
        }
        
        bool Patcher::objectsChanged() const noexcept
        {
            return m_objects.changed();
        }
        
        bool Patcher::linksChanged() const noexcept
        {
            return m_links.changed();
        }
        
        bool Patcher::usersChanged() const noexcept
        {
            return m_users.changed();
        }
        
        Patcher::User* Patcher::getUser(uint32_t user_id)
        {
            const auto has_same_id = [user_id] (User const& user)
            {
                return user_id == user.m_user_id;
            };
            
            const auto it = std::find_if(m_users.begin(), m_users.end(), has_same_id);
            
            if(it != m_users.end())
            {
                return it.operator->();
            }
            
            return nullptr;
        }
        
        Patcher::User& Patcher::createUserIfNotAlreadyThere(uint32_t user_id)
        {
            auto* user = getUser(user_id);
            
            if(user == nullptr)
            {
                return *m_users.emplace(user_id);
            }
            
            return *user;
        }
        
        void Patcher::unselectForAllUsers(model::Object& object)
        {
            for(Patcher::User& user : m_users)
            {
                for(Patcher::View& view : user.getViews())
                {
                    view.unselectObject(object);
                }
            }
        }
        
        void Patcher::unselectForAllUsers(model::Link& link)
        {
            for(Patcher::User& user : m_users)
            {
                for(Patcher::View& view : user.getViews())
                {
                    view.unselectLink(link);
                }
            }
        }
        
        flip::Array<model::Object>::const_iterator Patcher::findObject(model::Object const& object) const
        {
            const auto find_it = [&object](model::Object const& object_model)
            {
                return (object.ref() == object_model.ref());
            };
            
            return std::find_if(m_objects.begin(), m_objects.end(), find_it);
        }
        
        flip::Array<model::Link>::const_iterator Patcher::findLink(model::Link const& link) const
        {
            const auto find_it = [&link](model::Link const& link_model)
            {
                return (link.ref() == link_model.ref());
            };
            
            return std::find_if(m_links.begin(), m_links.end(), find_it);
        }
        
        flip::Array<model::Object>::iterator Patcher::findObject(model::Object const& object)
        {
            const auto find_it = [&object](model::Object const& object_model)
            {
                return (object.ref() == object_model.ref());
            };
            
            return std::find_if(m_objects.begin(), m_objects.end(), find_it);
        }
        
        flip::Array<model::Link>::iterator Patcher::findLink(model::Link const& link)
        {
            const auto find_it = [&link](model::Link const& link_model)
            {
                return (link.ref() == link_model.ref());
            };
            
            return std::find_if(m_links.begin(), m_links.end(), find_it);
        }
        
        // ================================================================================ //
        //                                   PATCHER VIEW                                   //
        // ================================================================================ //
        
        Patcher::View::View() : m_is_locked(false), m_zoom_factor(1.)
        {
            ;
        }
        
        Patcher::View::~View()
        {
            m_selected_links.clear();
            m_selected_objects.clear();
        }
        
        Patcher& Patcher::View::getPatcher()
        {
            return ancestor<Patcher>();
        }
        
        void Patcher::View::setLock(bool locked)
        {
            m_is_locked = locked;
        }
        
        bool Patcher::View::getLock() const noexcept
        {
            return m_is_locked;
        }
        
        bool Patcher::View::lockChanged() const noexcept
        {
            return m_is_locked.changed();
        }
        
        void Patcher::View::setZoomFactor(double zoom_factor)
        {
            const double min_zoom = 0.25;
            m_zoom_factor = zoom_factor < min_zoom ? min_zoom : zoom_factor;
        }
        
        double Patcher::View::getZoomFactor() const noexcept
        {
            return m_zoom_factor;
        }
        
        bool Patcher::View::zoomFactorChanged() const noexcept
        {
            return m_zoom_factor.changed();
        }
        
        std::vector<model::Object*> Patcher::View::getSelectedObjects()
        {
            std::vector<model::Object*> objects;
            for(auto& object : m_selected_objects)
            {
                objects.push_back(object.get());
            }
            
            return objects;
        }
        
        std::vector<model::Link*> Patcher::View::getSelectedLinks()
        {
            std::vector<model::Link*> links;
            for(auto& link : m_selected_links)
            {
                links.push_back(link.get());
            }
            
            return links;
        }
        
        bool Patcher::View::isSelected(model::Object const& object) const
        {
            for(auto& ref : m_selected_objects)
            {
                if(!ref.removed())
                {
                    auto* sel_object = ref.get();
                    
                    if(sel_object != nullptr && sel_object == &object) return true;
                }
            }
            
            return false;
        }
        
        bool Patcher::View::isSelected(model::Link const& link) const
        {
            for(auto& ref : m_selected_links)
            {
                if(!ref.removed())
                {
                    auto* sel_link = ref.get();
                    
                    if(sel_link != nullptr && sel_link == &link) return true;
                }
            }
            
            return false;
        }
        
        bool Patcher::View::selectionChanged() const
        {
            return (m_selected_objects.changed() || m_selected_links.changed());
        }
        
        void Patcher::View::selectObject(model::Object& object)
        {
            if(!isSelected(object))
            {
                m_selected_objects.emplace(object);
            }
        }
        
        void Patcher::View::selectLink(model::Link& link)
        {
            if(!isSelected(link))
            {
                m_selected_links.emplace(link);
            }
        }
        
        void Patcher::View::unselectObject(model::Object& object)
        {
            const auto find_object = [&object] (View::Object& ref)
            {
                if(!ref.removed())
                {
                    auto* sel_object = ref.get();
                    
                    if(sel_object != nullptr && sel_object == &object) return true;
                }
                
                return false;
            };
            
            auto it = std::find_if(m_selected_objects.begin(), m_selected_objects.end(), find_object);
            
            if(it != m_selected_objects.end())
            {
                m_selected_objects.erase(it);
            }
        }
        
        void Patcher::View::unselectLink(model::Link& link)
        {
            const auto find_link = [&link] (View::Link& ref)
            {
                if(!ref.removed())
                {
                    auto* sel_link = ref.get();
                    
                    if(sel_link != nullptr && sel_link == &link) return true;
                }
                
                return false;
            };
            
            auto it = std::find_if(m_selected_links.begin(), m_selected_links.end(), find_link);
            
            if(it != m_selected_links.end())
            {
                m_selected_links.erase(it);
            }
        }
        
        void Patcher::View::unselectAll()
        {
            m_selected_links.clear();
            m_selected_objects.clear();
        }
        
        void Patcher::View::selectAll()
        {
            unselectAll();
            
            auto& patcher = getPatcher();
            
            for(auto& object : patcher.getObjects())
            {
                if(!object.removed())
                {
                    m_selected_objects.emplace(object);
                }
            }
            
            for(auto& link : patcher.getLinks())
            {
                if(!link.removed())
                {
                    m_selected_links.emplace(link);
                }
            }
        }
        
        // ================================================================================ //
        //                                   PATCHER USER                                   //
        // ================================================================================ //
        
        Patcher::User::User(uint32_t user_id) : m_user_id(user_id)
        {
            
        }
        
        Patcher::View& Patcher::User::addView()
        {
            return *m_views.emplace();
        }
        
        flip::Collection<Patcher::View>::iterator Patcher::User::removeView(View const& view)
        {
            auto it = m_views.find_if([&view](View const& patcher_view) {
                return (&view == &patcher_view);
            });
            
            if(it != m_views.end())
            {
                return m_views.erase(it);
            }
            
            return m_views.end();
        }
        
        uint32_t Patcher::User::getId() const
        {
            return !removed() ? static_cast<uint32_t>(m_user_id) : static_cast<uint32_t>(m_user_id.before());
        }
        
        flip::Collection<Patcher::View> const& Patcher::User::getViews() const noexcept
        {
            return m_views;
        }
    }
}
