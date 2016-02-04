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

#ifndef KIWI_POINT_H_INCLUDED
#define KIWI_POINT_H_INCLUDED

#include "KiwiAffineMatrix.h"

namespace kiwi
{
    class Path;
    // ================================================================================ //
    //                                      POINT                                       //
    // ================================================================================ //
    
    //! The point holds two double values.
    /**
     The point is used to represent a point in a space and allows several modification.
     */
    class Point
    {
    private:
        friend class Path;
        double m_data[2];
        
        static ulong solve(double a, double b, double c, double const d, double &solution1, double &solution2, double &solution3);
        static ulong solve(std::array<Point, 6>& W, double *t, const ulong depth);
    public:
        
        //! Constructor.
        /** The function initializes a point at zero origin.
         */
        constexpr inline Point() noexcept : m_data{0., 0.}{}
        
        //! Constructor.
        /** The function initializes a point with two double values.
         @param x The abscissa.
         @param y The ordinate.
         */
        constexpr inline Point(const double x, const double y) noexcept : m_data{x, y}{}
        
        //! Constructor.
        /** The function initializes a point with another point.
         @param pt The other point.
         */
        constexpr inline Point(Point const& pt) noexcept : m_data{pt.m_data[0], pt.m_data[1]}{}
        
        //! Constructor.
        /** The function initializes a point with another point.
         @param pt The other point.
         */
        inline Point(Point&& pt) noexcept {std::swap(m_data, pt.m_data);}
        
        //! Destructor.
        /** The function deletes the point.
         */
        inline ~Point() noexcept {}
        
        //! Generates a point from a line and a position.
        /** The function retrieves the point over a line with its relative distance from the origin.
         @param start The first point of the line.
         @param end   The end point of the line.
         @param delta The relative distance from the origin (first point is 0 and end point is 1).
         @return The point.
         */
        static Point fromLine(Point const& start, Point const& end, const double delta) noexcept;
        
        //! Generates a point from a quadratic bezier line and a position.
        /** The function retrieves the point over a quadratic bezier line with its relative distance from the origin.
         @param start The first point of the line.
         @param ctrl  The control point of the line.
         @param end   The end point of the line.
         @param delta The relative distance from the origin (first point is 0 and end point is 1).
         @return The point.
         */
        static Point fromLine(Point const& start, Point const& ctrl, Point const& end, const double delta) noexcept;
        
        //! Generates a point from a cubic bezier line and a position.
        /** The function retrieves the point over a cubic bezier line with its relative distance from the origin.
         @param start The first point of the line.
         @param ctrl1 The first control point of the line.
         @param ctrl2 The second control point of the line.
         @param end   The end point of the line.
         @param delta The relative distance from the origin (first point is 0 and and end point is 1).
         @return The point.
         */
        static Point fromLine(Point const& start, Point const& ctrl1, Point const& ctrl2, Point const& end, const double delta) noexcept;
        
        //! Retrieves the abscissa.
        /** The function retrieves the abscissa.
         @return The abscissa.
         */
        inline double x() const noexcept
        {
            return m_data[0];
        }
        
        //! Retrieves the ordinate.
        /** The function retrieves the ordinate.
         @return The ordinate.
         */
        inline double y() const noexcept
        {
            return m_data[1];
        }
        
        //! Sets the abscissa.
        /** The function sets the abscissa.
         @param x The abscissa.
         */
        inline void x(const double x) noexcept
        {
            m_data[0] = x;
        }
        
        //! Sets the ordinate.
        /** The function sets the ordinate.
         @param y The ordinate.
         */
        inline void y(const double y) noexcept
        {
            m_data[1] = y;
        }
        
        //! Sets the abscissa and the ordinate with another point.
        /** The function sets the abscissa and the ordinate with another point.
         @param pt Another point.
         @return The point.
         */
        inline Point& operator=(Point const& pt) noexcept
        {
            m_data[0] = pt.m_data[0];
            m_data[1] = pt.m_data[1];
            return *this;
        }
        
