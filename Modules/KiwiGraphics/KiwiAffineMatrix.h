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

#ifndef __DEF_KIWI_GUI_AFFINEMATRIX__
#define __DEF_KIWI_GUI_AFFINEMATRIX__

#include "../KiwiCore/KiwiCore.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  AFFINE MATRIX                                   //
    // ================================================================================ //
    
    //! The color holds matrix values
    /**
     Represent an affine matrix suitable to do 2D graphical transformations such as translating, scaling, rotating, shearing or reflecting.
     */
    class AffineMatrix
    {
    private:
        double m_matrix[6];
        
    public:
        //! Constructor.
        /** The function initializes an identity matrix.
         */
        constexpr inline AffineMatrix() noexcept : m_matrix{1., 0., 0., 0., 1., 0.} {}
        
        //! Constructor.
        /** The function initializes an affine matrix.
         @param xx The xx component of the affine transformation [0][0].
         @param xy The xy component of the affine transformation [0][1].
         @param x0 The X translation component of the affine transformation [0][2].
         @param yx The yx component of the affine transformation [1][0].
         @param yy The yy component of the affine transformation [1][1].
         @param y0 The Y translation component of the affine transformation [1][2].
         */
        constexpr inline AffineMatrix(const double xx, const double xy, const double x0,
                                      const double yx, const double yy, const double y0) noexcept :
            m_matrix{xx, xy, x0, yx, yy, y0} {}
        
        //! Constructor.
        /** The function initializes an affine matrix with another.
         @param other The other matrix.
         */
        constexpr inline AffineMatrix(AffineMatrix const& other) noexcept :
            m_matrix{other.m_matrix[0], other.m_matrix[1], other.m_matrix[2],
                other.m_matrix[3], other.m_matrix[4], other.m_matrix[5]} {}
        
        //! Constructor.
        /** The function initializes an affine matrix with another.
         @param other The other matrix.
         */
        inline AffineMatrix(AffineMatrix&& other) noexcept {swap(m_matrix, other.m_matrix);}
        
        //! Reset the affine matrix to an identity matrix.
        /** The function reset the affine matrix to an identity matrix.
         */
        void reset() noexcept
        {
            *this = AffineMatrix();
        }
        
        bool isIdentity() const noexcept
        {
            return (m_matrix[0] == 1. && m_matrix[1] == 0. && m_matrix[2] == 0 &&
                    m_matrix[3] == 0. && m_matrix[4] == 1. && m_matrix[5] == 0.);
        }
        
        //! Apply this affine transformation matrix to a point.
        /** The function applies this affine transformation matrix to a point.
         */
        void applyTo(double& x, double& y) const noexcept
        {
            const double old_x = x;
            x = m_matrix[0] * old_x + m_matrix[1] * y + m_matrix[2];
            y = m_matrix[3] * old_x + m_matrix[4] * y + m_matrix[5];
        }
        
        //! Apply this affine transformation matrix to a point.
        /** The function applies this affine transformation matrix to a point.
         */
        template <class PointType> void applyTo(PointType& pt) const noexcept
        {
            const double old_x = pt.x();
            pt.x(m_matrix[0] * old_x + m_matrix[1] * pt.y() + m_matrix[2]);
            pt.y(m_matrix[3] * old_x + m_matrix[4] * pt.y() + m_matrix[5]);
        }
        
        //! Apply this affine transformation matrix to a vector of points.
        /** The function applies this affine transformation matrix to a vector of points.
         */
        template <class PointType> void applyTo(vector<PointType> const& points) const noexcept
        {
            for(auto pt : points)
            {
                const double old_x = pt.x();
                pt.x(m_matrix[0] * old_x + m_matrix[1] * pt.y() + m_matrix[2]);
                pt.y(m_matrix[3] * old_x + m_matrix[4] * pt.y() + m_matrix[5]);
            }
        }
        
        //! Sets the matrix with another.
        /** The function sets the matrix with another.
         @param other The other matrix.
         @return The affine matrix.
         */
        inline AffineMatrix& operator=(AffineMatrix const& other) noexcept
        {
            m_matrix[0] = other.m_matrix[0]; m_matrix[1] = other.m_matrix[1]; m_matrix[2] = other.m_matrix[2];
            m_matrix[3] = other.m_matrix[3]; m_matrix[4] = other.m_matrix[4]; m_matrix[5] = other.m_matrix[5];
            return *this;
        }
        
        //! Sets the matrix with another.
        /** The function sets the matrix with another.
         @param other The other matrix.
         @return The affine matrix.
         */
        inline AffineMatrix& operator=(AffineMatrix&& other) noexcept
        {
            swap(m_matrix, other.m_matrix);
            return *this;
        }
        
        //! Return true if this affine matrix is equal to another.
        /** The function returns true if this affine matrix is equal to another.
         @param other The other matrix.
         @return True if this affine matrix is equal to another, otherwise false.
         */
        bool operator==(AffineMatrix const& other) const noexcept
        {
            return (m_matrix[0] == other.m_matrix[0] && m_matrix[1] == other.m_matrix[1] && m_matrix[2] == other.m_matrix[2] &&
                    m_matrix[3] == other.m_matrix[3] && m_matrix[4] == other.m_matrix[4] && m_matrix[5] == other.m_matrix[5]);
        }
        
        //! Return true if this affine matrix is not equal to another.
        /** The function returns true if this affine matrix is not equal to another.
         @param other The other matrix.
         @return True if this affine matrix is not equal to another, otherwise false.
         */
        bool operator!=(AffineMatrix const& other) const noexcept
        {
            return ! operator== (other);
        }
        
        //! Return a new affine matrix that is the same as this one multiplicated with another.
        /** The function returns a new affine matrix that is the same as this one multiplicated with another.
         @param other The other matrix.
         @return An affine matrix.
         */
        AffineMatrix composedWith(AffineMatrix const& other) const noexcept
        {
            return AffineMatrix(other.m_matrix[0] * m_matrix[0] + other.m_matrix[1] * m_matrix[3],
                                other.m_matrix[0] * m_matrix[1] + other.m_matrix[1] * m_matrix[4],
                                other.m_matrix[0] * m_matrix[2] + other.m_matrix[1] * m_matrix[5] + other.m_matrix[2],
                                other.m_matrix[3] * m_matrix[0] + other.m_matrix[4] * m_matrix[3],
                                other.m_matrix[3] * m_matrix[1] + other.m_matrix[4] * m_matrix[4],
                                other.m_matrix[3] * m_matrix[2] + other.m_matrix[4] * m_matrix[5] + other.m_matrix[5]);
        }
        
        //! Return an affine matrix that represent a translation.
        /** The function returns an affine matrix that represent a translation.
         @param x The abscissa translation value.
         @param y The ordinate translation value.
         @return An affine matrix.
         */
        static inline AffineMatrix translation(const double x, const double y) noexcept
        {
            return AffineMatrix(1., 0., x,
                                0., 1., y);
        }
        
        //! Return a new affine matrix that is a translated version of this one.
        /** The function returns a new affine matrix that is a translated version of this one.
         @param x The abscissa delta translation value.
         @param y The ordinate delta translation value.
         @return An affine matrix.
         */
        inline AffineMatrix translated(const double x, const double y) const noexcept
        {
            return AffineMatrix(m_matrix[0], m_matrix[1], m_matrix[2] + x,
                                m_matrix[3], m_matrix[4], m_matrix[5] + y);
        }
        
        //! Return a new affine matrix that is the same as this one but with new translation values.
        /** The function returns a new affine matrix that is the same as this one but with new translation values.
         @param x The new abscissa translation value.
         @param y The new ordinate translation value.
         @return An affine matrix.
         */
        inline AffineMatrix withTranslation(const double x, const double y) const noexcept
        {
            return AffineMatrix(m_matrix[0], m_matrix[1], x,
                                m_matrix[3], m_matrix[4], y);
        }
        
        //! Return an affine matrix that represent a scale transformation.
        /** The function returns an affine matrix that represent a scale transformation.
         @param x The abscissa scale value.
         @param y The ordinate scale value.
         @return An affine matrix.
         */
        static inline AffineMatrix scale(const double x, const double y) noexcept
        {
            return AffineMatrix(x, 0., 0.,
                                0., y, 0.);
        }
        
        //! Return a new affine matrix that is a re-scaled version of this one.
        /** The function returns a new affine matrix that is a re-scaled version of this one.
         @param x The abscissa scale value.
         @param y The ordinate scale value.
         @return An affine matrix.
         */
        inline AffineMatrix scaled(const double x, const double y) const noexcept
        {
            return AffineMatrix(m_matrix[0] * x, m_matrix[1], m_matrix[2],
                                m_matrix[3], m_matrix[4] * y, m_matrix[5]);
        }
        
        //! Return an affine matrix that represent a rotation transformation.
        /** The function returns an affine matrix that represent an anti-clockwise rotation transformation, centred about the origin.
         @param radian The angle of the rotation in radian
         @return An affine matrix.
         */
        static inline AffineMatrix rotation(const double radian) noexcept
        {
            const double cosRad = cos(radian);
            const double sinRad = sin(radian);
            
            return AffineMatrix(cosRad,  sinRad, 0.,
                                -sinRad, cosRad, 0.);
        }
        
        //! Return a new affine matrix that is a rotated version of this one.
        /** The function returns a new affine matrix that is a rotated version of this one.
         @param radian The angle of the anti-clockwise rotation in radian
         @return An affine matrix.
         */
        inline AffineMatrix rotated(const double radian) const noexcept
        {
            const double cosRad = cos(radian);
            const double sinRad = sin(radian);
            
            return AffineMatrix(m_matrix[0] * cosRad - m_matrix[3] * sinRad,
                                m_matrix[1] * cosRad - m_matrix[4] * sinRad,
                                m_matrix[2] * cosRad - m_matrix[5] * sinRad,
                                m_matrix[0] * sinRad + m_matrix[3] * cosRad,
                                m_matrix[1] * sinRad + m_matrix[4] * cosRad,
                                m_matrix[2] * sinRad + m_matrix[5] * cosRad);
        }
        
        //! Return an affine matrix that represent a shear transformation.
        /** The function returns an affine matrix that represent a shear transformation.
         @param x The abscissa direction shear value.
         @param y The ordinate direction shear value.
         @return An affine matrix.
         */
        static inline AffineMatrix shear(const double x, const double y) noexcept
        {
            return AffineMatrix(1., x, 0.,
                                y , 1., 0.);
        }
        
        //! Return a new affine matrix that is a re-sheared version of this one.
        /** The function returns a new affine matrix that is a re-sheared version of this one.
         @param x The abscissa direction shear value.
         @param y The ordinate direction shear value.
         @return An affine matrix.
         */
        inline AffineMatrix sheared(const double x, const double y) const noexcept
        {
            return AffineMatrix(m_matrix[0] + x * m_matrix[3], m_matrix[1] + x * m_matrix[4], m_matrix[2] + x * m_matrix[5],
                                m_matrix[3] + y * m_matrix[0], m_matrix[4] + y * m_matrix[1], m_matrix[5] + y * m_matrix[2]);
        }
        
        //! Return an affine matrix that represent a reflection about origin transformation.
        /** The function returns an affine matrix that represent a reflection about origin transformation.
         @return An affine matrix.
         */
        static inline AffineMatrix reflection() noexcept
        {
            return AffineMatrix(1., 0.,  0,
                                0., -1., 0);
        }
        
        //! Return an affine matrix that represent a reflection about x-axis transformation.
        /** The function returns an affine matrix that represent a reflection about x-axis transformation.
         @return An affine matrix.
         */
        static inline AffineMatrix reflectionX() noexcept
        {
            return AffineMatrix(1., 0.,  0,
                                0., -1., 0);
        }
        
        //! Return an affine matrix that represent a reflection about y-axis transformation.
        /** The function returns an affine matrix that represent a reflection about y-axis transformation.
         @return An affine matrix.
         */
        static inline AffineMatrix reflectionY() noexcept
        {
            return AffineMatrix(-1., 0.,  0,
                                0., 1., 0);
        }
    };
}

#endif