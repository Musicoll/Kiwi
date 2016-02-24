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

#ifndef KIWI_SIZE_H_INCLUDED
#define KIWI_SIZE_H_INCLUDED

#include <KiwiGraphics/KiwiLine.h>

namespace kiwi
{
    namespace graphics
    {
        // ================================================================================ //
        //                                      SIZE                                        //
        // ================================================================================ //
        
        //! The size holds is an unsigned point with a ratio.
        /**
         The size holds is an unsigned point with a ratio that defines
         */
        class Size
        {
        public:
            
            //! Constructor.
            /** The function initializes a size null.
             */
            constexpr inline Size() noexcept :
            m_data{0., 0., 0., 0., 0.} {}
            
            //! Constructor.
            /** The function initializes a point with two double values.
             @param w       The width.
             @param h       The height.
             @param ratio   If the ratio width over height should be respect.
             */
            constexpr inline Size(const double w, const double h, const bool ratio = false) noexcept : m_data{sclip(w), sclip(h), 0., 0., sdiv(ratio, w, h)} {}
            
            
            //! Constructor.
            /** The function initializes a point with two double values.
             @param w       The width.
             @param h       The height.
             @param minw    The minimum width.
             @param minh    The minimum height.
             @param ratio   If the ratio width over height should be respect.
             */
            constexpr inline Size(const double w, const double h, const double minw, const double minh, const bool ratio = false) noexcept :
            m_data{sclip(w, sclip(minw)), sclip(h, sclip(minh)), sclip(minw), sclip(minh), sdiv(ratio, minw, minh)} {}
            
            //! Constructor.
            /** The function initializes a size with another size.
             */
            constexpr inline Size(Size const& size) noexcept : m_data{size.m_data[0], size.m_data[1], size.m_data[2], size.m_data[3], size.m_data[4]} {}
            
            //! Constructor.
            /** The function initializes a point with another point.
             @param size The other point.
             */
            inline Size(Size&& size) noexcept {std::swap(m_data, size.m_data);}
            
            //! Destructor.
            /** The function deletes the point.
             */
            inline ~Size() {};
            
            //! Set the width.
            /** The function sets the width.
             @param w The width.
             */
            inline void width(const double w) noexcept
            {
                m_data[0] = std::fmax(w, minimumWidth());
                if(m_data[4])
                {
                    m_data[1] = m_data[0] * m_data[4];
                }
            }
            
            //! Set the height.
            /** The function sets the height.
             @param h The height.
             */
            inline void height(const double h) noexcept
            {
                if(!m_data[4])
                {
                    m_data[1] = std::fmax(h, minimumHeight());
                }
            }
            
            //! Set the ratio.
            /** The function sets the ratio.
             @param r The ratio.
             */
            inline void ratio(const double r) noexcept
            {
                m_data[4] = std::fmax(r, 0.);
                if(m_data[4])
                {
                    m_data[1] = m_data[0] * m_data[4];
                }
            }
            
            //! Retrieve the width.
            /** The function retrieves the width.
             @return The width.
             */
            inline double width() const noexcept
            {
                return m_data[0];
            }
            
            //! Retrieve the height.
            /** The function retrieves the height.
             @return The height.
             */
            inline double height() const noexcept
            {
                return m_data[1];
            }
            
            //! Retrieve the minimum size.
            /** The function retrieves the minimum size.
             @return The minimum.
             */
            inline Size minimum() const noexcept
            {
                return Size(minimumWidth(), minimumHeight());
            }
            
            //! Retrieve the minimum width.
            /** The function retrieves the minimum width.
             @return The minimum width.
             */
            inline double minimumWidth() const noexcept
            {
                return m_data[2];
            }
            
            //! Retrieve the minimum height.
            /** The function retrieves the minimum height.
             @return The minimum height.
             */
            inline double minimumHeight() const noexcept
            {
                return m_data[3];
            }
            
            //! Retrieve the ratio.
            /** The function retrieves the ratio. If zero, the ratio width over height isn't respected.
             @return The ratio.
             */
            inline double ratio() const noexcept
            {
                return m_data[4];
            }
            