        //! Sets the abscissa and the ordinate with another point.
        /** The function sets the abscissa and the ordinate with another point.
         @param pt Another point.
         @return The point.
         */
        inline Point& operator=(Point&& pt) noexcept
        {
            std::swap(m_data, pt.m_data);
            return *this;
        }
        
        //! Sets the abscissa and the ordinate with a value.
        /** The function sets the abscissa and the ordinate with a value.
         @param value The value.
         @return The point.
         */
        inline Point& operator=(double const& value) noexcept
        {
            m_data[0] = value;
            m_data[1] = value;
            return *this;
        }
        
        //! Increments the abscissa and the ordinate with another point.
        /** The function increments the abscissa and the ordinate with another point.
         @param pt Another point.
         @return The point.
         */
        inline Point& operator+=(Point const& pt) noexcept
        {
            m_data[0] += pt.m_data[0];
            m_data[1] += pt.m_data[1];
            return *this;
        }
        
        //! Increments the abscissa and the ordinate with a value.
        /** The function increments the abscissa and the ordinate with a value.
         @param value The value.
         @return The point.
         */
        inline Point& operator+=(double const value) noexcept
        {
            m_data[0] += value;
            m_data[1] += value;
            return *this;
        }
        
        //! Decrements the abscissa and the ordinate with another point.
        /** The function decrements the abscissa and the ordinate with another point.
         @param value The value.
         @return The point.
         */
        inline Point& operator-=(Point const& pt) noexcept
        {
            m_data[0] -= pt.m_data[0];
            m_data[1] -= pt.m_data[1];
            return *this;
        }
        
        //! Decrements the abscissa and the ordinate with a value.
        /** The function decrements the abscissa and the ordinate with a value.
         @param pt Another point.
         @return The point.
         */
        inline Point& operator-=(double const value) noexcept
        {
            m_data[0] -= value;
            m_data[1] -= value;
            return *this;
        }
        
        //! Multiplies the abscissa and the ordinate with another point.
        /** The function multiplies the abscissa and the ordinate with another point.
         @param value The value.
         @return The point.
         */
        inline Point& operator*=(Point const& pt) noexcept
        {
            m_data[0] *= pt.m_data[0];
            m_data[1] *= pt.m_data[1];
            return *this;
        }
        
        //! Multiplies the abscissa and the ordinate with a value.
        /** The function multiplies the abscissa and the ordinate with a value.
         @param pt Another point.
         @return The point.
         */
        inline Point& operator*=(double const value) noexcept
        {
            m_data[0] *= value;
            m_data[1] *= value;
            return *this;
        }
        
        //! Divides the abscissa and the ordinate with another point.
        /** The function divides the abscissa and the ordinate with another point.
         @param value The value.
         @return The point.
         */
        inline Point& operator/=(Point const& pt) noexcept
        {
            m_data[0] /= pt.m_data[0];
            m_data[1] /= pt.m_data[1];
            return *this;
        }
        
        //! Divides the abscissa and the ordinate with a value.
        /** The function divides the abscissa and the ordinate with a value.
         @param pt Another point.
         @return The point.
         */
        inline Point& operator/=(double const value) noexcept
        {
            m_data[0] /= value;
            m_data[1] /= value;
            return *this;
        }
        
        //! Retrives a new point from the addition of two points.
        /** The function adds a value to the point.
         @param pt The point to add.
         @return The new point.
         */
        inline Point operator+(Point const pt) const noexcept
        {
            return Point(m_data[0] + pt.x(), m_data[1] + pt.y());
        }
        
        //! Add a value to the point.
        /** The function adds a value to the point.
         @param value The value to add.
         @return The new point.
         */
        inline Point operator+(double const value) const noexcept
        {
            return Point(m_data[0] + value, m_data[1] + value);
        }
        
