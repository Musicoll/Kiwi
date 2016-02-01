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

#include "KiwiRectangle.h"

namespace Kiwi
{
    Rectangle Rectangle::resized(const ulong flags, Point const& d, const Point smin, const Point smax, const bool preserve, const bool opposite) const noexcept
    {
        Rectangle newrect = *this;
        const double minx = max(0., smin.x());
        const double miny = max(0., smin.y());
        const double maxx = max(0., smax.x());
        const double maxy = max(0., smax.y());
        double initialRatio = m_size.ratio();
        
        if(flags & Left)
        {
            if(!opposite)
            {
                newrect.left(min(newrect.right() - minx, newrect.x() + d.x()));
            }
            else
            {
                newrect.left(min(x() + (width() * 0.5) - (minx * 0.5), newrect.x() + d.x()));
                newrect.right(max(newrect.left() + minx, newrect.right() - d.x()));
            }
        }
        if(flags & Right)
        {
            if(!opposite)
            {
                newrect.width(max(minx, newrect.width() + d.x()));
            }
            else
            {
                newrect.left(min(x() + (width() * 0.5) - (minx * 0.5), newrect.x() - d.x()));
                newrect.right(max(newrect.left() + minx, newrect.right() + d.x()));
            }
        }
        if(flags & Top)
        {
            if(!opposite)
            {
                newrect.top(min(newrect.bottom() - miny, newrect.y() + d.y()));
            }
            else
            {
                newrect.top(min(y() + (height() * 0.5) - (miny * 0.5), newrect.y() + d.y()));
                newrect.height(max(miny, newrect.height() - d.y()));
            }
        }
        if(flags & Bottom)
        {
            if(!opposite)
            {
                newrect.height(max(miny, newrect.height() + d.y()));
            }
            else
            {
                newrect.top(min(y() + (height() * 0.5) - (miny * 0.5), newrect.y() - d.y()));
                newrect.height(max(miny, newrect.height() + d.y()));
            }
        }
        
        if(preserve || (initialRatio > 0.))
        {
            bool adjustWidth;
            double ratio = 1.;
            if(initialRatio > 0.)
            {
                ratio = initialRatio;
            }
            else if(height() > 0)
            {
                ratio = width() / height();
            }
            
            if((flags & Top || flags & Bottom) && !(flags & Left || flags & Right))
            {
                adjustWidth = true;
            }
            else if((flags & Left || flags & Right) && ! (flags & Top || flags & Bottom))
            {
                adjustWidth = false;
            }
            else
            {
                const double oldRatio = (height() > 0) ? abs(width() / (double)height()) : 0.;
                const double newRatio = abs(newrect.width() / (double)newrect.height());
                adjustWidth = (oldRatio > newRatio);
            }
            
            if(adjustWidth)
            {
                newrect.width((int)(newrect.height() * ratio));
                if((maxx > 0. && newrect.width() > maxx) || newrect.width() < minx)
                {
                    newrect.width(clip(newrect.width(), minx, maxx));
                    newrect.height((int)(newrect.width() / ratio));
                }
            }
            else
            {
                newrect.height((int)(newrect.width() / ratio));
                if ((maxy > 0. && newrect.height() > maxy) || newrect.height() < miny)
                {
                    newrect.height(clip(newrect.height(), miny, maxy));
                    newrect.width((int)(newrect.height() * ratio));
                }
            }
            if((flags & Top || flags & Bottom) && !(flags & Left || flags & Right))
            {
                newrect.x(x() + (width() - newrect.width()) / 2);
            }
            else if((flags & Left || flags & Right) && ! (flags & Top || flags & Bottom))
            {
                newrect.y(y() + (height() - newrect.height()) / 2);
            }
            else
            {
                if(flags & Left)
                {
                    newrect.x(right() - newrect.width());
                }
                if(flags & Top)
                {
                    newrect.y(bottom() - newrect.height());
                }
            }
        }
        
        return newrect;
    }
    
    bool Rectangle::intersects(Segment const& segment) const noexcept
    {
        return (segment.intersects(Segment(topLeft(),    topRight()))    ||
                segment.intersects(Segment(topRight(),   bottomRight())) ||
                segment.intersects(Segment(bottomLeft(), bottomRight())) ||
                segment.intersects(Segment(topLeft(),    bottomLeft())));
    }
    
    bool Rectangle::overlaps(Segment const& segment) const noexcept
    {
        return (contains(segment.start()) || contains(segment.end()) || intersects(segment));
    }
    
    bool Rectangle::overlaps(BezierQuad const& curve) const noexcept
    {
        if(contains(curve.start()) || contains(curve.end()))
        {
            return true;
        }
        else
        {
            const vector<Point> points = curve.discretized(100);
            
            for(ulong i = 0; i < points.size(); i+=2)
            {
                if(intersects(Segment(points[i], points[i+1])))
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    bool Rectangle::overlaps(BezierCubic const& curve) const noexcept
    {
        if(contains(curve.start()) || contains(curve.end()))
        {
            return true;
        }
        else
        {
            const vector<Point> points = curve.discretized(100);
            
            for(ulong i = 0; i < points.size(); i+=2)
            {
                if(intersects(Segment(points[i], points[i+1])))
                {
                    return true;
                }
            }
        }
        
        return false;
    }
}