            //! Sets the size with another size.
            /** The function sets the size with another size.
             @param size Another size.
             @return The size.
             */
            inline Size& operator=(Size const& size) noexcept
            {
                memcpy(m_data, size.m_data, sizeof(double) * 5);
                return *this;
            }
            
            //! Sets the size with another size.
            /** The function sets the size with another size.
             @param size Another size.
             @return The size.
             */
            inline Size& operator=(Size&& size) noexcept
            {
                std::swap(m_data, size.m_data);
                return *this;
            }
            
            //! Increments the abscissa and the ordinate with another size.
            /** The function increments the abscissa and the ordinate with another size.
             @param size Another size.
             @return The size.
             */
            inline Size& operator+=(Size const& size) noexcept
            {
                width(m_data[0] + size.width());
                height(m_data[1] + size.height());
                return *this;
            }
            
            //! Increments the abscissa and the ordinate with a value.
            /** The function increments the abscissa and the ordinate with a value.
             @param value The value.
             @return The size.
             */
            inline Size& operator+=(double const value) noexcept
            {
                width(m_data[0] + value);
                height(m_data[1] + value);
                return *this;
            }
            
            //! Decrements the abscissa and the ordinate with another size.
            /** The function decrements the abscissa and the ordinate with another size.
             @param value The value.
             @return The size.
             */
            inline Size& operator-=(Size const& size) noexcept
            {
                width(m_data[0] - size.width());
                height(m_data[1] - size.height());
                return *this;
            }
            
            //! Decrements the abscissa and the ordinate with a value.
            /** The function decrements the abscissa and the ordinate with a value.
             @param size Another size.
             @return The size.
             */
            inline Size& operator-=(double const value) noexcept
            {
                width(m_data[0] - value);
                height(m_data[1] - value);
                return *this;
            }
            
            //! Multiplies the abscissa and the ordinate with another size.
            /** The function multiplies the abscissa and the ordinate with another size.
             @param value The value.
             @return The size.
             */
            inline Size& operator*=(Size const& size) noexcept
            {
                width(m_data[0] * size.width());
                height(m_data[1] * size.height());
                return *this;
            }
            
            //! Multiplies the abscissa and the ordinate with a value.
            /** The function multiplies the abscissa and the ordinate with a value.
             @param size Another size.
             @return The size.
             */
            inline Size& operator*=(double const value) noexcept
            {
                width(m_data[0] * value);
                height(m_data[1] * value);
                return *this;
            }
            
            //! Divides the abscissa and the ordinate with another size.
            /** The function divides the abscissa and the ordinate with another size.
             @param size Another size.
             @return The size.
             */
            inline Size& operator/=(Size const& size) noexcept
            {
                width(m_data[0] / size.width());
                height(m_data[1] / size.height());
                return *this;
            }
            
            //! Divides the abscissa and the ordinate with a value.
            /** The function divides the abscissa and the ordinate with a value.
             @param value The value.
             @return The size.
             */
            inline Size& operator/=(double const value) noexcept
            {
                width(m_data[0] / value);
                height(m_data[1] / value);
                return *this;
            }
            
            //! Retrives a new size from the addition of two sizes.
            /** The function adds a value to the size.
             @param size The size to add.
             @return The new size.
             */
            inline Size operator+(Size const& size) noexcept
            {
                return Size(m_data[0] + size.width(), m_data[1] + size.height());
            }
            
            //! Add a value to the size.
            /** The function adds a value to the size.
             @param value The value to add.
             @return The new size.
             */
            inline Size operator+(double const value) noexcept
            {
                return Size(m_data[0] + value, m_data[1] + value);
            }
            
            //! Subtract a a size to the size.
            /** The function subtract to the size.
             @param size The size to subtract.
             @return The new size.
             */
            inline Size operator-(Size const size) const noexcept
            {
                return Size(m_data[0] - size.width(), m_data[1] - size.height());
            }
            
            //! Subtract a a value to the size.
            /** The function subtracts a value to the size.
             @param value The value to subtract.
             @return The new size.
             */
            inline Size operator-(double const value) const noexcept
            {
                return Size(m_data[0] - value, m_data[1] - value);
            }
            
            
            //! Multiply a size with the size.
            /** The function multiplies a size with the size.
             @param size The size to multiply with.
             @return The new size.
             */
            inline Size operator*(Size const size) const noexcept
            {
                return Size(m_data[0] * size.width(), m_data[1] * size.height());
            }
            
