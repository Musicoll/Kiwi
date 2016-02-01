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


#include "KiwiTag.h"

namespace Kiwi
{    
    map<string, sTag> Tag::m_tags;
    mutex Tag::m_mutex;
    
    const sTag Tags::_empty                = Tag::create("");
    const sTag Tags::arguments             = Tag::create("arguments");
    const sTag Tags::Arial                 = Tag::create("Arial");
    
    const sTag Tags::bang                  = Tag::create("bang");
    const sTag Tags::bdcolor               = Tag::create("bdcolor");
    const sTag Tags::bgcolor               = Tag::create("bgcolor");
    const sTag Tags::bold                  = Tag::create("bold");
    const sTag Tags::bold_italic           = Tag::create("bold italic");
    
    const sTag Tags::center                = Tag::create("center");
    const sTag Tags::color                 = Tag::create("color");
    const sTag Tags::Color                 = Tag::create("Color");
    const sTag Tags::command               = Tag::create("command");
    const sTag Tags::circlecolor           = Tag::create("circlecolor");
    
    const sTag Tags::dsp                   = Tag::create("dsp");
    
    const sTag Tags::from                  = Tag::create("from");
    const sTag Tags::focus                 = Tag::create("focus");
    const sTag Tags::font                  = Tag::create("font");
    const sTag Tags::Font                  = Tag::create("Font");
    const sTag Tags::Font_Face             = Tag::create("Font Face");
    const sTag Tags::Font_Justification    = Tag::create("Font Justification");
    const sTag Tags::Font_Name             = Tag::create("Font Name");
    const sTag Tags::Font_Size             = Tag::create("Font Size");
    const sTag Tags::fontface              = Tag::create("fontface");
    const sTag Tags::fontjustification     = Tag::create("fontjustification");
    const sTag Tags::fontname              = Tag::create("fontname");
    const sTag Tags::fontsize              = Tag::create("fontsize");
    
    const sTag Tags::gridsize              = Tag::create("gridsize");
    const sTag Tags::hidden                = Tag::create("hidden");
    
    const sTag Tags::id                    = Tag::create("id");
    const sTag Tags::ignoreclick           = Tag::create("ignoreclick");
    const sTag Tags::italic                = Tag::create("italic");
    
    const sTag Tags::ledcolor              = Tag::create("ledcolor");
    const sTag Tags::left                  = Tag::create("left");
    const sTag Tags::link                  = Tag::create("link");
    const sTag Tags::links                 = Tag::create("links");
    const sTag Tags::locked_bgcolor        = Tag::create("locked_bgcolor");
    
    const sTag Tags::Menelo                = Tag::create("Menelo");
    const sTag Tags::mescolor              = Tag::create("mescolor");
    const sTag Tags::Message_Color         = Tag::create("Message Color");
    
    const sTag Tags::name                  = Tag::create("name");
    const sTag Tags::newlink               = Tag::create("newlink");
    const sTag Tags::newobject             = Tag::create("newobject");
    const sTag Tags::ninlets               = Tag::create("ninlets");
    const sTag Tags::normal                = Tag::create("normal");
    const sTag Tags::noutlets              = Tag::create("noutlets");
    
    const sTag Tags::object                = Tag::create("object");
    const sTag Tags::objects               = Tag::create("objects");
    
    const sTag Tags::patcher               = Tag::create("patcher");
    const sTag Tags::position              = Tag::create("position");
    const sTag Tags::presentation          = Tag::create("presentation");
    const sTag Tags::presentation_position = Tag::create("presentation_position");
    const sTag Tags::presentation_size     = Tag::create("presentation_size");
    
    const sTag Tags::removelink            = Tag::create("removelink");
    const sTag Tags::removeobject          = Tag::create("removeobject");
    const sTag Tags::right                 = Tag::create("right");
    
    const sTag Tags::set                   = Tag::create("set");
    const sTag Tags::sigcolor              = Tag::create("sigcolor");
    const sTag Tags::Signal_Color          = Tag::create("Signal Color");
    const sTag Tags::size                  = Tag::create("size");
    
    const sTag Tags::text                  = Tag::create("text");
    const sTag Tags::textcolor             = Tag::create("textcolor");
    const sTag Tags::to                    = Tag::create("to");
    
    const sTag Tags::unlocked_bgcolor      = Tag::create("unlocked_bgcolor");
}









