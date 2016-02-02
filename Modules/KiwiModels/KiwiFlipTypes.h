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

#include "KiwiModel.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      FLIP TAG                                    //
    // ================================================================================ //
    
    //! The tag is an unique object that matchs to a "unique" string in the scope of all the kiwi applications.
    class FlipTag : public flip::Object
    {
    public:
        
        FlipTag() = default;
        
        //! The constructor.
        inline explicit FlipTag(string const& name) : m_tag(name) {}
        
        //! The constructor.
        inline explicit FlipTag(sTag tag) : m_tag(tag->getName()) {}
        
        inline explicit FlipTag(Atom const& atom)
        {
            sTag tag = atom.isTag() ? (sTag)atom : Tags::_empty;
            m_tag = tag->getName();
        }
        
        //! The destructor.
        inline ~FlipTag() noexcept {}
        
        static void declare()
        {
            Model::declare<FlipTag>()
            .name("cicm.kiwi.FlipTag")
            .member<flip::String, &FlipTag::m_tag>("tag");
        }
        
        operator sTag () const { return Tag::create(m_tag); }
        
        //! Retrieve the string of the tag.
        inline string getName() const noexcept { return m_tag; }
        
        //! The constructor.
        inline FlipTag& operator =(sTag tag)
        {
            m_tag = tag->getName();
            return *this;
        }
        
        //! Retrieve the attribute value as a vector of atoms.
        //operator Atom() const {return Atom(Tag::create(m_tag));}
        
    private:
        flip::String m_tag;
    };
    
    // ================================================================================ //
    //                                  FLIP RGBA COLOR                                 //
    // ================================================================================ //
    
    class FlipRGBA : public flip::Object
    {
    private:
        static constexpr inline double cclip(const double val){return (val > 1.) ? 1. : ((val > 0.) ? val : 0.);}
        
        flip::Float m_red;
        flip::Float m_green;
        flip::Float m_blue;
        flip::Float m_alpha;
        
    public:
        
        FlipRGBA() = default;

        FlipRGBA(const double red, const double green, const double blue, const double alpha = 1.)
        : m_red(cclip(red)), m_green(cclip(green)), m_blue(cclip(blue)), m_alpha(cclip(alpha)) {}
        
        FlipRGBA(const Color& color)
        : m_red(cclip(color.red())), m_green(cclip(color.green())), m_blue(cclip(color.blue())), m_alpha(cclip(color.alpha())) {}
        
        //! Create a color with a vector of atoms.
        FlipRGBA(const Atom& atom)
        {
            if(atom.isVector())
            {
                Vector atoms = atom;
                if(atoms.size() >= 4)
                {
                    red(cclip(atoms[0]));
                    green(cclip(atoms[1]));
                    blue(cclip(atoms[2]));
                    alpha(cclip(atoms[3]));
                }
            }
        }
        
        // static flip declare method.
        static void declare()
        {
            Model::declare<FlipRGBA>()
            .name("cicm.kiwi.FlipRGBA")
            .member<flip::Float, &FlipRGBA::m_red>("red")
            .member<flip::Float, &FlipRGBA::m_green>("green")
            .member<flip::Float, &FlipRGBA::m_blue>("blue")
            .member<flip::Float, &FlipRGBA::m_alpha>("alpha");
        }
        
        //! Retrieve the attribute value as a vector of atoms.
        operator Atom() const
        {
            return Atom {red(), green(), blue(), alpha()};
        }
        
        //! Retrieve the attribute value as a vector of atoms.
        operator Color() const
        {
            return Color(red(), green(), blue(), alpha());
        }
        
        double red() const noexcept                 {return m_red;}
        double green() const noexcept               {return m_green;}
        double blue() const noexcept                {return m_blue;}
        double alpha() const noexcept               {return m_alpha;}
        
        void red(const double _red) noexcept        {m_red = _red;}
        void green(const double _green) noexcept    {m_green = _green;}
        void blue(const double _blue) noexcept      {m_blue = _blue;}
        void alpha(const double _alpha) noexcept    {m_alpha = _alpha;}
    };
    
    // ================================================================================ //
    //                                  FLIP RGBA COLOR                                 //
    // ================================================================================ //
    
    class FlipPoint : public flip::Object
    {
    private:
        flip::Float m_x;
        flip::Float m_y;
        
    public:
        FlipPoint() : m_x(0.), m_y(0.) {}
        
        FlipPoint(const double x, const double y) : m_x(0.), m_y(0.) {}
        
        FlipPoint(const Point& pt) : m_x(pt.x()), m_y(pt.y()) {}
        
        //! Create a color with a vector of atoms.
        FlipPoint(const Atom& atom)
        {
            if(atom.isVector())
            {
                Vector atoms = atom;
                if(atoms.size() >= 2 && atoms[0].isNumber() && atoms[1].isNumber())
                {
                    m_x = static_cast<double>(atoms[0]);
                    m_y = static_cast<double>(atoms[1]);
                }
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
        operator Atom() const { return Point(m_x, m_y); }
        
        //! Retrieve the attribute value as a vector of atoms.
        operator Point() const { return Point(m_x, m_y); }

    };
    
    // ================================================================================ //
    //                                  FLIP RGBA COLOR                                 //
    // ================================================================================ //
    
    template<class T>
    class FlipArray : public flip::Object
    {
    private:
        flip::Array<T> m_data;
        
    public:
        FlipArray() = default;
        
        FlipArray(initializer_list<T> elements)
        {
            m_data.clear();
            for(auto it = elements.begin(); it != elements.end(); it++)
            {
                m_data.insert(m_data.end(), *it);
            }
        }
        
        //! Create an array with a vector of atoms.

        FlipArray(const Atom& atom)
        {
            m_data.clear();
            if(atom.isVector())
            {
                Vector atoms = atom;
                for(ulong i = 0; i < atoms.size(); i++)
                {
                    //m_data.insert(m_data.end(), dynamic_cast<T&>(atoms[i]));
                    
                    Atom a = atoms[i];
                    //T at = a;
                    //const T tag = static_cast<T&>(atoms[i]);
                    //m_data[i] = static_cast<T&>(atoms[i]);
                }
            }
        }
        
        //! Retrieve the attribute value as a vector of atoms.
        operator Atom() const
        {
            Vector atoms;
            
            for(auto it = m_data.begin(); it != m_data.end(); it++)
            {
                //atoms.push_back(Atom(*it));
            }
            
            return atoms;
        }
        
        // static flip declare method.
        static void declare()
        {
            Model::declare<FlipArray>()
            .name("cicm.kiwi.FlipArray")
            .template member<flip::Array<T>, &FlipArray::m_data>("data");
        }
        
        /*
        
        //! Create a color with a vector of atoms.
        FlipPoint(const Atom& atom)
        {
            if(atom.isVector())
            {
                Vector atoms = atom;
                if(atoms.size() >= 2 && atoms[0].isNumber() && atoms[1].isNumber())
                {
                    m_x = static_cast<double>(atoms[0]);
                    m_y = static_cast<double>(atoms[1]);
                }
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
        operator Atom() const { return Point(m_x, m_y); }
        
        //! Retrieve the attribute value as a vector of atoms.
        operator Point() const { return Point(m_x, m_y); }
        */
    };
}

#endif


