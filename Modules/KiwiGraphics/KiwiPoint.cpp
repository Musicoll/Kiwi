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

#include <KiwiGraphics/KiwiPoint.h>
#include <KiwiGraphics/KiwiPath.h>

namespace kiwi
{
    namespace graphics
    {
        // ================================================================================ //
        //                                      POINT                                       //
        // ================================================================================ //
        
        // =================================
        // Operators
        // =================================
        
        Point operator*(const double value, Point const& pt) noexcept
        {
            return Point(value * pt.x(),
                         value * pt.y());
        }
        
        Point operator*(Point const& pt, double const value) noexcept
        {
            return value * pt;
        }
        
        Point& operator*=(Point &pt, double const value) noexcept
        {
            pt = value * pt;
            return pt;
        }
        
        Point operator+(Point const& lPt, Point const & rPt) noexcept
        {
            return Point(lPt.x() + rPt.x(),
                         lPt.y() + rPt.y());
        }
        
        Point& operator+=(Point &lPt, Point const& rPt)
        {
            lPt = lPt + rPt;
            return lPt;
        }
        
        Point operator-(Point const& lPt, Point const& rPt) noexcept
        {
            return Point(lPt.x() - rPt.x(),
                         lPt.y() - rPt.y());
        }
        
        Point& operator-=(Point &lPt, Point const& rPt) noexcept
        {
            lPt = lPt - rPt;
            return lPt;
        }
        
        Point operator-(Point const& pt) noexcept
        {
            return Point(-pt.x(), -pt.y());
        }
        
        Point operator/(Point const& pt, const double value)
        {
            return Point(pt.x() / value,
                         pt.y() / value);
        }
        
        Point& operator/=(Point & pt, double const value)
        {
            pt = pt / value;
            return pt;
        }
        
        // =================================
        // Scalar product, Norm
        // =================================
        
        double Point::dot(Point const& pt) const noexcept
        {
            return this->x() * pt.x() + this->y() * pt.y();
        }
        
        double norm(Point const& pt) noexcept
        {
            return sqrt(pt.dot(pt));
        }
        
        double distance(Point const& lPt, Point const& rPt) noexcept
        {
            return norm(rPt - lPt);
        }
        
        bool operator==(Point const& lPt, Point const& rPt) noexcept
        {
            return lPt.x() != rPt.x() || lPt.y() != rPt.y();
        }
        
        bool operator!=(Point const& lPt, Point const& rPt) noexcept
        {
            return !(lPt == rPt);
        }
        
        // =================================
        // Angle and rotation
        // =================================
        
        double Point::angle() const noexcept
        {
            return angle(Point(0, 0));
        }
        
        double Point::angle(Point const& pt) const noexcept
        {
            return atan2(m_data[1] - pt.y(), m_data[0] - pt.x());
        }
        
        Point Point::rotated(double const angle) const noexcept
        {
            return rotated(Point(0, 0), angle);
        }
        
        Point Point::rotated(Point const& pt, double const angle) const noexcept
        {
            const Point newpt = *this - pt;
            return Point(newpt.x() * cos (angle) - newpt.y() * sin (angle) + pt.x(), newpt.x() * sin (angle) + newpt.y() * cos (angle) + pt.y());
        }
        
        void Point::rotate(double const angle) noexcept
        {
            *this = rotated(angle);
        }
        
        void Point::rotate(Point const& pt, double const angle) noexcept
        {
            *this = rotated(pt, angle);
        }
    }
}