        //! Subtract a a value to the point.
        /** The function subtracts a value to the point.
         @param value The value to subtract.
         @return The new point.
         */
        inline Point operator-(double const value) const noexcept
        {
            return Point(m_data[0] - value, m_data[1] - value);
        }
        
        //! Subtract a a point to the point.
        /** The function subtract to the point.
         @param pt The point to subtract.
         @return The new point.
         */
        inline Point operator-(Point const pt) const noexcept
        {
            return Point(m_data[0] - pt.x(), m_data[1] - pt.y());
        }
        
        //! Multiply a point with the point.
        /** The function multiplies a point with the point.
         @param pt The point to multiply with.
         @return The new point.
         */
        inline Point operator*(Point const pt) const noexcept
        {
            return Point(m_data[0] * pt.x(), m_data[1] * pt.y());
        }
        
        //! Divide a value with the point.
        /** The function divides a value with the point.
         @param value The value to divide with.
         @return The new point.
         */
        inline Point operator/(const double value) const noexcept
        {
            return Point(m_data[0] / value, m_data[1] / value);
        }
        
        //! Divide a point with the point.
        /** The function divides a point with the point.
         @param pt The point to divide with.
         @return The new point.
         */
        inline Point operator/(Point const pt) const noexcept
        {
            return Point(m_data[0] / pt.x(), m_data[1] / pt.y());
        }
        
        //! Returns the inverse of the point.
        /** The function return the inverse of the point.
         @return The new point.
         */
        inline Point operator-() const noexcept
        {
            return Point(-m_data[0], -m_data[1]);
        }
        
        //! Get the equality of the point with another.
        /** The function retrieves the equality of the point with another.
         @param pt The other point.
         @return true if the two points are equal, otherwise false.
         */
        inline bool operator==(Point const& pt) const noexcept
        {
            return m_data[0] == pt.x() && m_data[1] == pt.y();
        }
        
        //! Get the equality of the point with another.
        /** The function retrieves the equality of the point with another.
         @param pt The other point.
         @return true if the two points are not equal, otherwise false.
         */
        inline bool operator!=(Point const& pt) const noexcept
        {
            return m_data[0] != pt.x() || m_data[1] != pt.y();
        }
        
        //! Test if this point is greather than another.
        /** The function return true if x > other.x and y > other.y.
         @param pt The other point.
         @return true if this point is greather than another, otherwise false.
         */
        inline bool operator>(Point const& pt) const noexcept
        {
            return m_data[0] > pt.x() && m_data[1] > pt.y();
        }
        
        //! Test if this point is greather than or equal to another.
        /** The function return true if x >= other.x and y >= other.y.
         @param pt The other point.
         @return true if this point is greather than or equal to another, otherwise false.
         */
        inline bool operator>=(Point const& pt) const noexcept
        {
            return m_data[0] >= pt.x() && m_data[1] >= pt.y();
        }
        
        //! Test if this point is smaller than another.
        /** The function return true if x < other.x and y < other.y.
         @param pt The other point.
         @return true if this point is smaller than another, otherwise false.
         */
        inline bool operator<(Point const& pt) const noexcept
        {
            return m_data[0] < pt.x() && m_data[1] < pt.y();
        }
        
        //! Test if this point is smaller than or equal to another.
        /** The function return true if x <= other.x and y <= other.y.
         @param pt The other point.
         @return true if this point is smaller than or equal to another, otherwise false.
         */
        inline bool operator<=(Point const& pt) const noexcept
        {
            return m_data[0] <= pt.x() && m_data[1] <= pt.y();
        }
        
        //! Retrieve a copy and apply a rotation from the origin.
        /** The function retrieves a copy and applies a rotation from the origin.
         @param angle The angle
         @return The copy with the rotation.
         */
        Point rotated(double const angle) const noexcept
        {
            return Point(m_data[0] * cos(angle) - m_data[1] * sin(angle), m_data[0] * sin(angle) + m_data[1] * cos(angle));
        }
        
