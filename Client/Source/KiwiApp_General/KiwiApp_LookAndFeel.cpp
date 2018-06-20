/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "../KiwiApp_General/KiwiApp_LookAndFeel.h"
#include "../KiwiApp_Ressources/KiwiApp_BinaryData.h"

#include <KiwiApp_Patcher/KiwiApp_PatcherView.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>
#include <KiwiApp_Patcher/KiwiApp_LinkView.h>

namespace bfonts = kiwi::binary_data::fonts;

namespace kiwi
{
    LookAndFeel::LookAndFeel()
    : juce::LookAndFeel_V4(getGreyColourScheme())
    {
        setUsingNativeAlertWindows(true);
        initColours();
    }
    
    float LookAndFeel::getObjectBorderSize() const
    {
        return m_box_border_size;
    }
    
    juce::TextLayout LookAndFeel::layoutTooltipText(juce::String const& text, juce::Colour colour) noexcept
    {
        const float font_size = 13.0f;
        const int max_width = 400;
        
        juce::AttributedString s;
        s.setColour(colour);
        s.setJustification(juce::Justification::centred);
        s.append(text, juce::Font(font_size, juce::Font::bold), colour);
        
        juce::TextLayout tl;
        tl.createLayoutWithBalancedLineLengths(s, (float) max_width);
        return tl;
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
    
    void LookAndFeel::drawTableHeaderColumn(juce::Graphics& g,
                                            juce::TableHeaderComponent&,
                                            juce::String const& columnName,
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
        
        g.setColour(getCurrentColourScheme()
                    .getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground)
                    .contrasting(0.9));
        
        g.setFont(juce::Font(height * 0.5f, juce::Font::bold));
        g.drawFittedText(columnName, area, juce::Justification::centredLeft, 1);
    }
    
    void LookAndFeel::drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header)
    {
        auto r(header.getLocalBounds());
        
        const auto bdcolor = juce::Colours::black.withAlpha(0.5f);
        
        g.setColour(getCurrentColourScheme()
                    .getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
        
        g.fillRect(r);
        
        g.setColour(bdcolor);
        g.fillRect(r.removeFromBottom(1));
        
        g.setColour(bdcolor);
        for(int i = header.getNumColumns(true) - 1; --i >= 0;)
        {
            g.fillRect(header.getColumnPosition(i).removeFromRight(1));
        }
    }
    
    void LookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& btn,
                                           juce::Colour const& bgcolor,
                                           bool mouse_over, bool mouse_down)
    {
        juce::Colour baseColour(bgcolor.withMultipliedSaturation(btn.hasKeyboardFocus (true) ? 1.3f : 0.9f).withMultipliedAlpha(btn.isEnabled() ? 0.9f : 0.5f));
        
        if (mouse_down || mouse_over)
            baseColour = baseColour.contrasting(mouse_down ? 0.2f : 0.1f);
        
        const bool flatOnLeft   = btn.isConnectedOnLeft();
        const bool flatOnRight  = btn.isConnectedOnRight();
        const bool flatOnTop    = btn.isConnectedOnTop();
        const bool flatOnBottom = btn.isConnectedOnBottom();
        
        const float width  = btn.getWidth() - 1.0f;
        const float height = btn.getHeight() - 1.0f;
        
        if(width > 0 && height > 0)
        {
            const float cornerSize = 1.0f;
            
            juce::Path outline;
            outline.addRoundedRectangle(0.5f, 0.5f, width, height, cornerSize, cornerSize,
                                        ! (flatOnLeft  || flatOnTop),
                                        ! (flatOnRight || flatOnTop),
                                        ! (flatOnLeft  || flatOnBottom),
                                        ! (flatOnRight || flatOnBottom));
            
            
            
            g.setColour(baseColour);
            g.fillPath(outline);
            
            g.setColour(btn.hasKeyboardFocus(false) ? baseColour.contrasting(0.5) : baseColour.contrasting(0.2));
            g.strokePath(outline, juce::PathStrokeType(1.0f));
        }
    }
    
    void LookAndFeel::paintToolbarBackground(juce::Graphics& g, int w, int h, juce::Toolbar& toolbar)
    {
        g.fillAll(toolbar.findColour(juce::Toolbar::backgroundColourId));
    }
    
    void LookAndFeel::paintToolbarButtonBackground(juce::Graphics& g, int /*width*/, int /*height*/,
                                                   bool isMouseOver, bool isMouseDown,
                                                   juce::ToolbarItemComponent& component)
    {
        // don't draw toolbar button background
    }
    
    void LookAndFeel::initColours()
    {
        // ------ Application
        
        setColour(juce::ScrollBar::ColourIds::thumbColourId,
                  juce::Colours::grey.withAlpha(0.7f));
        
        // ------ patcherview colors
        
        const auto patcherview_bg = juce::Colour::fromFloatRGBA(0.8, 0.8, 0.8, 1.);
        setColour(PatcherView::ColourIds::BackgroundUnlocked, patcherview_bg);
        setColour(PatcherView::ColourIds::BackgroundLocked, patcherview_bg);
        
        const auto selection_color = juce::Colour::fromFloatRGBA(0., 0.5, 1., 1.);
        setColour(PatcherView::ColourIds::Selection, selection_color);
        setColour(PatcherView::ColourIds::SelectionOtherView, selection_color.contrasting(0.4));
        setColour(PatcherView::ColourIds::SelectionOtherUser, juce::Colour(0xFFFF8C00));
        
        // ------ objectbox colors
        
        const juce::Colour box_bgcolor = juce::Colours::white;
        const auto box_bdcolor = box_bgcolor.contrasting(0.4);
        const auto link_control_color = box_bdcolor.contrasting(0.1f);
        const auto link_signal_color = juce::Colour(0xff444444);
        
        setColour(ObjectView::ColourIds::PinControl, link_control_color);
        setColour(ObjectView::ColourIds::PinSignal, link_signal_color);
        setColour(ObjectView::ColourIds::Error, juce::Colour::fromRGBA(223, 97, 94, 250));
        setColour(ObjectView::ColourIds::Background, box_bgcolor);
        setColour(ObjectView::ColourIds::Text, juce::Colours::black);
        setColour(ObjectView::ColourIds::Outline, box_bdcolor);
        setColour(ObjectView::ColourIds::Highlight, juce::Colour::fromFloatRGBA(0., 0.5, 1., 0.4));
        setColour(ObjectView::ColourIds::Active, juce::Colour(0xff21ba90));
        
        // ------ link colors
        
        setColour(LinkView::ColourIds::ControlBackground, link_control_color);
        setColour(LinkView::ColourIds::SignalBackground, link_signal_color);
    }
}

