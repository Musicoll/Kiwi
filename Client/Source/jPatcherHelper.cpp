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

#include "jPatcherHelper.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                    HITTESTER                                     //
    // ================================================================================ //
    
    HitTester::HitTester(jPatcher const& patcher) :
    m_patcher(patcher),
    m_object(nullptr),
    m_link(nullptr),
    m_target(Target::Nothing),
    m_zone(Zone::Outside),
    m_border(None),
    m_index(0)
    {
        ;
    }
    
    HitTester::~HitTester()
    {
        ;
    }
    
    void HitTester::reset()
    {
        m_object    = nullptr;
        m_link      = nullptr;
        m_target    = Target::Nothing;
        m_zone      = Zone::Outside;
		m_border    = None;
        m_index     = 0;
    }
    
    void HitTester::test(juce::Point<int> const& point) noexcept
    {
        reset();
        
        const bool hit_object = testObjects(point);
        
        if(!hit_object)
        {
            const bool hit_link = testLinks(point);
            
            if(!hit_link)
            {
                m_target = Target::Patcher;
                m_zone   = Zone::Inside;
            }
        }
    }
    
    bool HitTester::testObjects(juce::Point<int> const& point) noexcept
    {
        reset();
        
        for(auto& box_uptr : m_patcher.getObjects())
        {
            if(box_uptr)
            {
                const auto box_bounds = box_uptr->getBounds();
                if(box_bounds.contains(point.x, point.y))
                {
                    const auto relative_point = point - box_bounds.getPosition();
                    if(box_uptr->hitTest(relative_point, *this))
                    {
                        m_object = box_uptr.get();
                        m_target = Target::Box;
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    bool HitTester::testLinks(juce::Point<int> const& point) noexcept
    {
        reset();
        
        for(auto& link_uptr : m_patcher.getLinks())
        {
            if(link_uptr)
            {
                const auto link_bounds = link_uptr->getBounds();
                if(link_bounds.contains(point.x, point.y))
                {
                    const auto relative_point = point - link_bounds.getPosition();
                    if(link_uptr->hitTest(relative_point, *this))
                    {
                        m_link = link_uptr.get();
                        m_target = Target::Link;
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    void HitTester::test(juce::Rectangle<int> const& rect,
                         std::vector<jObject*>& objects, std::vector<jLink*>& links)
    {
        testObjects(rect, objects);
        testLinks(rect, links);
    }
    
    void HitTester::testObjects(juce::Rectangle<int> const& rect, std::vector<jObject*>& objects)
    {
        objects.clear();
        
        for(auto& box_uptr : m_patcher.getObjects())
        {
            if(box_uptr && box_uptr->hitTest(rect))
            {
                objects.push_back(box_uptr.get());
            }
        }
    }
    
    void HitTester::testLinks(juce::Rectangle<int> const& rect, std::vector<jLink*>& links)
    {
        links.clear();
        
        for(auto& link_uptr : m_patcher.getLinks())
        {
            if(link_uptr && link_uptr->hitTest(rect))
            {
                links.push_back(link_uptr.get());
            }
        }
    }
    
    jPatcher const& HitTester::getPatcher() const noexcept
    {
        return m_patcher;
    }
    
    jObject* HitTester::getObject() const noexcept
    {
        if(m_target == Target::Box)
        {
            return m_object;
        }
        
        return nullptr;
    }
    
    jLink* HitTester::getLink() const noexcept
    {
        if(m_target == Target::Link)
        {
            return m_link;
        }
        return nullptr;
    }
    
    HitTester::Zone HitTester::getZone() const noexcept
    {
        if(m_target == Target::Box)
        {
            return m_zone;
        }
        else if(m_target == Target::Link)
        {
            return std::max<Zone>(Zone::Outside, std::min<Zone>(m_zone, Zone::Outlet));
        }
        else if(m_target == Target::Patcher)
        {
            return std::max<Zone>(Zone::Outside, std::min<Zone>(m_zone, Zone::Inside));
        }
        
        return Zone::Outside;
    }
    
    int HitTester::getBorder() const noexcept
    {
        if(m_target == Target::Box)
        {
            return m_border;
        }
        return None;
    }
    
    size_t HitTester::getIndex() const noexcept
    {
        if(m_target == Target::Box)
        {
            return m_index;
        }
        return 0;
    }
    
    jPatcherViewport::jPatcherViewport(jPatcher& patcher) :
    m_patcher(patcher)
    {
        setSize(600, 400);
        m_patcher.setSize(600, 400);
        m_patcher_holder.setSize(600, 400);
        m_patcher_holder.addAndMakeVisible(m_patcher);
        setViewedComponent(&m_patcher_holder, false);
        setScrollBarThickness(12);
    }
    
    void jPatcherViewport::visibleAreaChanged(juce::Rectangle<int> const& new_visible_area)
    {
        //Console::post("visibleAreaChanged : " + new_visible_area.toString().toStdString());
        //m_last_view_area = new_visible_area;
        //m_patcher.updatePatcherArea(false);
        
        bool resized = (new_visible_area.getWidth() != m_last_view_area.getWidth() ||
                        new_visible_area.getHeight() != m_last_view_area.getHeight());
        
        if(resized)
        {
            m_last_view_position = new_visible_area.getPosition();
        }
        else
        {
            m_last_view_area = new_visible_area;
        }
    }
    
    void jPatcherViewport::resized()
    {
        m_patcher.viewportResized(m_last_bounds, getBounds());
        
        Viewport::resized();
           
        m_last_bounds = getBounds();
    }
    
    juce::Point<int> jPatcherViewport::getLastViewPosition()
    {
        return m_last_view_area.getPosition();
    }
}
