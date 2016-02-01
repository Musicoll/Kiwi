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

#include "KiwiFont.h"

namespace Kiwi
{
    double Font::Intern::getCharacterWidth(char const& c) const noexcept
    {
        return 0.;
    }
    
    double Font::Intern::getCharacterWidth(wchar_t const& c) const noexcept
    {
        return 0.;
    }
    
    double Font::Intern::getLineWidth(string const& line) const noexcept
    {
        return 0.;
    }
    
    double Font::Intern::getLineWidth(wstring const& line) const noexcept
    {
        return 0.;
    }
    
    Size Font::Intern::getTextSize(string const& text, const double width) const noexcept
    {
        return Size(0., 0.);
    }
    
    Size Font::Intern::getTextSize(wstring const& text, const double width) const noexcept
    {
        return Size(0., 0.);
    }
    
    Font::Font() noexcept
    {
        m_intern = getDefaultFont().m_intern->getNewReference();
    }
    
    Font::Font(string const& name, double height, Style style) noexcept
    {
        auto it = getAvailableFonts().find(name);
        if(it != getAvailableFonts().end())
        {
            m_intern = move(it->second.m_intern->getNewReference());
        }
        else
        {
            m_intern = getDefaultFont().m_intern->getNewReference();
        }
        setHeight(height);
        setStyle(style);
    }
    
    void Font::setName(const string& name)
    {
        const double height = getHeight();
        const Style  style  = Style(getStyle());
        auto it = getAvailableFonts().find(name);
        if(it != getAvailableFonts().end())
        {
            m_intern = move(it->second.m_intern->getNewReference());
        }
        else
        {
            m_intern = getDefaultFont().m_intern->getNewReference();
        }
        setHeight(height);
        setStyle(style);
    }
    
    string Font::getStyleName() const noexcept
    {
        switch(getStyle())
        {
            case Regular:
                return string("Regular");
                break;
            case Bold:
                return string("Bold");
                break;
            case Italic:
                return string("Italic");
                break;
            case Underlined:
                return string("Underlined");
                break;
            case BoldItalic:
                return string("Bold Italic");
                break;
            case BoldUnderlined:
                return string("Bold Underlined");
                break;
            case ItalicUnderlined:
                return string("Italic Underlined");
                break;
            default:
                return string("Bold Italic Underlined");
                break;
        }
    }
    
    void Font::setStyle(string const& style)
    {
        if(style == string("Regular"))
        {
            setStyle(Regular);
        }
        else if(style == string("Bold"))
        {
            setStyle(Bold);
        }
        else if(style == string("Italic"))
        {
            setStyle(Italic);
        }
        else if(style == string("Underlined"))
        {
            setStyle(Underlined);
        }
        else if(style == string("Bold Italic"))
        {
            setStyle(BoldItalic);
        }
        else if(style == string("Bold Underlined"))
        {
            setStyle(BoldUnderlined);
        }
        else if(style == string("Italic Underlined"))
        {
            setStyle(ItalicUnderlined);
        }
        else if(style == string("Bold Italic Underlined"))
        {
            setStyle(BoldItalicUnderlined);
        }
    }
}

