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

#include "KiwiModelDataModel.hpp"
#include "KiwiModelPatcherView.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                  PATCHER::declare                                //
        // ================================================================================ //
        
        void Patcher::View::Object::declare()
        {
            assert(! DataModel::has<Patcher::View::Object>());
            
            DataModel::declare<Patcher::View::Object>()
            .name("cicm.kiwi.Patcher.View.Object")
            .template member<flip::ObjectRef<model::Object>, &View::Object::m_ref>("ref");
        }
        
        void Patcher::View::Link::declare()
        {
            assert(! DataModel::has<Patcher::View::Link>());
            
            DataModel::declare<Patcher::View::Link>()
            .name("cicm.kiwi.Patcher.View.Link")
            .member<flip::ObjectRef<model::Link>, &View::Link::m_ref>("ref");
        }
        
        void Patcher::View::declare()
        {
            assert(! DataModel::has<Patcher::View>());
            
            Patcher::View::Object::declare();
            Patcher::View::Link::declare();
            
            DataModel::declare<Patcher::View>()
            .name("cicm.kiwi.Patcher.View")
            .member<flip::Collection<View::Object>, &View::m_selected_objects>("selected_objects")
            .member<flip::Collection<View::Link>, &View::m_selected_links>("selected_links")
            .member<flip::Bool, &View::m_is_locked>("locked")
            .member<flip::Float, &View::m_zoom_factor>("zoom_factor");
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
    }
}
