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

#include "KiwiApp_PatcherViewHitTester.hpp"
#include "KiwiApp_PatcherView.hpp"
#include "KiwiApp_ObjectView.hpp"
#include "KiwiApp_LinkView.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                    HITTESTER                                     //
    // ================================================================================ //
    
    HitTester::HitTester(PatcherView const& patcher) :
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
        
        const auto& objects = m_patcher.getObjects();
        for(auto it = objects.rbegin(); it != objects.rend(); ++it)
        {
            auto& box_uptr = *it;
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
        
        const auto& links = m_patcher.getLinks();
        for(auto it = links.rbegin(); it != links.rend(); ++it)
        {
            auto& link_uptr = *it;
            if(link_uptr)
            {
                const auto link_bounds = link_uptr->getBounds();
                if(link_bounds.contains(point.x, point.y))
                {
                    const auto relative_point = point - link_bounds.getPosition();
                    if(link_uptr->hitTest(relative_point, *this))
                    {
                        m_zone = HitTester::Zone::Inside;
                        m_link = link_uptr.get();
                        m_target = Target::Link;
                        m_index = 0;
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    void HitTester::test(juce::Rectangle<int> const& rect,
                         std::vector<ObjectView*>& objects, std::vector<LinkView*>& links)
    {
        testObjects(rect, objects);
        testLinks(rect, links);
    }
    
    void HitTester::testObjects(juce::Rectangle<int> const& rect, std::vector<ObjectView*>& objects)
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
    
    void HitTester::testLinks(juce::Rectangle<int> const& rect, std::vector<LinkView*>& links)
    {
        links.clear();
        
        for(auto& link_uptr : m_patcher.getLinks())
        {
            if(link_uptr && link_uptr->hitTest(rect.toFloat()))
            {
                links.push_back(link_uptr.get());
            }
        }
    }
    
    PatcherView const& HitTester::getPatcher() const noexcept
    {
        return m_patcher;
    }
    
    ObjectView* HitTester::getObject() const noexcept
    {
        if(m_target == Target::Box)
        {
            return m_object;
        }
        
        return nullptr;
    }
    
    LinkView* HitTester::getLink() const noexcept
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
}