        //! Retrieve a copy and apply a rotation from another point.
        /** The function retrieves a copy and applies a rotation from another point.
         @pram pt The other point.
         @param angle The angle
         @return The copy with the rotation.
         */
        inline Point rotated(Point const& pt, double const angle) const noexcept
        {
            const Point newpt = *this - pt;
            return Point(newpt.x() * cos (angle) - newpt.y() * sin (angle) + pt.x(), newpt.x() * sin (angle) + newpt.y() * cos (angle) + pt.y());
        }
        
        //! Apply a rotation from the origin.
        /** The function retrieves Applies a rotation from the origin.
         @param angle The angle
         */
        void rotate(double const angle) noexcept
        {
            *this = rotated(angle);
        }
        
        //! Retrieve a copy and apply a rotation from another point.
        /** The function retrieves a copy and applies a rotation from another point.
         @param pt The other point.
         @param angle The angle
         */
        void rotate(Point const& pt, double const angle) noexcept
        {
            *this = rotated(pt, angle);
        }
        
        //! Retrieve the length from the origin.
        /** The function retrieves the length from the origin.
         @return The length.
         */
        inline double length() const noexcept
        {
            return m_data[0] * m_data[0] + m_data[1] * m_data[1];
        }
        
        //! Retrieve the distance from the origin.
        /** The function retrieves the distance from the origin.
         @return The distance.
         */
        inline double distance() const noexcept
        {
            return sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1]);
        }
        
        //! Retrieve the distance from another point.
        /** The function retrieves the distance from another point.
         @param pt The other point.
         @return The distance.
         */
        inline double distance(Point const& pt) const noexcept
        {
            return sqrt((m_data[0] - pt.x()) * (m_data[0] - pt.x()) + (m_data[1] - pt.y()) * (m_data[1] - pt.y()));
        }
        
        //! Retrieve the angle from the origin.
        /** The function retrieves the angle from origin.
         @return The angle.
         */
        inline double angle() const noexcept
        {
            return atan2(m_data[1], m_data[0]);
        }
        
        //! Retrieve the angle from another point.
        /** The function retrieves the angle from another point.
         @param pt The other point.
         @return The angle.
         */
        inline double angle(Point const& pt) const noexcept
        {
            return atan2(m_data[1] - pt.y(), m_data[0] - pt.x());
        }
        
        //! Retrieve the dot product with another point.
        /** The function retrieves the dot product with another point.
         @param pt The other point.
         @return The dot product.
         */
        inline double dot(Point const& pt) const noexcept
        {
            return m_data[0] * pt.x() + m_data[1] * pt.y();
        }
        
        //! Retrieve the distance from a line.
        /** The function retrieves the distance a line.
         @param start The first point of the line.
         @param end   The end point of the line.
         @return The distance.
         */
        double distance(Point const& start, Point const& end) const noexcept;
        
        //! Retrieve the distance from a quadratic bezier line.
        /** The function retrieves the distance a quadratic bezier line.
         @param start The first point of the line.
         @param ctrl  The control point of the line.
         @param end   The end point of the line.
         @return The distance.
         */
        double distance(Point const& start, Point const& ctrl, Point const& end) const noexcept;
        
        //! Retrieve the distance from a cubic bezier line.
        /** The function retrieves the distance from a cubic bezier line.
         @param start The first point of the line.
         @param ctrl1 The first control point of the line.
         @param ctrl2 The second control point of the line.
         @param end   The end point of the line.
         @return The distance.
         */
        double distance(Point const& start, Point const& ctrl1, Point const& ctrl2, Point const& end) const noexcept;
        
        //! Retrieve the nearest point from a line.
        /** The function retrieves the nearest point a line.
         @param start The first point of the line.
         @param end   The end point of the line.
         @return The distance.
         */
        Point nearest(Point const& start, Point const& end) const noexcept;
        
        //! Retrieve the nearest point from a quadratic bezier line.
        /** The function retrieves the nearest point from a quadratic bezier line.
         @param start The first point of the line.
         @param ctrl  The control point of the line.
         @param end   The end point of the line.
         @return The distance.
         */
        Point nearest(Point const& start, Point const& ctrl, Point const& end) const noexcept;
        
        //! Retrieve the nearest point from a cubic bezier line.
        /** The function retrieves the nearest point from a cubic bezier line.
         @param start The first point of the line.
         @param ctrl1 The first control point of the line.
         @param ctrl2 The second control point of the line.
         @param end   The end point of the line.
         @return The distance.
         */
        Point nearest(Point const& start, Point const& ctrl1, Point const& ctrl2, Point const& end) const noexcept;
        
        //! Get if the point is near to another point.
        /** The function gets if the point is near to another point.
         @param pt The other point.
         @param distance The distance of neighborhood.
         @return true if the two points are near, otherwise false.
         */
        bool near(Point const& pt, double const distance) const noexcept;
        
        //! Get if the point is near or over a line.
        /** The function gets if the point is near or over a line.
         @param start The first point of the line.
         @param end   The end point of the line.
         @param distance The distance of neighborhood (0 means over the line).
         @return true if the point is near to the line, otherwise false.
         */
        bool near(Point const& start, Point const& end, double const distance) const noexcept;
        
        //! Get if the point is near or over a quadratic bezier line.
        /** The function gets if the point is near or over a quadratic bezier line.
         @param start The first point of the line.
         @param ctrl  The control point of the line.
         @param end   The end point of the line.
         @param distance The distance of neighborhood (0 means over the line).
         @return true if the point is near to the line, otherwise false.
         */
        bool near(Point const& start, Point const& ctrl, Point const& end, double const distance) const noexcept;
        
        //! Get if the point is near or over a cubic bezier line.
        /** The function gets if the point is near or over a cubic bezier line.
         @param start The first point of the line.
         @param ctrl1 The first control point of the line.
         @param ctrl2 The second control point of the line.
         @param end   The end point of the line.
         @param distance The distance of neighborhood (0 means over the line).
         @return true if the point is near to the line, otherwise false.
         */
        bool near(Point const& start, Point const& ctrl1, Point const& ctrl2, Point const& end, double const distance) const noexcept;
        
        // ================================================================================ //
        //                                      ATTR                                        //
        // ================================================================================ //
        
        //! Retrieve the point as an atom.
        /** The function retrieves the point as an atom.
         @return The atom.
         */
        inline operator Atom() const noexcept
        {
            return Atom({m_data[0], m_data[1]});
        }
        
        //! Set the point with an atom.
        /** The function sets the point with an atom.
         @param atom The atom.
         */
        inline Point operator=(Atom const& atom) noexcept
        {
            if(atom.isVector())
            {
                Vector vector = atom;
                if(vector.size() > 1 && vector[0].isNumber() && vector[1].isNumber())
                {
                    m_data[0] = double(vector[0]);
                    m_data[1] = double(vector[1]);
                }
            }
            return *this;
        }
    };
    
    //! Multiply a value with a point.
    /** The function multiplies a value a the point.
     @param pt The point to multiply.
     @param value The value to multiply with.
     @return The new point.
     */
    static inline Point operator*(Point const& pt, double const value) noexcept
    {
        return Point(pt.x() * value, pt.y() * value);
    }
    
    //! Multiply a value with a point.
    /** The function multiplies a value a the point.
     @param pt The point to multiply.
     @param value The value to multiply with.
     @return The new point.
     */
    static inline Point operator*(const double value, Point const& pt) noexcept
    {
        return Point(pt.x() * value, pt.y() * value);
    }
    
    typedef std::shared_ptr<Attr::Typed<Point>>  sAttrPoint;
}

#endif // KIWI_POINT_H_INCLUDED
