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

#include <KiwiGraphics/KiwiFont.h>

namespace kiwi
{
    namespace graphics
    {
        double Font::Intern::getCharacterWidth(char const& c) const noexcept
        {
            return 0.;
        }
        
        double Font::Intern::getCharacterWidth(wchar_t const& c) const noexcept
        {
            return 0.;
        }
        
        double Font::Intern::getLineWidth(std::string const& line) const noexcept
        {
            return 0.;
        }
        
        double Font::Intern::getLineWidth(std::wstring const& line) const noexcept
        {
            return 0.;
        }
        
        Size Font::Intern::getTextSize(std::string const& text, const double width) const noexcept
        {
            return Size(0., 0.);
        }
        
        Size Font::Intern::getTextSize(std::wstring const& text, const double width) const noexcept
        {
            return Size(0., 0.);
        }
        
        Font::Font() noexcept
        {
            m_intern = getDefaultFont().m_intern->getNewReference();
        }
        
        Font::Font(std::string const& name, double height, Style style) noexcept
        {
            auto it = getAvailableFonts().find(name);
            if(it != getAvailableFonts().end())
            {
                m_intern = std::move(it->second.m_intern->getNewReference());
            }
            else
            {
                m_intern = getDefaultFont().m_intern->getNewReference();
            }
            setHeight(height);
            setStyle(style);
        }
        
        void Font::setName(const std::string& name)
        {
            const double height = getHeight();
            const Style  style  = Style(getStyle());
            auto it = getAvailableFonts().find(name);
            if(it != getAvailableFonts().end())
            {
                m_intern = std::move(it->second.m_intern->getNewReference());
            }
            else
            {
                m_intern = getDefaultFont().m_intern->getNewReference();
            }
            setHeight(height);
            setStyle(style);
        }
        
        std::string Font::getStyleName() const noexcept
        {
            const auto style = getStyle();
            
            switch(style)
            {
                case Regular:           return "Regular";
                case Bold:              return "Bold";
                case Italic:            return "Italic";
                case Underlined:        return "Underlined";
                case BoldItalic:        return "Bold Italic";
                case BoldUnderlined:    return "Bold Underlined";
                case ItalicUnderlined:  return "Italic Underlined";
                default:                return "Bold Italic Underlined";
            }
            
            return "Regular";
        }
        
        void Font::setStyle(std::string const& style_name)
        {
            if (style_name != getStyleName())
            {
                if(style_name == "Regular")
                {
                    setStyle(Regular);
                }
                else if(style_name == "Bold")
                {
                    setStyle(Bold);
                }
                else if(style_name == "Italic")
                {
                    setStyle(Italic);
                }
                else if(style_name == "Underlined")
                {
                    setStyle(Underlined);
                }
                else if(style_name == "Bold Italic")
                {
                    setStyle(BoldItalic);
                }
                else if(style_name == "Bold Underlined")
                {
                    setStyle(BoldUnderlined);
                }
                else if(style_name == "Italic Underlined")
                {
                    setStyle(ItalicUnderlined);
                }
                else if(style_name == "Bold Italic Underlined")
                {
                    setStyle(BoldItalicUnderlined);
                }
            }
        }
    }
}

