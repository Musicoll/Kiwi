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

#include "KiwiLine.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      SEGMENT                                     //
    // ================================================================================ //
    
    Point Segment::getNearestPoint(Point const& pt) const noexcept
    {
        return pt.nearest(m_start, m_end);
    }
    
    bool Segment::intersects(Segment const& s) const noexcept
    {
        Point ignored;
        return intersects(s, ignored);
    }
    
    bool Segment::intersects(Segment const& s, Point& intersection) const noexcept
    {
        if(m_start == s.m_start || m_start == s.m_start)
        {
            intersection = m_start;
            return true;
        }
        else if(m_end == s.m_end || m_end == s.m_end)
        {
            intersection = m_end;
            return true;
        }
        
        const Point d1(m_end - m_start);
        const Point d2(s.m_end - s.m_start);
        const double divisor = d1.x() * d2.y() - d2.x() * d1.y();
        
        if(divisor == 0)
        {
            if(!(d1 == Point() || d2 == Point()))
            {
                if (d1.y() == 0 && d2.y() != 0)
                {
                    const double along = (m_start.y() - s.m_start.y()) / d2.y();
                    intersection = Point(s.m_start.x() + along * d2.x(), m_start.y());
                    return along >= 0. && along <= 1.;
                }
                else if (d2.y() == 0 && d1.y() != 0)
                {
                    const double along = (s.m_start.y() - m_start.y()) / d1.y();
                    intersection = Point(m_start.x() + along * d1.x(), s.m_start.y());
                    return along >= 0. && along <= 1.;
                }
                else if (d1.x() == 0. && d2.x() != 0)
                {
                    const double along = (m_start.x() - s.m_start.x()) / d2.x();
                    intersection = Point(m_start.x(), s.m_start.y() + along * d2.y());
                    return along >= 0. && along <= 1.;
                }
                else if (d2.x() == 0 && d1.x() != 0)
                {
                    const double along = (s.m_start.x() - m_start.x()) / d1.x();
                    intersection = Point(s.m_start.x(), m_start.y() + along * d1.y());
                    return along >= 0. && along <= 1.;
                }
            }
            
            intersection = (m_end + s.m_start) / 2.;
            return false;
        }
        
        const double along1 = ((m_start.y() - s.m_start.y()) * d2.x() - (m_start.x() - s.m_start.x()) * d2.y()) / divisor;
        intersection = m_start + d1 * along1;
        
        if (along1 < 0 || along1 > 1.)
            return false;
        
        const double along2 = ((m_start.y() - s.m_start.y()) * d1.x() - (m_start.x() - s.m_start.x()) * d1.y()) / divisor;
        return along2 >= 0 && along2 <= 1.;
    }
    
    bool Segment::intersects(BezierQuad const& curve) const noexcept
    {
        return curve.intersects(*this);
    }
    
    bool Segment::intersects(BezierCubic const& curve) const noexcept
    {
        return curve.intersects(*this);
    }
    
    vector<Point> BezierCubic::fromArc(Point const& center, const Point& radius, double startAngle, double endAngle) noexcept
    {
        vector<Point> points;
        
        double range = wrap(endAngle - startAngle, 0., M_PI * 2.);
        
        points.push_back(Point(center.x() + radius.x() * cos(startAngle),
                               center.y() - radius.y() * sin(startAngle)));
        
        while (range >= 0.)
        {
            const double r2 = ((range > M_PI_2) ? M_PI_2 : range) * 0.5;
            const double b = sin(r2);
            const double c = cos(r2);
            const double x = (1. - c) * 4. / 3.;
            const double y = b - x * c / (b != 0. ? b : 1.);
            const double ss = sin(startAngle + r2);
            const double cc = cos(startAngle + r2);
            const Point coords[3] = {Point(c + x, y), Point(c + x, -y), Point(c, -b)};
            
            for(ulong i = 0; i < 3; i++)
            {
                points.push_back(Point(center.x() + radius.x() * (coords[i].x() * cc + coords[i].y() * ss),
                                       center.y() + radius.y() * (-coords[i].x() * ss + coords[i].y() * cc)));
            }
            
            range -= M_PI_2;
            startAngle += M_PI_2;
        }
        
        return points;
    }
}
