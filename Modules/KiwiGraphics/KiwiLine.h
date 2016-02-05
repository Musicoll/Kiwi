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

#ifndef KIWI_LINE_H_INCLUDED
#define KIWI_LINE_H_INCLUDED

#include "KiwiPoint.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      LINE                                        //
    // ================================================================================ //
    
    class Segment;
    class BezierQuad;
    class BezierCubic;
    
    //! The line is a base class to represent a line.
    /**
     The line is used to represent a finite line in a space and allows several modification.
     */
    class Line
    {
    public:
        
        //! Constructor.
        /** The function initializes a line as a segment, using (0, 0) as its start and end points.
         */
        inline Line() noexcept : m_start(), m_end(){}
        
        //! Constructor.
        /** The function initializes a line with two point.
         @param start The start point.
         @param end   The end point.
         */
        inline Line(const Point& start, const Point& end) noexcept : m_start(start), m_end(end){}
        
        //! Destructor.
        /** The function deletes the line.
         */
        virtual inline ~Line() noexcept {}
        
        //! Retrieves the start point.
        /** The function retrieves the start point.
         @return The start point.
         */
        inline Point start() const noexcept
        {
            return m_start;
        }
        
        //! Retrieves the end point.
        /** The function retrieves the end point.
         @return The end point.
         */
        inline Point end() const noexcept
        {
            return m_end;
        }
        
        //! Set a new start point.
        /** The function sets a new start point.
         @param start The new start point.
         */
        inline void start(Point const& newStart) noexcept
        {
            m_start = newStart;
        }
        
        //! Set a new end point.
        /** The function sets a new end point.
         @param start The new end point.
         */
        inline void end(Point const& newEnd) noexcept
        {
            m_end = newEnd;
        }
        
        //! Retrieves the abscissa of the start point.
        /** The function retrieves the abscissa of the start point.
         @return The abscissa of the start point.
         */
        inline double startX() const noexcept
        {
            return m_start.x();
        }
        
        //! Retrieves the abscissa of the end point.
        /** The function retrieves the abscissa of the end point.
         @return The abscissa of the end point.
         */
        inline double endX() const noexcept
        {
            return m_end.x();
        }
        
        //! Retrieves the ordinate of the start point.
        /** The function retrieves the ordinate of the start point.
         @return The ordinate of the start point.
         */
        inline double startY() const noexcept
        {
            return m_start.y();
        }
        
        //! Retrieves the ordinate of the end point.
        /** The function retrieves the ordinate of the end point.
         @return The ordinate of the end point.
         */
        inline double endY() const noexcept
        {
            return m_end.y();
        }
        
        //! Sets the abscissa of the start point.
        /** The function sets the abscissa of the start point.
         @param x The abscissa of the start point.
         */
        inline void startX(const double x) noexcept
        {
            m_start.x(x);
        }
        
        //! Sets the abscissa of the end point.
        /** The function sets the abscissa of the end point.
         @param x The abscissa of the end point.
         */
        inline void endX(const double x) noexcept
        {
            m_end.x(x);
        }
        
        //! Sets the ordinate of the start point.
        /** The function sets the ordinate of the start point.
         @param y The ordinate of the start point.
         */
        inline void startY(const double y) noexcept
        {
            m_start.y(y);
        }
        
        //! Sets the ordinate of the end point.
        /** The function sets the ordinate of the end point.
         @param y The ordinate of the end point.
         */
        inline void endY(const double y) noexcept
        {
            m_end.y(y);
        }
        
        //! Apply a rotation from the origin.
        /** The function Applies a rotation from the origin.
         @param angle  The angle
         */
        virtual void rotate(double const angle) noexcept = 0;
        
        //! Apply a rotation from another point.
        /** The function Applies a rotation from another point.
         @param origin The origin point.
         @param angle  The angle
         */
        virtual void rotate(Point const& origin, double const angle) noexcept = 0;
        
        //! Reverse start and end points.
        /** The function reverses start and end points.
         */
        virtual const void reverse() noexcept = 0;
        
        //! Retrieve the length of the line.
        /** The function retrieves the length of the line.
         @return The length.
         */
        virtual inline double length() const noexcept = 0;
        
        //! Retrieve the center point of the line.
        /** The function retrieves the center point of the line.
         @return The center.
         */
        inline Point center() const noexcept
        {
            return getPointAt(0.5);
        }
        
        //! Retrieve the smallest distance from a point.
        /** The function retrieves the smallest distance from a point.
         @param pt The point.
         @return The distance from the point.
         */
        virtual double distance(Point const& pt) const noexcept = 0;
        
        //! Retrieve the point along this line that is nearest to another point.
        /** This function Retrieve the point along this line that is nearest to another point.
         @param pt The point.
         @return The nearest point.
         */
        virtual Point getNearestPoint(Point const& pt) const noexcept = 0;
        
        /** Retrieves the point which is at a given distance along this line proportional to the line's length.
         This function retrieves the the point which is at a given distance along this line proportional to the line's length.
         @param proportionOfLength the distance to move along the line from its start point, in multiples of the line's length.
         So a value of 0.0 will return the line's start point
         and a value of 1.0 will return its end point.
         */
        virtual Point getPointAt(const double proportionOfLength) const noexcept = 0;
        
        /** Discretize the line into n points.
         This function discretizes the line into n points.
         @param steps The number of steps.
         @return A vector of points (at least two for start and end values);
         */
        std::vector<Point> discretized(const ulong steps) const noexcept
        {
            std::vector<Point> points;
            
            points.push_back(m_start);
            
            if (steps > 1)
            {
                const double factor = 1./(double)steps;
                for(double t = 0.; t < 1.; t+=factor)
                {
                    points.push_back(getPointAt(t));
                }
            }
            
            points.push_back(m_end);
            
            return points;
        }
        
        //! Returns true if this line intersects itself.
        /** The function returns true if this line intersects itself (can only be true for cubic bezier lines).
         @return True if this line intersects another.
         */
        virtual bool intersects() const noexcept
        {
            return false;
        }
        
    protected:
        Point m_start, m_end;
        
    };
    
    // ================================================================================ //
    //                                     SEGMENT                                      //
    // ================================================================================ //
    
    //! The segment holds a start and end points value.
    /**
     The segment is used to represent a finite line in a space and allows several modification.
     */
    class Segment : public Line
    {
    public:
        //! Constructor.
        /** The function initializes a segment, using (0, 0) as its start and end points.
         */
        inline Segment() noexcept {}
        
        //! Constructor.
        /** The function initializes a segment with two point.
         @param start The start point.
         @param end   The end point.
         */
        inline Segment(const Point& start, const Point& end) noexcept : Line(start, end) {}
        
        //! Constructor.
        /** The function initializes a segment with another segment.
         @param pt The other segment.
         */
        inline Segment(Segment const& segment) noexcept : Line(segment.start(), segment.end()){}
        
        //! Constructor.
        /** The function initializes a segment with two points.
         @param start The start point.
         @param end   The end point.
         */
        inline Segment(Point&& start, Point&& end) noexcept
        {
            std::swap(m_start, start);
            std::swap(m_end, end);
        }
        
        //! Constructor.
        /** The function initializes a segment with another.
         @param start The segment.
         */
        inline Segment(Segment&& segment) noexcept
        {
            std::swap(m_start, segment.m_start);
            std::swap(m_end, segment.m_end);
        }
        
        //! Destructor.
        /** The function deletes the segment.
         */
        inline ~Segment() noexcept {}
        
        //! Retrieve the segment as an atom.
        /** The function retrieves the segment as an atom.
         */
        inline operator Atom() const noexcept
        {
            return Atom({m_start.x(), m_start.y(), m_end.x(), m_end.y()});
        }
        
        //! Set the segment with an atom.
        /** The function sets the segment with an atom.
         @param atom The atom.
         */
        inline Segment operator=(Atom const& atom) noexcept
        {
            if(atom.isVector())
            {
                Vector vector = atom;
                if(vector.size() > 3 && vector[0].isNumber() && vector[1].isNumber() && vector[2].isNumber() && vector[3].isNumber())
                {
                    m_start = Point(double(vector[0]), double(vector[1]));
                    m_end   = Point(double(vector[2]), double(vector[3]));
                }
            }
            return *this;
        }
        
        //! Retrieve a point of the segment;
        /** The function retrieve a point of the segment.
         @param index The point index (0 or 1 for start, end).
         @return The point.
         */
        inline Point operator[](const ulong index) noexcept
        {
            switch(index)
            {
                case 0:  return m_start;
                case 1:  return m_end;
                default: return Point();
            }
        }
        
        //! Sets the segment with another one.
        /** The function sets the segment with another one.
         @param segment another segment.
         @return The segment.
         */
        inline Segment& operator=(Segment const& segment) noexcept
        {
            m_start = segment.start();
            m_end   = segment.end();
            return *this;
        }
        
        //! Sets the segment with another one.
        /** The function sets the segment with another one.
         @param segment another segment.
         @return The segment.
         */
        inline Segment& operator=(Segment&& segment) noexcept
        {
            std::swap(m_start, segment.m_start);
            std::swap(m_end, segment.m_end);
            return *this;
        }
        
        //! Shift the segment by a point.
        /** The function shifts the segment by a point.
         @param pt The point.
         @return The segment.
         */
        inline Segment& operator+=(Point const& pt) noexcept
        {
            m_start += pt;
            m_end   += pt;
            return *this;
        }
        
        //! Shift the segment by a point.
        /** The function shifts the segment by a point.
         @param value The value.
         @return The segment.
         */
        inline Segment& operator+=(double const value) noexcept
        {
            m_start += value;
            m_end   += value;
            return *this;
        }
        
        //! Shift the segment by a point.
        /** The function shifts the segment by a point.
         @param pt The point.
         @return The segment.
         */
        inline Segment& operator-=(Point const& pt) noexcept
        {
            m_start -= pt;
            m_end   -= pt;
            return *this;
        }
        
        //! Shift the segment by a point.
        /** The function shifts the segment by a point.
         @param pt The point.
         @return The segment.
         */
        inline Segment& operator-=(double const value) noexcept
        {
            m_start -= value;
            m_end   -= value;
            return *this;
        }
        
        //! Retrieves a new segment shifted by a point.
        /** The function retrieves a new segment shifted by a point.
         @param pt The point to add.
         @return The new segment.
         */
        inline Segment operator+(Point const& pt) const noexcept
        {
            return Segment(m_start + pt, m_end + pt);
        }
        
        //! Retrieves a new segment shifted by a value.
        /** The function retrieves a new segment shifted by a value.
         @param value The value to add.
         @return The new segment.
         */
        inline Segment operator+(double const value) const noexcept
        {
            return Segment(m_start + value, m_end + value);
        }
        
        //! Retrieves a new segment shifted by a point.
        /** The function retrieves a new segment shifted by a point.
         @param pt The point to add.
         @return The new segment.
         */
        inline Segment operator-(Point const& pt) const noexcept
        {
            return Segment(m_start - pt, m_end - pt);
        }
        
        //! Retrieves a new segment shifted by a value.
        /** The function retrieves a new segment shifted by a value.
         @param value The value to add.
         @return The new segment.
         */
        inline Segment operator-(double const value) const noexcept
        {
            return Segment(m_start - value, m_end - value);
        }
        
        //! Get the equality of the segment with another.
        /** The function retrieves the equality of the segment with another.
         @param segment The other segment.
         @return true if the two segments are equal, otherwise false.
         */
        inline bool operator==(Segment const& segment) const noexcept
        {
            return m_start == segment.start() && m_end == segment.end();
        }
        
        //! Get the equality of the segment with another.
        /** The function retrieves the equality of the segment with another.
         @param segment The other segment.
         @return true if the two segments are not equal, otherwise false.
         */
        inline bool operator!=(Segment const& segment) const noexcept
        {
            return m_start != segment.start() && m_end != segment.end();
        }
        
        //! Retrieve a copy and apply a rotation from the origin.
        /** The function retrieves a copy and applies a rotation from the origin.
         @param The angle
         @return The copy with the rotation.
         */
        Segment rotated(double const angle) const noexcept
        {
            return Segment(m_start.rotated(angle), m_end.rotated(angle));
        }
        
        //! Retrieve a copy and apply a rotation from another point.
        /** The function retrieves a copy and applies a rotation from another point.
         @param origin The origin point.
         @param angle  The angle
         @return The copy with the rotation.
         */
        inline Segment rotated(Point const& origin, double const angle) const noexcept
        {
            return Segment(m_start.rotated(origin, angle), m_end.rotated(origin, angle));
        }
        
        //! Apply a rotation from the origin.
        /** The function Applies a rotation from the origin.
         @param angle  The angle
         */
        void rotate(double const angle) noexcept override
        {
            m_start.rotate(angle);
            m_end.rotate(angle);
        }
        
        //! Apply a rotation from the origin from another point.
        /** The function Applies a rotation from another point.
         @param origin The origin point.
         @param angle  The angle
         */
        void rotate(Point const& origin, double const angle) noexcept override
        {
            *this = rotated(origin, angle);
        }
        
        //! Returns a reversed segment version.
        /** The function returns a segment that is the same as this one, but with the start and end reversed.
         */
        const Segment reversed() const noexcept
        {
            return Segment(m_end, m_start);
        }
        
        //! Reverse start and end points.
        /** The function reverses start and end points.
         */
        const void reverse() noexcept override
        {
            std::swap(m_start, m_end);
        }
        
        //! Retrieve the angle of the segment.
        /** This value is the number of radians clockwise from the 3 o'clock direction,
         where the segment's start point is considered to be at the centre.
         @return The angle of the segment.
         */
        inline double angle() const noexcept
        {
            return m_start.angle(m_end);
        }
        
        //! Retrieve the length of the segment.
        /** The function retrieves the length of the segment.
         @return The length.
         */
        inline double length() const noexcept override
        {
            return std::abs(m_start.distance(m_end));
        }
        
        //! Retrieve the smallest distance from a point.
        /** The function retrieves the smallest distance from a point.
         @param pt The point.
         @return The distance from the point.
         */
        double distance(Point const& pt) const noexcept override
        {
            return pt.distance(m_start, m_end);
        }
        
        //! Retrieve the point along this segment that is nearest to another point.
        /** This function Retrieve the point along this segment that is nearest to another point.
         @param pt The point.
         @return The nearest point.
         */
        Point getNearestPoint(Point const& pt) const noexcept override;
        
        /** Retrieves the point which is at a given distance along this segment proportional to the segment's length.
         This function retrieves the the point which is at a given distance along this segment proportional to the segment's length.
         @param proportionOfLength the distance to move along the segment from its start point, in multiples of the line's length.
         So a value of 0.0 will return the segment's start point
         and a value of 1.0 will return its end point.
         */
        Point getPointAt(const double proportionOfLength) const noexcept override
        {
            return lerp(m_start, m_end, proportionOfLength);
        }
        
        //! Returns true if this segment intersects another.
        /** The function returns true if this segment intersects another.
         @param segment The other segment.
         @return True if this segment intersects another.
         */
        bool intersects(Segment const& segment) const noexcept;
        
        //! Returns true if this segment intersects another.
        /** The function returns true if this segment intersects another, and find the intersection point.
         @param segment The other segment.
         @param intersection The position of the point where the segments meet (or where they would meet if they were infinitely long)
         @return True if this segment intersects another.
         */
        bool intersects(Segment const& segment, Point& intersection) const noexcept;
        
        //! Returns true if this segment intersects a quadratic bezier curve.
        /** The function returns true if this segment intersects a quadratic bezier curve.
         @param curve The quadratic bezier curve.
         @return True if this segment intersects.
         */
        bool intersects(BezierQuad const& curve) const noexcept;
        
        //! Returns true if this segment intersects a cubic bezier curve.
        /** The function returns true if this segment intersects a cubic bezier curve.
         @param curve The cubic bezier curve.
         @return True if this segment intersects.
         */
        bool intersects(BezierCubic const& curve) const noexcept;
    };
    
    // ================================================================================ //
    //                                      BEZIER                                      //
    // ================================================================================ //
    
    //! The quadratic curve is a curved line that holds one control value.
    /**
     The quadratic curve is used to represent a curved line in a space and allows several modification.
     */
    class BezierCurve : public Line
    {
    public:
        //! Constructor.
        /** The function initializes a quadratic curve, using (0, 0) as its start and end points.
         */
        inline BezierCurve() noexcept : Line() {}
        
        //! Constructor.
        /** The function initializes a quadratic curve with a segment.
         @param start The start point.
         @param end   The end point.
         */
        inline BezierCurve(const Segment& segment) noexcept : Line(segment.start(), segment.end()){}
        
        //! Constructor.
        /** The function initializes a quadratic curve with three points.
         @param start The start point.
         @param ctrl  The control point.
         @param end   The end point.
         */
        inline BezierCurve(const Point& start, const Point& end) noexcept : Line(start, end) {}
        
        //! Returns true if this curve intersects a segment.
        /** The function returns true if this curve intersects a segment.
         @param segment The other segment.
         @return True if this curve intersects a segment.
         */
        bool intersects(Segment const& segment) const noexcept
        {
            const ulong steps = 100;
            
            const double factor = 1./(double)steps;
            Point last = m_start;
            
            for(double t = 0.; t < 1.; t+=factor)
            {
                const Point next = getPointAt(t);
                if(Segment(last, next).intersects(segment))
                {
                    return true;
                }
                last = next;
            }
            
            if(Segment(last, m_end).intersects(segment))
            {
                return true;
            }
            
            return false;
        }
        
        //! Returns true if this curve intersects another.
        /** The function returns true if this curve intersects another.
         @param curve The other curve.
         @return True if this curve intersects another.
         */
        bool intersects(BezierCurve const& curve) const noexcept
        {
            const std::vector<Point> points = curve.discretized(100);
            for(std::vector<Point>::size_type i = 0; i < points.size() -1; i++)
            {
                if(intersects(Segment(points[i], points[i+1])))
                {
                    return true;
                }
            }
            return false;
        }
        
        //! Retrieve the length of the bezier line.
        /** The function retrieves the length of the bezier line.
         @return The length of the bezier line.
         */
        inline double length() const noexcept override
        {
            double len = 0.;
            const std::vector<Point> points = discretized(100);
            for(std::vector<Point>::size_type i = 0; i < points.size() - 1; i++)
            {
                len += Segment(points[i], points[i+1]).length();
            }
            return len;
        }
    };
    
    // ================================================================================ //
    //                                    BEZIER QUAD                                   //
    // ================================================================================ //
    
    //! The quadratic curve is a curved line that holds one control value.
    /**
     The quadratic curve is used to represent a curved line in a space and allows several modification.
     */
    class BezierQuad : public BezierCurve
    {
    public:
        //! Constructor.
        /** The function initializes a quadratic curve, using (0, 0) as its start and end points.
         */
        inline BezierQuad() noexcept : m_ctrl() {}
        
        //! Constructor.
        /** The function initializes a quadratic curve with a segment.
         @param start The start point.
         @param end   The end point.
         */
        inline BezierQuad(const Segment& segment) noexcept : BezierCurve(segment), m_ctrl(segment.start()){}
        
        //! Constructor.
        /** The function initializes a quadratic curve with three points.
         @param start The start point.
         @param ctrl  The control point.
         @param end   The end point.
         */
        inline BezierQuad(const Point& start, const Point& ctrl, const Point& end) noexcept : BezierCurve(start, end), m_ctrl(ctrl){}
        
        //! Constructor.
        /** The function initializes a quadratic curve with another quadratic curve.
         @param curve The other quadratic curve.
         */
        inline BezierQuad(BezierQuad const& curve) noexcept : BezierCurve(curve.m_start, curve.m_end), m_ctrl(curve.m_ctrl){}
        
        //! Constructor.
        /** The function initializes a quadratic curve with another.
         @param curve The quadratic curve.
         */
        inline BezierQuad(BezierQuad&& curve) noexcept
        {
            std::swap(m_start, curve.m_start);
            std::swap(m_ctrl, curve.m_ctrl);
            std::swap(m_end, curve.m_end);
        }
        
        //! Constructor.
        /** The function initializes a quadratic curve with three points.
         @param start The start point.
         @param ctrl  The control point.
         @param end   The end point.
         */
        inline BezierQuad(Point&& start, Point&& ctrl, Point&& end) noexcept
        {
            std::swap(m_start, start);
            std::swap(m_ctrl, ctrl);
            std::swap(m_end, end);
        }
        
        //! Destructor.
        /** The function deletes the quadratic curve.
         */
        inline ~BezierQuad() noexcept {}
        
        //! Retrieve the quadratic curve as an atom.
        /** The function retrieves the quadratic curve as an atom.
         */
        inline operator Atom() const noexcept
        {
            return Atom({m_start.x(), m_start.y(), m_ctrl.x(), m_ctrl.y(), m_end.x(), m_end.y()});
        }
        
        //! Set the quadratic curve with an atom.
        /** The function sets the quadratic curve with an atom.
         @param atom The atom.
         */
        inline BezierQuad operator=(Atom const& atom) noexcept
        {
            if(atom.isVector())
            {
                Vector vector = atom;
                if(vector.size() > 5 &&
                   vector[0].isNumber() && vector[1].isNumber() &&
                   vector[2].isNumber() && vector[3].isNumber() &&
                   vector[4].isNumber() && vector[5].isNumber())
                {
                    m_start = Point(double(vector[0]), double(vector[1]));
                    m_ctrl  = Point(double(vector[2]), double(vector[3]));
                    m_end   = Point(double(vector[4]), double(vector[5]));
                }
            }
            return *this;
        }
        
        //! Retrieves the control point.
        /** The function retrieves the control point.
         @return The control point.
         */
        inline Point controlPoint() const noexcept
        {
            return m_ctrl;
        }
        
        //! Set a new control point.
        /** The function sets a new control point.
         @param newPoint The new control point.
         */
        inline void controlPoint(Point const& newPoint) noexcept
        {
            m_ctrl = newPoint;
        }
        
        //! Retrieves the abscissa of the control point.
        /** The function retrieves the abscissa of the control point.
         @return The abscissa of the control point.
         */
        inline double controlPointX() const noexcept
        {
            return m_ctrl.x();
        }
        
        //! Retrieves the ordinate of the control point.
        /** The function retrieves the ordinate of the control point.
         @return The ordinate of the control point.
         */
        inline double controlPointY() const noexcept
        {
            return m_ctrl.y();
        }
        
        //! Sets the abscissa of the control point.
        /** The function sets the abscissa of the control point.
         @param x The abscissa of the control point.
         */
        inline void controlPointX(const double x) noexcept
        {
            m_ctrl.x(x);
        }
        
        //! Sets the ordinate of the control point.
        /** The function sets the ordinate of the control point.
         @param y The ordinate of the control point.
         */
        inline void controlPointY(const double y) noexcept
        {
            m_ctrl.y(y);
        }
        
        //! Retrieve a point of the quadratic bezier line;
        /** The function retrieve a point of the quadratic bezier line.
         @param index The point index (0 to 2 for start, ctrl, end).
         @return The point.
         */
        inline Point operator[](const ulong index) noexcept
        {
            switch(index)
            {
                case 0:  return m_start;
                case 1:  return m_ctrl;
                case 2:  return m_end;
                default: return Point();
            }
        }
        
        //! Sets the quadratic curve with another one.
        /** The function sets the quadratic curve with another one.
         @param curve another quadratic curve.
         @return The quadratic curve.
         */
        inline BezierQuad& operator=(BezierQuad const& curve) noexcept
        {
            m_start = curve.start();
            m_ctrl  = curve.controlPoint();
            m_end   = curve.end();
            return *this;
        }
        
        //! Sets the quadratic curve with another one.
        /** The function sets the quadratic curve with another one.
         @param quadratic curve another quadratic curve.
         @return The quadratic curve.
         */
        inline BezierQuad& operator=(BezierQuad&& curve) noexcept
        {
            std::swap(m_start, curve.m_start);
            std::swap(m_ctrl, curve.m_ctrl);
            std::swap(m_end, curve.m_end);
            return *this;
        }
        
        //! Shift the quadratic curve by a point.
        /** The function shifts the quadratic curve by a point.
         @param pt The point.
         @return The quadratic curve.
         */
        inline BezierQuad& operator+=(Point const& pt) noexcept
        {
            m_start += pt;
            m_ctrl  += pt;
            m_end   += pt;
            return *this;
        }
        
        //! Shift the quadratic curve by a point.
        /** The function shifts the quadratic curve by a point.
         @param value The value.
         @return The quadratic curve.
         */
        inline BezierQuad& operator+=(double const value) noexcept
        {
            m_start += value;
            m_ctrl  += value;
            m_end   += value;
            return *this;
        }
        
        //! Shift the quadratic curve by a point.
        /** The function shifts the quadratic curve by a point.
         @param pt The point.
         @return The quadratic curve.
         */
        inline BezierQuad& operator-=(Point const& pt) noexcept
        {
            m_start -= pt;
            m_ctrl  -= pt;
            m_end   -= pt;
            return *this;
        }
        
        //! Shift the quadratic curve by a point.
        /** The function shifts the quadratic curve by a point.
         @param pt The point.
         @return The quadratic curve.
         */
        inline BezierQuad& operator-=(double const value) noexcept
        {
            m_start -= value;
            m_ctrl  -= value;
            m_end   -= value;
            return *this;
        }
        
        //! Retrieves a new quadratic curve shifted by a point.
        /** The function retrieves a new quadratic curve shifted by a point.
         @param pt The point to add.
         @return The new quadratic curve.
         */
        inline BezierQuad operator+(Point const& pt) const noexcept
        {
            return BezierQuad(m_start + pt, m_ctrl + pt, m_end + pt);
        }
        
        //! Retrieves a new quadratic curve shifted by a value.
        /** The function retrieves a new quadratic curve shifted by a value.
         @param value The value to add.
         @return The new quadratic curve.
         */
        inline BezierQuad operator+(double const value) const noexcept
        {
            return BezierQuad(m_start + value, m_ctrl + value, m_end + value);
        }
        
        //! Retrieves a new quadratic curve shifted by a point.
        /** The function retrieves a new quadratic curve shifted by a point.
         @param pt The point to add.
         @return The new quadratic curve.
         */
        inline BezierQuad operator-(Point const& pt) const noexcept
        {
            return BezierQuad(m_start - pt, m_ctrl - pt, m_end - pt);
        }
        
        //! Retrieves a new quadratic curve shifted by a value.
        /** The function retrieves a new quadratic curve shifted by a value.
         @param value The value to add.
         @return The new quadratic curve.
         */
        inline BezierQuad operator-(double const value) const noexcept
        {
            return BezierQuad(m_start - value, m_ctrl - value, m_end - value);
        }
        
        //! Get the equality of the quadratic curve with another.
        /** The function retrieves the equality of the quadratic curve with another.
         @param curve The other quadratic curve.
         @return true if the two quadratic curves are equal, otherwise false.
         */
        inline bool operator==(BezierQuad const& curve) const noexcept
        {
            return m_start == curve.m_start && m_end == curve.m_end && m_ctrl == curve.m_ctrl;
        }
        
        //! Get the equality of the quadratic curve with another.
        /** The function retrieves the equality of the quadratic curve with another.
         @param quadratic curve The other quadratic curve.
         @return true if the two quadratic curves are not equal, otherwise false.
         */
        inline bool operator!=(BezierQuad const& curve) const noexcept
        {
            return m_start != curve.m_start && m_end != curve.m_end && m_ctrl != curve.m_ctrl;
        }
        
        //! Retrieve a copy and apply a rotation from the origin.
        /** The function retrieves a copy and applies a rotation from the origin.
         @param The angle
         @return The copy with the rotation.
         */
        BezierQuad rotated(double const angle) const noexcept
        {
            return BezierQuad(m_start.rotated(angle), m_ctrl.rotated(angle), m_end.rotated(angle));
        }
        
        //! Retrieve a copy and apply a rotation from another point.
        /** The function retrieves a copy and applies a rotation from another point.
         @param origin The origin point.
         @param angle  The angle
         @return The copy with the rotation.
         */
        inline BezierQuad rotated(Point const& origin, double const angle) const noexcept
        {
            return BezierQuad(m_start.rotated(origin, angle), m_ctrl.rotated(origin, angle), m_end.rotated(origin, angle));
        }
        
        //! Apply a rotation from the origin.
        /** The function Applies a rotation from the origin.
         @param angle  The angle
         */
        void rotate(double const angle) noexcept override
        {
            m_start.rotate(angle);
            m_ctrl.rotate(angle);
            m_end.rotate(angle);
        }
        
        //! Apply a rotation from the origin from another point.
        /** The function Applies a rotation from another point.
         @param origin The origin point.
         @param angle  The angle
         */
        void rotate(Point const& origin, double const angle) noexcept override
        {
            *this = rotated(origin, angle);
        }
        
        //! Returns a reversed quadratic curve version.
        /** The function returns a quadratic curve that is the same as this one, but with the start and end reversed.
         */
        const BezierQuad reversed() const noexcept
        {
            return BezierQuad(m_end, m_ctrl, m_start);
        }
        
        //! Reverse start and end points.
        /** The function reverses start and end points.
         */
        const void reverse() noexcept override
        {
            std::swap(m_start, m_end);
        }
        
        //! Retrieve the smallest distance from a point.
        /** The function retrieves the smallest distance from a point.
         @param pt The point.
         @return The distance from the point.
         */
        double distance(Point const& pt) const noexcept override
        {
            return pt.distance(m_start, m_ctrl, m_end);
        }
        
        //! Retrieve the point along this quadratic curve that is nearest to another point.
        /** This function Retrieve the point along this quadratic curve that is nearest to another point.
         @param pt The point.
         @return The nearest point.
         */
        Point getNearestPoint(Point const& pt) const noexcept override
        {
            return pt.nearest(m_start, m_ctrl, m_end);
        }
        
        /** Retrieves the point which is at a given distance along this quadratic curve proportional to the quadratic curve's length.
         This function retrieves the the point which is at a given distance along this quadratic curve proportional to the quadratic curve's length.
         @param delta the distance to move along the quadratic curve from its start point, in multiples of the line's length.
         So a value of 0.0 will return the quadratic curve's start point
         and a value of 1.0 will return its end point.
         */
        Point getPointAt(const double delta) const noexcept override
        {
            return Point::fromLine(m_start, m_ctrl, m_end, delta);
        }
        
    private:
        Point m_ctrl;
    };
    
    // ================================================================================ //
    //                                   BEZIER CUBIC                                   //
    // ================================================================================ //
    
    //! The cubic curve is a curved line that holds one control value.
    /**
     The cubic curve is used to represent a curved line in a space and allows several modification.
     */
    class BezierCubic : public BezierCurve
    {
    public:
        //! Constructor.
        /** The function initializes a cubic curve, using (0, 0) as its start, end and control points.
         */
        inline BezierCubic() noexcept {}
        
        //! Constructor.
        /** The function initializes a cubic curve with a segment.
         @param segment The segment.
         */
        inline BezierCubic(const Segment& segment) noexcept :
            BezierCurve(segment), m_ctrl1(segment.start()), m_ctrl2(segment.end()) {}
        
        //! Constructor.
        /** The function initializes a cubic curve with four points.
         @param start The start point.
         @param ctrl1 The first control point.
         @param ctrl2 The second control point.
         @param end   The end point.
         */
        inline BezierCubic(const Point& start, const Point& ctrl1, const Point& ctrl2, const Point& end) noexcept :
            BezierCurve(start, end), m_ctrl1(ctrl1), m_ctrl2(ctrl2) {}
        
        //! Constructor.
        /** The function initializes a cubic curve with three points.
         @param start The start point.
         @param ctrl  The first and second control point.
         @param end   The end point.
         */
        inline BezierCubic(const Point& start, const Point& ctrl, const Point& end) noexcept :
            BezierCurve(start, end), m_ctrl1(ctrl), m_ctrl2(ctrl) {}
        
        //! Constructor.
        /** The function initializes a cubic curve with another cubic curve.
         @param curve The other cubic curve.
         */
        inline BezierCubic(BezierCubic const& curve) noexcept :
            BezierCurve(curve.m_start, curve.m_end), m_ctrl1(curve.m_ctrl1), m_ctrl2(curve.m_ctrl2) {}
        
        //! Constructor.
        /** The function initializes a cubic curve with a quadratic curve.
         @param curve The other cubic curve.
         */
        inline BezierCubic(BezierQuad const& curve) noexcept :
            BezierCurve(curve.start(), curve.end()), m_ctrl1(curve.controlPoint()), m_ctrl2(curve.controlPoint()) {}
        
        //! Constructor.
        /** The function initializes a cubic curve with another.
         @param curve The cubic curve.
         */
        inline BezierCubic(BezierCubic&& curve) noexcept
        {
            std::swap(m_start, curve.m_start);
            std::swap(m_ctrl1, curve.m_ctrl1);
            std::swap(m_ctrl2, curve.m_ctrl2);
            std::swap(m_end, curve.m_end);
        }
        
        //! Constructor.
        /** The function initializes a cubic curve with three points.
         @param start The start point.
         @param ctrl1 The first control point.
         @param ctrl2 The second control point.
         @param end   The end point.
         */
        inline BezierCubic(Point&& start, Point&& ctrl1, Point&& ctrl2, Point&& end) noexcept
        {
            std::swap(m_start, start);
            std::swap(m_ctrl1, ctrl1);
            std::swap(m_ctrl2, ctrl2);
            std::swap(m_end, end);
        }
        
        //! Create an elliptical arc with a set of cubic bezier points.
        /** The function create an elliptical arc with a set of cubic bezier points.
         @param center      The center of the arc.
         @param radius      The radius of the arc.
         @param startAngle  The start angle.
         @param endAngle    The end angle.
         @return A vector of cubic bezier points (start, ctrl1, ctrl2, end, ctrl1, ctrl2, end, ...)
         */
        static std::vector<Point> fromArc(Point const& center, const Point& radius, double startAngle, double endAngle) noexcept;
        
        //! Destructor.
        /** The function deletes the cubic curve.
         */
        inline ~BezierCubic() noexcept {}
        
        //! Retrieve the cubic curve as an atoms.
        /** The function retrieves the cubic curve as an atoms.
         */
        inline operator Atom() const noexcept
        {
            return Atom({m_start.x(), m_start.y(), m_ctrl1.x(), m_ctrl1.y(), m_ctrl2.x(), m_ctrl2.y(), m_end.x(), m_end.y()});
        }
        
        //! Set the cubic curve with an atoms.
        /** The function sets the cubic curve with an atoms.
         @param atom The atom.
         */
        inline BezierCubic operator=(Atom const& atom) noexcept
        {
            if(atom.isVector())
            {
                Vector vector = atom;
                if(vector.size() > 7 &&
                   vector[0].isNumber() && vector[1].isNumber() &&
                   vector[2].isNumber() && vector[3].isNumber() &&
                   vector[4].isNumber() && vector[5].isNumber() &&
                   vector[6].isNumber() && vector[7].isNumber())
                {
                    m_start = Point(double(vector[0]), double(vector[1]));
                    m_ctrl1 = Point(double(vector[2]), double(vector[3]));
                    m_ctrl2 = Point(double(vector[4]), double(vector[5]));
                    m_end   = Point(double(vector[6]), double(vector[7]));
                }
            }
            return *this;
        }
        
        //! Retrieves the first control point.
        /** The function retrieves the first control point.
         @return The first control point.
         */
        inline Point controlPoint1() const noexcept
        {
            return m_ctrl1;
        }
        
        //! Retrieves the second control point.
        /** The function retrieves the second control point.
         @return The second control point.
         */
        inline Point controlPoint2() const noexcept
        {
            return m_ctrl2;
        }
        
        //! Set a new first control point.
        /** The function sets a new first control point.
         @param newPoint The new first control point.
         */
        inline void controlPoint1(Point const& newPoint) noexcept
        {
            m_ctrl1 = newPoint;
        }
        
        //! Set a new second control point.
        /** The function sets a new second control point.
         @param newPoint The new second control point.
         */
        inline void controlPoint2(Point const& newPoint) noexcept
        {
            m_ctrl2 = newPoint;
        }
        
        //! Retrieves the abscissa of the first control point.
        /** The function retrieves the abscissa of the first control point.
         @return The abscissa of the first control point.
         */
        inline double controlPoint1X() const noexcept
        {
            return m_ctrl1.x();
        }
        
        //! Retrieves the ordinate of the first control point.
        /** The function retrieves the ordinate of the first control point.
         @return The ordinate of the first control point.
         */
        inline double controlPoint1Y() const noexcept
        {
            return m_ctrl1.y();
        }
        
        //! Retrieves the abscissa of the second control point.
        /** The function retrieves the abscissa of the second control point.
         @return The abscissa of the second control point.
         */
        inline double controlPoint2X() const noexcept
        {
            return m_ctrl2.x();
        }
        
        //! Retrieves the ordinate of the second control point.
        /** The function retrieves the ordinate of the second control point.
         @return The ordinate of the second control point.
         */
        inline double controlPoint2Y() const noexcept
        {
            return m_ctrl2.y();
        }
        
        //! Sets the abscissa of the first control point.
        /** The function sets the abscissa of the first control point.
         @param x The abscissa of the first control point.
         */
        inline void controlPoint1X(const double x) noexcept
        {
            m_ctrl1.x(x);
        }
        
        //! Sets the ordinate of the first control point.
        /** The function sets the ordinate of the first control point.
         @param y The ordinate of the first control point.
         */
        inline void controlPoint1Y(const double y) noexcept
        {
            m_ctrl1.y(y);
        }
        
        //! Sets the abscissa of the second control point.
        /** The function sets the abscissa of the second control point.
         @param x The abscissa of the second control point.
         */
        inline void controlPoint2X(const double x) noexcept
        {
            m_ctrl2.x(x);
        }
        
        //! Sets the ordinate of the second control point.
        /** The function sets the ordinate of the second control point.
         @param y The ordinate of the second control point.
         */
        inline void controlPoint2Y(const double y) noexcept
        {
            m_ctrl2.y(y);
        }
        
        //! Retrieve a point of the cubic bezier line;
        /** The function retrieve a point of the cubic bezier line.
         @param index The point index (0 to 3 for start, ctrl1, ctrl2, end).
         @return The point.
         */
        inline Point operator[](const ulong index) noexcept
        {
            switch(index)
            {
                case 0:  return m_start;
                case 1:  return m_ctrl1;
                case 2:  return m_ctrl2;
                case 3:  return m_end;
                default: return Point();
            }
        }
        
        //! Sets the cubic curve with another one.
        /** The function sets the cubic curve with another one.
         @param curve another cubic curve.
         @return The cubic curve.
         */
        inline BezierCubic& operator=(BezierCubic const& curve) noexcept
        {
            m_start = curve.m_start;
            m_ctrl1 = curve.m_ctrl1;
            m_ctrl2 = curve.m_ctrl2;
            m_end   = curve.m_end;
            return *this;
        }
        
        //! Sets the cubic curve with another one.
        /** The function sets the cubic curve with another one.
         @param cubic curve another cubic curve.
         @return The cubic curve.
         */
        inline BezierCubic& operator=(BezierCubic&& curve) noexcept
        {
            std::swap(m_start, curve.m_start);
            std::swap(m_ctrl1, curve.m_ctrl1);
            std::swap(m_ctrl2, curve.m_ctrl2);
            std::swap(m_end,   curve.m_end);
            return *this;
        }
        
        //! Shift the cubic curve by a point.
        /** The function shifts the cubic curve by a point.
         @param pt The point.
         @return The cubic curve.
         */
        inline BezierCubic& operator+=(Point const& pt) noexcept
        {
            m_start += pt;
            m_ctrl1 += pt;
            m_ctrl2 += pt;
            m_end   += pt;
            return *this;
        }
        
        //! Shift the cubic curve by a point.
        /** The function shifts the cubic curve by a point.
         @param value The value.
         @return The cubic curve.
         */
        inline BezierCubic& operator+=(double const value) noexcept
        {
            m_start += value;
            m_ctrl1 += value;
            m_ctrl2 += value;
            m_end   += value;
            return *this;
        }
        
        //! Shift the cubic curve by a point.
        /** The function shifts the cubic curve by a point.
         @param pt The point.
         @return The cubic curve.
         */
        inline BezierCubic& operator-=(Point const& pt) noexcept
        {
            m_start -= pt;
            m_ctrl1 -= pt;
            m_ctrl2 -= pt;
            m_end   -= pt;
            return *this;
        }
        
        //! Shift the cubic curve by a point.
        /** The function shifts the cubic curve by a point.
         @param pt The point.
         @return The cubic curve.
         */
        inline BezierCubic& operator-=(double const value) noexcept
        {
            m_start -= value;
            m_ctrl1 -= value;
            m_ctrl2 -= value;
            m_end   -= value;
            return *this;
        }
        
        //! Retrieves a new cubic curve shifted by a point.
        /** The function retrieves a new cubic curve shifted by a point.
         @param pt The point to add.
         @return The new cubic curve.
         */
        inline BezierCubic operator+(Point const& pt) const noexcept
        {
            return BezierCubic(m_start + pt, m_ctrl1 + pt, m_ctrl2 + pt, m_end + pt);
        }
        
        //! Retrieves a new cubic curve shifted by a value.
        /** The function retrieves a new cubic curve shifted by a value.
         @param value The value to add.
         @return The new cubic curve.
         */
        inline BezierCubic operator+(double const value) const noexcept
        {
            return BezierCubic(m_start + value, m_ctrl1 + value, m_ctrl2 + value, m_end + value);
        }
        
        //! Retrieves a new cubic curve shifted by a point.
        /** The function retrieves a new cubic curve shifted by a point.
         @param pt The point to add.
         @return The new cubic curve.
         */
        inline BezierCubic operator-(Point const& pt) const noexcept
        {
            return BezierCubic(m_start - pt, m_ctrl1 - pt, m_ctrl2 - pt, m_end - pt);
        }
        
        //! Retrieves a new cubic curve shifted by a value.
        /** The function retrieves a new cubic curve shifted by a value.
         @param value The value to add.
         @return The new cubic curve.
         */
        inline BezierCubic operator-(double const value) const noexcept
        {
            return BezierCubic(m_start - value, m_ctrl1 - value, m_ctrl2 - value, m_end - value);
        }
        
        //! Get the equality of the cubic curve with another.
        /** The function retrieves the equality of the cubic curve with another.
         @param curve The other cubic curve.
         @return true if the two cubic curves are equal, otherwise false.
         */
        inline bool operator==(BezierCubic const& curve) const noexcept
        {
            return m_start == curve.m_start && m_end == curve.m_end && m_ctrl1 == curve.m_ctrl1 && m_ctrl2 == curve.m_ctrl2;
        }
        
        //! Get the equality of the cubic curve with another.
        /** The function retrieves the equality of the cubic curve with another.
         @param cubic curve The other cubic curve.
         @return true if the two cubic curves are not equal, otherwise false.
         */
        inline bool operator!=(BezierCubic const& curve) const noexcept
        {
            return m_start != curve.m_start && m_end != curve.m_end && m_ctrl1 != curve.m_ctrl1 && m_ctrl2 != curve.m_ctrl2;
        }
        
        //! Retrieve a copy and apply a rotation from the origin.
        /** The function retrieves a copy and applies a rotation from the origin.
         @param The angle
         @return The copy with the rotation.
         */
        BezierCubic rotated(double const angle) const noexcept
        {
            return BezierCubic(m_start.rotated(angle), m_ctrl1.rotated(angle), m_ctrl2.rotated(angle), m_end.rotated(angle));
        }
        
        //! Retrieve a copy and apply a rotation from another point.
        /** The function retrieves a copy and applies a rotation from another point.
         @param origin The origin point.
         @param angle  The angle
         @return The copy with the rotation.
         */
        inline BezierCubic rotated(Point const& origin, double const angle) const noexcept
        {
            return BezierCubic(m_start.rotated(origin, angle),
                               m_ctrl1.rotated(origin, angle),
                               m_ctrl2.rotated(origin, angle),
                               m_end.rotated(origin, angle));
        }
        
        //! Apply a rotation from the origin.
        /** The function Applies a rotation from the origin.
         @param angle  The angle
         */
        void rotate(double const angle) noexcept override
        {
            m_start.rotate(angle);
            m_ctrl1.rotate(angle);
            m_ctrl2.rotate(angle);
            m_end.rotate(angle);
        }
        
        //! Apply a rotation from the origin from another point.
        /** The function Applies a rotation from another point.
         @param origin The origin point.
         @param angle  The angle
         */
        void rotate(Point const& origin, double const angle) noexcept override
        {
            *this = rotated(origin, angle);
        }
        
        //! Returns a reversed cubic curve version.
        /** The function returns a cubic curve that is the same as this one, but with the start, end and control points reversed.
         */
        const BezierCubic reversed() const noexcept
        {
            return BezierCubic(m_end, m_ctrl2, m_ctrl1, m_start);
        }
        
        //! Reverse start and end points.
        /** The function reverses start, end and control points.
         */
        const void reverse() noexcept override
        {
            std::swap(m_start, m_end);
            std::swap(m_ctrl1, m_ctrl2);
        }
        
        //! Retrieve the smallest distance from a point.
        /** The function retrieves the smallest distance from a point.
         @param pt The point.
         @return The distance from the point.
         */
        double distance(Point const& pt) const noexcept override
        {
            return pt.distance(m_start, m_ctrl1, m_ctrl2, m_end);
        }
        
        //! Retrieve the point along this cubic curve that is nearest to another point.
        /** This function Retrieve the point along this cubic curve that is nearest to another point.
         @param pt The point.
         @return The nearest point.
         */
        Point getNearestPoint(Point const& pt) const noexcept override
        {
            return pt.nearest(m_start, m_ctrl1, m_ctrl2, m_end);
        }
        
        /** Retrieves the point which is at a given distance along this cubic curve proportional to the cubic curve's length.
         This function retrieves the the point which is at a given distance along this cubic curve proportional to the cubic curve's length.
         @param delta the distance to move along the cubic curve from its start point, in multiples of the line's length.
         So a value of 0.0 will return the cubic curve's start point
         and a value of 1.0 will return its end point.
         */
        Point getPointAt(const double delta) const noexcept override
        {
            return Point::fromLine(m_start, m_ctrl1, m_ctrl2, m_end, delta);
        }
        
    private:
        Point m_ctrl1, m_ctrl2;
    };
}

#endif // KIWI_LINE_H_INCLUDED
