/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiApp_LookAndFeel.hpp"
#include "KiwiApp_BinaryData.hpp"

namespace bfonts = kiwi::binary_data::fonts;

namespace kiwi
{
    LookAndFeel::LookAndFeel()
    {
        setColour(juce::TooltipWindow::backgroundColourId, juce::Colours::lightgrey);
        setColour(juce::TooltipWindow::textColourId, juce::Colour(0xff444444));
    }
    
    juce::Typeface::Ptr LookAndFeel::getTypefaceForFont(juce::Font const& font)
    {
        juce::Typeface::Ptr typeface;
        
        if (font.getTypefaceName() == juce::Font::getDefaultSansSerifFontName())
        {
            if (font.getStyleFlags() & juce::Font::FontStyleFlags::bold)
            {
                if (font.getStyleFlags() & juce::Font::FontStyleFlags::italic)
                {
                    typeface =
                    juce::Typeface::createSystemTypefaceFor(bfonts::open_sans::OpenSans_BoldItalic_ttf,
                                                            bfonts::open_sans::OpenSans_BoldItalic_ttf_size);
                }
                else
                {
                    typeface =
                    juce::Typeface::createSystemTypefaceFor(bfonts::open_sans::OpenSans_Bold_ttf,
                                                            bfonts::open_sans::OpenSans_Bold_ttf_size);
                }
            }
            else if(font.getStyleFlags() & juce::Font::FontStyleFlags::italic)
            {
                typeface =
                juce::Typeface::createSystemTypefaceFor(bfonts::open_sans::OpenSans_SemiboldItalic_ttf,
                                                        bfonts::open_sans::OpenSans_SemiboldItalic_ttf_size);
            }
            else
            {
                typeface =
                juce::Typeface::createSystemTypefaceFor(bfonts::open_sans::OpenSans_Semibold_ttf,
                                                        bfonts::open_sans::OpenSans_Semibold_ttf_size);
            }
        }
        
        if (typeface == nullptr)
        {
            typeface = juce::Font::getDefaultTypefaceForFont (font);
        }
        
        return typeface;
    }
    
    void LookAndFeel::drawPropertyPanelSectionHeader(juce::Graphics& g,
                                                     const juce::String& name,
                                                     bool is_open, int width, int height)
    {
        //@see drawPropertyPanelCategoryHeader
        const float button_size = height * 0.75f;
        const float button_indent = (height - button_size) * 0.5f;
        
        //@see drawConcertinaPanelHeader
        const juce::Colour bkg(juce::Colours::grey);
        g.setGradientFill(juce::ColourGradient(bkg.withAlpha(0.2f), 0.f, 0.f,
                                               bkg.withAlpha(0.4f), 0., height,
                                               false));
        g.fillAll();
        
        g.setColour(bkg.contrasting(0.5).withAlpha(0.1f));
        g.drawHorizontalLine(0.f, 0.f, width);
        g.drawHorizontalLine(height - 1, 0.f, width);
        
        const int text_indent = (int)(button_indent * 2.0f + button_size + 2.0f);
        
        g.setColour(bkg.contrasting());
        g.setFont(juce::Font(height * 0.6f).boldened());
        g.drawFittedText(name, text_indent, 0, width - text_indent - 4, height,
                         juce::Justification::centredLeft, 1);
        
        //@see drawTreeviewPlusMinusBox
        const juce::Rectangle<float> plusRect{button_indent, button_indent, button_size, button_size};
        
        juce::Path p;
        p.addTriangle(0.0f, 0.0f, 1.0f, is_open ? 0.0f : 0.5f, is_open ? 0.5f : 0.0f, 1.0f);
        g.setColour(juce::Colours::white.contrasting().withAlpha(0.5f));
        g.fillPath(p, p.getTransformToScaleToFit(plusRect.reduced(2, plusRect.getHeight() / 4), true));
    }
    
    void LookAndFeel::drawTableHeaderColumn(juce::Graphics& g, juce::String const& columnName,
                                            int /*columnId*/,
                                            int width, int height,
                                            bool isMouseOver, bool isMouseDown,
                                            int columnFlags)
    {
        if(isMouseDown)
            g.fillAll(juce::Colours::white.withAlpha(0.2f));
        else if(isMouseOver)
            g.fillAll(juce::Colours::white.withAlpha(0.1f));
        
        juce::Rectangle<int> area(width, height);
        area.reduce(4, 0);
        
        if ((columnFlags & (juce::TableHeaderComponent::sortedForwards | juce::TableHeaderComponent::sortedBackwards)) != 0)
        {
            juce::Path sortArrow;
            sortArrow.addTriangle (0.0f, 0.0f,
                                   0.5f, (columnFlags & juce::TableHeaderComponent::sortedForwards) != 0 ? -0.8f : 0.8f,
                                   1.0f, 0.0f);
            
            g.setColour(juce::Colour (0x99000000));
            g.fillPath(sortArrow, sortArrow.getTransformToScaleToFit(area.removeFromRight(height / 2).reduced (2).toFloat(), true));
        }
        
        g.setColour(juce::Colours::whitesmoke);
        g.setFont(juce::Font(height * 0.5f, juce::Font::bold));
        g.drawFittedText(columnName, area, juce::Justification::centredLeft, 1);
    }
    
    void LookAndFeel::drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header)
    {
        auto r(header.getLocalBounds());
        
        const auto bdcolor = juce::Colours::black.withAlpha(0.5f);
        
        g.setColour(juce::Colour(0xFF4E4E4E));
        g.fillRect(r);
        
        g.setColour(bdcolor);
        g.fillRect(r.removeFromBottom(1));
        
        g.setColour(bdcolor);
        for(int i = header.getNumColumns(true) - 1; --i >= 0;)
        {
            g.fillRect(header.getColumnPosition(i).removeFromRight(1));
        }
    }
}
