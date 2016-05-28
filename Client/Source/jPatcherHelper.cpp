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
            if(link_uptr && link_uptr->hitTest(rect.toFloat()))
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
    
    // ================================================================================ //
    //                                  JPATCHER VIEWPORT                               //
    // ================================================================================ //
    
    jPatcherViewport::jPatcherViewport(jPatcher& patcher) :
    m_patcher(patcher),
    m_can_hook_resized(false)
    {
        m_patcher.setSize(600, 400);
        m_magnifier.setSize(600, 400);
        m_magnifier.addAndMakeVisible(m_patcher);
        setViewedComponent(&m_magnifier, false);
        setScrollBarThickness(12);
        
        setSize(600, 400);
    }
    
    void jPatcherViewport::visibleAreaChanged(juce::Rectangle<int> const& new_visible_area)
    {
        ;
    }
    
    void jPatcherViewport::resized()
    {
        if(!m_can_hook_resized)
        {
            Viewport::resized();
            m_can_hook_resized = true;
            m_last_bounds = getBounds();
        }
        else
        {
            viewportResized(m_last_bounds, getBounds());
            
            Viewport::resized();
            
            m_last_bounds = getBounds();
        }
    }
    
    void jPatcherViewport::setZoomFactor(double zoom_factor)
    {
        if(zoom_factor != m_zoom_factor)
        {
            const double min_zoom = 0.25;
            m_zoom_factor = zoom_factor < min_zoom ? min_zoom : zoom_factor;
            m_patcher.setTransform(AffineTransform::scale(m_zoom_factor));
            
            updatePatcherArea(false);
        }
    }
    
    double jPatcherViewport::getZoomFactor() const noexcept
    {
        return m_zoom_factor;
    }
    
    juce::Rectangle<int> jPatcherViewport::getRelativeViewArea() const noexcept
    {
        const auto rel_view_pos = getRelativeViewPosition();
        
        return
        {
            rel_view_pos.getX(),
            rel_view_pos.getY(),
            static_cast<int>(getViewWidth() / m_zoom_factor),
            static_cast<int>(getViewHeight() / m_zoom_factor)
        };
    }
    
    juce::Point<int> jPatcherViewport::getRelativePosition(juce::Point<int> point) const noexcept
    {
        return (point / m_zoom_factor) - getOriginPosition();
    }
    
    juce::Point<int> jPatcherViewport::getRelativeViewPosition() const noexcept
    {
        return getRelativePosition(getViewPosition());
    }
    
    void jPatcherViewport::setRelativeViewPosition(juce::Point<int> position)
    {
        setViewPosition((position + getOriginPosition()) * m_zoom_factor);
    }
    
    void jPatcherViewport::jumpViewToObject(jObject const& object_j)
    {
        const auto view_area = getRelativeViewArea();
        auto object_bounds = object_j.getBoxBounds();
        object_bounds.setPosition(((object_bounds.getPosition() - getOriginPosition())));
        
        if(! view_area.contains(object_bounds))
        {
            juce::Point<int> view_pos = view_area.getPosition();
            juce::Point<int> area_bottom_right = view_area.getBottomRight();
            juce::Point<int> object_bottom_right = object_bounds.getBottomRight();
            
            if(object_bounds.getX() < view_area.getX())
            {
                view_pos.setX(object_bounds.getX());
            }
            else if(object_bottom_right.getX() > area_bottom_right.getX())
            {
                view_pos.setX(view_pos.getX() + object_bottom_right.getX() - area_bottom_right.getX());
            }
            
            if(object_bounds.getY() < view_area.getY())
            {
                view_pos.setY(object_bounds.getY());
            }
            else if(object_bottom_right.getY() > area_bottom_right.getY())
            {
                view_pos.setY(view_pos.getY() + object_bottom_right.getY() - area_bottom_right.getY());
            }
            
            setRelativeViewPosition(view_pos);
        }
    }
    
    void jPatcherViewport::resetObjectsArea()
    {
        m_patching_area = m_patcher.getCurrentObjectsArea();
        updatePatcherArea(true);
    }
    
    juce::Rectangle<int> jPatcherViewport::getObjectsArea() const noexcept
    {
        return m_patching_area;
    }
    
    juce::Point<int> jPatcherViewport::getOriginPosition() const noexcept
    {
        const int x = m_patching_area.getX();
        const int y = m_patching_area.getY();
        
        return juce::Point<int>( x < 0 ? -x : 0, y < 0 ? -y : 0 );
    }
    
    void jPatcherViewport::viewportResized(juce::Rectangle<int> const& last_bounds,
                                           juce::Rectangle<int> const& new_bounds)
    {
        const int delta_width = new_bounds.getWidth() - last_bounds.getWidth();
        const int delta_height = new_bounds.getHeight() - last_bounds.getHeight();
        
        int new_width = m_patcher.getWidth() + delta_width;
        int new_height = m_patcher.getHeight() + delta_height;
        
        const Rectangle<int> objects_area = m_patcher.getCurrentObjectsArea();
        const int objects_width = objects_area.getWidth();
        const int objects_height = objects_area.getHeight();
        
        const Point<int> view_pos = getViewPosition();
        const double zoom = getZoomFactor();
        
        if(delta_width != 0)
        {
            const int viewport_width = (getMaximumVisibleWidth() + view_pos.getX()) / zoom;
            
            bool need_resize = false;
            
            if(delta_width < 0)
            {
                if(viewport_width - delta_width > objects_width)
                {
                    new_width = viewport_width - delta_width;
                    need_resize = true;
                }
                else
                {
                    new_width = objects_width;
                }
            }
            else if(delta_width > 0)
            {
                const bool smaller_than_objects_area = (objects_width > viewport_width);
                
                if(smaller_than_objects_area)
                {
                    const bool will_be_bigger = (objects_width < (new_bounds.getWidth() + view_pos.getX()) / zoom);
                    
                    if(!will_be_bigger)
                    {
                        new_width = objects_width;
                    }
                    else
                    {
                        new_width = objects_width + (delta_width * 10);
                        need_resize = true;
                    }
                }
                else
                {
                    new_width = viewport_width + (delta_width * 10);
                    need_resize = true;
                }
            }
            
            if(need_resize)
            {
                m_magnifier.setSize(new_width * zoom, new_height * zoom);
                new_width = (getMaximumVisibleWidth() + view_pos.getX()) / zoom + 1;
            }
        }
        
        if(delta_height != 0)
        {
            const int viewport_height = (getMaximumVisibleHeight() + view_pos.getY()) / zoom;
            
            bool need_resize = false;
            
            if(delta_height < 0)
            {
                if(viewport_height - delta_height > objects_height)
                {
                    new_height = viewport_height - delta_height;
                    need_resize = true;
                }
                else
                {
                    new_height = objects_height;
                }
            }
            else if(delta_height > 0)
            {
                const bool smaller_than_objects_area = (objects_height > viewport_height);
                
                if(smaller_than_objects_area)
                {
                    const bool will_be_bigger = (objects_height < (new_bounds.getHeight() + view_pos.getY()) / zoom);
                    
                    if(!will_be_bigger)
                    {
                        new_height = objects_height;
                    }
                    else
                    {
                        new_height = objects_height + (delta_height * 10);
                        need_resize = true;
                    }
                }
                else
                {
                    new_height = viewport_height + (delta_height * 10);
                    need_resize = true;
                }
            }
            
            if(need_resize)
            {
                //m_magnifier.setSize(new_width * zoom, new_height * zoom);
                //new_height = (getMaximumVisibleHeight() + view_pos.getY()) / zoom + 1;
            }
        }
        
        m_patching_area.setSize(new_width, new_height);
        m_patcher.setSize(new_width, new_height);
        m_magnifier.setSize(new_width * zoom, new_height * zoom);
    }
    
    void jPatcherViewport::updatePatcherArea(bool keep_view_pos)
    {
        const juce::Point<int> view_pos = getViewPosition();
        const juce::Point<int> last_origin = getOriginPosition();
        const double zoom = getZoomFactor();
        
        const int viewport_width = getMaximumVisibleWidth() / zoom;
        const int viewport_height = getMaximumVisibleHeight() / zoom;
        
        juce::Rectangle<int> objects_current_area = m_patcher.getCurrentObjectsArea();
        
        m_patching_area.setLeft(objects_current_area.getX());
        m_patching_area.setTop(objects_current_area.getY());
        
        if(objects_current_area.getWidth() > m_patching_area.getWidth())
        {
            m_patching_area.setWidth(objects_current_area.getWidth());
        }

        if(objects_current_area.getHeight() > m_patching_area.getHeight())
        {
            m_patching_area.setHeight(objects_current_area.getHeight());
        }

        const juce::Point<int> origin = getOriginPosition();
        const int origin_x = origin.getX();
        const int origin_y = origin.getY();
        
        if(last_origin != origin)
        {
            m_patcher.originPositionChanged();
        }
        
        const Rectangle<int> objects_area = m_patching_area.withPosition(origin);
        const int objects_width = objects_area.getWidth();
        const int objects_height = objects_area.getHeight();
        
        int new_width = (viewport_width > objects_width) ? viewport_width : objects_width;
        int new_height = (viewport_height > objects_height) ? viewport_height : objects_height;
        
        // patcher positive area should never be smaller than viewport area
        new_width = new_width < (viewport_width + origin_x) ? (viewport_width + origin_x) : new_width;
        new_height = new_height < (viewport_height + origin_y) ? (viewport_height + origin_y) : new_height;
        
        m_patcher.setSize(new_width, new_height);
        m_magnifier.setSize(new_width * zoom, new_height * zoom);
        
        if(m_patching_area.getWidth() > viewport_width + (view_pos.getX() / zoom))
        {
            m_patching_area.setWidth(viewport_width + (view_pos.getX() / zoom));
        }
        
        if(m_patching_area.getHeight() > viewport_height + (view_pos.getY() / zoom))
        {
            m_patching_area.setHeight(viewport_height + (view_pos.getY() / zoom));
        }
        
        if(keep_view_pos)
        {
            const juce::Point<int> delta = (view_pos - origin) - (view_pos - last_origin);
            setViewPosition(view_pos - delta);
        }
    }
    
    // ================================================================================ //
    //                                   IOLET HILIGHTER                                //
    // ================================================================================ //
    
    IoletHighlighter::IoletHighlighter()
    {
        setInterceptsMouseClicks(false, false);
        setAlwaysOnTop(true);
        
        setVisible(false);
        setBounds(0, 0, 1, 1);
    }
    
    void IoletHighlighter::hide()
    {
        setVisible(false);
    }
    
    void IoletHighlighter::paint(juce::Graphics& g)
    {
        const juce::Colour bgcolor = m_is_inlet ? juce::Colour(0xFF17BEBB) : juce::Colour(0xFFCD5334);
        const juce::Colour bd_color(0xFF2E282A);
        
        const juce::Rectangle<float> bounds = getLocalBounds().reduced(1).toFloat();
        
        g.setColour(bgcolor);
        g.fillRect(bounds);
        
        g.setColour(bd_color);
        g.drawRect(bounds);
    }
    
    void IoletHighlighter::highlightInlet(jObject const& object, const size_t index)
    {
        const juce::Point<int> io_center = object.getInletPatcherPosition(index);
        juce::Rectangle<int> new_bounds = juce::Rectangle<int>(io_center, io_center).expanded(5);
        
        m_is_inlet = true;
        
        setBounds(new_bounds);
        setVisible(true);
    }
    
    void IoletHighlighter::highlightOutlet(jObject const& object, const size_t index)
    {
        const juce::Point<int> io_center = object.getOutletPatcherPosition(index);
        juce::Rectangle<int> new_bounds = juce::Rectangle<int>(io_center, io_center).expanded(5);
        
        m_is_inlet = false;
        
        setBounds(new_bounds);
        setVisible(true);
    }
    
    // ================================================================================ //
    //										JLASSO                                      //
    // ================================================================================ //
    jLasso::jLasso(jPatcher& patcher) : m_patcher(patcher), m_dragging(false)
    {
        setInterceptsMouseClicks(false, false);
        setWantsKeyboardFocus(false);
        setAlwaysOnTop(true);
        setVisible(false);
        setBounds(0, 0, 1, 1);
    }
    
    jLasso::~jLasso()
    {
        
    }
    
    bool jLasso::isPerforming() const noexcept
    {
        return m_dragging;
    }
    
    void jLasso::paint(Graphics& g)
    {
        const juce::Rectangle<int> bounds = getLocalBounds();
        const juce::Colour color = juce::Colour::fromFloatRGBA(0.96, 0.96, 0.96, 1.);
        
        g.setColour(color.withAlpha(0.5f));
        g.fillAll();
        g.setColour(color);
        g.drawRect(bounds, 1.);
    }
    
    void jLasso::begin(juce::Point<int> const& point, const bool preserve_selection)
    {
        assert(!m_dragging);
        
        if(!preserve_selection)
        {
            m_patcher.unselectAll();
        }
        else
        {
            m_objects = m_patcher.getSelectedObjects();
            m_links = m_patcher.getSelectedLinks();
        }

        m_start = point;
        m_dragging = true;
    }

    void jLasso::perform(juce::Point<int> const& point,
                         bool include_objects, bool include_links, const bool preserve)
    {
        bool selection_changed = false;
        
        juce::Rectangle<int> bounds = juce::Rectangle<int>(m_start, point);
        
        if(bounds.getWidth() == 0)
        {
            bounds.setWidth(1);
        }
        if(bounds.getHeight() == 0)
        {
            bounds.setHeight(1);
        }
        
        setBounds(bounds);
        
        m_dragging = true;
        setVisible(true);
        toFront(false);
        
        if(preserve)
        {
            if(include_objects)
            {
                jPatcher::jObjects const& objects = m_patcher.getObjects();
                
                HitTester hit(m_patcher);
                std::vector<jObject*> lasso_objects;
                hit.testObjects(bounds, lasso_objects);
                
                for(auto& object_j_uptr : objects)
                {
                    if(object_j_uptr)
                    {
                        jObject& object = *object_j_uptr.get();
                        
                        const bool is_selected = object.isSelected();
                        const bool was_selected = m_objects.find(object.getModel().ref()) != m_objects.end();
                        
                        const bool in_lasso = std::find(lasso_objects.begin(), lasso_objects.end(), &object) != lasso_objects.end();
                        
                        if (!is_selected && (was_selected != in_lasso))
                        {
                            m_patcher.selectObject(object);
                            selection_changed = true;
                        }
                        else if(is_selected && (was_selected == in_lasso))
                        {
                            m_patcher.unselectObject(object);
                            selection_changed = true;
                        }
                    }
                }
            }
            
            if(include_links)
            {
                jPatcher::jLinks const& links = m_patcher.getLinks();
                
                HitTester hit(m_patcher);
                std::vector<jLink*> lasso_links;
                hit.testLinks(bounds, lasso_links);
                
                for(auto& link_j_uptr : links)
                {
                    if(link_j_uptr)
                    {
                        jLink& link = *link_j_uptr.get();
                        
                        const bool is_selected = link.isSelected();
                        const bool was_selected = m_links.find(link.getModel().ref()) != m_links.end();
                        
                        const bool in_lasso = std::find(lasso_links.begin(), lasso_links.end(), &link) != lasso_links.end();
                        
                        if (!is_selected && (was_selected != in_lasso))
                        {
                            m_patcher.selectLink(link);
                            selection_changed = true;
                        }
                        else if(is_selected && (was_selected == in_lasso))
                        {
                            m_patcher.unselectLink(link);
                            selection_changed = true;
                        }
                    }
                }
            }
            
            if(selection_changed)
            {
                //ctrl->selectionChanged();
            }
        }
        else
        {
            m_patcher.unselectAll();
            HitTester hit(m_patcher);
            
            if(include_objects)
            {
                std::vector<jObject*> objects;
                hit.testObjects(bounds, objects);
                m_patcher.selectObjects(objects);
            }
            
            if(include_links)
            {
                std::vector<jLink*> links;
                hit.testLinks(bounds, links);
                m_patcher.selectLinks(links);
            }
        }
    }

    void jLasso::end()
    {
        m_dragging = false;
        setVisible(false);
        
        m_objects.clear();
        m_links.clear();
    }
}
