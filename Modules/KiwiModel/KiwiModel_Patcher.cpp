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

#include "KiwiModel_DataModel.h"
#include "KiwiModel_PatcherUser.h"
#include "KiwiModel_PatcherUser.h"
#include "KiwiModel_Factory.h"

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
            .member<Objects, &Patcher::m_objects>("objects")
            .member<Links, &Patcher::m_links>("links")
            .member<Users, &Patcher::m_users>("users");
        }
        
        // ================================================================================ //
        //                                   PATCHER MODEL                                  //
        // ================================================================================ //
        
        Patcher::Patcher()
        : signal_user_connect(Signal_USER_CONNECT, *this)
        , signal_user_disconnect(Signal_USER_DISCONNECT, *this)
        , signal_get_connected_users(Signal_GET_CONNECTED_USERS, *this)
        , signal_receive_connected_users(Signal_RECEIVE_CONNECTED_USERS, *this)
        {
            // user changes doesn't need to be stored in an history.
            m_users.disable_in_undo();
        }
        
        Patcher::~Patcher()
        {
            m_links.clear();
            m_objects.clear();
        }
        
        model::Object & Patcher::addObject(std::unique_ptr<model::Object> && object)
        {
            assert(object != nullptr);
            return *m_objects.insert(m_objects.end(), std::move(object));
        }
        
        model::Object & Patcher::replaceObject(model::Object const& old_object,
                                               std::unique_ptr<model::Object> && new_object)
        {
            assert(new_object != nullptr);
            model::Object & object_ref = addObject(std::move(new_object));
            
            // re-link object
            const size_t new_inlets = object_ref.getNumberOfInlets();
            const size_t new_outlets = object_ref.getNumberOfOutlets();
            
            for(model::Link& link : getLinks())
            {
                if(!link.removed())
                {
                    const model::Object& from = link.getSenderObject();
                    const size_t outlet_index = link.getSenderIndex();
                    const model::Object& to = link.getReceiverObject();
                    const size_t inlet_index = link.getReceiverIndex();
                    
                    if(&from == &old_object)
                    {
                        if(outlet_index < new_outlets)
                        {
                            addLink(object_ref, outlet_index, to, inlet_index);
                        }
                    }
                    
                    if(&to == &old_object)
                    {
                        if(inlet_index < new_inlets)
                        {
                            addLink(from, outlet_index, object_ref, inlet_index);
                        }
                    }
                }
            }
            
            removeObject(old_object);
            
            return object_ref;
        }
        
        model::Object& Patcher::addObject(std::string const& name, flip::Mold const& mold)
        {
            return *m_objects.insert(m_objects.end(), Factory::create(name, mold));
        }
        
        model::Link* Patcher::addLink(model::Object const& from, const size_t outlet,
                                      model::Object const& to, const size_t inlet)
        {
            if(canConnect(from, outlet, to, inlet))
            {
                const auto it = m_links.emplace(m_links.end(), from, outlet, to, inlet);
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
                
                // Check if inlets and outlets types are compatible
                
                if (std::find_if(m_links.begin(), m_links.end(), find_link) == m_links.cend())
                {
                    return to_it->getInlet(inlet).hasType(from_it->getOutlet(outlet).getType());
                }
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
        
        Patcher::Objects const& Patcher::getObjects() const noexcept
        {
            return m_objects;
        }
        
        Patcher::Objects& Patcher::getObjects() noexcept
        {
            return m_objects;
        }
        
        Patcher::Links const& Patcher::getLinks() const noexcept
        {
            return m_links;
        }
        
        Patcher::Links& Patcher::getLinks() noexcept
        {
            return m_links;
        }
        
        Patcher::Users const& Patcher::getUsers() const noexcept
        {
            return m_users;
        }
        
        Patcher::Users& Patcher::getUsers() noexcept
        {
            return m_users;
        }
        
        bool Patcher::hasSelfUser() const
        {
            auto it = std::find_if(m_users.begin(), m_users.end(),
                                   [self_id = document().user()](User const& user)
            {
                return (user.getId() == self_id);
                
            });
            
            return it != m_users.end();
        }
        
        Patcher::User& Patcher::useSelfUser()
        {
            auto it = std::find_if(m_users.begin(), m_users.end(),
                                   [self_id = document().user()](User const& user) {
                return (user.getId() == self_id);
            });
            
            // creates and returns a new user if it didn't exist.
            return (it != m_users.end()) ? *it : *m_users.emplace();
        }
        
        Patcher::Objects::const_iterator Patcher::findObject(model::Object const& object) const
        {
            const auto find_it = [&object](model::Object const& object_model)
            {
                return (object.ref() == object_model.ref());
            };
            
            return std::find_if(m_objects.begin(), m_objects.end(), find_it);
        }
        
        Patcher::Links::const_iterator Patcher::findLink(model::Link const& link) const
        {
            const auto find_it = [&link](model::Link const& link_model)
            {
                return (link.ref() == link_model.ref());
            };
            
            return std::find_if(m_links.begin(), m_links.end(), find_it);
        }
        
        Patcher::Objects::iterator Patcher::findObject(model::Object const& object)
        {
            const auto find_it = [&object](model::Object const& object_model)
            {
                return (object.ref() == object_model.ref());
            };
            
            return std::find_if(m_objects.begin(), m_objects.end(), find_it);
        }
        
        Patcher::Links::iterator Patcher::findLink(model::Link const& link)
        {
            const auto find_it = [&link](model::Link const& link_model)
            {
                return (link.ref() == link_model.ref());
            };
            
            return std::find_if(m_links.begin(), m_links.end(), find_it);
        }
    }
}
