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

#include <KiwiGraphics/KiwiPath.h>

namespace kiwi
{
    namespace graphics
    {
        void Path::transform(AffineMatrix const& matrix) noexcept
        {
            for(auto&& node : m_nodes)
            {
                node.transform(matrix);
            }
            //m_bounds update
        }
        
        Path Path::transformed(AffineMatrix const& matrix) const noexcept
        {
            Path p = *this;
            p.transform(matrix);
            return p;
        }
        
        void Path::addRectangle(Rectangle const& rect, const double r) noexcept
        {
            addRectangle(rect.x(), rect.y(), rect.width(), rect.height(), r, r,
                         Rectangle::TopLeft | Rectangle::TopRight | Rectangle::BottomLeft | Rectangle::BottomRight);
        }
        
        void Path::addRectangle(const double x, const double y, const double w, const double h,
                                double rx, double ry, const char corner) noexcept
        {
            rx = clip(rx, 0., w * 0.5f);
            ry = clip(ry, 0., h * 0.5f);
            const double r45x = rx * 0.45f;
            const double r45y = ry * 0.45f;
            const double x2 = x + w;
            const double y2 = y + h;
            
            if (corner & Rectangle::TopLeft)
            {
                addNode(Node(Point(x, y + ry), Move));
                cubicTo(Point(x, y + r45y), Point(x + r45x, y), Point(x + rx, y));
            }
            else
            {
                addNode(Node(Point(x, y), Move));
            }
            
            if (corner & Rectangle::TopRight)
            {
                lineTo(Point(x2 - rx, y));
                cubicTo(Point(x2 - r45x, y), Point(x2, y + r45y), Point(x2, y + ry));
            }
            else
            {
                lineTo(Point(x2, y));
            }
            
            if (corner & Rectangle::BottomRight)
            {
                lineTo(Point(x2, y2 - ry));
                cubicTo(Point(x2, y2 - r45y), Point(x2 - r45x, y2), Point(x2 - rx, y2));
            }
            else
            {
                lineTo(Point(x2, y2));
            }
            
            if (corner & Rectangle::BottomLeft)
            {
                lineTo(Point(x + rx, y2));
                cubicTo(Point(x + r45x, y2), Point(x, y2 - r45y), Point(x, y2 - ry));
            }
            else
            {
                lineTo(Point(x, y2));
            }
            
            close();
        }
        
        void Path::addEllipse(Rectangle const& rect) noexcept
        {
            const double hw = rect.width() * 0.5f;
            const double hw55 = hw * 0.55f;
            const double hh = rect.height() * 0.5f;
            const double hh55 = hh * 0.55f;
            const double cx = rect.x() + hw;
            const double cy = rect.y() + hh;
            
            moveTo(Point(cx, cy - hh));
            cubicTo(Point(cx + hw55, cy - hh), Point(cx + hw, cy - hh55), Point(cx + hw, cy));
            cubicTo(Point(cx + hw, cy + hh55), Point(cx + hw55, cy + hh), Point(cx, cy + hh));
            cubicTo(Point(cx - hw55, cy + hh), Point(cx - hw, cy + hh55), Point(cx - hw, cy));
            cubicTo(Point(cx - hw, cy - hh55), Point(cx - hw55, cy - hh), Point(cx, cy - hh));
            close();
        }
        
        void Path::addEllipse(Point const& center, const double rx, const double ry) noexcept
        {
            const Point delta(std::max(0., rx), std::max(0., ry));
            addEllipse(Rectangle::withCorners(center - delta, center + delta));
        }
        
        void Path::addArc(Point const& center, const Point& radius, const double start, const double end) noexcept
        {
            const std::vector<Point> points = BezierCubic::fromArc(center, radius, start, end);
            moveTo(points[0]);
            for(std::vector<Point>::size_type i = 1; i < points.size(); i++)
            {
                addNode(Node(points[i], Cubic));
            }
        }
        
        void Path::addArc(Point const& center, const Point& radius, const double start, const double end, const double rot) noexcept
        {
            const std::vector<Point> points = BezierCubic::fromArc(center, radius, start, end);
            moveTo(points[0].rotated(center, rot));
            for(std::vector<Point>::size_type i = 1; i < points.size(); i++)
            {
                addNode(Node(points[i].rotated(center, rot), Cubic));
            }
        }
        
        void Path::addPieChart(Point const& center, const Point& radius, const double start, const double end) noexcept
        {
            const std::vector<Point> points = BezierCubic::fromArc(center, radius, start, end);
            moveTo(center);
            lineTo(points[0]);
            for(std::vector<Point>::size_type i = 1; i < points.size(); i++)
            {
                addNode(Node(points[i], Cubic));
            }
            lineTo(center);
            close();
        }
        
        
        bool Path::overlaps(Rectangle const& rect) const noexcept
        {
            if(m_nodes.size() == 1)
            {
                return rect.contains(m_nodes[0].point());
            }
            else if(m_nodes.size() > 1)
            {
                Point previous;
                for(std::vector<Node>::size_type i = 0; i < m_nodes.size(); i++)
                {
                    Point current = m_nodes[i].point();
                    switch(m_nodes[i].mode())
                    {
                        case Move:
                            if(rect.contains(current))
                            {
                                return true;
                            }
                            break;
                        case Linear:
                            if(rect.overlaps(Segment(previous, current)))
                            {
                                return true;
                            }
                            break;
                        case Quadratic:
                            i++;
                            if(i < m_nodes.size())
                            {
                                const Point ctrl = current;
                                current = m_nodes[i].point();
                                if(rect.overlaps(BezierQuad(previous, ctrl, current)))
                                {
                                    return true;
                                }
                                
                            }
                            break;
                        case Cubic:
                            i += 2;
                            if(i < m_nodes.size())
                            {
                                const Point ctrl1 = current;
                                const Point ctrl2 = m_nodes[i-1].point();
                                current = m_nodes[i].point();
                                if(rect.overlaps(BezierCubic(previous, ctrl1, ctrl2, current)))
                                {
                                    return true;
                                }
                                
                            }
                            break;
                            
                        default:
                            break;
                    }
                    previous = current;
                }
            }
            return false;
        }
    }
}