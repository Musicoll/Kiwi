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

#include "KiwiColor.h"

namespace kiwi
{
    const string Color::m_hex_digits = string("0123456789ABCDEF");
    
    Color Color::contrasted(const double value) const noexcept
    {
        if(luminance() < 0.5)
        {
            return Color(red() * value, green() * value, blue() * value, alpha());
        }
        else
        {
            Color white(1., 1., 1., value);
            return Color(fmod(red() * value + 1., 1.), fmod(green() * value + 1., 1.), fmod(blue() * value + 1., 1.), alpha());
        }
    }

    Color Color::withHSLA(const double hue, const double saturation, const double lightness, const double alpha) noexcept
    {
        if(saturation == 0.)
        {
            return Color(lightness, lightness, lightness, alpha);
        }
        else
        {
            double var2;
            if(lightness < 0.5)
            {
                var2 = lightness * (1. + saturation);
            }
            else
            {
                var2 = (lightness + saturation) - (saturation * lightness);
            }
            const double var1 = 2. * lightness - var2;
            return Color(hueToRGB(var1, var2, hue + (1. / 3.)), hueToRGB(var1, var2, hue), hueToRGB(var1, var2, hue - (1. / 3.)),alpha);
        }

    }
    
    Color Color::withHexa(string const& color) noexcept
    {
        if(color.size() > 1 && color[0] == '#')
        {
            int hexvalue = atoi(color.c_str()+1);
            return Color(((hexvalue >> 16) & 0xFF) / 255., ((hexvalue >> 8) & 0xFF) / 255., ((hexvalue) & 0xFF) / 255., 1.);
        }
        else
        {
            return Color();
        }
    }
    
    double Color::hue() const noexcept
    {
        const double _max = max(max(red(), green()),  blue());
        if(!_max)
        {
            return 0.;
        }
        else
        {
            const double _min = min(min(red(), green()),  blue());
            const double _delta = _max - _min;
            double hue;
            if(red() == _max)
            {
                const double deltag = (((_max - green()) / 6 ) + (_delta / 2)) / _delta;
                const double deltab = (((_max - blue()) / 6 ) + (_delta / 2)) / _delta;
                hue = deltab - deltag;
            }
            else if(green() == _max)
            {
                const double deltar = (((_max - red()) / 6 ) + (_delta / 2)) / _delta;
                const double deltab = (((_max - blue()) / 6 ) + (_delta / 2)) / _delta;
                hue = (1. / 3.) + deltar - deltab;
            }
            else
            {
                const double deltag = (((_max - green()) / 6 ) + (_delta / 2)) / _delta;
                const double deltar = (((_max - red()) / 6 ) + (_delta / 2)) / _delta;
                hue = (2. / 3.) + deltag - deltar;
            }
            
            if(hue < 0.)
            {
                return hue + 1;
            }
            else if(hue > 1.)
            {
                return hue - 1;
            }
            else
            {
                return hue;
            }
        }
    }
    
    double Color::saturation() const noexcept
    {
        const double _max = max(max(red(), green()),  blue());
        if(!_max)
        {
            return 0.;
        }
        else
        {
            const double _min = min(min(red(), green()),  blue());
            const double _light = (_max + _min) * 0.5;
            if(_light < 0.5)
            {
                return (_max - _min) / (_max + _min);
            }
            else
            {
                return (_max - _min) / (2. - _max - _min);
            }
        }
    }
    
    double Color::lightness() const noexcept
    {
        const double _max = max(max(red(), green()),  blue());
        const double _min = min(min(red(), green()),  blue());
        return (_max + _min) * 0.5;
    }
    
    string Color::hexa() const noexcept
    {
        string color("#");
        int r = red() * 255;
        int g = green() * 255;
        int b = blue() * 255;
        color += m_hex_digits[(r >> 4) & 15];
        color += m_hex_digits[r & 15];
        color += m_hex_digits[(g >> 4) & 15];
        color += m_hex_digits[g & 15];
        color += m_hex_digits[(b >> 4) & 15];
        color += m_hex_digits[b & 15];
        return color;
    }
    
    double Color::hueToRGB(double const v1, double const v2, double vH)
    {
        if(vH < 0.)
            vH += 1.;
        if(vH > 1.)
            vH -= 1.;
        if((6. * vH) < 1.)
            return (v1 + (v2 - v1) * 6. * vH);
        if((2. * vH) < 1.)
            return v2;
        if(( 3 * vH) < 2.)
            return (v1 + (v2 - v1) * ((2. / 3.) - vH) * 6.);
        return v1;
    }
    
    // ================================================================================ //
    //                                      DEFAULTS                                    //
    // ================================================================================ //
    
    
    const Color Colors::black       = Color(0., 0., 0., 1.);
    const Color Colors::white       = Color(1., 1., 1., 1.);
    const Color Colors::red         = Color(1., 0., 0., 1.);
    const Color Colors::green       = Color(0., 1., 0., 1.);
    const Color Colors::blue        = Color(0., 0., 1., 1.);
    const Color Colors::yellow      = Color(1., 1., 0., 1.);
    const Color Colors::transparent = Color(0., 0., 0., 0.);
    const Color Colors::grey        = Color(0.5, 0.5, 0.5, 1.);
}