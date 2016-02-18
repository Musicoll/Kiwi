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
            return distance(lPt, rPt) == 0;
        }
        
        // =================================
        // Angle and rotation
        // =================================
        
        Point Point::rotated(Point const& pt, double const angle) const noexcept
        {
            const Point newpt = *this - pt;
            return Point(newpt.x() * cos (angle) - newpt.y() * sin (angle) + pt.x(), newpt.x() * sin (angle) + newpt.y() * cos (angle) + pt.y());
        }
        
        // =================================
        // Algorithm
        // =================================
        
        Point Point::fromLine(Point const& start, Point const& end, double delta) noexcept
        {
            return (end - start) * delta + start;
        }
        
        Point Point::fromLine(Point const& start, Point const& ctrl, Point const& end, const double delta) noexcept
        {
            const double mdelta = (1. - delta);
            return start * (mdelta * mdelta) + ctrl * (2. * delta * mdelta) + end * (delta * delta);
        }
        
        Point Point::fromLine(Point const& start, Point const& ctrl1, Point const& ctrl2, Point const& end, const double delta) noexcept
        {
            const double d2 = delta * delta;
            const double md = (1. - delta);
            const double md2 = md * md;
            return start * md2 * md + 3 * ctrl1 * md2 * delta + 3 * ctrl2 * md * d2 + end * d2 * delta;
        }
        
        double Point::distance(Point const& start, Point const& end) const noexcept
        {
            const Point delta(end - start);
            const double length = delta.length();
            if(length > 0.)
            {
                const double ratio = (*this - start).dot(delta) / length;
                if(ratio < 0.)
                {
                    return norm(start);
                }
                else if(ratio > 1.)
                {
                    return norm(end);
                }
                else
                {
                    return norm((ratio * delta) + start);
                }
            }
            else
            {
                return std::min(norm(start), norm(end));
            }
        }
        
        double Point::distance(Point const& start, Point const& ctrl, Point const& end) const noexcept
        {
            const Point A = ctrl - start;
            const Point B = start - ctrl * 2 - end;
            const Point C = start - *this;
            double sol1, sol2, sol3;
            
            const ulong nresult = solve(B.length(), 3 * A.dot(B), 2 * A.length() + C.dot(B), A.dot(C), sol1, sol2, sol3);
            if(nresult)
            {
                double dist = norm(fromLine(start, ctrl, end, sol1));
                if(nresult > 1)
                {
                    const double dist2 = norm(fromLine(start, ctrl, end, sol2));
                    if(dist2 < dist)
                    {
                        dist = dist2;
                    }
                }
                if(nresult > 2)
                {
                    const double dist2 = norm(fromLine(start, ctrl, end, sol3));
                    if(dist2 < dist)
                    {
                        dist  = dist2;
                    }
                }
                return dist;
            }
            else
            {
                return std::min(norm(start), norm(end));
            }
        }
        
        double Point::distance(Point const& start, Point const& ctrl1, Point const& ctrl2, Point const& end) const noexcept
        {
            std::array<Point, 6> W = {Point(0., 0.), Point(0.2, 0.), Point(0.4, 0.), Point(0.6, 0.), Point(0.8, 0.), Point(1., 0.)};
            std::array<Point, 4> C = {Point(start - *this), Point(ctrl1 - *this), Point(ctrl2 - *this), Point(end - *this)};
            std::array<Point, 3> D = {Point((ctrl1 - start) * 3.), Point((ctrl2 - ctrl1) * 3.), Point((end - ctrl2) * 3.)};
            static const double z[3][4] ={{1.0, 0.6, 0.3, 0.1}, {0.4, 0.6, 0.6, 0.4}, {0.1, 0.3, 0.6, 1.0}};
            double 	cd[3][4];
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 4; j++)
                {
                    cd[i][j] = D[i].dot(C[j]);
                }
            }
            for(int k = 0; k < 6; k++)
            {
                for(int i = std::max(0, k - 2); i <= std::min(k, 3); i++)
                {
                    W[k].y(W[k].y() + cd[k - i][i] * z[k - i][i]);
                }
            }
            
            double 	t_candidate[5];
            ulong n_solutions = solve(W, t_candidate, 0ul);
            
            double dist = norm(end);
            for(int i = 0; i < n_solutions; i++)
            {
                const double new_dist = norm(Point::fromLine(start, ctrl1, ctrl2, end, t_candidate[i]));
                if(new_dist < dist)
                {
                    dist = new_dist;
                }
            }
            return dist;
        }
        
        Point Point::nearest(Point const& start, Point const& end) const noexcept
        {
            const Point delta(end - start);
            const double length = delta.length();
            if(length > 0.)
            {
                const double ratio = (*this - start).dot(delta) / length;
                if(ratio < 0.)
                {
                    return start;
                }
                else if(ratio > 1.)
                {
                    return end;
                }
                else
                {
                    return (ratio * delta) + start;
                }
            }
            else
            {
                return (norm(start) < norm(end)) ? start : end;
            }
        }
        
        Point Point::nearest(Point const& start, Point const& ctrl, Point const& end) const noexcept
        {
            const Point A = ctrl - start;
            const Point B = start - ctrl * 2 - end;
            const Point C = start - *this;
            double sol1, sol2, sol3;
            
            const ulong nresult = solve(B.length(), 3 * A.dot(B), 2 * A.length() + C.dot(B), A.dot(C), sol1, sol2, sol3);
            if(nresult)
            {
                Point pt = fromLine(start, ctrl, end, sol1);
                double dist = norm(pt);
                if(nresult > 1)
                {
                    const Point pt2 = fromLine(start, ctrl, end, sol2);
                    const double dist2 = norm(pt2);
                    if(dist2 < dist)
                    {
                        dist = dist2;
                        pt = pt2;
                    }
                }
                if(nresult > 2)
                {
                    const Point pt2 = fromLine(start, ctrl, end, sol3);
                    const double dist2 = norm(pt2);
                    if(dist2 < dist)
                    {
                        dist  = dist2;
                        pt = pt2;
                    }
                }
                return pt;
            }
            else
            {
                return (norm(start) < norm(end)) ? start : end;
            }
        }
        
        Point Point::nearest(Point const& start, Point const& ctrl1, Point const& ctrl2, Point const& end) const noexcept
        {
            std::array<Point, 6> W = {Point(0., 0.), Point(0.2, 0.), Point(0.4, 0.), Point(0.6, 0.), Point(0.8, 0.), Point(1., 0.)};
            std::array<Point, 4> C = {Point(start - *this), Point(ctrl1 - *this), Point(ctrl2 - *this), Point(end - *this)};
            std::array<Point, 3> D = {Point((ctrl1 - start) * 3.), Point((ctrl2 - ctrl1) * 3.), Point((end - ctrl2) * 3.)};
            static const double z[3][4] ={{1.0, 0.6, 0.3, 0.1}, {0.4, 0.6, 0.6, 0.4}, {0.1, 0.3, 0.6, 1.0}};
            double 	cd[3][4];
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 4; j++)
                {
                    cd[i][j] = D[i].dot(C[j]);
                }
            }
            for(int k = 0; k < 6; k++)
            {
                for(int i = std::max(0, k - 2); i <= std::min(k, 3); i++)
                {
                    W[k].y(W[k].y() + cd[k - i][i] * z[k - i][i]);
                }
            }
            
            double 	t_candidate[5];
            ulong n_solutions = solve(W, t_candidate, 0ul);
            
            Point pt = end;
            double dist = norm(end);
            for(int i = 0; i < n_solutions; i++)
            {
                const Point pt2 = fromLine(start, ctrl1, ctrl2, end, t_candidate[i]);
                const double new_dist = norm(pt2);
                if(new_dist < dist)
                {
                    dist = new_dist;
                    pt = pt2;
                }
            }
            return pt;
        }
        
        bool Point::near(Point const& pt, double const dist) const noexcept
        {
            return norm(pt) <= dist;
        }
        
        bool Point::near(Point const& start, Point const& end, double const dist) const noexcept
        {
            return distance(start, end) <= dist;
        }
        
        bool Point::near(Point const& start, Point const& ctrl, Point const& end, double const dist) const noexcept
        {
            return distance(start, ctrl, end) <= dist;
        }
        
        bool Point::near(Point const& start, Point const& ctrl1, Point const& ctrl2, Point const& end, double const dist) const noexcept
        {
            return distance(start, ctrl1, ctrl2, end) <= dist;
        }
        
        ulong Point::solve(double a, double b, double c, double const d, double &solution1, double &solution2, double &solution3)
        {
            if(std::abs(a) > 0.)
            {
                double z = a;
                double a = b / z, b = c / z; c = d / z;
                double p = b - a * a / 3.;
                double q = a * (2. * a * a - 9. * b) / 27. + c;
                double p3 = p * p * p;
                double D = q * q + 4. * p3 / 27.;
                double offset = -a / 3.;
                if(D == 0.)
                {
                    double u;
                    if(q < 0.)
                    {
                        u = pow( -q / 2., 1. / 3.);
                    }
                    else
                    {
                        u = -pow( q / 2., 1. / 3.);
                    }
                    solution1 = 2. * u + offset;
                    solution2 = -u + offset;
                    return 2;
                }
                else if(D > 0.)
                {
                    z = sqrt(D);
                    double u = ( -q + z) / 2.;
                    double v = ( -q - z) / 2.;
                    u = (u >= 0.) ? pow(u, 1. / 3.) : - pow( -u, 1. / 3.);
                    v = (v >= 0.) ? pow(v, 1. / 3.) : - pow( -v, 1. / 3.);
                    solution1 = u + v + offset;
                    return 1;
                }
                else
                {
                    double u = 2. * sqrt( -p / 3.);
                    double v = acos(-sqrt( -27. / p3) * q / 2.) / 3.;
                    solution1 = u * cos(v) + offset;
                    solution2 = u * cos(v + 2. * M_PI / 3.) + offset;
                    solution3 = u * cos(v + 4. * M_PI / 3.) + offset;
                    return 3;
                }
            }
            else
            {
                a = b;
                b = c;
                c = d;
                if(std::abs(a) <= 0.)
                {
                    if(std::abs(b) <= 0.)
                    {
                        return 0;
                    }
                    else
                    {
                        solution1 = -c / b;
                        return 1;
                    }
                }
                
                double D = b*b - 4.*a*c;
                if(D == 0)
                {
                    solution1 = -b / (2. * a);
                    return 1;
                }
                if(D > 0.)
                {
                    D = sqrt(D);
                    solution1 = ( -b - D) / (2. * a);
                    solution2 = ( -b + D) / (2. * a);
                    return 2;
                }
                else
                {
                    return 0;
                }
            }
        }
        
        ulong Point::solve(std::array<Point, 6>& W, double *t, const ulong depth)
        {
            ulong count = 0;
            bool sign, old_sign = old_sign = W[0].y() < 0. ? true : false;
            for(ulong i = 1; i < 6; i++)
            {
                sign = W[i].y() < 0. ? true : false;
                if(sign != old_sign)
                {
                    count++;
                }
                old_sign = sign;
            }
            
            switch(count)
            {
                case 0 :
                {
                    return 0;
                    break;
                }
                case 1 :
                {
                    if(depth >= 64)
                    {
                        t[0] = (W[0].x() + W[5].x()) * 0.5;
                        return 1;
                    }
                    double 	distance[6];
                    double a = W[0].y() - W[5].y(), b = W[5].x() - W[0].x(), c = W[0].x() * W[5].y() - W[5].x() * W[0].y();
                    double abSquared = (a * a) + (b * b);
                    for(ulong i = 1; i < 5; i++)
                    {
                        distance[i] = a * W[i].x() + b * W[i].y() + c;
                        if (distance[i] > 0.0)
                        {
                            distance[i] = (distance[i] * distance[i]) / abSquared;
                        }
                        if (distance[i] < 0.0)
                        {
                            distance[i] = -((distance[i] * distance[i]) / abSquared);
                        }
                    }
                    
                    double max_distance_above = 0., max_distance_below = 0.;
                    for(ulong i = 1; i < 5; i++)
                    {
                        if(distance[i] < 0.0)
                        {
                            max_distance_below = std::min(max_distance_below, distance[i]);
                        }
                        if(distance[i] > 0.0)
                        {
                            max_distance_above = std::max(max_distance_above, distance[i]);
                        }
                    }
                    
                    double intercept_1 = (c + max_distance_above - b) / -a, intercept_2 = (c + max_distance_below - b ) / -a;
                    if(0.5 * (std::max(intercept_1, intercept_2) - std::min(intercept_1, intercept_2)) < std::numeric_limits<double>::epsilon())
                    {
                        const Point A = W[5] - W[0];
                        t[0] = (A.x() * W[0].y() - A.y() * W[0].y()) / -A.y();
                        return 1;
                    }
                    
                    break;
                }
            }
            std::array<Point, 6> Left, Right;
            double 	left_t[6], right_t[6];
            
            Point Vtemp[6][6];
            for(ulong j = 0; j <= 5; j++)
            {
                Vtemp[0][j] = W[j];
            }
            for(ulong i = 1; i <= 5; i++)
            {
                for(ulong j =0 ; j <= 5 - i; j++)
                {
                    Vtemp[i][j] = 0.5 * Vtemp[i-1][j] + 0.5 * Vtemp[i-1][j+1];
                }
            }
            
            for(ulong j = 0; j < 6; j++)
            {
                Left[j]  = Vtemp[j][0];
            }
            for(ulong j = 0; j < 6; j++)
            {
                Right[j] = Vtemp[6-j][j];
            }
            
            ulong left_count  = solve(Left, left_t, depth+1);
            ulong right_count = solve(Right, right_t, depth+1);
            
            for(ulong i = 0; i < left_count; i++)
            {
                t[i] = left_t[i];
            }
            for(ulong i = 0; i < right_count; i++)
            {
                t[i+left_count] = right_t[i];
            }
            
            return (left_count+right_count);
        }
    }
}
