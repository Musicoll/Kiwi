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

#include "KiwiObject.hpp"
#include "KiwiPatcher.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      OBJECT                                      //
    // ================================================================================ //
    
    Object::Object(std::string const& name, std::string const& text, const int64_t id) :
        m_name(name),
        m_text(text),
        m_id(id)
    {
        /*
        createFlipAttr(Tags::position,              "Position",                 "Appearance", flip::Array<flip::String>("", ""));
        createFlipAttr(Tags::position,              "Position",                 "Appearance", Point(0., 0.));
        createFlipAttr(Tags::size,                  "Size",                     "Appearance", Size(10., 10.));
        createFlipAttr(Tags::presentation_position, "Presentation Position",    "Appearance", Point(0., 0.));
        createFlipAttr(Tags::presentation_size,     "Presentation Size",        "Appearance", Size(10., 10.));
        */
        //createFlipAttr(Tags::position, "Position", "Appearance", FlipPoint(0., 0.));
        //createFlipAttr(Tags::presentation_position, "Presentation Position", "Appearance", FlipPoint(0., 0.));
        /*
        createFlipAttr(Tags::hidden,        "Hide on Lock",             "Appearance", flip::Bool(false));
        createFlipAttr(Tags::presentation,  "Include in presentation",  "Appearance", flip::Bool(false));
        createFlipAttr(Tags::ignoreclick,   "Ignore Click",             "Behavior",   flip::Bool(false));
        */
    }
    
    Object::Object(const Object& rhs) noexcept :
    m_name(rhs.m_name),
    m_text(rhs.m_text),
    m_id(rhs.m_id)
    {
        //m_attributes = rhs.m_attributes;
    }
    
    Object::~Object() noexcept
    {
        ;
    }
}


