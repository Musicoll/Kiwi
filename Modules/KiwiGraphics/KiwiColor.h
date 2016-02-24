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

#ifndef KIWI_COLOR_H_INCLUDED
#define KIWI_COLOR_H_INCLUDED

#include <KiwiGraphics/KiwiPath.h>

namespace kiwi
{
    namespace graphics
    {
        // ================================================================================ //
        //                                      COLOR                                       //
        // ================================================================================ //
        
        //! The color is used to represent a color in the rgba format (red, green, blue and alpha).
        /**
         The color is used to represent a color in the rgba format then, you should use the hsla methods with parsimony.
         */
        class Color
        {
        public:
            //! Constructor.
            /** The function initialize a default black color.
             */
            constexpr inline Color() noexcept :
            m_rgba{0., 0., 0., 1}, m_mode(RGBA) {}
            
            //! Constructor.
            /** The function initialize a color with rgba values.
             @param red The red value.
             @param green The green value.
             @param blue The blue value.
             @param alpha The alpha value.
             */
            constexpr inline Color(const double red, const double green, const double blue, const double alpha = 1.) noexcept :
            m_rgba{cclip(red), cclip(green), cclip(blue), cclip(alpha)}, m_mode(RGBA) {}
            
            //! Constructor.
            /** The function initialize a color with another.
             @param other The other color.
             */
            constexpr inline Color(Color const& other) noexcept :
            m_rgba{other.m_rgba[0], other.m_rgba[1], other.m_rgba[2], other.m_rgba[3]}, m_mode(other.m_mode) {}
            
            //! Constructor.
            /** The function initialize a color with another.
             @param other The other color.
             */
            inline Color(Color&& other) noexcept : m_mode(other.m_mode) {std::swap(m_rgba, other.m_rgba);}
            
            //! Destructor.
            /** The function deletes the color.
             */
            inline ~Color() noexcept {}
            
            //! Retrieve a color with hsla values.
            /** The function retrieves the a color with hsla values.
             @param hue The hue value.
             @param saturation The saturation value.
             @param lightness The lightness value.
             @param alpha The alpha value.
             @return The color.
             */
            static Color withHSLA(const double hue, const double saturation, const double lightness, const double alpha = 1.) noexcept;
            
            //! Retrieve a color with hexadecimal.
            /** The function retrieves the a color with hexadecimal.
             @param color The color in hexadecimal.
             @return The color.
             */
            static Color withHexa(std::string const& color) noexcept;
            
            //! Retrieve a color from the interpolation of two colors.
            /** The function retrieves the a color from the interpolation of two colors.
             @param color1 The first color.
             @param color2 The second color.
             @param factor The interpolation factor (between 0. and 1.).
             @return The color.
             */
            static Color interpolate(const Color& color1, const Color& color2, const double factor)
            {
                return Color(lerp(color1.red(),   color2.red(),   factor),
                             lerp(color1.green(), color2.green(), factor),
                             lerp(color1.blue(),  color2.blue(),  factor),
                             lerp(color1.alpha(), color2.alpha(), factor));
            }
            
            //! Retrieve a brighter color.
            /** The function retrieves a the brighter color.
             @param value The amount of brightness.
             @return The new color.
             */
            inline Color brighter(const double value) const noexcept
            {
                return Color(red() + value, green() + value, blue() + value, alpha());
            }
            
            //! Retrieve a darker color.
            /** The function retrieves a the darker color.
             @param value The amount of darkness.
             @return The new color.
             */
            inline Color darker(const double value) const noexcept
            {
                return Color(red() - value, green() - value, blue() - value, alpha());
            }
            
            //! Retrieve a contrasted color.
            /** The function retrieves a the contrasted color.
             @param value The amount of contrast.
             @return The new color.
             */
            Color contrasted(const double value) const noexcept;
            
            //! Retrieve the same color with a different alpha.
            /** The function retrieves the same color with a different alpha.
             @param alpha The alpha.
             @return The new color.
             */
            inline Color withAlpha(const double value) const noexcept
            {
                return Color(red(), green(), blue(), clip(value, 0., 1.));
            }
            
            //! Retrieve a different color with the same alpha.
            /** The function retrieves a different color with the same alpha.
             @param red The red value.
             @param green The green value.
             @param blue The blue value.
             @return The new color.
             */
            inline Color withRGB(const double red, const double green, const double blue) const noexcept
            {
                return Color(red, green, blue, alpha());
            }
            
            //! Retrieve the red value.
            /** The function retrieves the red value.
             @return The red value.
             */
            inline double red() const noexcept
            {
                return m_rgba[0];
            }
            
            //! Retrieve the green value.
            /** The function retrieves the green value.
             @return The green value.
             */
            inline double green() const noexcept
            {
                return m_rgba[1];
            }
            
            //! Retrieve the blue value.
            /** The function retrieves the blue value.
             @return The blue value.
             */
            inline double blue() const noexcept
            {
                return m_rgba[2];
            }
            
            //! Retrieve the alpha value.
            /** The function retrieves the alpha value.
             @return The alpha value.
             */
            inline double alpha() const noexcept
            {
                return m_rgba[3];
            }
            
            //! Retrieve the hue value.
            /** The function retrieves the hue value. Please use this method with parsimony.
             @return The hue value.
             */
            double hue() const noexcept;
            
