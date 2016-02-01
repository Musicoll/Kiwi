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

#ifndef __DEF_KIWI_GUI_RECTANGLE__
#define __DEF_KIWI_GUI_RECTANGLE__

#include "KiwiSize.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      RECTANGLE                                   //
    // ================================================================================ //
    
    //! The rectangle holds four double values.
    /**
     The rectangle is used to represent a rectangle in a space and allows several modification.
     */
    class Rectangle
    {
    private:
        Point m_position;
        Size  m_size;
        
    public:
        
        //! Rectangle borders
        enum Border
        {
            Top         = 1,
            Right       = 2,
            Bottom      = 4,
            Left        = 8
        };
        
        //! Rectangle corners
        enum Corner
        {
            TopLeft     = 1,
            TopRight    = 2,
            BottomRight = 4,
            BottomLeft  = 8
        };
        
        //! Constructor.
        /** The function initializes a rectangle.
         */
        inline Rectangle() noexcept {}
        
        //! Constructor.
        /** The function initializes a rectangle with four double values.
         @param x       The abscissa of the top-left corner.
         @param y       The ordinate of the top-left corner.
         @param w       The width of the rectangle.
         @param h       The height of the rectangle.
         @param ratio   If width over height optional ratio.
         */
        inline Rectangle(const double x, const double y, const double w, const double h, const bool ratio = false) noexcept :
        m_position(x, y), m_size(w, h, ratio) {}
        
        //! Constructor.
        /** The function initializes a rectangle with two points.
         @param position The poisition of the top-left corner.
         @param size     The size of the rectangle.
         */
        inline Rectangle(Point const& position, Size const& size) noexcept :
        m_position(position), m_size(size) {}
        
        //! Constructor.
        /** The function initializes another rectangle.
         @param rect The other rectangle.
         */
        inline Rectangle(Rectangle const& rect) noexcept :
        m_position(rect.m_position), m_size(rect.m_size) {}
        
        //! Constructor.
        /** The function initializes a rectangle with two points.
         @param position The poisition of the top-left corner.
         @param size     The size of the rectangle.
         */
        inline Rectangle(Point&& position, Size&& size) noexcept
        {
            swap(m_position, position);
            swap(m_size, size);
        }
        
        //! Constructor.
        /** The function initializes another rectangle.
         @param rect The other rectangle.
         */
        inline Rectangle(Rectangle&& rect) noexcept
        {
            swap(m_position, rect.m_position);
            swap(m_size, rect.m_size);
        }
        
        //! Destructor.
        /** The function deletes the rectangle.
         */
        inline ~Rectangle() noexcept {}
        
        //! Return a rectangle with the positions of two opposite corners.
        /** The function returns a rectangle with the positions of two opposite corners.
         @param corner1 The position of the first corner.
         @param corner1 The position of the second corner.
         @return The new rectangle.
         */
        static inline Rectangle withCorners(Point const& corner1, Point const& corner2) noexcept
        {
            return Rectangle(min(corner1.x(), corner2.x()), min(corner1.y(), corner2.y()), abs(corner1.x() - corner2.x()), abs(corner1.y() - corner2.y()));
        }
        
        //! Return a rectangle with the center positions and the size.
        /** The function returns a rectangle with the center positions and the size.
         @param centre The position of centre.
         @param size The size.
         @return The new rectangle.
         */
        static inline Rectangle withCentre(Point const& centre, Size const& size) noexcept
        {
            return Rectangle(centre - Point(size) * 0.5, size);
        }
        
        //! Return a rectangle by specifying its edges.
        /** The function returns a rectangle with the center positions and the size.
         @param left    The left edge.
         @param top     The top edge.
         @param right   The right edge.
         @param bottom  The bottom edge.
         @return A rectangle.
         */
        static inline Rectangle withEdges(const double left, const double top, const double right, const double bottom) noexcept
        {
            return Rectangle(left, top, right - left, bottom - top);
        }
        
        //! Return the same rectangle with a different position.
        /** The function returns the same rectangle with a different position.
         @param newpos The new position of the rectangle.
         @return The new rectangle.
         */
        inline Rectangle withPosition(Point const& newpos) const noexcept
        {
            return Rectangle(newpos, m_size);
        }
        
        //! Return the same rectangle with a different position.
        /** The function returns the same rectangle with a different position.
         @param newpos The new position of the rectangle.
         @return The new rectangle.
         */
        inline Rectangle withPosition(Point&& newpos) const noexcept
        {
            return Rectangle(forward<Point>(newpos), size());
        }
        
        //! Return the same rectangle with a different abscissa position.
        /** The function returns the same rectangle with a different abscissa position.
         @param abscissa The new abscissa position of the rectangle.
         @return The new rectangle.
         */
        inline Rectangle withX(const double abscissa) const noexcept
        {
            return Rectangle(Point(abscissa, y()), size());
        }
        
        //! Return the same rectangle with a different ordinate position.
        /** The function returns the same rectangle with a different ordinate position.
         @param ordinate The new ordinate position of the rectangle.
         @return The new rectangle.
         */
        inline Rectangle withY(const double ordinate) const noexcept
        {
            return Rectangle(Point(x(), ordinate), size());
        }
        
        //! Return the same rectangle with a different size.
        /** The function returns the same rectangle with a different size.
         @param newSize The new size of the rectangle.
         @return The new rectangle.
         */
        inline Rectangle withSize(Size const& newSize) const noexcept
        {
            return Rectangle(m_position, newSize);
        }
        
        //! Return the same rectangle with a different size.
        /** The function returns the same rectangle with a different size.
         @param newSize The new size of the rectangle.
         @return The new rectangle.
         */
        inline Rectangle withSize(Size&& newSize) const noexcept
        {
            return Rectangle(position(), forward<Size>(newSize));
        }
        
        //! Return the same rectangle with a different width.
        /** The function returns the same rectangle with a different width.
         @param width The new width of the rectangle.
         @return The new rectangle.
         */
        inline Rectangle withWidth(const double newWidth) const noexcept
        {
            return Rectangle(position(), Size(newWidth, height()));
        }
        
        //! Return the same rectangle with a different width.
        /** The function returns the same rectangle with a different width.
         @param width The new width of the rectangle.
         @return The new rectangle.
         */
        inline Rectangle withHeight(const double newHeight) const noexcept
        {
            return Rectangle(position(), Size(width(), newHeight));
        }
        
        //! Return the same rectangle with a new left position.
        /** The function returns the same rectangle with a new left position.
         @param newTop The new left position of the rectangle.
         @return The new rectangle.
         */
        inline Rectangle withLeft(const double newLeft) const noexcept
        {
            return Rectangle(Point(newLeft, y()), Size(width() - newLeft, height()));
        }
        
        //! Return the same rectangle with a new top position.
        /** The function returns the same rectangle with a new top position.
         @param newTop The new top position of the rectangle.
         @return The new rectangle.
         */
        inline Rectangle withTop(const double newTop) const noexcept
        {
            return Rectangle(Point(x(), newTop), Size(width(), bottom() - newTop));
        }
        
        //! Return the same rectangle with a new right position.
        /** The function returns the same rectangle with a new right position.
         @param newRight The new right position of the rectangle.
         @return The new rectangle.
         */
        inline Rectangle withRight(const double newRight) const noexcept
        {
            return Rectangle(position(), Size(newRight - x(), height()));
        }
        
        //! Return the same rectangle with a new bottom position.
        /** The function returns the same rectangle with a new bottom position.
         @param newRight The new bottom position of the rectangle.
         @return The new rectangle.
         */
        inline Rectangle withBottom(const double newBottom) const noexcept
        {
            return Rectangle(position(), Size(width(), newBottom - y()));
        }
        
        //! Return the same rectangle with a position at zero origin.
        /** The function returns the same rectangle with a position at zero origin.
         @return The new rectangle.
         */
        inline Rectangle withZeroOrigin() const noexcept
        {
            return Rectangle(Point(0., 0.), size());
        }
        //! Retrieve the abscissa.
        /** The function retrieves the abscissa.
         @return The abscissa.
         */
        inline double x() const noexcept
        {
            return m_position.x();
        }
        
        //! Retrieve the ordinate.
        /** The function retrieves the ordinate.
         @return The ordinate.
         */
        inline double y() const noexcept
        {
            return m_position.y();
        }
        
        //! Retrieve the width.
        /** The function retrieves the width.
         @return The width.
         */
        inline double width() const noexcept
        {
            return m_size.width();
        }
        
        //! Retrieve the height.
        /** The function retrieves the height.
         @return The height.
         */
        inline double height() const noexcept
        {
            return m_size.height();
        }
        
        //! Retrieve the position.
        /** The function retrieves the position.
         @return The position.
         */
        inline Point position() const noexcept
        {
            return m_position;
        }
        
        //! Retrieve the size.
        /** The function retrieves the size.
         @return The size.
         */
        inline Size size() const noexcept
        {
            return m_size;
        }
        
        //! Retrieve the ordinate of the top aka the y position of the rectangle.
        /** The function retrieves the ordinate of the top aka the  y position of the rectangle.
         @return The ordinate of the top.
         */
        inline double top() const noexcept
        {
            return y();
        }
        
        //! Retrieve the abscissa of the left aka the x position of the rectangle.
        /** The function retrieves the abscissa of the left aka the x position of the rectangle.
         @return The abscissa of the left.
         */
        inline double left() const noexcept
        {
            return x();
        }
        
        //! Retrieve the ordinate of the bottom.
        /** The function retrieves the ordinate of the bottom.
         @return The ordinate of the bottom.
         */
        inline double bottom() const noexcept
        {
            return y() + height();
        }
        
        //! Retrieve the abscissa of the right.
        /** The function retrieves the abscissa of the right.
         @return The abscissa of the right.
         */
        inline double right() const noexcept
        {
            return x() + width();
        }
        
        //! Retrieve the top-left point.
        /** The function retrieves the top-left point.
         @return The top-left point.
         */
        inline Point topLeft() const noexcept
        {
            return m_position;
        }
        
        //! Retrieve the top-right point.
        /** The function retrieves the top-right point.
         @return The top-right point.
         */
        inline Point topRight() const noexcept
        {
            return Point(right(), y());
        }
        
        //! Retrieve the bottom-right point.
        /** The function retrieves the bottom-right point.
         @return The bottom-right point.
         */
        inline Point bottomRight() const noexcept
        {
            return Point(right(), bottom());
        }
        
        //! Retrieve the bottom-right point.
        /** The function retrieves the bottom-right point.
         @return The bottom-right point.
         */
        inline Point bottomLeft() const noexcept
        {
            return Point(x(), bottom());
        }
        
        //! Retrieves the centre of the rectangle.
        /** The function retrieves the centre of the rectangle.
         @return The centre of the rectangle.
         */
        inline Point centre() const noexcept
        {
            return Point(x() + width() * 0.5, y() + height() * 0.5);
        }
        
        //! Retrieve the ratio.
        /** The function retrieves the ratio. If zero, the ratio width over height isn't respected.
         @return The ratio.
         */
        inline double ratio() const noexcept
        {
            return m_size.ratio();
        }
        
        //! Set the abscissa.
        /** The function sets the abscissa.
         @param x The abscissa.
         */
        inline void x(const double x) noexcept
        {
            m_position.x(x);
        }
        
        //! Set the ordinate.
        /** The function sets the ordinate.
         @param y The ordinate.
         */
        inline void y(const double y) noexcept
        {
            m_position.y(y);
        }
        
        //! Set the width.
        /** The function sets the width.
         @param width The width.
         */
        inline void width(const double width) noexcept
        {
            m_size.width(width);
        }
        
        //! Set the height.
        /** The function sets the height.
         @param height The height.
         */
        inline void height(const double height) noexcept
        {
            m_size.height(height);
        }
        
        //! Set the position.
        /** The function sets the position.
         @param position The position.
         */
        inline void position(Point const& position) noexcept
        {
            m_position = position;
        }
        
        //! Set the position.
        /** The function sets the position.
         @param position The position.
         */
        inline void position(Point&& position) noexcept
        {
            m_position = position;
        }
        
        //! Set the size.
        /** The function sets the size.
         @param size The size.
         */
        inline void size(Size const& size) noexcept
        {
            m_size = size;
        }
        
        //! Set the size.
        /** The function sets the size.
         @param size The size.
         */
        inline void size(Size&& size) noexcept
        {
            m_size = size;
        }
        
        //! Move the abscissa position of the rectangle.
        /** The function moves the abscissa position of the rectangle  and expands or retracts it.
         @param left The abscissa position.
         */
        void left(const double left) noexcept
        {
            width(right() - left);
            x(left);
        }
        
        //! Move the ordinate position of the rectangle.
        /** The function moves the ordinate position of the rectangle  and expands or retracts it.
         @param top The ordinate position.
         */
        void top(double const top) noexcept
        {
            height(bottom() - top);
            y(top);
        }
        
        //! Move the right position of the rectangle.
        /** The function moves the right position of the rectangle and expands or retracts it.
         @param top The right position.
         */
        void right(const double right) noexcept
        {
            x(min(x(), right));
            width(right - x());
        }
        
        //! Move the bottom position of the rectangle.
        /** The function moves the bttom position of the rectangle and expands or retracts it.
         @param bottom The bottom position.
         */
        void bottom(double bottom) noexcept
        {
            y(min(y(), bottom));
            height(bottom - y());
        }
        
        //! Set the position and the size with another rectangle.
        /** The function sets the position and the size with another rectangle.
         @param rect The other rectangle.
         @return The rectangle.
         */
        inline Rectangle& operator=(Rectangle const& rect) noexcept
        {
            m_position = rect.position();
            m_size = rect.size();
            return *this;
        }
        
        //! Set the position and the size with another rectangle.
        /** The function sets the position and the size with another rectangle.
         @param rect The other rectangle.
         @return The rectangle.
         */
        inline Rectangle& operator=(Rectangle&& rect) noexcept
        {
            swap(m_position, rect.m_position);
            swap(m_size, rect.m_size);
            return *this;
        }
        
        //! Shift the rectangle with a point.
        /** The function shifts the rectangle with a point.
         @param pt The point.
         @return The rectangle.
         */
        inline Rectangle& operator+=(Point const& pt) noexcept
        {
            m_position += pt;
            return *this;
        }
        
        //! Shift the rectangle with a value.
        /** The function shifts the rectangle with a value.
         @param value The value.
         @return The rectangle.
         */
        inline Rectangle& operator+=(double const value) noexcept
        {
            m_position += value;
            return *this;
        }
        
        //! Shift the rectangle with a point.
        /** The function shifts the rectangle with a point.
         @param pt The point.
         @return The rectangle.
         */
        inline Rectangle& operator-=(Point const& pt) noexcept
        {
            m_position -= pt;
            return *this;
        }
        
        //! Shift the rectangle with a value.
        /** The function shifts the rectangle with a value.
         @param value The value.
         @return The rectangle.
         */
        inline Rectangle& operator-=(double const value) noexcept
        {
            m_position -= value;
            return *this;
        }
        
        //! Retrieve the rectangle shifted with a point.
        /** The function the rectangle shifted with a point.
         @param pt The point.
         @return The rectangle.
         */
        inline Rectangle operator+(Point const& pt) noexcept
        {
            return Rectangle(m_position + pt, m_size);
        }
        
        //! Retrieve the rectangle shifted with a value.
        /** The function the rectangle shifted with a value.
         @param value The value.
         @return The rectangle.
         */
        inline Rectangle operator+(double const value) noexcept
        {
            return Rectangle(m_position + value, m_size);
        }
        
        //! Retrieve the rectangle shifted with a point.
        /** The function the rectangle shifted with a point.
         @param pt The point.
         @return The rectangle.
         */
        inline Rectangle operator-(Point const& pt) noexcept
        {
            return Rectangle(m_position - pt, m_size);
        }
        
        //! Retrieve the rectangle shifted with a value.
        /** The function the rectangle shifted with a value.
         @param value The value.
         @return The rectangle.
         */
        inline Rectangle operator-(double const value) noexcept
        {
            return Rectangle(m_position - value, m_size);
        }
        
        //! Get the equality two rectangle.
        /** The function retrieves the equality two rectangle.
         @param rect The other rectangle.
         @return true if the two retrieves are equal, otherwise false.
         */
        inline bool operator==(Rectangle const& rect) const noexcept
        {
            return m_position == rect.position() && m_size == rect.size();
        }
        
        //! Get the equality two rectangle.
        /** The function retrieves the equality two rectangle.
         @param rect The other rectangle.
         @return true if the two retrieves are not equal, otherwise false.
         */
        inline bool operator!=(Rectangle const& rect) const noexcept
        {
            return m_position != rect.position() || m_size != rect.size();
        }
        
        //! Expand the rectangle.
        /** The function expands the rectangle.
         @param pt The amount of expansion.
         */
        inline void expand(Point const& pt) noexcept
        {
            m_position = m_position - pt;
            width(width() + pt.x() * 2.);
            height(height() + pt.y() * 2.);
        }
        
        //! Expand the rectangle.
        /** The function expands the rectangle.
         @param size The amount of expansion.
         */
        inline void expand(Size const& size) noexcept
        {
            position(m_position - Point(size));
            m_size += size * 2.;
        }
        
        //! Expand the rectangle.
        /** The function expands the rectangle.
         @param value The amount of expansion.
         */
        inline void expand(double const value) noexcept
        {
            expand(Point(value, value));
        }
        
        //! Return an expanded version of this rectangle.
        /** The function returns an expanded version of this rectangle.
         @param pt The amount of expansion.
         @return The new rectangle.
        */
        inline Rectangle expanded(Point const& pt) const noexcept
        {
            return Rectangle(m_position.x() - pt.x(), m_position.y() - pt.y(), max(0., width() + pt.x() * 2.), max(0., height() + pt.y() * 2.));
        }
        
        //! Return an the expanded rectangle.
        /** The function returns an expanded rectangle.
         @param value The amount of expansion.
         @return The new rectangle.
         */
        inline Rectangle expanded(const double value) const noexcept
        {
            return expanded(Point(value, value));
        }
        
        //! Reduce the rectangle.
        /** The function reduces the rectangle.
         @param pt The amount of reduction.
         */
        inline void reduce(Point const& pt) noexcept
        {
            expand(-pt);
        }
        
        //! Reduce the rectangle.
        /** The function reduces the rectangle.
         @param delta The amount of reduction.
         */
        inline void reduce(double const value) noexcept
        {
            expand(Point(-value, -value));
        }
        
        //! Return an the reduced rectangle.
        /** The function returns an reduced rectangle.
         @param pt The amount of reduction.
         @return The new rectangle.
         */
        inline Rectangle reduced(Point const& pt) const noexcept
        {
            return expanded(-pt);
        }
        
        //! Return an the reduced rectangle.
        /** The function returns an reduced rectangle.
         @param value The amount of reduction.
         @return The new rectangle.
         */
        inline Rectangle reduced(const double value) const noexcept
        {
            return reduced(Point(value, value));
        }
        
        //! Return a resized version of this rectangle.
        /** The function returns a resized version of this rectangle.
         @param borderflag      The border flag from which to resize the rectangle.
         @param delta           The amount of reduction.
         @param smin            The minimum size.
         @param smax            The maximum size.
         @param delta           The amount of reduction.
         @param preserveRatio   True if you want to preserve the original width over height ratio.
         @param opposite        True if you want to resize the opposite border.
         @return A rectangle.
         */
        Rectangle resized(const ulong borderflag, Point const& delta,
                          const Point smin = Point(0., 0.), const Point smax = Point(0., 0.),
                          const bool preserveRatio = false, const bool opposite = false) const noexcept;
        
        //! Return a new version of this rectangle with its edges clipped.
        /** The function returns a new version of this rectangle with its edges clipped.
         @param left    The left edge.
         @param top     The top edge.
         @param right   The right edge.
         @param bottom  The bottom edge.
         @return A rectangle.
         */
        Rectangle withClippedEdges(const double left, const double top, const double right, const double bottom) const noexcept
        {
            return Rectangle::withEdges(max(left, this->left()),
                                        max(top, this->top()),
                                        min(right, this->right()),
                                        min(bottom, this->bottom()));
        }
        
        //! Get if the rectangle overlaps another rectangle.
        /** The function retrieves if the rectangle overlaps another rectangle.
         @param other The other rectangle.
         @return true if the rectangle overlaps the other rectangle, otherwise false.
         */
        inline bool overlaps(Rectangle const& other) const noexcept
        {
            return (x() + width() >= other.x() && y() + height() >= other.y() && x() <= other.x() + other.width() && y() <= other.y() + other.height());
        }
        
        //! Get if the rectangle contains a point.
        /** The function retrieves if the rectangle contains a point.
         @param pt The point.
         @return true if the rectangle contains the point, otherwise false.
         */
        inline bool contains(Point const& pt) const noexcept
        {
            return (pt.x() >= x() && pt.y() >= y() && pt.x() <= right() && pt.y() <= bottom());
        }
        
        //! Get if the rectangle contains a segment.
        /** The function returns true if the rectangle contains both the start and end point.
         @param segment The segment.
         @return true if the rectangle contains the segment, otherwise false.
         */
        bool contains(Segment const& segment) const noexcept
        {
            return (contains(segment.start()) && contains(segment.end()));
        }
        
        //! Get if the rectangle contains another rectangle.
        /** The function returns true if the rectangle contains another rectangle.
         @param rect The rectangle.
         @return true if the rectangle contains the other, otherwise false.
         */
        bool contains(Rectangle const& rect) const noexcept
        {
            return contains(rect.position()) && contains(rect.bottomRight());
        }
        
        //! Get if the rectangle intesects a segment.
        /** The function gets if the rectangle intersects a segment.
         @param segment The segment.
         @return true if the rectangle intersects the segment, otherwise false.
         */
        bool intersects(Segment const& segment) const noexcept;
        
        //! Get if the rectangle overlaps a segment.
        /** The function gets if the rectangle overlaps a segment.
         @param begin The first point of the line.
         @param end   The end point of the line.
         @return true if the rectangle overlaps a line, otherwise false.
         */
        bool overlaps(Segment const& segment) const noexcept;
        
        //! Get if the rectangle overlaps a quadratic bezier line.
        /** The function gets if the rectangle overlaps a quadratic bezier line.
         @param curve The quadratic bezier line.
         @return true if the rectangle overlaps a line, otherwise false.
         */
        bool overlaps(BezierQuad const& curve) const noexcept;
        
        //! Get if the rectangle overlaps a cubic bezier line.
        /** The function gets if the rectangle overlaps a cubic bezier line.
         @param curve The cubic bezier line.
         @return true if the rectangle overlaps a line, otherwise false.
         */
        bool overlaps(BezierCubic const& curve) const noexcept;
        
        // ================================================================================ //
        //                                      ATTR                                        //
        // ================================================================================ //
        
        //! Retrieve the rectangle as an atom.
        /** The function retrieves the rectangle as an atom.
         @return The atom.
         */
        inline operator Atom() const noexcept
        {
            return Atom({x(), y(), width(), height()});
        }
        
        //! Set the point with an atom.
        /** The function sets the rectangle with an atom.
         @param atom The atom.
         */
        inline Rectangle operator=(Atom const& atom) noexcept
        {
            if(atom.isVector())
            {
                Vector vector = atom;
                if(vector.size() > 3 && vector[0].isNumber() && vector[1].isNumber() && vector[2].isNumber() && vector[3].isNumber())
                {
                    x(double(vector[0]));
                    y(double(vector[1]));
                    width(double(vector[2]));
                    height(double(vector[3]));
                }
            }
            return *this;
        }
    };
}

#endif