            //! Divide a value with the size.
            /** The function divides a value with the size.
             @param value The value to divide with.
             @return The new size.
             */
            inline Size operator*(const double value) const noexcept
            {
                return Size(width() * value, height() * value);
            }
            
            //! Divide a size with the size.
            /** The function divides a size with the size.
             @param size The size to divide with.
             @return The new size.
             */
            inline Size operator/(Size const size) const noexcept
            {
                return Size(width() / size.width(), height() / size.height());
            }
            
            //! Divide a value with the size.
            /** The function divides a value with the size.
             @param value The value to divide with.
             @return The new size.
             */
            inline Size operator/(const double value) const noexcept
            {
                return Size(width() / value, height() / value);
            }
            
            //! Retieves a point with the size.
            /** The function retrieves a point with the size.
             @return The point.
             */
            inline explicit operator Point() const noexcept
            {
                return Point(width(), height());
            }
            
            //! Get the equality of the size with another.
            /** The function retrieves the equality of the size with another.
             @param size The other size.
             @return true if the two sizes are equal, otherwise false.
             */
            inline bool operator==(Size const& size) const noexcept
            {
                return width() == size.width() && height() == size.height();
            }
            
            //! Get the equality of the size with another.
            /** The function retrieves the equality of the size with another.
             @param size The other size.
             @return true if the two sizes are not equal, otherwise false.
             */
            inline bool operator!=(Size const& size) const noexcept
            {
                return width() != size.width() || height() != size.height();
            }
            
            // ================================================================================ //
            //                                      ATTR                                        //
            // ================================================================================ //
            
            //! Retrieve the size as an atom.
            /** The function retrieves the size as an atom.
             @return The atom.
             */
            inline operator Atom() const noexcept
            {
                return Atom({width(), height()});
            }
            
            //! Set the size with an atom.
            /** The function sets the size with an atom.
             @param atom The atom.
             */
            inline Size& operator=(Atom const& atom) noexcept
            {
                if(atom.isVector())
                {
                    Vector vector = atom;
                    if(vector.size() > 1 && vector[0].isNumber() && vector[1].isNumber())
                    {
                        width(double(vector[0]));
                        height(double(vector[1]));
                    }
                }
                return *this;
            }
            
        private:
            
            //! @internal
            constexpr static inline double sclip(const double v) {return v > 0. ? v : 0.;}
            //! @internal
            constexpr static inline double sclip(const double v, const double m) {return v > m ? v : m;}
            //! @internal
            constexpr static inline double sdiv(bool v, const double v1, const double v2) {return (v && v1 > 0. && v2 > 0.) ? v1 / v2 : 0.;}
            
            double m_data[5];
        };
        
        //! Get the equality between a size and point.
        /** The function retrieves the equality of the size with another.
         @param size The other size.
         @return true if the size and the point are not equal, otherwise false.
         */
        inline bool operator!=(Size const& size, Point const& pt) noexcept
        {
            return size.width() != pt.x() || size.height() != pt.y();
        }
        
        //! Get the equality between a size and point.
        /** The function retrieves the equality of the size with another.
         @param size The other size.
         @return true if the size and the point are not equal, otherwise false.
         */
        inline bool operator!=(Point const& pt, Size const& size) noexcept
        {
            return size.width() != pt.x() || size.height() != pt.y();
        }
        
        //! Get the equality between a size and point.
        /** The function retrieves the equality of the size with another.
         @param size The other size.
         @return true if the size and the point are equal, otherwise false.
         */
        inline bool operator==(Size const& size, Point const& pt) noexcept
        {
            return size.width() == pt.x() && size.height() == pt.y();
        }
        
        //! Get the equality between a size and point.
        /** The function retrieves the equality of the size with another.
         @param size The other size.
         @return true if the size and the point are equal, otherwise false.
         */
        inline bool operator==(Point const& pt, Size const& size) noexcept
        {
            return size.width() == pt.x() && size.height() == pt.y();
        }
    }
}

#endif // KIWI_SIZE_H_INCLUDED