            //! Retrieve the saturation value.
            /** The function retrieves the saturation value. Please use this method with parsimony.
             @return The saturation value.
             */
            double saturation() const noexcept;
            
            //! Retrieve the lightness value.
            /** The function retrieves the lightness value. Please use this method with parsimony.
             @return The lightness value.
             */
            double lightness() const noexcept;
            
            //! Retrieve the hexadecimal value.
            /** The function retrieves the hexadecimal value.
             @return The hexadecimal value.
             */
            std::string hexa() const noexcept;
            
            //! Retrieve the luminance of the color.
            /** The function retrieves the luminance value.
             @return The luminance value.
             */
            inline double luminance() const noexcept
            {
                return 1. - (0.299 * red() + 0.587 * green() + 0.114 * blue());
            }
            
            //! Set the red value.
            /** The function set the red value.
             @param value The red value.
             */
            inline void red(const double value) noexcept
            {
                m_rgba[0] = clip(value, 0., 1.);
            }
            
            //! Set the green value.
            /** The function set the green value.
             @param value The green value.
             */
            inline void green(const double value) noexcept
            {
                m_rgba[1] = clip(value, 0., 1.);
            }
            
            //! Set the blue value.
            /** The function set the blue value.
             @param value The blue value.
             */
            inline void blue(const double value) noexcept
            {
                m_rgba[2] = clip(value, 0., 1.);
            }
            
            //! Set the alpha value.
            /** The function set the alpha value.
             @param value The alpha value.
             */
            inline void alpha(const double value) noexcept
            {
                m_rgba[3] = clip(value, 0., 1.);
            }
            
            //! Set the hue value.
            /** The function set the hue value. Please use this method with parsimony.
             @param value The hue value.
             */
            inline void hue(const double value) noexcept
            {
                *this = withHSLA(value, saturation(), lightness(), alpha());
            }
            
            //! Set the saturation value.
            /** The function set the saturation value. Please use this method with parsimony.
             @param value The saturation value.
             */
            inline void saturation(const double value) noexcept
            {
                *this = withHSLA(hue(), value, lightness(), alpha());
            }
            
            //! Set the lightness value.
            /** The function set the lightness value. Please use this method with parsimony.
             @param value The lightness value.
             */
            inline void lightness(const double value) noexcept
            {
                *this = withHSLA(hue(), saturation(), value, alpha());
            }
            
            //! Set the color with another.
            /** The function sets the color with another.
             @param other The other color.
             */
            inline Color& operator=(Color const& other) noexcept
            {
                m_rgba = other.m_rgba;
                m_mode = other.m_mode;
                return *this;
            }
            
            //! Set the color with another.
            /** The function sets the color with another.
             @param other The other color.
             */
            inline Color& operator=(Color&& other) noexcept
            {
                std::swap(m_rgba, other.m_rgba);
                m_mode = other.m_mode;
                return *this;
            }
            
            //! Compare the color with another.
            /** The function compare the color with another.
             @param other The other color.
             @return true is the colors are not equals, otherwise false.
             */
            inline bool operator!=(Color const& other) const noexcept
            {
                return red() != other.red() || green() != other.green() ||  blue() != other.blue() ||  alpha() != other.alpha();
            }
            
            //! Compare the color with another.
            /** The function compare the color with another.
             @param other The other color.
             @return true is the colors are equals, otherwise false.
             */
            inline bool operator==(Color const& other) const noexcept
            {
                return red() == other.red() && green() == other.green() && blue() == other.blue() && alpha() == other.alpha();
            }
            
            // ================================================================================ //
            //                                      ATTR                                        //
            // ================================================================================ //
            
            //! Retrieve the color as an atom.
            /** The function retrieves the color as an atom.
             @return The atom.
             */
            inline operator Atom() const noexcept
            {
                return Atom({red(), green(), blue(), alpha()});
            }
            
            //! Set the color with an atom.
            /** The function sets the color with an atom.
             @param atom The atom.
             */
            inline Color& operator=(Atom const& atom) noexcept
            {
                if(atom.isVector())
                {
                    Vector vector = atom;
                    if(vector.size() > 2 && vector[0].isNumber() && vector[1].isNumber() && vector[2].isNumber())
                    {
                        red((double)vector[0]);
                        green((double)vector[1]);
                        blue((double)vector[2]);
                        if(vector.size() > 3 && vector[3].isNumber())
                        {
                            alpha((double)vector[3]);
                        }
                    }
                }
                return *this;
            }
            
        private:
            
            //! @internal clip value between 0 and 1.
            static constexpr inline double cclip(const double val){return (val > 1.) ? 1. : ((val > 0.) ? val : 0.);}
            
            //! @internal convert hue value to rgb.
            static double hueToRGB(double const v1, double const v2, double vH);
            
            //! @internal Color Mode
            enum Mode
            {
                RGBA = 0,
                HSLA = 1
            };
            
            std::array<double, 4>       m_rgba;
            bool                        m_mode;
            static const std::string    m_hex_digits;
        };
        
        // ================================================================================ //
        //                                      DEFAULTS                                    //
        // ================================================================================ //
        
        class Colors
        {
        public:
            
            static const Color black;
            static const Color white;
            static const Color red;
            static const Color green;
            static const Color blue;
            static const Color yellow;
            static const Color transparent;
            static const Color grey;
        };
    }
}

#endif // KIWI_COLOR_H_INCLUDED
