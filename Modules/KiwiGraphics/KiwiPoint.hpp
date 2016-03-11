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

#ifndef KIWI_GRAPHICS_POINT_HPP_INCLUDED
#define KIWI_GRAPHICS_POINT_HPP_INCLUDED

#include <math.h>

namespace kiwi
{
    namespace graphics
    {
        // ================================================================================ //
        //                                      POINT                                       //
        // ================================================================================ //
        
        //!@todo create a class angle and define constants like pi or angle precision
        class Point final
        {
        public:
            
            //! @brief Default Constructor
            //! @details Initializes with null coordinates
            constexpr Point() noexcept : m_data{0., 0.}{}
            
            //! @brief Constructor
            //! @details Initializes the coordinates
            explicit constexpr Point(const double x, const double y) noexcept : m_data{x, y}{}
            
            //! @brief Copy Constrctor
            //! @details Initializes with same coordinates
            constexpr Point(Point const& pt) noexcept : m_data{pt.m_data[0], pt.m_data[1]}{}
            
            //! @brief Desctructor
            inline ~Point() noexcept = default;
            
            //! @brief Gets the abscissa
            constexpr double x() const noexcept;
            
            // @brief Gets the ordinate
            constexpr double y() const noexcept;
            
            //! @brief Sets the abscissa
            inline void x(const double x) noexcept;
            
            //! @brief Sets the ordinate
            inline void y(const double y) noexcept;
            
            //! @brief Copies the coordinate of another point
            inline Point& operator=(Point const& pt) noexcept;
            
            //! @brief Retrieves a copy of the rotated point around the origin
            //! @return A copy of the rotated Point
            Point rotated(double const angle) const noexcept;
            
            //! @brief Retrieves a copy of the rotated point around another pt
            //! @param pt The rotation center
            //! @return A copy of the rotated point around another pt
            Point rotated(Point const& pt, double const angle) const noexcept;
            
            //! @brief Applies a rotation around the origin
            void rotate(double const angle) noexcept;
            
            //! @brief Applies a rotation arount pt
            //! @param pt The rotation center
            void rotate(Point const& pt, double const angle) noexcept;
            
            //! @brief Retrieves the angle in radian from abscissa clockwise
            //! @return Angle in radian
            double angle() const noexcept;
            
            //! @brief Retrieves the angle to another point
            //! @details Angle of the vector between the two points
            double angleToPoint(Point const& pt) const noexcept;
            
            //! @brief Returns the scalar product of two points
            constexpr double dot(Point const& pt) const noexcept;
            
        private:
            double m_data[2];
        };
        
        //! @brief Retrun true if the distance between the points is less or equal to eps
        bool areNear(Point const& l_pt, Point const& r_pt, const double eps) noexcept;
        
        //! @brief Retrieves the distance from the origin
        double norm(Point const& pt) noexcept;
        
        //! @brief Returns the distance between two points
        double distance(Point const &lPt, Point const& rPt) noexcept;
        
        //! @brief Multiplies point with scalar value
        //! @details Returns a point with both coordinates multiplied by value
        constexpr Point operator*(const double value, Point const& pt) noexcept;
        
        //! @brief Multiplies point with scalar value
        //! @details Returns a point with both coordinates multiplied by value
        constexpr Point operator*(Point const& pt, double const value) noexcept;
        
        //! @brief Mutltiplies the coordinates by value
        Point& operator*=(Point &pt, double const value) noexcept;
        
        //! @brief Divides point with scalar value
        //! @details Returns a point with both coordinates divided by value
        constexpr Point operator/(Point const& pt, const double value);
        
        //! @brief Divides the abscissa and the ordinate with a value.
        Point& operator/=(Point & pt, double const value);
        
        //! @brief Retrieves a point with the added ccordinates of lPt and rPt
        //! @details l_pt + r_pt
        constexpr Point operator+(Point const& lPt, Point const & rPt) noexcept;
        
        //! @brief Increments the coordinates with another point's coordinates
        Point& operator+=(Point &lPt, Point const& rPt) noexcept;
        
        //! @brief Retrieves a point with the substraction of lPt and rPt coordinates
        //! @details l_pt - r_pt
        constexpr Point operator-(Point const& lPt, Point const& rPt) noexcept;
        
        //! @brief Decrements the coordinates with another point's coordinates
        Point& operator-=(Point &lPt, Point const& rPt) noexcept;
        
        //! @brief Returns a point with -abscissa and -ordinate
        constexpr Point operator-(Point const& pt) noexcept;
        
        // =================================
        // Implementation
        // =================================
        
        constexpr double Point::x() const noexcept
        {
            return m_data[0];
        }
        
        constexpr double Point::y() const noexcept
        {
            return m_data[1];
        }
        
        inline void Point::x(const double x) noexcept
        {
            m_data[0] = x;
        }
        
        inline void Point::y(const double y) noexcept
        {
            m_data[1] = y;
        }
        
        inline Point& Point::operator=(Point const& pt) noexcept
        {
            if (&pt != this)
            {
                m_data[0] = pt.m_data[0];
                m_data[1] = pt.m_data[1];
            }
            return *this;
        }
        
        constexpr double Point::dot(Point const& pt) const noexcept
        {
            return x() * pt.x() + y() * pt.y();
        }
        
        constexpr Point operator*(Point const& pt, double const value) noexcept
        {
            return value * pt;
        }
        
        constexpr Point operator*(const double value, Point const& pt) noexcept
        {
            return Point(value * pt.x(),
                         value * pt.y());
        };
        
        constexpr Point operator-(Point const& lPt, Point const& rPt) noexcept
        {
            return Point(lPt.x() - rPt.x(),
                         lPt.y() - rPt.y());
        }
        
        constexpr Point operator-(Point const& pt) noexcept
        {
            return Point(-pt.x(), -pt.y());
        };
        
        constexpr Point operator+(Point const& lPt, Point const & rPt) noexcept
        {
            return Point(lPt.x() + rPt.x(),
                         lPt.y() + rPt.y());
        };
        
        constexpr Point operator/(Point const& pt, const double value)
        {
            return Point(pt.x() / value,
                         pt.y() / value);
        };
    }
}

#endif // KIWI_GRAPHICS_POINT_HPP_INCLUDED
