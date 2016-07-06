/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiModelDataModel.hpp"
#include "KiwiModelPatcher.hpp"
#include "KiwiModelPatcherUser.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                  PATCHER::declare                                //
        // ================================================================================ //
        
        void Patcher::declare()
        {
            assert(! DataModel::has<Patcher>());
            
            Patcher::View::declare();
            Patcher::User::declare();
            
            DataModel::declare<Patcher>()
            .name("cicm.kiwi.Patcher")
            .member<flip::Array<model::Object>, &Patcher::m_objects>("objects")
            .member<flip::Array<model::Link>, &Patcher::m_links>("links")
            .member<flip::Collection<Patcher::User>, &Patcher::m_users>("users")
            .member<flip::String, &Patcher::m_patcher_name>("patcher_name");
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
        
        model::Object& Patcher::addObject(std::string const& name, std::vector<Atom> const& args)
        {
            return *m_objects.insert(m_objects.end(), Factory::create(name, args));
        }
        
        model::Object& Patcher::addObject(flip::Mold const& mold)
        {
            return *m_objects.emplace(m_objects.end(), mold);
        }
        
        model::Object& Patcher::replaceObjectWith(model::Object& object_to_replace,
                                                  flip::Mold const& mold,
                                                  Patcher::View& view)
        {
            model::Object& new_object = addObject(mold);
            
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
                    }
                    
                    if(&to == &object_to_replace)
                    {
                        if(inlet_index < new_inlets)
                        {
                            addLink(from, outlet_index, new_object, inlet_index);
                        }
                    }
                }
            }
            
            view.unselectObject(object_to_replace);
            removeObject(object_to_replace);
            
            view.selectObject(new_object);
            
            return new_object;
        }
        
        model::Link* Patcher::addLink(model::Object const& from, const size_t outlet,
                                      model::Object const& to, const size_t inlet)
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
        
        bool Patcher::nameChanged() const noexcept
        {
            return m_patcher_name.changed();
        }
        
        std::string Patcher::getName() const
        {
            return !m_patcher_name.removed() ? static_cast<std::string>(m_patcher_name) : "Untitled";
        }
        
        void Patcher::setName(std::string const& new_name)
        {
            m_patcher_name = new_name;
        }
        
        Patcher::User* Patcher::getUser(uint32_t user_id)
        {
            const auto has_same_id = [user_id] (User const& user)
            {
                return user_id == user.m_user_id;
            };
            
            const auto it = std::find_if(m_users.begin(), m_users.end(), has_same_id);
            
            if(it != m_users.end() && !it->removed())
            {
                return it.operator->();
            }
            
            return nullptr;
        }
        
        flip::Array<model::Object> const& Patcher::getObjects() const noexcept
        {
            return m_objects;
        }
        
        flip::Array<model::Link> const& Patcher::getLinks() const noexcept
        {
            return m_links;
        }
        
        flip::Collection<Patcher::User> const& Patcher::getUsers() const noexcept
        {
            return m_users;
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
    }
}
