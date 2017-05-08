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

#include "KiwiApp_PatcherViewport.hpp"
#include "KiwiApp_PatcherView.hpp"
#include "KiwiApp_ObjectView.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  PATCHER VIEWPORT                                //
    // ================================================================================ //
    
    PatcherViewport::PatcherViewport(PatcherView& patcher) :
    m_patcher(patcher),
    m_can_hook_resized(false)
    {
        m_patcher.setSize(600, 400);
        m_magnifier.setSize(600, 400);
        m_magnifier.addAndMakeVisible(m_patcher);
        setViewedComponent(&m_magnifier, false);
        setScrollBarThickness(8);
        
        setSize(600, 400);
    }
    
    void PatcherViewport::visibleAreaChanged(juce::Rectangle<int> const& new_visible_area)
    {
        ;
    }
    
    void PatcherViewport::resized()
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
    
    void PatcherViewport::setZoomFactor(double zoom_factor)
    {
        if(zoom_factor != m_zoom_factor)
        {
            const double min_zoom = 0.25;
            m_zoom_factor = zoom_factor < min_zoom ? min_zoom : zoom_factor;
            m_patcher.setTransform(juce::AffineTransform::scale(m_zoom_factor));
            
            updatePatcherArea(false);
        }
    }
    
    double PatcherViewport::getZoomFactor() const noexcept
    {
        return m_zoom_factor;
    }
    
    juce::Rectangle<int> PatcherViewport::getRelativeViewArea() const noexcept
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
    
    juce::Point<int> PatcherViewport::getRelativePosition(juce::Point<int> point) const noexcept
    {
        return (point / m_zoom_factor) - getOriginPosition();
    }
    
    juce::Point<int> PatcherViewport::getRelativeViewPosition() const noexcept
    {
        return getRelativePosition(getViewPosition());
    }
    
    void PatcherViewport::setRelativeViewPosition(juce::Point<int> position)
    {
        setViewPosition((position + getOriginPosition()) * m_zoom_factor);
    }
    
    void PatcherViewport::jumpViewToObject(ObjectView const& object_view)
    {
        const auto view_area = getRelativeViewArea();
        auto object_bounds = object_view.getBoxBounds();
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
    
    void PatcherViewport::bringRectToCentre(juce::Rectangle<int> bounds)
    {
        const juce::Rectangle<int> view_area = getRelativeViewArea();
        const juce::Point<int> center = bounds.getCentre();
        
        const juce::Point<int> new_view_pos
        {
            static_cast<int>(center.getX() - view_area.getWidth()*0.5),
            static_cast<int>(center.getY() - view_area.getHeight()*0.5)
        };
        
        setRelativeViewPosition(new_view_pos);
    }
    
    void PatcherViewport::resetObjectsArea()
    {
        m_patching_area = m_patcher.getCurrentObjectsArea();
        updatePatcherArea(true);
    }
    
    juce::Rectangle<int> PatcherViewport::getObjectsArea() const noexcept
    {
        return m_patching_area;
    }
    
    juce::Point<int> PatcherViewport::getOriginPosition() const noexcept
    {
        const int x = m_patching_area.getX();
        const int y = m_patching_area.getY();
        
        return juce::Point<int>( x < 0 ? -x : 0, y < 0 ? -y : 0 );
    }
    
    void PatcherViewport::viewportResized(juce::Rectangle<int> const& last_bounds,
                                           juce::Rectangle<int> const& new_bounds)
    {
        const int delta_width = new_bounds.getWidth() - last_bounds.getWidth();
        const int delta_height = new_bounds.getHeight() - last_bounds.getHeight();
        
        int new_width = m_patcher.getWidth() + delta_width;
        int new_height = m_patcher.getHeight() + delta_height;
        
        const juce::Rectangle<int> objects_area = m_patcher.getCurrentObjectsArea();
        const int objects_width = objects_area.getWidth();
        const int objects_height = objects_area.getHeight();
        
        const juce::Point<int> view_pos = getViewPosition();
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
    
    void PatcherViewport::updatePatcherArea(bool keep_view_pos)
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
        
        const juce::Rectangle<int> objects_area = m_patching_area.withPosition(origin);
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
}
