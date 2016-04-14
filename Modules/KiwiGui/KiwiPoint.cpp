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

#include <KiwiGui/KiwiPoint.hpp>
#include <math.h>

namespace kiwi
{
    namespace gui
    {
        // ================================================================================ //
        //                                      POINT                                       //
        // ================================================================================ //
        
        // =================================
        // Operators
        // =================================
        
        Point& operator*=(Point &pt, double const value) noexcept
        {
            pt = value * pt;
            return pt;
        }
        
        Point& operator+=(Point &lPt, Point const& rPt) noexcept
        {
            lPt = lPt + rPt;
            return lPt;
        }
        
        Point& operator-=(Point &lPt, Point const& rPt) noexcept
        {
            lPt = lPt - rPt;
            return lPt;
        }
        
        Point& operator/=(Point & pt, double const value)
        {
            pt = pt / value;
            return pt;
        }
        
        // =================================
        // Scalar product, Norm
        // =================================
        
        double norm(Point const& pt) noexcept
        {
            return sqrt(pt.dot(pt));
        }
        
        double distance(Point const& lPt, Point const& rPt) noexcept
        {
            return norm(rPt - lPt);
        }
        
        bool areNear(Point const& l_pt, Point const& r_pt, const double eps) noexcept
        {
            return distance(l_pt, r_pt) <= eps;
        }
        
        // =================================
        // Angle and rotation
        // =================================
        
        double Point::angle() const noexcept
        {
            return atan2(m_data[1], m_data[0]);
        }
        
        double Point::angleToPoint(Point const& pt) const noexcept
        {
            return (pt - *this).angle();
        }
        
        Point Point::rotated(double const angle) const noexcept
        {
            return rotated(Point(0, 0), angle);
        }
        
        Point Point::rotated(Point const& pt, double const angle) const noexcept
        {
            const Point newpt = *this - pt;
            
            return Point(newpt.x() * cos (angle) - newpt.y() * sin (angle) + pt.x(),
                         newpt.x() * sin (angle) + newpt.y() * cos (angle) + pt.y());
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
