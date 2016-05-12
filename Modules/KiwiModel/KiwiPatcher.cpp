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
        
        void Patcher::View::Selection::declare()
        {
            assert(! PatcherModel::has<Patcher::View::Selection>());
            
            PatcherModel::declare<Patcher::View::Object>()
            .name("cicm&.kiwi.Patcher.View.Object")
            .template member<flip::ObjectRef<model::Object>, &View::Object::m_ref>("ref");
            
            PatcherModel::declare<Patcher::View::Link>()
            .name("cicm.kiwi.Patcher.View.Link")
            .member<flip::ObjectRef<model::Link>, &View::Link::m_ref>("ref");
            
            PatcherModel::declare<Patcher::View::Selection>()
            .name("cicm.kiwi.Patcher.View.Selection")
            .member<flip::Collection<View::Object>, &Selection::m_objects>("objects")
            .member<flip::Collection<View::Link>, &Selection::m_links>("links");
        }
        
        void Patcher::View::declare()
        {
            assert(! PatcherModel::has<Patcher::View>());
            
            Patcher::View::Selection::declare();
            
            PatcherModel::declare<Patcher::View>()
            .name("cicm.kiwi.Patcher.View")
            .member<View::Selection, &View::m_selection>("selection");
        }
        
        void Patcher::declare()
        {
            assert(! PatcherModel::has<Patcher>());
            
            Patcher::View::declare();
            
            PatcherModel::declare<Patcher::User>()
            .name("cicm.kiwi.Patcher.User")
            .member<flip::Int, &Patcher::User::m_user_id>("user_id")
            .member<flip::Collection<Patcher::View>, &Patcher::User::m_views>("views");
            
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
            return *m_objects.insert(m_objects.end(), std::move(object_uptr));
        }
        
        bool Patcher::canConnect(model::Object const& from, const uint32_t outlet,
                                 model::Object const& to, const uint32_t inlet) const
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
                            link_model.getSenderIndex()         == outlet &&
                            link_model.getReceiverIndex()       == inlet);
                };
                
                return (std::find_if(m_links.begin(), m_links.end(), find_link) == m_links.cend());
            }
            
            return false;
        }
        
        void Patcher::addLink(model::Object const& from, const uint32_t outlet, model::Object const& to, const uint32_t inlet)
        {
            if(canConnect(from, outlet, to, inlet))
            {
                m_links.insert(m_links.end(), std::unique_ptr<model::Link>(new model::Link(from, outlet, to, inlet)));
            }
        }
        
        void Patcher::removeObject(model::Object const& object)
        {
            auto obj_it = findObject(object);
            if(obj_it != m_objects.end())
            {
                // first remove links connected to this object
                for(auto link_it = m_links.begin(); link_it != m_links.end();)
                {
                    if(link_it->getSenderObject().ref() == object.ref()
                       || link_it->getReceiverObject().ref() == object.ref())
                    {
                        link_it = m_links.erase(link_it);
                    }
                    else
                    {
                        ++link_it;
                    }
                }
                
                m_objects.erase(obj_it);
            }
        }
        
        void Patcher::removeLink(model::Link const& link)
        {
            const auto link_it = findLink(link);
            if(link_it != m_links.end())
            {
                m_links.erase(link_it);
            }
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
        
        void Patcher::View::unSelectAll()
        {
            m_selection.m_links.clear();
            m_selection.m_objects.clear();
        }
        
        void Patcher::View::selectAll()
        {
            unSelectAll();
            
            auto& patcher = getPatcher();
            
            for(auto& object : patcher.getObjects())
            {
                m_selection.m_objects.emplace(object);
            }
            
            for(auto& link : patcher.getLinks())
            {
                m_selection.m_links.emplace(link);
            }
        }
        
        // ================================================================================ //
        //                                   PATCHER USER                                   //
        // ================================================================================ //
        
        Patcher::View& Patcher::User::addView()
        {
            return *m_views.emplace();
        }
        
        void Patcher::User::removeView(View const& view)
        {
            auto it = m_views.find_if([&view](View const& patcher_view) {
                return (&view == &patcher_view);
            });
            
            if(it != m_views.end())
            {
                m_views.erase(it);
            }
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
