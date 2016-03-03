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

#ifndef __DEF_KIWI_MODELS_FLIP_TYPES__
#define __DEF_KIWI_MODELS_FLIP_TYPES__

#include "KiwiModel.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  FLIP RGBA COLOR                                 //
    // ================================================================================ //
    
    class FlipRGBA : public flip::Object
    {
    public:
        
        FlipRGBA() = default;

        FlipRGBA(const double red, const double green, const double blue, const double alpha = 1.)
        : m_red(cclip(red)), m_green(cclip(green)), m_blue(cclip(blue)), m_alpha(cclip(alpha)) {}
        
        //! Create a color with a vector of atoms.
        FlipRGBA(std::vector<Atom> const& atoms)
        {
            if(atoms.size() > 2 &&
               atoms[0].isNumber() &&
               atoms[1].isNumber() &&
               atoms[2].isNumber())
            {
                red(cclip(atoms[0].getFloat()));
                green(cclip(atoms[1].getFloat()));
                blue(cclip(atoms[2].getFloat()));
                
                if(atoms.size() >= 4 &&
                   atoms[3].isNumber())
                {
                    alpha(cclip(atoms[3].getFloat()));
                }
            }
        }
        
        //! static flip declare method.
        static void declare()
        {
            Model::declare<FlipRGBA>()
            .name("cicm.kiwi.FlipRGBA")
            .member<flip::Float, &FlipRGBA::m_red>      ("red")
            .member<flip::Float, &FlipRGBA::m_green>    ("green")
            .member<flip::Float, &FlipRGBA::m_blue>     ("blue")
            .member<flip::Float, &FlipRGBA::m_alpha>    ("alpha");
        }
        
        //! Retrieve the FlipRGBA value as a vector of atoms.
        operator std::vector<Atom>() const
        {
            return {red(), green(), blue(), alpha()};
        }
        
        double red() const noexcept                 {return m_red;}
        double green() const noexcept               {return m_green;}
        double blue() const noexcept                {return m_blue;}
        double alpha() const noexcept               {return m_alpha;}
        
        void red(const double _red) noexcept        {m_red = _red;}
        void green(const double _green) noexcept    {m_green = _green;}
        void blue(const double _blue) noexcept      {m_blue = _blue;}
        void alpha(const double _alpha) noexcept    {m_alpha = _alpha;}
        
    private:
        
        //! @internal clip value between 0 and 1
        static constexpr inline double cclip(const double val){return (val > 1.) ? 1. : ((val > 0.) ? val : 0.);}
        
        flip::Float m_red;
        flip::Float m_green;
        flip::Float m_blue;
        flip::Float m_alpha;
    };
    
    // ================================================================================ //
    //                                  FLIP RGBA COLOR                                 //
    // ================================================================================ //
    
    class FlipPoint : public flip::Object
    {
    public:
        FlipPoint() : m_x(0.), m_y(0.) {}
        
        FlipPoint(const double x, const double y) : m_x(x), m_y(y) {}
        
        //! Create a color with a vector of atoms.
        FlipPoint(std::array<Atom, 2> const& atoms)
        {
            if(atoms.size() >= 2 && atoms[0].isNumber() && atoms[1].isNumber())
            {
                m_x = atoms[0].getFloat();
                m_y = atoms[1].getFloat();
            }
        }
        
        // static flip declare method.
        static void declare()
        {
            Model::declare<FlipPoint>()
            .name("cicm.kiwi.FlipPoint")
            .member<flip::Float, &FlipPoint::m_x>("x")
            .member<flip::Float, &FlipPoint::m_y>("y");
        }
        
        //! Retrieve the attribute value as a vector of atoms.
        operator std::array<Atom, 2>() const
        {
            return {{static_cast<double>(m_x), static_cast<double>(m_y)}};
        }
        
    private:
        flip::Float m_x;
        flip::Float m_y;
    };
}

#endif


